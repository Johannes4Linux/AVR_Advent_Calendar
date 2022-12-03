# Software installation

We will need the following packages:

- avr-gcc: a compiler for the AVR 8-bit architecture
- avr-libc: precompiled libraries and header files for programming
- arddude: tool to flash the program to target

To install it on a Debian based system you need to run:

```
sudo apt install gcc-avr avr-libc avrdude
```

On Arch Linux you need to run:


```
sudo pacman -S avr-gcc avr-libc avrdude
```

On Fedora:

```
sudo dnf install avr-gcc avr-libc avrdude
```

