LIBS = `root-config --glibs` -lGOSIAFitter -lSpectrum -lTreePlayer -lMathMore
CPP = g++
CFLAGS = -std=c++11 -g -fPIC `root-config --cflags --libs --glibs` `gsl-config --cflags` -Wno-unused-parameter
INCDIR = $(shell pwd)/../../include
LIBDIR = $(shell pwd)/../../bin
CURDIR = $(shell pwd)
BINDIR = $(CURDIR)

SRCS := $(wildcard *.cxx)
EXES := $(patsubst %.cxx,$(BINDIR)/%,$(SRCS))
#OBJS := $(patsubst %,%.o,$(EXES))

all : $(EXES)

$(BINDIR)/%: $(CURDIR)/%.cxx gosia
	@mkdir -p $(BINDIR)
	@printf "Creating executable $@ from $<\n"
	@$(CPP) $< -o $@ $(CFLAGS) -I$(INCDIR) -L$(LIBDIR) $(LIBS)

gosia: $(LIBDIR)/gosia
	@cp $(LIBDIR)/gosia gosia

clean:
	@printf "Cleaning up\n"
	@rm $(EXES)
