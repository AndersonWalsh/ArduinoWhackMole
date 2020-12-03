/*Author: Anderson Walsh
Arduino Whack-a-Mole
LEDs are moles, click the corresponding button for a point
Program starts by prompting user for starting blink speed
and desired game time. On a hit, blink speed will increase
half of the way down to a limit of 1/2 a second. On a miss,
Blink speed will decrease 1/4 of the way up to 3 seconds.
Audio feedback for hits and misses.
*/

//global variables for pins, score tracking, speed tracking,
//time tracking, and ensuring the same mole is not picked twice
const int moles[] = {8, 9, 10, 11, 12};
const int buttons[] = {2, 3, 4, 5, 6};
const int tone_pin = 13;
int rand_mole = 0;
int prev_rand = 0;
int user_speed = 0;
int user_score = 0;
int prev_score = 0;
unsigned long game_time = 0;
unsigned long prevMill = 0;
unsigned long curMill = 0;

//precondition: rand_mole previously initialized, Serial.begin() called
void gameInit()
{
    rand_mole = random(5); //random indexing # between 0-4
    Serial.print("Welcome to Arduino Whack-a-Mole!\nClick the button under each LED as they light up for points.\n");
    Serial.print("Moles will speed up with hits, and slow down with misses.\n");
} //postcondition: initial mole set, game explained to user via serial output

//precondition: Serial.begin() called, user_speed and game_time previously initialized
void setSpeedTime()
{
    Serial.println("Provide how long you want moles to remain whackable, to start, in MS (shorter is harder): ");
    while (user_speed == 0)
    {
        user_speed = Serial.parseInt(); //loop to bypass the once per second read of "0" from pareInt() by default
    }
    Serial.println("Provide how long you want your game to last in seconds: ");
    while (game_time == 0)
    {
        game_time = Serial.parseInt();
    }
    game_time *= 1000;     //asking for input in seconds, arduino time works in MS
    game_time += millis(); //compensate for time spent inputting game values
} //postcondition: starting speed and game time is set by user, game_time is converted to MS

//preconditionL global variables for pin #s initialized, serial port available, called functions defined
void setup()
{
    Serial.begin(9600);        //open dataline for serial I/O
    randomSeed(analogRead(0)); //random numbers seeded from noise on pin 0
    //there should always be a button for each output
    for (int i = 0; i < (sizeof(moles) / sizeof(int)); i++)
    {
        pinMode(moles[i], OUTPUT);
        pinMode(buttons[i], INPUT);
    }
    gameInit();
    setSpeedTime();
} //postcondition: pin modes set, initial game explanation plus user set value functions called, Serial port open

//precondition: game has been played, user_score and user_speed appropriately updated
void gameEnd()
{
    Serial.print("Thanks for playing!\nYour final score: ");
    Serial.print(user_score);
    Serial.print("\nYour final speed: ");
    Serial.print(user_speed);
    Serial.print("\nReset Arduino to play again!");
    while (1)
        ; //could use exit(0), while this does work the function is not intended for use on Arduino
} //postcondition: function creates infinite loop to "quit", never ends

//precondition: user_speed previously defined, pin available for tone
void incSpeed()
{
    if (user_speed > 500)
    {
        user_speed -= (user_speed - 500) / 2; //make moles blink twice as fast up to 1/2 a second
    }
    Serial.print("Hit, new speed: ");
    Serial.println(user_speed);
    tone(tone_pin, 1000, 300); //high pitch sound for score
} //postcondition: if user_speed is greater than half a second, cut the difference in half and give + audio feedback

//precondition: user_speed previously defined, pin available for tone
void decSpeed()
{
    if (user_speed < 3000)
    {
        user_speed += (3000 - user_speed) / 4; //make moles blink 25% slower up to 3 seconds
    }
    Serial.print("Miss, new speed: ");
    Serial.println(user_speed);
    tone(tone_pin, 100, 300); //low pitch sound for miss
} //postconditionL if user_speed is less than 3 seconds, increase user_speed towards 3s by 25% and give - audio feedback

//precondition: prev_rand and rand_mole previously initialized and given valid data
void moleSelect()
{
    prev_rand = rand_mole;
    rand_mole = random(5); //random indexing # between 0-5
    //if our new random number isn't so random, either go up a mole if we were at mole 0, down a mole if we were at mole 4
    //or, 50/50 chance to either go up/down 1 if we aren't in danger of trying to address a nonexistent mole
    if (prev_rand == rand_mole)
    {
        switch (rand_mole)
        {
        case 0:
            rand_mole += 1;
            break;
        case 4:
            rand_mole -= 1;
            break;
        default:
            if (random(2))
                rand_mole += 1;
            else
                rand_mole -= 1;
            break;
        }
    }
} //postcondition: new random mole, unique from previously selected mole, chosen

//precondition: none
void wait()
{
    delay(random(900) + 100);
} //postcondition: program pauses for random period between 1/10-1 second between mole appearances

//game loop
void loop()
{
    curMill = millis(); //have to find curMill outside of while and in while because of call to wait() at end of loop
    if (game_time < curMill)
    {
        gameEnd(); //if our current time has surpassed user set game time, it's all over
    }
    prev_score = user_score; //detect whether user hit or missed by checking if score has changed at end of loop, for speed change
    prevMill = curMill;
    while (curMill - prevMill <= user_speed)
    { //check for button press up until user_speed time has passed
        curMill = millis();
        digitalWrite(moles[rand_mole], HIGH);
        if (digitalRead(buttons[rand_mole]) == HIGH)
        { //on a hit, increase score and speed
            user_score += 1;
            Serial.print("Score: ");
            Serial.println(user_score);
            incSpeed();
            break;
        }
    }
    if (user_score == prev_score)
    { //on a miss, decrease speed
        decSpeed();
    }
    digitalWrite(moles[rand_mole], LOW);
    moleSelect();
    wait();
}