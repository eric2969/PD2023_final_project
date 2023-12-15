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
We would like to use socket as our api to transmit enemy player data by TCP protocol
We about to use some techniques like File I/O, Polymorphism, Operator Overloading, Inheritance, Exception Handling, etc...
And we divided our game architecture into several parts, listed at the below.
  - [Main](#main)
  - [Table](#table)
  - [Block](#block)
  - [Socket](#socket)

## Version History
> ### 2023/12/16.a (v2.0beta)
> Rebuild game architecture by encapsulate game playing section <br>
> Added singlePlayer and multiPlayer func. <br>
> Added game_init/exit, record_update/reset, conf setting func. <br>
> Finishing the part of record playing data and settings (record reset, conf setting, etc...) <br>
> Create bright/dark mode

> ### 2023/12/15.d (v1.5.1beta)
> Fixed the bug that blocks can sometimes pass through each other <br>
> Added the function "single_player()"
> Trying to add a new menu

> ### 2023/12/15.c (v1.5)
> Refine socket server-client mechanism

> ### 2023/12/15.b (v1.4)
> Complete block keep (need to complete keep block display)

> ### 2023/12/15.a (v1.3)
> Fix some bugs <br>
> Complete game over and verify every Block's rotation <br>
> Modify our introduction <br>
> Create game menu (when game is over) (can be better)

> ### 2023/12/14.g
> Add server.h v1
> Add client.h v1

> ### 2023/12/14.f (v1.2.2)
> Fixed delta Position of Block_I rotation

> ### 2023/12/14.e (v1.2.2beta)
> Fixed display issue by adding Block *before <br>
> Added SetFont() <br>
> Modified print_block()

> ### 2023/12/14.d (v1.2.1beta)
> Fixed Block_I rotation (still need to implement delta in x/y axis)

> ### 2023/12/14.c (v1.2)
> Fix bugs <br>
> Improve color displaying (clear column)

> ### 2023/12/14.b (v1.1beta)
> finished hard_drop fix_line and line_clear

> ### 2023/12/14.a (v1.0)
> Fix some major issue in game<br>
> Fix isValid bug<br>
> Fix rotating bug<br>
> changed y-axis direction<br>
> Change block verifying in table.h<br>
> this version has completed some major function in tetris

> ### 2023/12/13.b
> Fixed the copy issue by adding clone<br>
> Added createKickTable <br>
> Changed some Block into Block* in order to fix block-copy issue

> ### 2023/12/13.a
> Fixed some bugs

> ### 2023/12/12.g
> finished key timer

> ### 2023/12/12.f
> finished remove line

> ### 2023/12/12.e
> add_shuffle_block<br>
> Finished hard_drop

> ### 2023/12/12.d
> Fixed conflict

> ### 2023/12/12.c
> turn current's type from block to block_pointer <br>
> fixed block printing <br>
> XDTurtleXD is horny <br>

> ### 2023/12/12.b
> Rearrange main function <br>
> Set randomizing block <br>
> Turn 'id' into 'board' <br>

> ### 2023/12/12.a
> Temporary merge <br>
> Fix move_block <br>

> ### 2023/12/11.a
> Finish all the initialize of seven blocks(using the "location" point)
> 
> ### 2023/12/07.a
> Add I block kick table <br>
> Add I block rotate_set function (still working on) <br>
> Modify rotate_set function <br>
> Add rotating detection in main

> ### 2023/12/01.b
> Add key press detector <br>
> print_block is yet to be fixed

> ### 2023/12/01.a
> Simplify the basic data of block (symbol, color => id)

> ### 2023/11/30.d
> Change the architecture of block and table

> ### 2023/11/30.c
> Small modify in block.h

> ### 2023/11/30.b
> Add symbol matrix in table.h, complete some functions in table.h <br>
> modify flush_tick, cols lines, etc... in main.cpp

> ### 2023/11/30.a (v0.0)
> Fix print_table()

> ### 2023/11/29.e
> Fix conflict <br>
> modify constructor

> ### 2023/11/29.d
> Add some foundmental constuctor to the different type block,
> the initialize is not done yet

> ### 2023/11/29.c (there is a typo "2023/11/28.c")
> Debug block.h and creating kick table and implement to rotation <br>
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
> Debug table.h and block.h <br>
> Add some abstract function to make it more completed <br>
> Work has been distributed <br>

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
> Create position returning function of "block.h" <br>
> Change our block data container from STL to C++ basic container (int/short...)

> ### 2023/11/16
> Construct the repository <br>
> Create "main.cpp", "block.h", "table.h"
