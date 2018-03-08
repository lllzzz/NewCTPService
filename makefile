objects = Config.o MessageService.o MessageTestHandler.o Tool.o MdSpi.o

market.o: src/market/main.cpp
	g++ -c -o market.o src/market/main.cpp

MdSpi.o: src/market/MdSpi.h src/market/MdSpi.cpp
	g++ -c src/market/MdSpi.cpp

test.o: src/market/test.cpp 
	g++ -c src/market/test.cpp

Config.o: src/common/Config.h src/common/Config.cpp
	g++ -c src/common/Config.cpp

MessageService.o: src/message/MessageService.h src/message/MessageService.cpp
	g++ -c src/message/MessageService.cpp

MessageTestHandler.o: src/message/MessageHandler.h src/message/MessageTestHandler.cpp
	g++ -c src/message/MessageTestHandler.cpp

Tool.o: src/common/Tool.h src/common/Tool.cpp
	g++ -c src/common/Tool.cpp

test: test.o $(objects)
	g++ -o bin/test test.o $(objects) -ljsoncpp -lglog -lhiredis

market: market.o $(objects)
	g++ -o bin/marketService market.o $(objects) -ljsoncpp -lglog -lhiredis -lthostmduserapi

clean:
	rm  test.o $(objects)
