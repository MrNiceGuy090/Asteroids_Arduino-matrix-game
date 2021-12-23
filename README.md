# Asteroids
This is an Arduino matrix game made as part of the Robotics course at University oF Bucharest, Faculty of Mathematics and Computer Science. It is inspired by the popular and beloved arcade game [Asteroids](https://en.wikipedia.org/wiki/Asteroids_(video_game)) made by Atari in 1979. In addition to the very well known mechanics there are some twists described below.

### Game description 
Being a spaceship stranded in the vast space (represented in the game as a blinking dot), your goal is to cross an asteroid belt unharmed. Of course, that would have been a little bit too boring for this age and time, so the game has available 15 levels to play through, all procedural generated and increasing in difficulty as you play through. Each level last 20 seconds and is your goal to outlast it and get all the score points. As you go through levels, the asteroids move faster and are increasingly more of them at any given time, but in return, each second you live, you get the current level number of points. At level 5, the notion of power-ups is introduced (represented in the game as a sparingly blinking dot). The first bonus that you can get from power-ups are additional lives, so you can hit some asteroids afterwards. Closely after, at level 10, another power-up is introduced, DESTROY ALL ASTEROIDS, so you can get a clean view of the space. Once you hit level 10, there is a 50-50 chance to get either of the power-ups, so don't worry if you get the wrong one for your situation, that was not intended.

### How to play
You are offered a joystick with which you can move your spaceship on the 8x8 matrix. There is not much to it, you just have to be warry of the direction the asteroids move and dodge them. A nice bonus for player preference is that with the press of the joystick button, you can change your spaceship blinking state to one in which the dot is always on. One of the most important thing is to collect power-ups by moving your spaceship on top of it, unless you are an achievement hunter. Sorry there are none of those in here. 

### Menu
If you want to know more about the game, tweak some things to make it your own, or just to brag to your friends about your highscore, a menu is required, so naturally, this game has one too. Divided in two (Main menu and Settings menu), it comes like this:
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
  
### Components
Down to the real world, the game is made from one of each of these hardware components:

- Arduino Uno
- joystick
- 8x8 LED Matrix
- 16x2 LCD
- buzzer
- potentiometer
- 74HC595 shift register

And picture so you can check. 

### Demo

And last, but not least, you can check or invalidate anything written above in this video. Enjoy!

