#Files path define
vpath %.hpp ../include
vpath %.cc ../src
vpath %.o bin
vpath %.a ../libs/

LIB_PATH := ../libs/
SRC := $(wildcard *.cc)
OBJ := $(patsubst %.cc,%.o, $(SRC))

all_file :env server client

env : bin HandleText
bin :
	mkdir bin
HandleText :
	mkdir HandleText

server : $(OBJ)
	g++ -o $@ $^ -L $(LIB_PATH) -lpthread -ljsoncpp

client : client.o socket.o socket_IO.o inet_addr.o
	g++ -o $@ $^ -L $(LIB_PATH) -ljsoncpp

$(OBJ) : $(SRC)
	g++ -o bin/$@ -c $(patsubst %.o, %.cc, $@)

.PHONY : clean clean_bin clean_text
clean : clean_bin clean_text
	rm server client
clean_bin :
	rm -rf bin
clean_text :
	rm -rf HandleText
