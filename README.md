# ArduinoWhackMole
This is Arduino Whack-a-Mole. The bulk of the circuit beyond resistors, wires and such is 5 LEDs, 5 buttons, and a piezoelectric speaker. 
The LEDs function as “moles”, with a button corresponding to each individual mole. The LEDs light up randomly. 
The user must press the button corresponding to the lit mole before the time limit elapses (and the mole “disappears”). 
At the beginning of the game, the user is given instructions and prompted to provide a starting duration for the moles’ flashing in MS, in addition to a 
duration for the game overall in S via serial input. From the starting mole speed, each hit (signaled by serial output and a high tone from the speaker) 
will increase the speed of the mole by 1/2 the difference between the current speed and half a second, the maximum mole speed. 
Each miss (signaled by serial output and a low tone from the speaker) will decrease the speed of the mole by 1/4 the difference 
between the slowest speed of 3 seconds and the current speed. The user’s total score is tracked, and after the user-defined game time concludes, 
final score and mole speed is outputted. The Arduino prompts the user to reset for additional games if desired, and enters an infinite loop. 
This is the most effective means to “quit” a program on these microcontrollers.
