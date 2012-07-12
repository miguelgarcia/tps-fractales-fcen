CCX=g++
CXFLAGS=-g `pkg-config gtkmm-3.0 --cflags --libs` -I ../fparser/
LIBS = ../fparser/fparser.o ../fparser/fpoptimizer.o

OBJS = julia.o MainWindow.o main.o
BIN = tp4.a

tp4:  $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS) $(CXFLAGS)

%.o: %.cc
	$(CCX) -c -o $@ $< $(CXFLAGS)

clean:
	rm $(BIN) $(OBJS)
