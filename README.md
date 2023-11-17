# PD2023_final_project
2023 IM Program Designing Final Project

- [What](#what)
- [Architecture](#architecture)
- [Version History](#version-history)
- [Team Member](#team-member)
- [Contact](#contact)

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

- [2023/11/16](#2023/11/16)
- [2023/11/17.a](#2023/11/17.a)

  ## 2023/11/16
    Construct the repository
    Create "main.cpp", "block.h", "table.h"
  ## 2023/11/17.a
    Create position returning function of "block.h"
    Change our block data container from STL to C++ basic container (int/short...)

## Team Member
  趙子佾 資管一 B12705064
  詹詠翔 資管一 B12705063
  陳泊華 資管一 B12705014
  吳秉哲 資管一 B12705049
