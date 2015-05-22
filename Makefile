#CC = gcc
#CFLAGS = -O3 -DUSE_TOMCRYPT -Isrc
CFLAGS = -O3 -DUSE_OPENSSL -Isrc
#LIBS = -ltomcrypt
LIBS = -lcrypto
DEPS =

HEADERS =
SOURCES = brute.c
EXEC = brute

OBJDIR = obj
SRCDIR = src

all: $(EXEC)

DEPS = $(patsubst %,$(SRCDIR)/%,$(HEADERS))
OBJ = $(patsubst %,$(OBJDIR)/%,$(SOURCES:.c=.o))

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: run
run: $(EXEC)
	time ./$(EXEC)

.PHONY: clean
clean:
	rm -f $(OBJ)
	rmdir $(OBJDIR) || true
	rm -f $(EXEC)
