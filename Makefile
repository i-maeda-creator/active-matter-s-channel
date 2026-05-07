CXX ?= g++
CXXFLAGS ?= -O2 -Wall -Wextra

.PHONY: all check smoke shape-r04 ring run clean

all: active-matter

active-matter: activematter.C include/*.h
	$(CXX) $(CXXFLAGS) activematter.C -o active-matter

check:
	$(CXX) -fsyntax-only activematter.C

smoke: active-matter
	./active-matter runs/smoke-2d-baseline 1 10 random 0.5 250 200 80

shape-r04: active-matter
	./active-matter runs/shape-r04-smoke 1 10 shape 0.4 10 200 80

legacy-shape-r04: active-matter
	./active-matter runs/legacy-shape-r04 1 1000 shape 0.4 10 68 35

legacy-shape-r04-ly34: active-matter
	./active-matter runs/legacy-shape-r04-ly34 1 1000 shape 0.4 10 68 34

legacy-best: active-matter
	./active-matter runs/legacy-shape-r04-ly33-step7000 1 7000 shape 0.4 10 68 33

ring: active-matter
	./active-matter runs/ring-road-smoke 120 100 ring 0.4 80 80 80 25 8

run: active-matter
	./active-matter runs/latest

clean:
	rm -f active-matter active-matter.exe
