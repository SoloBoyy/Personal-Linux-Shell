# LinuxShell

This is a C++ assignment that implements a linux shell. 

A shell has wide ranging ability, and is used for file navigation and general command usage. Using parsing techniques, piping, and I/O redirection, I created a makeshift linux terminal. While implementing this project, I learnt about the workings of background/zombie processes, how directory navigation works, and details on the intricaces of how a shell parses commands. I faced a lot of issues when working with a multiple piping system, espcially when it came to redirecting, but was able to overcome these issues via functions that targetted the "<" and ">" characters, and maneveured the system to account for I/O streams.

This project was part of my operating systems class, and was recently moved to my public repo.

### Start The Program

* make all
* g++ start.cpp

### Shell Commands

#### File Navigation & General Parsing

* echo 'dsfdskj'
* ls
* ls -l /sbin
* ls -l -a
* ps -aux
* ls -l ..

#### Input/Output Redirection

* ps aux > a
* grep /init < a
* grep /init < a > b

#### Single and Multiple Piping

* ls -l | grep <pattern>
* ps aux | awk '/init/{print $1}' | sort -r
* ps aux | awk '/init/{print $1}' | sort -r | awk '/ro/' |grep ro
* ps aux | awk '{print $1$11}' | sort -r | grep root
* awk '{print $1$11}'<test.txt | head -10 | head -8 | head -7| sort > output.txt
  
#### Background Processes

* sleep 20 &
* sleep 5
* sleep 10

#### Directory Processing
* cd ../../
* cd .
* cd /home/
* cd -
 
Type quit to end the program


