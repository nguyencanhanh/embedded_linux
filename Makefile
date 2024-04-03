CC=gcc
SRCDIR= .
BINDIR=bin

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%.o)
EXECUTABLE=$(BINDIR)/app

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJECTS): $(BINDIR)/%.o : $(SRCDIR)/%.c | $(BINDIR)
	$(CC) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR)
