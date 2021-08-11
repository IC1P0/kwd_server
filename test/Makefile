#Files path define
vpath %.hpp ../include
vpath %.cc ../src
vpath %.o bin/
vpath %.a ../libs

CONF = ../config.json
LIB_PATH := ../libs
LIBS := -lpthread -L $(LIB_PATH) -ljsoncpp
SRC := $(wildcard ../src/*.cc)
OBJ := $(patsubst ../src/%.cc, %.o, $(SRC))
CXX := g++

all_file : env server client

env : bin HandledText
bin :
	mkdir bin
HandledText :
	mkdir HandledText

server : test_server $(OBJ)
	$(CXX) -o $@ bin/*.o $(LIBS)

test_server :
	cp test_server.cc ../src/.

$(OBJ) : $(CONF) $(SRC)
	$(CXX) -c $(patsubst %.o, ../src/%.cc, $@) -o bin/$@

client : client.o bin/socket.o bin/socket_IO.o bin/inet_addr.o
	$(CXX) -o $@ $^ -L $(LIB_PATH) -ljsoncpp

client.o : client.cc
	$(CXX) -o $@ -c $<

.PHONY : rebuild
rebuild : clean all_file

.PHONY : clean clean_bin clean_text
clean : clean_bin clean_text
	rm server client *.o;
	rm ../src/test_server.cc
clean_bin :
	rm -rf bin
clean_text :
	rm -rf HandledText
