#CC = gcc
#CFLAGS = -O3 -DUSE_TOMCRYPT -Isrc
CFLAGS = -O3 -DUSE_OPENSSL -Isrc -Wno-deprecated-declarations
#LIBS = -ltomcrypt
LIBS = -lcrypto
DEPS =

HEADERS = brute.h
SOURCES = brute.c
EXEC = brute

OBJDIR = obj
SRCDIR = src

all: $(EXEC) $(EXEC)_mpi

DEPS = $(patsubst %,$(SRCDIR)/%,$(HEADERS))
OBJ = $(patsubst %,$(OBJDIR)/%,$(SOURCES:.c=.o))

$(OBJDIR)/_exists:
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/_exists

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(OBJDIR)/_exists
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ) $(OBJDIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(EXEC)_mpi: $(OBJ) $(OBJDIR)/mpi_main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) -lmpi

.PHONY: run
run: $(EXEC)
	time ./$(EXEC)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(OBJDIR)/_exists
	rmdir $(OBJDIR) || true
	rm -f $(EXEC)
