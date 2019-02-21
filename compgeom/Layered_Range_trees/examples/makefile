# Layered Range Trees 
#
# Copyright (c) 2011 Vissarion Fisikopoulos
#
# Licensed under GNU LGPL.3, see LICENCE file

CC=g++
CFLAGS=-c -Wall
LDFLAGS=
X = 
INCLUDE= ../include/
SOURCES= Layered_Range_tree_10.cpp Layered_Range_tree_2.cpp
HEADERS= $(INCLUDE)Layered_Range_tree.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(SOURCES:.cpp= )

all: $(SOURCES) $(EXECUTABLE)
	
Layered_Range_tree_10: Layered_Range_tree_10.o  
	$(CC) $(LDFLAGS) $<  -o $@

Layered_Range_tree_2.: Layered_Range_tree_2.o
	$(CC) $(LDFLAGS) $<  -o $@

Layered_Range_tree_10.o : Layered_Range_tree_10.cpp $(INCLUDE)Layered_Range_tree_traits_10.h $(HEADERS)
	$(CC) $(CFLAGS)  $<  -o $@

Layered_Range_tree_2.o: Layered_Range_tree_2.cpp $(INCLUDE)Layered_Range_tree_traits_2.h $(HEADERS)
	$(CC) $(CFLAGS) $<  -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

