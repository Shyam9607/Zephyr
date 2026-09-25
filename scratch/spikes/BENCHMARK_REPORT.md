# Empirical Framework Comparison: Arduino + NimBLE vs ESP-IDF + NimBLE

This report captures the local empirical benchmarks and architectural capability comparison between **Arduino + NimBLE** and **ESP-IDF + NimBLE** on the ESP32 DevKit V1 target (`esp32doit-devkit-v1`), executing Task 2 of Plan 1.1.

Both spike implementations were constructed to evaluate the exact same architectural capability: a minimal BLE HID mouse service with a 50-byte report map, 4-byte report payload (3 buttons L/M/R, relative X/Y movement, vertical wheel), Device Information Service (DIS), and HID Mouse appearance advertising (0x03C2).

---

## 1. Test Environment & Toolchain Details

- **Host OS**: Windows 11 (build 26100)
- **Host Architecture**: x86_64
- **PlatformIO Core**: 6.2.0 (`~/.platformio/penv/Scripts/pio.exe`)
- **Target Board**: ESP32 DevKit V1 (`esp32doit-devkit-v1`)
- **Target SoC**: ESP32-D0WDQ6 (240MHz Xtensa dual-core, 320KB internal SRAM, 4MB Flash)
- **Measurement Methodology**: Automated local stopwatch measurement via PowerShell scripts (`measure_arduino.ps1`, `measure_espidf.ps1`), clean build (`pio run -t clean` followed by `pio run`), incremental build (file timestamp touch on primary source file followed by `pio run`), and memory report from PlatformIO linker output (`size` tool).

---

## 2. Side-by-Side Empirical Metrics

| Metric | Arduino + NimBLE | ESP-IDF + NimBLE | Delta (ESP-IDF vs Arduino) | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **Framework Version** | PlatformIO `espressif32 @ 6.10.0` / Arduino Core `2.0.17` | PlatformIO `espressif32 @ 7.1.3` / ESP-IDF `v5.3.2` | ESP-IDF uses newer upstream IDF | Native ESP-IDF toolchain includes GCC 15 |
| **BLE Stack** | `NimBLE-Arduino @ ^1.4.2` | Native ESP-IDF NimBLE (`CONFIG_BT_NIMBLE_ENABLED=y`) | Native C stack vs C++ wrapper | Both use Apache NimBLE host/controller |
| **HID Report Map** | 50 bytes (ID 1: Buttons 1-3, X, Y, Wheel) | 50 bytes (ID 1: Buttons 1-3, X, Y, Wheel) | **Identical** | Byte-for-byte identical descriptor |
| **HID Services** | HID (0x1812) + DIS (0x180A) + PnP | HID (0x1812) + DIS (0x180A) + PnP | **Identical** | Standard HID over GATT profile |
| **Flash Utilization** | **591,501 bytes** (45.1% of 1.31 MB partition) | **446,237 bytes** (42.6% of 1.05 MB partition) | **-145,264 bytes (-24.6%)** | ESP-IDF has noticeably smaller flash footprint |
| **Static RAM** | **35,684 bytes** (10.9% of 327,680 bytes) | **27,132 bytes** (8.3% of 327,680 bytes) | **-8,552 bytes (-24.0%)** | ESP-IDF allocates less static RAM |
| **Clean Build Time** (packages cached) | **19.43 seconds** | **155.81 seconds** (2m 35.8s) | **+136.38s (8.0x slower)** | ESP-IDF builds entire IDF component tree from source |
| **Incremental Build Time** | **4.04 seconds** | **5.20 seconds** | **+1.16s (comparable)** | Single file modification rebuilds in ~4-5s on both |
| **Cold Clean Time** (package download) | 857.75 seconds | 637.06 seconds | - | Initial toolchain package download & unpack |
| **Tooling Disk Footprint** | ~360 MB | ~1.5 GB | **~4.2x larger** | ESP-IDF requires toolchains, python venv, and component manager |
| **Path Handling** | Tolerates spaces in paths | **Fails if path contains spaces** | Blocking constraint | ESP-IDF builder checks `any(" " in p for p in (FRAMEWORK_DIR, BUILD_DIR))` |

---

## 3. Key Observations & Trade-Offs

### 3.1 Memory & Resource Footprint
- **ESP-IDF** is significantly leaner in both Flash (-24.6%) and static RAM (-24.0%). Because ESP-IDF compiles only the configured Kconfig options into static archives and links with dead-code elimination across custom CMake targets, the resulting binary is ~145 KB smaller.
- **Arduino** includes the Arduino core runtime layer (wiring, HardwareSerial, Print, Stream, timing shims) and the C++ `NimBLE-Arduino` library layer on top of ESP-IDF precompiled base libraries, costing ~145 KB flash and ~8.5 KB static RAM. However, with 45.1% flash usage on a 1.31 MB partition (leaving >700 KB flash and >290 KB RAM headroom), both frameworks comfortably fit the Zephyr firmware requirements.

### 3.2 Developer Experience & Tooling Ergonomics
- **Build Times**: Arduino delivers an 8.0x faster clean build (19.4s vs 155.8s). Both offer very fast incremental rebuilds (~4.0s vs 5.2s).
- **Workspace Path Whitespace Limitation**: ESP-IDF's PlatformIO builder script (`espidf.py:2045`) strictly asserts:
  ```
  if any(" " in p for p in (FRAMEWORK_DIR, BUILD_DIR)):
      sys.stderr.write("Error: Detected a whitespace character in project paths.\n")
      env.Exit(1)
  ```
  Because the repository root folder contains whitespace (`Zephyr - Airmouse`), ESP-IDF fails immediately out-of-the-box unless worked around (e.g. via virtual drive mapping `subst Z:` or path relocation). Arduino builds without any path restriction.
- **API Ergonomics**:
  - `NimBLE-Arduino` provides a concise, idiomatic C++ API (`pHid->inputReport(1)`, `pHid->reportMap(...)`, `pHid->startServices()`).
  - ESP-IDF native NimBLE requires writing raw C GATT attribute table definitions (`ble_gatt_svc_def`, `ble_gatt_chr_def`, `ble_gatt_dsc_def`), manual UUID byte structures, and custom access callbacks handling mbuf allocations (`os_mbuf_append`).

---

## 4. Benchmark Reproducibility

To reproduce these measurements locally:
```powershell
# For Arduino + NimBLE:
powershell -ExecutionPolicy Bypass -File scratch/measure_arduino.ps1

# For ESP-IDF + NimBLE (requires virtual drive subst if repo path has spaces):
powershell -ExecutionPolicy Bypass -File scratch/measure_espidf.ps1
```
