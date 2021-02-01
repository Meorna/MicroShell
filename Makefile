srcdir = src/
bindir = bin/
docdir = doc/
CC = gcc -Wall
RM = rm -f
SRC = $(wildcard $(srcdir)*.c)
HEAD = $(wildcard $(srcdir)*.h)
OBJ = $(subst $(srcdir),$(bindir),$(SRC:.c=.o))
PROG = exe
CP = cp -r

all : $(PROG)

$(PROG): $(OBJ)
	$(CC) $^ -o $@ -lm

$(bindir)main.o: $(srcdir)main.c
	$(CC) -c $^ -o $@

$(bindir)%.o: $(srcdir)%.c $(srcdir)%.h
	$(CC) -c $< -o $(@)


.PHONY : clean
clean :
	$(RM) $(OBJ) $(PROG)
		
.PHONY : give
give :
		$(RM) $(OBJ) $(PROG)
		tar zcvf sallioroma-MicroShellSystemeEtReseau.tar.gz $(srcdir) $(bindir) $(docdir) Makefile

.PHONY : restore

