# QEMU Execution

This directory contains helper scripts for running the `emdash_embedded`
binary under [QEMU](https://www.qemu.org/). The provided
`qemu_start.sh` script will build the project (if necessary) and invoke
`qemu-x86_64` to emulate the executable.

## Usage

```bash
cd embedded/qemu_config
./qemu_start.sh
```

The script requires `qemu-x86_64` to be installed. On Debian-based
systems this can be obtained via:

```bash
sudo apt-get install qemu-user
```
