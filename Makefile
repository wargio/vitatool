TOOLS	=	pkg_reader pupunpack readself
COMMON	=	tools.o aes.o sha1.o ec.o bn.o sfo_stuff.o
DEPS	=	Makefile tools.h types.h
OSNAME  =	$(shell uname -s)

ifeq ($(findstring MINGW, $(OSNAME)), MINGW)
COMMON  += mingw_mmap.o
endif

CC	=  gcc
CFLAGS	=  -g -O2 -Wall -W
LDLIBS  =  -lz

# Darwin's MacPorts Default Path
ifeq ($(shell test -e /opt/local/include/gmp.h; echo $$?),0)
CFLAGS  += -I/opt/local/include
LDLIBS  += -L/opt/local/lib
endif
 
OBJS	= $(COMMON) $(addsuffix .o, $(TOOLS))

all: $(TOOLS)

$(TOOLS): %: %.o $(COMMON) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $< $(COMMON) $(LDLIBS) 

$(OBJS): %.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f $(OBJS) $(TOOLS) *.exe

