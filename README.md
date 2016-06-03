Rolled Oats
===========

raw serial -> stdout

Usage
-----

This is intended primarily for users of Mac OS X with FTDI and other
USB to Serial adapter chips. You can sometimes get away with `stty` on
a serial device:

`stty -f /dev/tty.usbserial speed 115200 cs8 -cstopb -parenb -crtscts && od -x < /dev/tty.usbserial`

But on a Mac (at least) using an FTDI USB to Serial adapter, the
configuration made by `stty` doesn't stick, and the port reverts to
defaults when it closes.

Handy for dumping serial out of a device that is not ASCII text,
like `ro | hexdump -C` or `ro | od -x`

For Arduino, embedded devices, reverse engineering, etc.

License
-------

Copyright (C) 2016 Aaron Kondziela - Release under MIT License
