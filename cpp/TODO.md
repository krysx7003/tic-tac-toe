# Tic-tac-toe

## TODO

- [x] Spawn a window
- [x] Display game board (3x3 grid. Each tile should be accesible)
- [x] Display pieces (X and O)
- [x] Have two players
- [x] On click place piece
- [x] Add game rules
- [x] Add win condition
- [x] Make game playable without GUI
- [x] Start game in i3 floating mode
- [x] Override ability to resize window
- [x] Replace commandline args with config.json
- [x] Symlink ./resources to build dirs
- [x] Remove includes dir
- [x] Methods should start from uppercase
- [x] Move game logic form main to Game
- [x] Add class for drawing simple shapes (handle Vao,vertices etc..)
- [x] Move line drawing logic to
- [x] Move rect drawing logic to
- [x] Move tile related logic to Tile
- [x] Add fps counter
- [x] Yoink TetxRenderer
- [x] Make window/menu class(Custom shape rectangle)
- [x] Make button class(Custom shape rectangle, changes color when mouse is
        over it, can be clicked)
- [x] Add start menu(Start, Exit)
- [x] Add spinner class(On click expands box with buttons)
- [x] Start allows to set users(human/AI)
- [x] Add menu bar(Always active menu, displaying current player and pause menu button)
- [x] Open pause menu either by ESC or button
- [x] Add pause menu(Resume, Restart, Exit)
- [x] Add end game menu
- [ ] Split current implementation of menu to Row\_Menu and Col\_Menu
- [x] Fix draw displaying win
- [x] Make sensible defaults in menu class

## Game rules

- Each tile can contain X, O or nothing
- At the start the board is empty
- X can be placed on empty file but not on file with O
- O can be placed on empty file but not on file with X
- Players rounds alternate
- One player only places X other O

## Win condition

- 3 X's in a row (Player 1 wins)
- 3 O's in a row (Player 2 wins)
- Board is filled and previous conditions are not filled
