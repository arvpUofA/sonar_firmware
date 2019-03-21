# Integrated Sonar Preprocessor + ADC #

This firmware _will_ implement the sonar preprocessor board's current
functionality, as well as sampling at 1MSPS from each of the hydrophone
channels, all on a single STM32F303.

Mayyybe as a pipe dream we could also do FFT on this board but really
that's not necessary and frankly more work than necessary.

Eventually this will pump out the raw sonar data over CAN bus (at very low priority).

I'm thinking I might break out USB pins so we could implement USB-FS
in the future.

## TODO

Described in src/main.c
