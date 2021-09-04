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
### 1. About Map
| Map of Thief | Map of Police |
| ------ | ------ |
| ![map-of-thief](https://user-images.githubusercontent.com/42794463/132094185-b79f7f4c-855a-4eee-8b77-b7ad1554b23e.png)  | ![map-of-police](https://user-images.githubusercontent.com/42794463/132094228-62f973a4-60aa-44bd-a245-87146d838147.png)  |
- Init state
  - The picture above is the shell screen that the thieves and police see when they start the game.
  - Thief client is marked with a 'T' in their location, and police client is marked with a 'P'.
  - Thief and Police clients start the game at random locations on the left and right sides of the map, respectively.
- Field of vision
  - For the thrill of the game, only a specific view of the map is visible.
  - The thief must complete the mission quickly because the police have a wider field of vision than the thief.
- Speed limit
  - Since the thief has to carry out a mission, the police move slower than the thief.
- Mission Gauge
  - A mission gauge bar is fixed at the right top of the screen to indicate the thief client's mission progress.
  - Whenever a thief clears a mission, the gauge goes up as much as the score of that mission.
  - The Thief's mission gauge is visible to both police and thief clients.
- Minimap
  - Although the view is limited when moving, two clients need to know the location of the entire map and mission. The entire map is fixed at the bottom right of the screen in a small size.

### 2. Rule of Game
1. The game starts when the server is running and thief and police clients connect.
2. Use the up (w), down (s), left (a), right (d) keys to change the moving direction.
3. In the case of thief client, he/she have to avoid the police and search for the mission ('M') and proceed with the mission. Police roam the map looking for thieves.
4. When the thief succeeds in a mission, a certain mission gauge for each mission is filled.
When all mission gauge bars are filled, the thief wins and the game ends.
5. If a thief enters within a certain range(3x3) of the police, the theif are arrested and the game ends with a police victory.

### 3. About Mission
If thief client meets 'M' while moving, the thief can do the mission. There are 3 types of missions, and the total number of missions in the game is 6. If the thief gets 10 points, he/she wins!
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
