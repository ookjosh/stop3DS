+++
title = "UI"
+++

## Overview

I think that decent user interfaces are extremely rare, and that user interfaces are hard to design well. My hope in this project is to design an interface that is natural for the user and presents exactly what information they need to use the app. I think that in interfaces simpler is better, and funtionality and intent should be obvious visually. A danger as a programmer is that the application is your design and as such you can navigate its idiocincracies without much thought, but others who are naive to the system are faced with a mess.

In order to combat these pitfalls my goal is to keep sight of the application's purpose at all times when designing and implementing the user interface. This is a program meant to let people draw and animate, so it should make drawing and animating simple and fun. Anything beyond that needs to be either clearly presented or removed altogether if it doesn't fall in line with the programs purpose. I hope that by focusing on the experience I will be guided to a more natural and usable interface.

### Progress

I've never designed a UI before, and as such there is a lot of testing going on from a conceptual level down to the core code. I want the application code to be flexible (as much as possible) but I'm also designing for a piece of hardware and as such I have some restrictions and freedoms that come with it. On the 3DS (old model, not XL) we have 8 buttons, a 4 way dpad, an analog pad, and a touchscreen. The newer models include 2 more shoulder buttons.

I would like the application to be ambidextrous (as I am left handed) so that may make the four face buttons (A,B,X,Y) be equivalent to the D-pad, but that is to be seen.

My general thought for UI is a big drawing screen on the bottom (the touch screen), meaning that the drawing part of the animation is very present and uncluttered. I don't want any UI elements on the bottom screen unless they are created due to some input and are easily dismissed. The top screen is where the majority of general menu-type elements will reside, as it is not useful for drawing. The top screen should also be a preview display for animations so you can edit your work and preview in real-time.


#### Menus

So far I have a basic bar menu on the top screen that has some functionality. You can select tools and the color and that's about it at the moment. There are items for setting framerate and current frame/scene but they are unimplemented otherwise.


#### Key events

To make things more flexible I looked into how to implement event driven programming with C++/stop3DS. My search led me to a simple approach in the style of Qt's "signals and slots". Essentially we can create "Signals" that we bind a function to (including lambdas which makes life much nicer) and whenever that signal is "emitted" then the function runs. The type of stuff you'd see in Node.js or others very often. It feels very natural with input in particular, so this describes my thought process on giving this a try. Aside: The reason I'm testing this is because I didn't have a very good idea of how to cleanly implement the logic trees associated with mode x and input y, so I'm hoping this will give me a chance to think it through and hopefully come to a flexible and natural solution.

My thought is to have a signal for each button that can be pressed, and to bind functionality to those buttons through the signals. I believe this should allow me to define functionality in the places where it makes sense (e.g in the menu source file instead of the input source file) as well as allow me to assign/reassign functionality at runtime. The latter part I haven't tested yet but would make different modes much easier to handle.

Signals| Note
-------|------
KEY_A  |
KEY_B  |
KEY_X  |
KEY_Y  |
KEY_UP |
KEY_DOWN |
KEY_LEFT |
KEY_RIGHT |
KEY_START |
KEY_SELECT |
KEY_R |
KEY_L |
KEY_ZR | N3DS only
KEY_ZL | N3DS only

I think to set the signals I'll have an enum of Inputs and a function like `assignFunction(Input key, function toDo)` that other
things can interface with.