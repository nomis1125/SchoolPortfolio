Simon Walker
CSc 473 Assignment 2

VISUAL NOTE: You will need to zoom out to see the animations.

I have rewritten my emitter.cpp/.h and ballobj.cpp/.h for this assignment.

ballobj.cpp is where the particles are created and stored.
This is where the commands dim, particle and all_velocities are located within ballobj::command().

emitter.cpp is where the simulation occurs.
Here are the commands link, spring, fix, integration, ground, gravity and drag are located.

I was able to accurately represent the Forward Euler and Symplectic Euler but was unable to get the verlet to work.
Within I have the spring loop which searching through the saveSpr array which holds all the information for every spring.
I was unable to get this spring system to work as when running the sample_spring and simple_cube I got unreliable results.
The cube_complex and cube_complex_full were not runnable as they reacted poorly to my spring equations.
I have assumed this was due to there being a lack of loss of force when it connects, but I was unable to fix this problem.

Otherwise, when running the one_particle and many_particles, they reacted as expected for bother Forward and Symplectic Euler.

Also I was unable to draw the dots and lines together accuracy so am only able to draw lines.