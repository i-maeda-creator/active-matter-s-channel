CXX ?= g++
CXXFLAGS ?= -O2 -Wall -Wextra

.PHONY: all check run clean

all: active-matter

active-matter: activematter.C include/*.h
	$(CXX) $(CXXFLAGS) activematter.C -o active-matter

check:
	$(CXX) -fsyntax-only activematter.C

run: active-matter
	./active-matter

clean:
	rm -f active-matter active-matter.exe
