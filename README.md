# Alien_Annihilation_Shooting_Range
Teammembers:
Ahmed Alzubairi: Team leader/ project manager, head programmer, electrican, laser cutted targets circuit creator.
James Bollas: Programmer and electrican.
Catherine Chrin: Art Work Designer
Rami Matar: Programmer
Giza Molenaar: Fabrication (Works on physical structure)
Ahmet Karadeniz: Fabrication (Works on creating physical structure as well as CAD designing the laser gun)
Wesley Uwaezuoke: Created the laser gun circuit

My Role: This is a project I created with my team for our Art of Engineering main project. I acted as the team
leader as well as the main programmer and electrican. By this, I mean I worked on most of the code and
I, with James Bollas, worked on the wirings of the project. I also worked on the creating CAD supports for
the shooting range so that the user can actually pick up the project with less risk of the game breaking
apart. With 3D printed supports, the phsical project has more stability. I also used a laser cutter to create
the targets for the projects, in which I attached a photoresister with acrylic that I created to sense a 
change of light exposure.

The Game: This game is an alien themed 2-player shooting gallery. The game starts when one person clicks
the green button in the top corner of the game. Once it is clicked, all of the targets would be flipped up
and register the current light exposure as the normal light exposure. The light exposure is calculated through
a photoresister behind the acrylic, which is connected in the center of the targets. Once every target registers
the current light exposure, the game would start. The players are now instructed by the LED Display. 
The LED Display would tell the players to get ready and for player one to start first. 
After player one plays his round, it would display his score and queue player 2 to play. After player 2
finishes, the display would tell both users their scores and tell them which user won. If the user beats
the current high score, the high score would be updated. The way the code is created, the high score would
forever be in the Arduino's memory, thus if someone would disconnect the game from a power source, the game
still remembers the high score for future rounds. When the players play their round, targets would move up and right
through a servo motor. As the round time progresses, more targets would appear and the user would need
to shoot more targets through a CAD designed Laser gun. When the player shoots a target, the LED Display would
increment their score by one. The LED Display would also display the current time left for the their round. The user
has one minute to shoot as much targets as possible. The way the target is registered is shot is throught the photoresister
behind the target. As mentioned before, we created a photoreister behind the acrylic circle in the center of the 
target. Once the laser shoots the center (the acrylic circle of the target) the acrylic would diffuse the beam
of light into the photoresiter. Since the photoresister would since this current light exposure as much higher
than the normal light exposure it calculated at the start of the game, it would register a point for the player.
To show that the user actually managed to shoot the target, there is a piezo buzzer that would buzz whenever
a target is shot.


Easter Egg:
We also created an easter egg in the game. Whenever the player would shoot the middle right targets six times
in a row, the piezo buzzer would play the Star Wars imperial march song. Furthermore, the targets would
go up and right via servo motor matching the beat of the song, thus appearing like the targets are dancing
to the rhythm of the song.

Video and Presentation:
There are two videos demoing the game:
1. https://www.youtube.com/watch?v=iYsyEf4SHjA
2. https://www.youtube.com/watch?v=X7NUDMBleEI
