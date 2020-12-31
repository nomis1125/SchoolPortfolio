Simon Walker
CSc 473 Assignment 3

N O T E: In order to run this program you must type 'part1' first.

Problems:
Pressing 's' to resume simulation breaks.
Running simulation without a spline breaks.
Animation is slow.
Does not loop.

From the Tasks:
1 + 2: I was able to create the scene for a classroom included is the wall, blackboard and floor. Each has its own distinct color.
3: The simulation can read in a spline and display it on the black board.
4: There is a human character displayed using ellipses. Though this character is unable to move following the spline. Was unable to have the ellipses move correctly so implemented a stick arm for the animation.
5: Tried to implement the Inverse Kinematics(IK). Tried to do the other two as I could not fix the IK causing the arm to occasionally dissapear and or stop moving. But arm does seem to move in reasonable motion when it does.
6: The arm movement does draw the spline on the board. Sadly the arm does break and the speed is slow, even when increasing maxSP which speeds up the animation.
7. When the spline loads the character does not move to the starting position until the 's' key is pressed. Then it will move to the start position and begin to trace the spline.
8. -I am using the left instead of right hand but the hand that is present is the endpoint of the arm.
   -The following commands run:
	> simulator iksim read spline.txt
	> system bob position < x y z>
	Note that position moves the ellipses, not the arm. Was unable to compelte this.
   -Systems are namede iksim and bob
   -Trajectory is computed for the spline when the characters moves from rest position

Following commands are used for practice from the file start2.txt*:

> part1
> simulator iksim read animation2.txt

*start2.txt is the file that this code uses.