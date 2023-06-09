CC = g++
CFLAGS = -std=c++11 -fPIC
LIBS = -lpthread

all: proactor_server st_proactor.so

proactor_server: proactor_server.cpp st_proactor.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

st_proactor.so: st_proactor.o
	$(CC) -shared -o $@ $<

st_proactor.o: st_proactor.cpp st_proactor.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o st_proactor st_proactor.so