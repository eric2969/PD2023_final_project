# PD2023_final_project
2023 IM Program Designing Final Project

- [What](#what)
- [Team Member](#team-member)
- [Architecture](#architecture)
- [Version History](#version-history)

## What

Our project is to build a tetris game by "windows.h", allowing playing in double-player or single-player mode.

## Team Member
&nbsp; 趙子佾 資管一 B12705064 <br>
&nbsp; 詹詠翔 資管一 B12705063 <br>
&nbsp; 陳泊華 資管一 B12705014 <br>
&nbsp; 吳秉哲 資管一 B12705049 <br>

## Architecture

Our game is based on cpp, and interface is powered by "windows.h".
We would like to use socket as our api to transmit enemy player data by UDP protocol
And we divided our game architecture into several parts, listed at the below.
  - [Main](#main)
  - [Table](#table)
  - [Block](#block)
  - [Socket](#socket)

## Version History

> ### 2023/12/12.a
> Temporary merge
> Fix move_block

> ### 2023/12/11.a
> Finish all the initialize of seven blocks(using the "location" point)
> 
> ### 2023/12/07.a
> Add I block kick table
> Add I block rotate_set function (still working on)
> Modify rotate_set function
> Add rotating detection in main

> ### 2023/12/01.b
> Add key press detector
> print_block is yet to be fixed

> ### 2023/12/01.a
> Simplify the basic data of block (symbol, color => id)

> ### 2023/11/30.d
> Change the architecture of block and table

> ### 2023/11/30.c
> Small modify in block.h

> ### 2023/11/30.b
> Add symbol matrix in table.h, complete some functions in table.h
> modify flush_tick, cols lines, etc... in main.cpp

> ### 2023/11/30.a (v0.0)
> Fix print_table()

> ### 2023/11/29.e
> Fix conflict
> modify constructor

> ### 2023/11/29.d
> Add some foundmental constuctor to the different type block,
> the initialize is not done yet

> ### 2023/11/29.c (there is a typo "2023/11/28.c")
> Debug block.h and creating kick table and implement to rotation
> Merge conflict

> ### 2023/11/29.b
> Debug table.h

> ### 2023/11/29.a (v0.0beta)
> Implement print_table aand print_block in table.h

> ### 2023/11/28.e
> Debug table.h and block.h

> ### 2023/11/28.d
> Change class name to upper case

> ### 2023/11/28.c
> Debug table.h and add some detail in table.h

> ### 2023/11/28.b
> Create some functions in block

> ### 2023/11/28.a
> Debug table.h and block.h
> Add some abstract function to make it more completed
> Work has been distributed

> ### 2023/11/24.a
> Debug table.h

> ### 2023/11/23.g
> Reorder the README

> ### 2023/11/23.f
> Create some functions which involve in moving or rotating blocks in table.h

> ### 2023/11/23.e
> Create every specific shape block class  which inherits general block class

> ### 2023/11/23.d
> Create some functions which control the existence of block in table.h

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
