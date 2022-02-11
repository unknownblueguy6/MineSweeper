![](https://img.shields.io/aur/votes/minesweeper-cli-git?label=aur%20votes)
![](https://img.shields.io/aur/version/minesweeper-cli-git)<br/>
[![Run on Repl.it](https://repl.it/badge/github/unknownblueguy6/MineSweeper)](https://repl.it/github/unknownblueguy6/MineSweeper)
# MineSweeper

Command Line version of MineSweeper for Unix-like systems (GNU/Linux, macOS, BSD).

<p align="center">
    <img align="center" src="assets/demo.gif"></img>
</p>

## Prerequisites
1. C++11 compiler


## Installation

### Arch
```bash
$ yay -S minesweeper-cli-git
```

### Others
1. Clone the repo.

```bash
$ git clone https://github.com/unknownblueguy6/MineSweeper.git
```

2. cd to the MineSweeper folder, and then build it using the Makefile.
   
   (Change the compiler in the Makefile, if required. Default compiler is g++)
   
```bash
$ cd MineSweeper
$ make
```

3. Run it.

```bash
$ ./mine
```

## TODO:

1. Update the gif with new controls
