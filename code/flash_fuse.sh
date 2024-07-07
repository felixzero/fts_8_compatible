#!/bin/sh

avrdude -c usbasp -p m328p -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m -U efuse:w:0xfd:m
