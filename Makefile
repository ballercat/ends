SHELL = /bin/sh
CC = gcc

FLAGS		 = -std=gnu99 -Iinclude 
CCFLAGS		 = -m64 -pedantic -Wall -Wextra -MMD -MP -MF
DEBUGFLAGS	 = -O0 -D _DEBUG -ggdb3 -g -p 
RELEASEFLAGS = -O3 -D NDEBUG -combine -fwhole-program \
				-mfpmath=sse -ffast-math -ftree-vectorize 

TARGET 	= ends
SOURCES = $(shell echo src/*.c src/memory/*.c src/cpu/*.c src/gui/*.c)
COMMON 	= src/types.h
HEADERS = $(shell echo src/cpu/*.h src/memory/*.h src/gui/*.h)
OBJECTS = $(SOURCES:.c=.o)
GTKLIBS	= $(shell pkg-config --libs gtk+-2.0)
GTKINCS = $(shell pkg-config --cflags gtk+-2.0)

PREFIX = $(DESTDIR)/usr/local
BINDRI = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(GTKLIBS)  $(OBJECTS)

Debug: $(TARGET)
release: $(SOURCES) $(HEADERS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(RELEASEFLAGS) -o $(TARGET) $(SOURCES)

profile: CFLAGS += -pg
profile: $(TARGET)


install: release
	install -D $(TARGET) $(BINDIR)/$(TARGET)

install-strip: release
	install -D -s $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	-rm $(BINDIR)/$(TARGET)


clean:
	-rm -f $(OBJECTS)
	-rm -f gmon.out

cleanDebug: clean

distclean: clean
	-rm -f $(TARGET)

.SECONDEXPANSION:

#$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell gcc -MM $(OBJ:.o=.c) | sed s/.*://)))
#%.o: %.c $$($$@_DEPS)
#	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -c -o $@ $< 

%.o: %.c $(HEADERS) $(COMMON)
	$(CC) $(FLAGS) $(CFLAGS) $(GTKINCS) $(DEBUGFLAGS) -c -o $@ $< 

.PHONY: all profile release \
	install install-strip uninstall clean distclean
 
