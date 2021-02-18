# Breakdown

Breakdown is a game played between two players. Each player takes turns vertically or horizontally dropping a stick of their color onto the board. As an added element, either player can use their turn to breakdown the board. This causes the sticks to 'break apart', dropping every element to the bottom of the board. When a player successfully makes a square of the their color, they are declared the winner.

### Overview

This is a simple game of breakdown designed to be played from the command line. Run play.c from the command line. You can also include parameters -m or -b to use either a matrix or bits representation of the board in memory. You also must supply the height (-h), width (-w), the length of stick you're dropping (-k), and the size of the winning square (-q).

Each turn, the game will first ask for a move. Input "-" for a horizontal drop, "|" for a vertical drop, or "!" to breakdown. If you input the former two, the game will then ask for the column number were you want to drop your stick. Have fun!
