common = Config.o Tool.o Cache.o MessageSender.o ShareData.o
trade = MessageService.o MessageProcesser.o \
		MessageNormalTradeProcesser.o MessageNormalTradeHandler.o \
		MessageCancelProcesser.o MessageCancelHandler.o \
		MessageFAKProcesser.o MessageFAKHandler.o \
		MessageFOKProcesser.o MessageFOKHandler.o \
		MessageIOCProcesser.o MessageIOCHandler.o


market.o: src/market/main.cpp
	g++ -c -o market.o src/market/main.cpp

trade.o: src/trade/main.cpp
	g++ -c -o trade.o src/trade/main.cpp

TdSpi.o: src/trade/TdSpi.h src/trade/TdSpi.cpp
	g++ -c src/trade/TdSpi.cpp

MdSpi.o: src/market/MdSpi.h src/market/MdSpi.cpp
	g++ -c src/market/MdSpi.cpp



Config.o: src/common/Config.h src/common/Config.cpp
	g++ -c src/common/Config.cpp

Cache.o: src/common/Cache.h src/common/Cache.cpp
	g++ -c src/common/Cache.cpp

Tool.o: src/common/Tool.h src/common/Tool.cpp
	g++ -c src/common/Tool.cpp

ShareData.o: src/market/ShareData.h src/market/ShareData.cpp
	g++ -c src/market/ShareData.cpp



MessageService.o: src/message/MessageService.h src/message/MessageService.cpp
	g++ -c src/message/MessageService.cpp

MessageSender.o: src/message/MessageSender.h src/message/MessageSender.cpp
	g++ -c src/message/MessageSender.cpp

MessageProcesser.o: src/message/MessageProcesser.h src/message/MessageProcesser.cpp
	g++ -c src/message/MessageProcesser.cpp

MessageNormalTradeHandler.o: src/message/MessageHandler.h src/message/normal_trade/MessageNormalTradeHandler.cpp
	g++ -c src/message/normal_trade/MessageNormalTradeHandler.cpp

MessageNormalTradeProcesser.o: src/message/MessageProcesser.h src/message/normal_trade/MessageNormalTradeProcesser.cpp
	g++ -c src/message/normal_trade/MessageNormalTradeProcesser.cpp

MessageCancelHandler.o: src/message/MessageHandler.h src/message/cancel/MessageCancelHandler.cpp
	g++ -c src/message/cancel/MessageCancelHandler.cpp

MessageCancelProcesser.o: src/message/MessageProcesser.h src/message/cancel/MessageCancelProcesser.cpp
	g++ -c src/message/cancel/MessageCancelProcesser.cpp

MessageFAKHandler.o: src/message/MessageHandler.h src/message/fak/MessageFAKHandler.cpp
	g++ -c src/message/fak/MessageFAKHandler.cpp

MessageFAKProcesser.o: src/message/MessageProcesser.h src/message/fak/MessageFAKProcesser.cpp
	g++ -c src/message/fak/MessageFAKProcesser.cpp

MessageFOKHandler.o: src/message/MessageHandler.h src/message/fok/MessageFOKHandler.cpp
	g++ -c src/message/fok/MessageFOKHandler.cpp

MessageFOKProcesser.o: src/message/MessageProcesser.h src/message/fok/MessageFOKProcesser.cpp
	g++ -c src/message/fok/MessageFOKProcesser.cpp

MessageIOCHandler.o: src/message/MessageHandler.h src/message/ioc/MessageIOCHandler.cpp
	g++ -c src/message/ioc/MessageIOCHandler.cpp

MessageIOCProcesser.o: src/message/MessageProcesser.h src/message/ioc/MessageIOCProcesser.cpp
	g++ -c src/message/ioc/MessageIOCProcesser.cpp

market: market.o MdSpi.o $(common)
	g++ -o bin/marketService market.o MdSpi.o $(common) -ljsoncpp -lglog -lhiredis -lthostmduserapi

trade: trade.o TdSpi.o $(common) $(trade)
	g++ -o bin/tradeService trade.o TdSpi.o $(common) $(trade) -ljsoncpp -lglog -lhiredis -lthosttraderapi

clean:
	rm *.o
