CURDIR = $(shell pwd)
SRCDIR = $(CURDIR)/src
INCDIR = $(CURDIR)/include
BINDIR = $(CURDIR)/bin

ROOT_LIBS = `root-config --glibs` -lSpectrum -lTreePlayer -lMathMore

LIBRS = $(ROOT_LIBS) $(GSLLIBS) $(FORTRAN_LIBS)
INCLUDE = $(INCDIR)

CFLAGS = -std=c++11 -g -fPIC `root-config --cflags` `gsl-config --cflags` -Wno-unused-parameter

PLATFORM:=$(shell uname)
$(info PLATFORM: $(PLATFORM))
 
ifeq ($(PLATFORM),Darwin)
export __APPLE__:= 1
CFLAGS     += -Qunused-arguments
CPP        = clang++
FORTRAN_LIBS = -L/opt/local/lib/libgcc/ -lgfortran
else
export __LINUX__:= 1
CPP        = g++
FORTRAN_LIBS = -lgfortran
endif

HEAD = $(wildcard include/*.h)
OBJECTS = $(patsubst include/%.h,lib/%.so,$(HEAD))

$(info   OBJECTS = $(OBJECTS))

TARGET = bin/libGOSIAFitter.so

main: $(TARGET) bin/gosia 
	@printf "Make complete\n"

$(TARGET): $(OBJECTS) bin/DictOutput.cxx lib bin obj/gosia.o
	@printf "Now compiling shared library $@\n"
	@$(CPP) $(CFLAGS) -I$(INCDIR) -I. $(LIBRS) -o $@ -shared bin/DictOutput.cxx $(OBJECTS) obj/gosia.o

bin/DictOutput.cxx: $(HEAD)
	@printf "Linking libraries\n"
	@rootcint -f $@ -c -p $(HEAD) lib/linkdef.h

lib bin:
	@mkdir -p bin lib

obj/gosia.o: src/gosia_20081208.18.tjg.f include/Gosia.h
	@printf "Now compiling object gosia.o\n"
	@gfortran -fPIC -std=legacy -m64 -o obj/gosia.o -c src/gosia_20081208.18.tjg.f $(FORTRAN_LIBS)

lib/%.so: src/%.cxx include/%.h 
	@printf "Now compiling library $@\n"
	@$(CPP) $(CFLAGS) -I$(INCDIR) -L$(LIBRS) -o $@ -shared -c $< 

bin/gosia: bin src/gosia_20081208.18.f
	@printf "Compiling GOSIA \n" 
	@gfortran src/gosia_20081208.18.f -o bin/gosia 

clean:  
	@printf "Tidying up...\n"
	@rm $(OBJECTS)
	@rm -r bin/*
	@rm -r obj/*
