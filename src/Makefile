CC=gcc

IDIR =../include
ODIR=./obj

CFLAGS=-I $(IDIR)
LIBS=-lm

_DEPS = utilities.h bloom_filter.h murmurhash2.h red_black_tree.h voter_list.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o utilities.o bloom_filter.o murmurhash2.o red_black_tree.o voter_list.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	@echo "Executable file \"$@\" created";

$(ODIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
