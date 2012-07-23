CCX=g++
CXFLAGS=-g `pkg-config gtkmm-3.0 --cflags --libs`
LIBS =

OBJS = main.o
BIN = tp8.a

tp8:  $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS) $(CXFLAGS)

%.o: %.cc
	$(CCX) -c -o $@ $< $(CXFLAGS)

clean:
	rm $(BIN) $(OBJS)
