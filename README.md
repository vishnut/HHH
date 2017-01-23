# HHHR

This is all the code behind the HHH robot at Robotathon 2015. 

Dependencies include OpenOCD, LM4F Cross Compilers, and StellarisWare. For documentation on Rasware, visit the [wiki](https://github.com/ut-ras/Rasware/wiki).

## The Competition

Two robots would be placed on a hexagonal field. The robots would have to autonomously navigate around the field, pick up any balls on the field, sort based on size, and score the balls in the goals. The larger balls would go into the goal on the opponent's side and the smaller ones would go into the goals on your side. The motivation behind this competition is to give new members an introduction to robotics. I had a great time competiting and the following year, I was a mentor and I helped to organize the competition.

## The Code

The robot can wall follow, line follow (and edge follow), and do a number of cool tricks. Some of the code written close to competition day has been lost. So, a few methods may seem incomplete or messy. For example, when switching from our prototype to the final robot, we added flaps at the back so that the robot could score. Those flaps were connected to a non-continous servo and the code for that was only written/tested a few hours before competition due to problems with the servo and delays in getting the final hardware ready. TODO: Improve time management :) 

## The Team

I worked on this bot for two months during my first semester at UT with a team of 5. We had many meetings every week where we would check on progress and design both the appearance and functionality. Although all of us had a part in every aspect, I primarily wrote the software for the robot while some in the team worked on wiring and soldering, and others built the robot using Lexan, wood, ABS plastic (3D printed), aluminum, etc.
