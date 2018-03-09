objects = Config.o MessageService.o Tool.o MdSpi.o Cache.o

market.o: src/market/main.cpp
	g++ -c -o market.o src/market/main.cpp

MdSpi.o: src/market/MdSpi.h src/market/MdSpi.cpp
	g++ -c src/market/MdSpi.cpp

test.o: src/market/test.cpp
	g++ -c src/market/test.cpp

Config.o: src/common/Config.h src/common/Config.cpp
	g++ -c src/common/Config.cpp

Cache.o: src/common/Cache.h src/common/Cache.cpp
	g++ -c src/common/Cache.cpp

MessageService.o: src/message/MessageService.h src/message/MessageService.cpp
	g++ -c src/message/MessageService.cpp

Tool.o: src/common/Tool.h src/common/Tool.cpp
	g++ -c src/common/Tool.cpp

test: test.o $(objects)
	g++ -o bin/test test.o $(objects) -ljsoncpp -lglog -lhiredis

market: market.o $(objects)
	g++ -o bin/marketService market.o $(objects) -ljsoncpp -lglog -lhiredis -lthostmduserapi

clean:
	rm  test.o $(objects)
