CFLAGS = -Wall -O2 -s -Werror
LDFLAGS = -lusb -s -lusb-1.0
PROG = reset_usb

all:	$(PROG)

$(PROG):	$(PROG).c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROG)
