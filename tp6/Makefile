CCX=g++
CXFLAGS=-g `pkg-config gtkmm-3.0 --cflags --libs`
LIBS =

OBJS = GoL.o MainWindow.o main.o
BIN = tp6.a

tp6:  $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS) $(CXFLAGS)

%.o: %.cc
	$(CCX) -c -o $@ $< $(CXFLAGS)

clean:
	rm $(BIN) $(OBJS)
