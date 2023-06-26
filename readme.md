# Bomber Duck
This is the bomber man game using sdl and some c++ feature.

## Compile and play


* to compile 
```console
> make
```


* after that  to play
```console
> make  run
```

### PLAY MODE
First of all, players can select from several play modes:

* Single Player mode with extra enemies
* MultiPlayer mode

After choose the map will loaded from file in assets folder.

### PLAYING
Player A can move using the 'W', 'A', 'S', and 'D' keys on the keyboard and can drop a bomb using the 'C' key. 
After constant second, the bomb will explode, sending a blast in a cross shape to a neighbor one block,
and player can set bomb again by default.

Similarly, Player B can move using the 'Up', 'Left', 'Down', and 'Right' keys, and can drop a bomb using the 'SLASH' key.

Players can earn points throughout the game by destroying objects:

* Breakable Wall: 5 points.
* Extra Enemy: 10 points.
* Other Player: 50 points.
* Item: 10 points.

Current points are shown in upper side of window. and best results are shown at upper left side.

### ITEMS (Bonuses)
There are some items, it is hidden under the breakblewalls:

* unlimited_blast
By default, the blast spread only extends to 1 block. However, a player with this item can spread the blast unlimitedly, meaning it will continue until it hits the object.

* Unlimited Bombs
By default, a player can only drop another bomb after the previously dropped bomb has exploded. But with this item, a player can drop bombs without any time restrictions.

* Remote control
Player can trigger the exprosion of bomb after spend bomb cycle, or if player has Unlimited Bombs, player can trigger exprosion any time for one bomb.

those two item can holds simultaniously.

When all player is destroyed, game ends.

After the end of game, the Game conclusion is shown.

In this window, the points of each player got is shown, and top 5 records from file and this game is shown.
Thus player known rank 1 to 5 or below than 5.  

In this window, Player can choose retry the game or quit the game.
If it is quit, quit the game, close window.
if it is retry, player can again choose Play mode and play. and so on.

Game objects are implement by using plymorphism.
