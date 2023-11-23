# PD2023_final_project
2023 IM Program Designing Final Project

- [What](#what)
- [Architecture](#architecture)
- [Version History](#version-history)
- [Team Member](#team-member)

## What

Our project is to build a tetris game by "windows.h", allowing playing in double-player or single-player mode.

## Architecture

Our game is based on cpp, and interface is powered by "windows.h".
We would like to use socket as our api to transmit enemy player data by UDP protocol
And we divided our game architecture into several parts, listed at the below.
  - [Main](#main)
  - [Table](#table)
  - [Block](#block)
  - [Socket](#socket)

## Version History

> ### 2023/11/23.c
> Merge test

> ### 2023/11/23.b
> Establish block::block_position

> ### 2023/11/23.a
> Modify the rotating strategy, construct point struct

> ### 2023/11/21.b
> Hi

> ### 2023/11/21.a
> Include "windows.h", add compiler optimizing pragma

> ### 2023/11/17.b
> Create gitignore

> ### 2023/11/17.a
> Create position returning function of "block.h"
> Change our block data container from STL to C++ basic container (int/short...)

> ### 2023/11/16
> Construct the repository
> Create "main.cpp", "block.h", "table.h"
  
## Team Member
&nbsp; 趙子佾 資管一 B12705064 <br>
&nbsp; 詹詠翔 資管一 B12705063 <br>
&nbsp; 陳泊華 資管一 B12705014 <br>
&nbsp; 吳秉哲 資管一 B12705049 <br>
