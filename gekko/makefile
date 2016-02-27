domainModel=src/Gekko/Domain/Model
infrastructure=src/Gekko/Infrastructure
applicationService=src/Gekko/Application/Service

stock=$(domainModel)/Stock/Stock
stockList=$(domainModel)/Stock/StockList
seller=$(domainModel)/Seller/Seller
sellerList=$(domainModel)/Seller/SellerList

fileGekkoStarter=$(infrastructure)/File/FileGekkoStarter
stream=$(infrastructure)/Socket/Stream
socketTdConnection=$(infrastructure)/Socket/SocketTumblingDiceConnection
gekkoServer=$(infrastructure)/Socket/GekkoServer
gekkoMessages=$(infrastructure)/View/Console/GekkoMessages
console=$(infrastructure)/View/Console/Helpers/Console
controllers=$(infrastructure)/View/Console/Controllers/Controllers

app=$(infrastructure)/View/Console/App

all: gekko
Seller.o: $(seller).c $(seller).h
	gcc -Wall -Wextra -c $(seller).c
SellerList.o: $(sellerList).c $(sellerList).h $(seller).h
	gcc -Wall -Wextra -c $(sellerList).c
Stock.o: $(stock).c $(stock).h
	gcc -Wall -Wextra -c $(stock).c
StockList.o: $(stockList).c $(stockList).h $(stock).h
	gcc -Wall -Wextra -c $(stockList).c
FileGekkoStarter.o: $(fileGekkoStarter).c $(fileGekkoStarter).h $(stock).h $(stockList).h
	gcc -Wall -Wextra -c $(fileGekkoStarter).c
Stream.o: $(stream).c $(stream).h
	gcc -Wall -Wextra -c $(stream).c
SocketTumblingDiceConnection.o: $(socketTdConnection).c $(socketTdConnection).h $(stream).h $(stock).h $(stockList).h
	gcc -Wall -Wextra -c $(socketTdConnection).c
GekkoServer.o: $(gekkoServer).c $(gekkoServer).h $(stream).h
	gcc -Wall -Wextra -c $(gekkoServer).c
Console.o: $(console).c $(console).h
	gcc -Wall -Wextra -c $(console).c
Controllers.o: $(controllers).c $(controllers).h $(stream).h $(stock).h $(stockList).h $(console).h
	gcc -Wall -Wextra -std=gnu99 -c $(controllers).c -lpthread
App.o: $(app).c $(controllers).h $(fileGekkoStarter).h $(stockList).h $(socketTdConnection).h $(gekkoMessages).h $(console).h $(gekkoServer).h
	gcc -Wall -Wextra -std=gnu99 -c $(app).c -lpthread
gekko: App.o Controllers.o Console.o SocketTumblingDiceConnection.o FileGekkoStarter.o StockList.o Stock.o Stream.o GekkoServer.o Seller.o SellerList.o
	gcc App.o Controllers.o Console.o SocketTumblingDiceConnection.o FileGekkoStarter.o StockList.o Stock.o Stream.o GekkoServer.o Seller.o SellerList.o -Wall -Wextra -std=gnu99 -lpthread -o gekko
clean:
	rm -rf *.o gekko
