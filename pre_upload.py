Import("env")
import os
import subprocess

# Path to the export.sh script
EXPORT_SH_PATH = os.path.expanduser("~/esp/esp-idf/export.sh")
ESPTOOL_PATH = os.path.expanduser("~/esp/esp-idf/components/esptool_py/esptool/esptool.py")

# Define the QEMU command
QEMU_CMD = "qemu-system-xtensa -nographic -machine esp32 -drive file=firmware_combined.bin,if=mtd,format=raw"

def run_command(command):
    try:
        result = subprocess.run(command, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(result.stdout.decode())
        if result.returncode != 0:
            print("Error: Command execution failed")
        else:
            print("Command executed successfully")
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(e.stdout.decode())
        print(e.stderr.decode())

def before_upload(source, target, env):
    # Paths to firmware components
    build_dir = env.subst("$BUILD_DIR")
    bootloader_path = os.path.join(build_dir, "bootloader.bin")
    partitions_path = os.path.join(build_dir, "partitions.bin")
    application_path = os.path.join(build_dir, "firmware.bin")
    combined_firmware_path = os.path.join(build_dir, "firmware_combined.bin")

    # Ensure the firmware binary exists
    if not os.path.isfile(application_path):
        print("Firmware binary not found:", application_path)
        return

    # Generate the bootloader and partition table binaries if they don't exist
    if not os.path.isfile(bootloader_path) or not os.path.isfile(partitions_path):
        run_command(f"source {EXPORT_SH_PATH} && idf.py build")

    # Ensure all components exist
    if not (os.path.isfile(bootloader_path) and os.path.isfile(partitions_path) and os.path.isfile(application_path)):
        print("Required firmware components not found")
        return

    # Combine the bootloader, partition table, and application firmware
    run_command(f"source {EXPORT_SH_PATH} && python3 {ESPTOOL_PATH} --chip esp32 merge_bin -o {combined_firmware_path} --flash_mode dio --flash_size 4MB 0x1000 {bootloader_path} 0x8000 {partitions_path} 0x10000 {application_path}")

    # Ensure the combined firmware is padded correctly
    firmware_size = os.path.getsize(combined_firmware_path)
    supported_sizes = [4 * 1024 * 1024, 8 * 1024 * 1024, 16 * 1024 * 1024]
    if firmware_size not in supported_sizes:
        # Find the next supported size
        new_size = next(size for size in supported_sizes if size > firmware_size)
        with open(combined_firmware_path, 'ab') as f:
            f.write(b'\xFF' * (new_size - firmware_size))
        print(f"Firmware padded to {new_size} bytes")

    # Command to execute QEMU with the firmware
    qemu_command = QEMU_CMD.replace("firmware_combined.bin", combined_firmware_path)
    print(f"Running QEMU command: {qemu_command}")

    # Full command to source export.sh and then run QEMU
    full_command = f"bash -c 'source {EXPORT_SH_PATH} && {qemu_command}'"
    print(f"Full command: {full_command}")

    # Run the QEMU command and capture output in real-time
    try:
        process = subprocess.Popen(full_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        while True:
            output = process.stdout.readline()
            if output == b'' and process.poll() is not None:
                break
            if output:
                print(output.decode().strip())
        rc = process.poll()
        return rc
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(e.stdout.decode())
        print(e.stderr.decode())

# Attach the before_upload function to the upload process
env.AddPreAction("upload", before_upload)
