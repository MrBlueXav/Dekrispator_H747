Dekrispator for STM32H747
===========

Hello ! I'm happy (maybe proud ;-)) to share this project with you.

It's the Dekrispator infamous synth ported on a powerful dual core mcu development board : Discovery Kit STM32H747I-DISCO from STMicroelectronics.

More info on this synth here (project started in 2012) : https://github.com/MrBlueXav/Dekrispator_v2/tree/dekrispatorV3

* M4 core (at 200 MHz) manages MIDI USB Host function (stable !).

* M7 core (at 400 MHz) computes sound.

* Each core communicates through OPENAMP infrastructure.

* No LCD at the moment.

* Debug messages with virtual com port from STlink.

I encountered endless issues to make this project work and suffered a lot... I hope some people will find it useful !

It was first a STM32CubeMX project (the only way I found to reach that point) but can't be regenerated now, be careful !

You should be able to import the project in STM32CubeIDE, I think all necessary files are in the repository.

If not, makefiles are in Debug and Release folders with also binaries for direct flashing.

- - - -

**Dekrispator features** :

 * monophonic
 * all digital !
 * sound generators :
   * oscillators with very low aliased analog waveforms with superb minBLEP oscillators (thanks to Sean Bolton)
   * 4 operators FM generator
   * multisaw (several saws with individual "drifters")
   * 10 sine additive generator
   * noise
 * 16 step random sequencer
 * several scales to choose from
 * 2 parallel filters LP/BP/HP with LFOs
 * Effects :
   * vibrato
   * tremolo
   * drive/distortion
   * echo
   * chorus/flanger
   * phaser
 * random sound and FX patch generator
 * LED information :
   * The orange LED reflects the CPU usage : when it's dim CPU gets overloaded.
   * The red LED lights on when sequencer is stopped or when a system error occurs.
   * The green LED indicates a midi usb is connected and recognized.
   * The blue LED indicates midi activity.

- - - -
Special thanks : Sean Bolton, Perry R. Cook and Gary P. Scavone, Gabriel Rivas, Ross Bencina, GaryA, Thorsten Klose, erwincoumans, ST

- - - - 

