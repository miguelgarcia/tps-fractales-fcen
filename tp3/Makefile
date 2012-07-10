CCX=g++
CXFLAGS=`pkg-config gtkmm-3.0 --cflags --libs` -lfparser

OBJS = graficador.o MainWindow.o main.o
BIN = tp3

tp3:  $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(CXFLAGS)

%.o: %.cc
	$(CCX) -c -o $@ $< $(CXFLAGS)

clean:
	rm $(BIN) $(OBJS)
