# ECE387_Trellis

The end goal is to build a simon says type game using an Adafruit Trellis. The game will include the trellis with blue Leds, as well as a small speaker/buzzer that produces sound when certain requirements are met.


Basic_test_3:
  Retest of functionality after changing LEDs and wiring and fixing soldering issues.
  Link: https://youtube.com/shorts/IoshY7suL4Y?feature=share
  
  
  
trellis_press_light:
  Turns toggles light on button press. Modeled after Adafruit's "TrellisTest". Used to check the board after any modifications.
  
  
  
trellis_simon_says_buzzer:
  
  Demo: https://www.youtube.com/watch?v=KBswI7jwS8Q
    
  Essentially the final product.
  The Trellis will cycle through all lights when first loaded up.
  The arduino will take 4 values from 0 - 15. These values will be the sequence the player has to remember for the game. Button repeats can happen.
  
  The game has 4 stages. Each stage will add an extra button for the player to remember. If at any point a wrong button is pressed, the trellis will show the player      failed, before changing the pattern and restarting.
  
  Upon stage success:
    The trellis will light up in a "O" pattern, before moving on to the next stage.
    
  Upon pattern success (the entire 4 stage pattern was succesfully pressed):
    The entire board will light up in ascending order
    Then the game will reset.
    
  Upon failure:
    The trellis will flash a "X" pattern twice before changing the pattern and restarting.
    
  All keys have a tone assigned to them:
    D4 to FS6, in ascending order.
    
    
  Currently not all button light up.
  This is due to a soldering issue, but I do not have a soldering iron with me to fix this.
