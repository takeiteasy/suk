PROGEXT=
CFLAGS=-x objective-c -DSOKOL_METAL -fobjc-arc -framework Metal -framework Cocoa -framework MetalKit -framework Quartz -framework AudioToolbox
INC=-Ideps -Itest -I.

test:
	$(CC) $(INC) $(CFLAGS) -o test/test$(PROGEXT) test/*.c

default: test

.PHONY: test
