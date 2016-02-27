domainModel=src/Dozer/Domain/Model
infrastructure=src/Dozer/Infrastructure
applicationServie=src/Dozer/Application/Service

stock=$(domainModel)/Stock/Stock
stockList= $(domainModel)/Stock/StockList
user=$(domainModel)/User/User

dozerLogin=$(applicationServie)/DozerLogin
fileDozerLogin=$(infrastructure)/File/FileDozerLogin
socketGekkoConnection=$(infrastructure)/Socket/SocketGekkoConnection
stream=$(infrastructure)/Socket/Stream
dozerConsole=$(infrastructure)/View/Console/DozerConsole
dozerMessages=$(infrastructure)/View/Console/DozerMessages
console=$(infrastructure)/View/Console/Helpers/Console
controllers=$(infrastructure)/View/Console/Controllers/Controllers

app=$(infrastructure)/View/Console/App

all: dozer
Stock.o: $(stock).c $(stock).h
	gcc -Wall -Wextra -c $(stock).c
StockList.o: $(stockList).c $(stockList).h $(stock).h
	gcc -Wall -Wextra -c $(stockList).c
User.o: $(user).c $(user).h $(stockList).h $(stock).h
	gcc -Wall -Wextra -c $(user).c
FileDozerLogin.o: $(fileDozerLogin).c $(fileDozerLogin).h $(stock).h $(user).h
	gcc -Wall -Wextra -c $(fileDozerLogin).c
Stream.o: $(stream).c $(stream).h
	gcc -Wall -Wextra -c $(stream).c
SocketGekkoConnection.o: $(socketGekkoConnection).c $(socketGekkoConnection).h $(stream).h
	gcc -Wall -Wextra -c $(socketGekkoConnection).c
Console.o: $(console).c $(console).h
	gcc -Wall -Wextra -c $(console).c
Controllers.o: $(controllers).c $(controllers).h $(stock).h $(stockList).h $(user).h $(console).h $(controllers).h
	gcc -Wall -Wextra -c $(controllers).c
DozerConsole.o: $(dozerConsole).c $(dozerConsole).h $(dozerMessages).h $(user).h $(console).h $(controllers).h
	gcc -Wall -Wextra -c $(dozerConsole).c
App.o: $(app).c $(dozerLogin).h $(fileDozerLogin).h $(user).h $(socketGekkoConnection).h $(dozerConsole).h $(dozerMessages).h $(console).h
	gcc -Wall -Wextra -c $(app).c
dozer: App.o DozerConsole.o Controllers.o Console.o SocketGekkoConnection.o FileDozerLogin.o User.o StockList.o Stock.o Stream.o
	gcc App.o DozerConsole.o Controllers.o Console.o SocketGekkoConnection.o FileDozerLogin.o User.o StockList.o Stock.o Stream.o -Wall -Wextra -o dozer
	cp dozer ../dozer-example-2/
	cp dozer ../dozer-example-3/
clean:
	rm -rf *.o dozer
