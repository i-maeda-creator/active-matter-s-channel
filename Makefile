CXX ?= g++
CXXFLAGS ?= -O2 -Wall -Wextra

.PHONY: all check smoke run clean

all: active-matter

active-matter: activematter.C include/*.h
	$(CXX) $(CXXFLAGS) activematter.C -o active-matter

check:
	$(CXX) -fsyntax-only activematter.C

smoke: active-matter
	./active-matter runs/smoke-2d-baseline 1 10 random

run: active-matter
	./active-matter runs/latest

clean:
	rm -f active-matter active-matter.exe
