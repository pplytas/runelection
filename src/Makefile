CC=gcc

IDIR =../include
ODIR=./obj

CFLAGS=-I $(IDIR) -Wall
LIBS=-lm

_DEPS = utilities.h bloom_filter.h murmurhash2.h red_black_tree.h postcode_list.h voter_list.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = runelection.o utilities.o bloom_filter.o murmurhash2.o red_black_tree.o postcode_list.o voter_list.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

runelection: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Executable file \"$@\" created";

$(ODIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
