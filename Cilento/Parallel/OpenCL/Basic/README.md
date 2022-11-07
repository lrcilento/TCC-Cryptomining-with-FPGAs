# 2D Wave Simulation Optimization in FPGA

## Set up

Mintrop-exclusive steps:

```bash
module load fpga-environment
aocl compile-config
aocl link-config
```

It is possible to configure the grid size and the number of total iterations.

```bash
make config nz=<z_dim> nx=<x_dim> iter=<iterations>
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

## Plots

The wave plotted is inside **./bin/plots** and the archive used by *plots.py* is inside **./bin/wavefield**.
