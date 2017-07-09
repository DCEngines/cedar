CXX=g++

all: lua-bindings

lua-bindings: cedar_wrap.o
	$(CXX) -shared -O2 -llua cedar_wrap.o -o cedar.so

luajit-bindings: cedar_wrap.o
	$(CXX) -shared -O2 -lluajit-5.1 cedar_wrap.o -o cedar.so

.cc.o:
	$(CXX) -DHAVE_CONFIG_H -I.. -I../.. -I../../src -fPIC -O2 $(CFLAGS) -c $<

clean:
	rm -f *.o cedar.so
