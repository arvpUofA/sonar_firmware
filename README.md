# Integrated Sonar Preprocessor + ADC #

This firmware _will_ implement the sonar preprocessor board's current
functionality, as well as sampling at 1MSPS from each of the hydrophone
channels, all on a single STM32F303.
The plan is to get the ping data then dump that out over CAN bus.
(quite a large transfer, but whatever)

Mayyybe as a pipe dream we could also do FFT on this board but really
that's not necessary and frankly more work than necessary.

I'm thinking I might break out USB pins so we could implement USB-FS
in the future.

# Toolchain installation

Install make tools

Install [ARM GCC](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)


## TODO

Described in src/main.c

## Generating DSDL

The custom DSDL should be moved to another repository, but for now, it's just sitting here.

```
make dsdl
```