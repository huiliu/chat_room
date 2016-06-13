
vpath %.pb.cc src
vpath %.pb.h src

PROTOC = protoc -Iproto
LDFLAGS 	= `pkg-config --libs protobuf` -lboost_filesystem -lboost_date_time\
				-lboost_thread -lboost_system 
CXXFLAGS 	= -g -Wall -std=c++11 -Isrc

LIBS = MessageID.pb.o Login.pb.o UserData.pb.o ChatRoom.pb.o\
	ConnectionInfo.pb.o ConnectionManager.o\
	iPublisher.o iServerBase.o iSubscriber.o NetPublisher.o NetSubscriber.o\
	ServerBase.o NetAcceptor.o NetConnection.o ConfigFile.o Timer.o\
	MessageDispatcher.o iCryptTool.o SessionManager.o ChatroomManager.o\
	UserManager.o\
	main.o
TARGET = main

all: proto $(LIBS) main

%.pb.o: %.pb.cc
	g++ -c $(CXXFLAGS) $(LDFLAGS) $^

proto: proto/*.proto
	$(PROTOC) --cpp_out=src/ proto/*.proto
	$(PROTOC) --python_out=python/ proto/*.proto

main: $(LIBS)
	g++ $(LDFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	rm -f *.pb.* src/*.* python/*.* *.o $(TARGET)

.PHONY: all proto clean main

