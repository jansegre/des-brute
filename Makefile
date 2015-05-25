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

EXEC_PBS = des-brute.pbs

push: $(EXEC_MPI)
	qsub -v PLAINTEXT="1234123412341234",CIPHERTEXT="57475a4f836b31d5",KEYFROM="5678567856780000",KEYTO="5678567856790000" $(EXEC_PBS)

endif


$(OBJDIR)/_exists:
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/_exists

DEPS = $(patsubst %,$(SRCDIR)/%,$(HEADERS)) $(MKS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(OBJDIR)/_exists
	$(CC) -c -o $@ $< $(CFLAGS)
