LIBS = -lGOSIAFitter `root-config --glibs` -lSpectrum -lTreePlayer -lMathMore
CPP = g++
CFLAGS = -std=c++11 -g -fPIC `root-config --cflags` `gsl-config --cflags` -Wno-unused-parameter
INCDIR = $(shell pwd)/../../include
LIBDIR = $(shell pwd)/../../lib
SRCDIR = $(shell pwd)/../../bin
CURDIR = $(shell pwd)
BINDIR = $(CURDIR)

SRCS := $(wildcard *.cxx)
EXES := $(patsubst %.cxx,$(BINDIR)/%,$(SRCS))
#OBJS := $(patsubst %,%.o,$(EXES))

all : $(EXES)

$(BINDIR)/%: $(CURDIR)/%.cxx gosia
	@mkdir -p $(BINDIR)
	@printf "Creating executable $@ from $<\n"
	$(CPP) $< -o $@ $(CFLAGS) -I$(INCDIR) -L$(SRCDIR) -L$(LIBDIR) $(LIBS)

gosia: $(SRCDIR)/gosia
	@cp $(SRCDIR)/gosia gosia

clean:
	@printf "Cleaning up\n"
	@rm $(EXES)
