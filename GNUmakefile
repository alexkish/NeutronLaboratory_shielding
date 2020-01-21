
name := NeutronLabUHM
G4TARGET := $(name)
G4EXLIB := true

#ifndef G4INSTALL
#   G4INSTALL = /Applications/Xapps/GEANT4/geant4.9.3/
#endif

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)

 EXTRALIBS += $(ROOTLIBS)
 EXTRALIBS += $(CLHEPLIBS)

CPPFLAGS += $(ROOTCFLAGS)

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
