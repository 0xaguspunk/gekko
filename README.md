# Gekko

Gekko is a stock exchange simulator that allows you to buy and sell stock options. It is a experimental C application
to try and use Hexagonal Architecture in a non object oriented language. There are three main parts in this application:
- **Dozer**: the client that connects to Gekko and allows users to buy and sell stock and check the IBEX values.
- **Gekko**: the main server of the application which controls all the stock exchange actions.
- **TumblingDice**: the server which provides all the IBEX updates to Gekko.



