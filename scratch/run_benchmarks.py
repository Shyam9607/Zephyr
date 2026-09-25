import os
import sys
import time
import subprocess
import re
from pathlib import Path

PIO_EXE = os.path.expanduser("~/.platformio/penv/Scripts/pio.exe")

def run_cmd(cmd, cwd):
    start = time.perf_counter()
    proc = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    elapsed = time.perf_counter() - start
    return elapsed, proc.returncode, proc.stdout

def parse_pio_output(output):
    # RAM:   [=         ]  11.2% (used 36720 bytes from 327680 bytes)
    # Flash: [====      ]  36.8% (used 482101 bytes from 1310720 bytes)
    ram_match = re.search(r"RAM:\s+\[.*?\]\s+([\d\.]+)%\s+\(used\s+(\d+)\s+bytes\s+from\s+(\d+)\s+bytes\)", output)
    flash_match = re.search(r"Flash:\s+\[.*?\]\s+([\d\.]+)%\s+\(used\s+(\d+)\s+bytes\s+from\s+(\d+)\s+bytes\)", output)
    
    ram_info = {
        "pct": ram_match.group(1) if ram_match else "N/A",
        "used": int(ram_match.group(2)) if ram_match else 0,
        "total": int(ram_match.group(3)) if ram_match else 0,
    } if ram_match else None

    flash_info = {
        "pct": flash_match.group(1) if flash_match else "N/A",
        "used": int(flash_match.group(2)) if flash_match else 0,
        "total": int(flash_match.group(3)) if flash_match else 0,
    } if flash_match else None

    return ram_info, flash_info

def benchmark_env(name, spike_dir, source_file):
    print(f"\n==========================================")
    print(f"BENCHMARKING: {name}")
    print(f"Directory: {spike_dir}")
    print(f"==========================================")

    # 1. First build to ensure packages/dependencies are installed
    print("Initial build (ensuring dependencies)...")
    init_time, ret, out = run_cmd([PIO_EXE, "run"], spike_dir)
    if ret != 0:
        print(f"Initial build failed for {name} with code {ret}:\n{out}")
        return None

    # 2. Clean
    print("Running clean...")
    _, ret, _ = run_cmd([PIO_EXE, "run", "-t", "clean"], spike_dir)

    # 3. Timed Clean Build (Compilation with warm toolchain cache)
    print("Executing timed Clean Build...")
    clean_time, ret, clean_out = run_cmd([PIO_EXE, "run"], spike_dir)
    if ret != 0:
        print(f"Clean build failed:\n{clean_out}")
        return None
    print(f"Clean build completed in {clean_time:.2f}s")

    ram, flash = parse_pio_output(clean_out)
    if ram:
        print(f"RAM: {ram['used']:,} / {ram['total']:,} bytes ({ram['pct']}%)")
    if flash:
        print(f"Flash: {flash['used']:,} / {flash['total']:,} bytes ({flash['pct']}%)")

    # 4. Touch source file for Incremental Build
    src_path = Path(spike_dir) / source_file
    src_path.touch()
    print("Executing timed Incremental Build...")
    inc_time, ret, inc_out = run_cmd([PIO_EXE, "run"], spike_dir)
    if ret != 0:
        print(f"Incremental build failed:\n{inc_out}")
        return None
    print(f"Incremental build completed in {inc_time:.2f}s")

    return {
        "name": name,
        "clean_time": clean_time,
        "inc_time": inc_time,
        "ram": ram,
        "flash": flash,
        "output_sample": clean_out
    }

if __name__ == "__main__":
    base_dir = Path(__file__).resolve().parent / "spikes"
    
    arduino_dir = base_dir / "arduino-nimble"
    arduino_res = benchmark_env("Arduino + NimBLE", str(arduino_dir), "src/main.cpp")
    
    espidf_dir = base_dir / "espidf-nimble"
    espidf_res = benchmark_env("ESP-IDF + NimBLE", str(espidf_dir), "src/main.c")

    print("\nSummary results ready.")
