
PROTOC = protoc --cpp_out=./
TARGETS = MessageID.pb.o Login.pb.o NetConnection.pb.o UserData.pb.o\
	ChatRoom.pb.o

all: $(TARGETS)

%.pb.cc: %.proto
	$(PROTOC) $^


clean:
	rm -f *.pb.*
