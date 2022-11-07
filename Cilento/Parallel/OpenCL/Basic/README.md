# SHA-256 Mining Simulation in FPGA

## Set up

Mintrop-exclusive steps:

```bash
module load fpga-environment
aocl compile-config
aocl link-config
```

## Compilation

### Compiling Host Code

```bash
make host
```

### Compiling Kernel

#### Emulation

```bash
make emu
```

#### FPGA Image

```bash
make fpga
```

## Running

### Running emulation

```bash
make run-emu
```

### Running FPGA Image Manually

```bash
source $AOCL_BOARD_PACKAGE_ROOT/linux64/libexec/sign_aocx.sh -H openssl_manager -i bin/<image_name>.aocx -r NULL -k NULL -o bin/<image_name>_unsigned.aocx
aocl program acl0 <image_name>_unsigned.aocx
```

Finally, run the FPGA image.

```bash
make run-fpga
```

## Cleaning

*WARNING*: This command will delete the **bin** folder created in **host**'s compilation.

### Clean All

```bash
make clean
```

---
