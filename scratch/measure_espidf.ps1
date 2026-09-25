$pio = "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe"

Write-Host "=== BENCHMARKING ESPIDF-NIMBLE ==="
Push-Location "scratch/spikes/espidf-nimble"

# Clean build
Write-Host "Running clean..."
& $pio run -t clean | Out-Null
Write-Host "Measuring Clean Build..."
$sw = [System.Diagnostics.Stopwatch]::StartNew()
$cleanBuildOut = & $pio run 2>&1
$sw.Stop()
$cleanBuildSeconds = $sw.Elapsed.TotalSeconds
Write-Host "Clean Build Time: $cleanBuildSeconds seconds"

# Incremental build
(Get-Item "src/main.c").LastWriteTime = (Get-Date)
Write-Host "Measuring Incremental Build..."
$sw = [System.Diagnostics.Stopwatch]::StartNew()
$incBuildOut = & $pio run 2>&1
$sw.Stop()
$incBuildSeconds = $sw.Elapsed.TotalSeconds
Write-Host "Incremental Build Time: $incBuildSeconds seconds"

Pop-Location

Write-Host "`n=== CLEAN BUILD OUTPUT (ESP-IDF) ==="
$cleanBuildOut | Select-String -Pattern "RAM:", "Flash:", "Building", "Compiling", "Linking", "SUCCESS", "took" | ForEach-Object { $_.Line }
$cleanBuildOut | Select-String -Pattern "RAM:", "Flash:" | ForEach-Object { Write-Host "MEMORY: $($_.Line)" }

Write-Host "`nClean seconds: $cleanBuildSeconds"
Write-Host "Incremental seconds: $incBuildSeconds"
