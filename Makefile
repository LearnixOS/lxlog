# Makefile for lxlog - See LICENSE for license information
include config.mk

BIN = lxlog
LIB = liblxlog.so
SRC = lxlog.c liblxlog.c
OBJ = lxlog.o liblxlog.o

all: $(BIN) $(LIB)

$(BIN): lxlog.o
	$(CC) -o $@ lxlog.o

$(LIB): liblxlog.o
	$(LD) $(LDFLAGS) -o $@ liblxlog.o

lxlog.o: lxlog.c
	$(CC) $(CFLAGS) -c $<

liblxlog.o: liblxlog.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(BIN) $(LIB) *.o

install: all
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(LIBDIR)
	cp -f $(BIN) $(DESTDIR)$(BINDIR)/
	cp -f $(LIB) $(DESTDIR)$(LIBDIR)/
	chmod 755 $(DESTDIR)$(BINDIR)/$(BIN)
	chmod 644 $(DESTDIR)$(LIBDIR)/$(LIB)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(BIN)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIB)

.PHONY: all clean install uninstall
