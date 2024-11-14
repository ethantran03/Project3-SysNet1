# Project3-SysNet1

## Overview

This project simulates a scenario where a herd of lizards must cross a driveway to get to their food, while avoiding being caught by two cats that monitor the driveway. The goal is to synchronize the lizards’ movements across the driveway, ensuring that no more than a safe number of lizards cross at a time and that they are not caught by the cats. The program uses multi-threading and synchronization primitives such as mutexes and semaphores to achieve this.

### Problem Description

The simulation consists of:

Lizard Threads: Each thread represents a hungry lizard that will randomly wake up, attempt to cross the driveway to the monkey grass (the food source), eat, and return home. The lizard must safely cross the driveway without exceeding the maximum number of lizards allowed at one time to avoid detection by the cats.

Cat Threads: Two cat threads periodically check for too many lizards crossing the driveway. If they detect an unsafe number of lizards, they will terminate the simulation.

#### How to Run 

To run the file you need to type in the terminal "make" to create lizards.o and lizards.

From there you can run the file with "./lizards" to run the simulation until the cat terminate the program when too many lizard are on the driveway.