# Chase of Guards and Thieves Game
![Build Status](https://img.shields.io/badge/-c-brightgreen)    ![Build Status](https://img.shields.io/badge/-shell-yellow) ![Build Status](https://img.shields.io/badge/-socket.h-lightgrey) ![Build Status](https://img.shields.io/badge/-pthread.h-lightgrey) ![Build Status](https://img.shields.io/badge/-ncurse.h-lightgrey)  


## How to launch Game

### 1.Installation
```sh
cd LaunchGame
make -f makeserver.mk
make -f makepolice.mk
make -f makethief.mk
```
### 2.Launch Game
```sh
./serv <PORT> // Execute server
./thiefclient <SERVER IP> <PORT> // For thief role, execute thiefclient
./policeclient <SERVER IP> <PORT> // For plolice role, execute policeclient
```
## About Game
### 1. Rule of Game
### 2. About Map
| ![map-of-thief](https://user-images.githubusercontent.com/42794463/132094185-b79f7f4c-855a-4eee-8b77-b7ad1554b23e.png) | |
### 3. About Mission
If thief client meets M while moving, the thief can do the mission. There are 3 types of missions, and the total number of missions in the game is 6. If the thief gets 10 points, he/she wins!
| Mission | Score |
| ------ | ------ |
| Four Operation(사칙연산) | 2 score |
| Rock Scissor Paper(가위바위보) | 3 score |
| A Brave Ninza (표창피하기) | 5 score |

- Four Operation (사칙연산)
  - The thief client wins if he/she enters the correct answer according to the given random arithmetic expression.
    
- Rock Scissor Papaer (가위바위보)
  - Move ROCK, SCISSOR, PAPER with ‘a’, ‘d’ arrow keys and press ‘e’ to start the mission. A random number is generated against the thief's input to determine server's rock-paper-scissors hand. If the match between the server and the thief is a draw, start over.
- A Brave Ninza (표창피하기)
  - The thief client's current location is marked with an 'o' . Weapons are marked with an 'x' to move. The 'x' weapons move in the left direction. The thief client('o') must reach the FINISH LINE without touching the weapon('x') 
## Tech


## Development

