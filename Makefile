#CC = gcc
#CFLAGS = -O3 -DUSE_TOMCRYPT -Isrc
CFLAGS = -O3 -DUSE_OPENSSL -Isrc
#LIBS = -ltomcrypt
LIBS = -lcrypto
MPILIBS = -lmpi
BUILD_MPI = 1

HEADERS = brute.h util.h
SOURCES = brute.c
EXEC = brute

OBJDIR = obj
SRCDIR = src

.PHONY: all clean run push

MKS = Makefile
OBJ = $(patsubst %,$(OBJDIR)/%,$(SOURCES:.c=.o))

all:: $(EXEC)

clean::
	rm -f $(OBJDIR)/*.o $(OBJDIR)/_exists
	rmdir $(OBJDIR) || true
	rm -f $(EXEC)

$(EXEC): $(OBJ) $(OBJDIR)/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run: $(EXEC)
	time ./$(EXEC)

ifeq ($(BUILD_MPI),1)

ifdef CRAY_CC_VERSION
include cray.mk
MKS += cray.mk
endif

EXEC_MPI = $(EXEC)_mpi

all:: $(EXEC_MPI)

clean::
	rm -f $(EXEC_MPI)

$(EXEC_MPI): $(OBJ) $(OBJDIR)/mpi_main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(MPILIBS)

push: $(EXEC_MPI)
	qsub cray_run.pbs
endif


$(OBJDIR)/_exists:
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/_exists

DEPS = $(patsubst %,$(SRCDIR)/%,$(HEADERS)) $(MKS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(OBJDIR)/_exists
	$(CC) -c -o $@ $< $(CFLAGS)
