CCX=g++
CXFLAGS=-g `pkg-config gtkmm-3.0 --cflags --libs`
LIBS =

OBJS = gaussian.o mdp2d.o MainWindow.o main.o
BIN = tp5.a

tp4:  $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LIBS) $(CXFLAGS)

%.o: %.cc
	$(CCX) -c -o $@ $< $(CXFLAGS)

clean:
	rm $(BIN) $(OBJS)
