# Asteroids
&emsp; This is an Arduino matrix game made as part of the Robotics course at University oF Bucharest, Faculty of Mathematics and Computer Science. It is inspired by the popular and beloved arcade game [Asteroids](https://en.wikipedia.org/wiki/Asteroids_(video_game)) made by Atari in 1979. In addition to the very well known mechanics there are some twists described below.

### Game description 
&emsp; Being a spaceship stranded in the vast space (represented in the game as a blinking dot), your goal is to cross an asteroid belt unharmed. Of course, that would have been a little bit too boring for this age and time, so the game has available 15 levels to play through, all procedural generated and increasing in difficulty as you play through. Each level last 20 seconds and is your goal to outlast it and get all the score points. As you go through levels, the asteroids move faster and are increasingly more of them at any given time, but in return, each second you live, you get the current level number of points. <br>
&emsp; At level 5, the notion of power-ups is introduced (represented in the game as a sparingly blinking dot). The first bonus that you can get from power-ups are additional lives, so you can hit some asteroids afterwards. Closely after, at level 10, another power-up is introduced, DESTROY ALL ASTEROIDS, so you can get a clean view of the space. Once you hit level 10, there is a 50-50 chance to get either of the power-ups, so don't worry if you get the wrong one for your situation, that was not intended.

### How to play
&emsp; You are offered a joystick with which you can move your spaceship on the 8x8 matrix. There is not much to it, you just have to be warry of the direction the asteroids move and dodge them. A nice bonus for player preference is that with the press of the joystick button, you can change your spaceship blinking state to one in which the dot is always on. One of the most important thing is to collect power-ups by moving your spaceship on top of it, unless you are an achievement hunter. Sorry there are none of those in here. 

### Menu
&emsp; If you want to know more about the game, tweak some things to make it your own, or just to brag to your friends about your highscore, a menu is required, so naturally, this game has one too. Divided in two (Main menu and Settings menu), it comes like this:
- Main Menu
  - Start game : here you can press the button to do what you came for
  - Highscores : only the top 3 scores are listed here alongside the name of player which scored them
  - Settings : this is the door to the other menu, drop your eyes below
  - About : a friendly reminder of the game title, it's developer name and this repo's link

- Settings menu 
  - Name : so you can be listed on the highscore board, you can enter or change your player name here
  - Level : if you find the first 14 or whatever levels boring, there's no need to play them all, just get straight to the point and play from your desired level.
  - Player speed : from 1 to 10, adjust it to your preference
  - Contrast : currently unavailable, check the next release
  - LCD Brightness : on a scale from 1 to 100, select the brightness of the lcd you are comfortable with
  - Game Brightness : between 1 and 15, you can lighten up your spaceship, and asteroids
  - Sound : turn it on or off
  - Reset highscore : if you really want to

### Code architecture
&emsp; The arhitecture is based on OOP principles. At it's core, the game has 13 states that come from playing the game, every setting, highscore, about section and menus. They are: ```MainMenu``` , ```Game```, ```Highscores```, ```SettingsMenu```, ```About```, ```EditName```, ```EditLevel```, ```EditPlayerSpeed```, ```EditContrast```, ```EditLCDBrightness```, ```EditGameBrightness```, ```EditSound```.

&emsp; The main file, Asteroids.ino, decides what actions to do based on the state. It is composed of a Joystick, Game and Menu instance, alongside others. It also listens to joystick movements to change the state. 

&emsp; The Joystick, Menu, Settings, Sound and Game classes are singletons and the first one just reads and checks the joystick inputs.

&emsp; The Menu class is responsible to display both Menu's options (2 at each given moment), which are:  
* ```Start game```
* ```Highscore```
* ```Settings```
* ```About``` <br>
for the Main menu and 
* ```Name```
* ```Level```
* ```Player speed```
* ```Contrast```
* ```LCD brightness```
* ```Game brightness```
* ```Sound```
* ```Reset highscore```
* ```Back``` <br>
for the Settings Menu, and highlight a ```selectingOption``` option. To change the selected option, Asteroids.ino listens to joystick inputs and increases or decreases this option with the help of ```increaseSelectingOption()``` and ```decreaseSelectingOption()``` methods of the Menu class. Following the same logic, once the joystick button is pressed, the state changes according to the Menu option selected.

&emsp; The ```Settings``` class holds the logic to change and save every setting that is available.

&emsp; The ```Sound``` class is used to play sounds, considering the mute option.

&emsp; The ```Game``` class holds all the game logic and achieves it's objectives with the help of ```startLevel()```, ```endLevel()```, ```endGame()``` and ```playGame()``` methods, last of which is called from the main file while the state is ```Game```. It uses a 8x8 ```displayMatrix``` matrix that contains each led's state of the physical matrix and also holds a linked list of Asteroid objects.

&emsp; The ```Asteroid``` class is used to generate Asteroid objects that have a spawn position at the edge of the matrix and a direction such that it travels at least a square inside the amtrix.

&emsp; The projects also holds 2 utils files: ```LCDUtils``` and ```EEPROMUtils``` that contains functions that help with the LCD displays and setup and EEPROM acces and retrieving respectively. And not least, the ```Const``` file that holds all the constants of the game shared with the other files.

### Components
&emsp; Down to the real world, the game is made from one of each of these hardware components:

- Arduino Uno
- joystick
- 8x8 LED Matrix
- 16x2 LCD
- buzzer
- potentiometer
- 74HC595 shift register

&emsp; And picture so you can check. 

![Setup](Setup/Setup.jpg)

### Demo

&emsp; And last, but not least, you can find out more and verify or invalidate anything written above in this [video](https://youtu.be/0AXGnT_RPt0). Enjoy!

