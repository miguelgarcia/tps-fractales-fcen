CCX=g++
CXFLAGS=
LIBS =
OBJS = fparser.o fpoptimizer.o

all: fparser.o fpoptimizer.o

%.o: %.cc
	$(CCX) -c -o $@ $< $(CXFLAGS)

clean:
	rm $(OBJS)
