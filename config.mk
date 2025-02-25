# config.mk - Configuration for compile settings

# Installation prefixes
PREFIX = /usr
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib

# Compiler and linker
CC = cc
LD = $(CC)

# Compiler flags
CFLAGS = -O2 -Wall -fPIC
LDFLAGS = -shared -ldl
