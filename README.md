Coinage
=======

A puzzle game in C using SDL

Quick Start
-----------

For 64-bit linux, a gzipped tarball with precompiled binary and required
assets is provided for convenience in
[coinage/releases](https://github.com/calebsmith/coinage/releases)

I hope to at least add a precompiled build for Mac eventually.

Notes
-----

This game is mostly code complete, but needs a game menu. It mostly exists
for my own learning but is a functional (and fun) game nonetheless.

This was developed on Ubuntu, but should work on pretty much any 64-bit linux.
Follow the build instructions below if the provided tarball doesn't work
for some reason. Almost might just need to install SDL1.2.

Development Notes
-----------------

Some of this code is a bit of a mess. It isn't that bad, but it's one of
the largest programs I've tried to make from scratch in C. I definitely
hit upon the wrong architecture in a couple of places and just haven't
addressed it because the time needed isn't worth the effort.

Perhaps more importantly, the game assets are still a bit of "programmer
graphics".

Any help is greatly appreciated, but I expect this project to just be
a bit of a fun little hack for a while. I'm using it to get some practice
at C, learn SDL, and possibly learn more about how to port my games to
other platforms.

Really if/when the graphics are cleaned up, the game mostly just needs more
levels!

The maps are easy and fun to make. Just open /data/maps/floor\*.dat. Read
floor.c to see what everything corresponds to.

Dependencies
--------------

The following are what I believe to be the required packages on Ubuntu 12.04

libsdl-image1.2-dev
libsdl-mixer1.2-dev
libsdl-ttf2.0-0-dev
libsdl1.2-dev

The general idea is: sdl1.2, sdl-image, sdl-mixer, and sdl-ttf.

I do hope to upgrade to SDL2 at some point.

Build
------

To build the project, just:

    git clone git@github.com:calebsmith/coinage.git
    cd coinage
    make

To create a gzipped tarball, a "create_build.sh" script is provided.

Authors
-------
Caleb Smith
