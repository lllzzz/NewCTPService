objects = test.o Config.o MessageService.o MessageTestHandler.o Tool.o


test.o: src/market/test.cpp src/common/Config.h src/common/MessageService.h src/common/MessageHandler.h
	g++ -c src/market/test.cpp

Config.o: src/common/Config.h src/common/Config.cpp
	g++ -c src/common/Config.cpp

MessageService.o: src/common/MessageService.h src/common/MessageService.cpp
	g++ -c src/common/MessageService.cpp

MessageTestHandler.o: src/common/MessageHandler.h src/common/MessageTestHandler.cpp
	g++ -c src/common/MessageTestHandler.cpp

Tool.o: src/common/Tool.h src/common/Tool.cpp
	g++ -c src/common/Tool.cpp

test: $(objects)
	g++ -o bin/test $(objects) -ljsoncpp -lglog -lhiredis

clean:
	rm  test.o Config.o MessageService.o MessageTestHandler.o Tool.o
