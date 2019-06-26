# Snake-Game
A snake game using C language and SDL2. Works on Ubuntu Platform. 
## Getting Started
After cloning this project on your linux PC, go to folder and open in terminal. Just a few things to do before you can play this amazing game!

***Some variables like starting speed of snake, initial position of snake may be changed by changing respective macros in `src/snake.h`***

### Prerequisites
This project required a C compiler (gcc prefered) and libraries like SDL2, SDL2_ttf installed on device. Make sure you have them.
simply running Install.sh would install all the required libraries. Run

`bash Install.sh`

Type password if asked for and press enter.

Or, to install individually if you already have some of them- first, to install gcc, type

```
sudo apt-get install gcc
```

Then to install SDL2 and SDL2_ttf

```
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
```

### Installing
To install the game, run make.sh 

`bash make.sh`

### Running

Finally, to start the game, run 

`./snake`

You can know more about the command line options to be given by `-h` or `--help` 

`./snake -h`

`./snake --help`

### License
This project is licensed under GPL-3.0. See LICENSE for details.
