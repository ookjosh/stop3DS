+++
title = "Application State"
+++

## Overview

Handling application state is an important and well discussed topic. Now I get to see what works well
for this application.

## Stop3DS' State

What does this application have to track on a high level? Things such as current animation, current
scene, current frame all seem like fair game. But why should knowledge of the current frame be anywhere 
outside of the Animation construct? Why should it be outside of the current Scene construct? To explore 
these questions let's break down the application.

### Top level

Application Loop
User Input
Rendering
Animation state

The application loop and associated bits are what I would consider the "top level" of state hierarchy. 
Everything else depends on the loop existing in the first place and any data being passed through it.

User input presents an interesting problem because it is important to abstract specific keycodes and 
such from absolute events. Keeping things generic and flexible will help keep the code portable but also
much more maintainable.

Rendering the still frames is also an interesting case when it comes to state because there are many ways
that it is best to keep the rendering itself stateless and just have it functionally act on incoming data.

Animation state is an important one to figure out because that is a large part of how the user input is 
going to interact with the application - starting/stopping, creating new frames/scenes/etc, touch input.
Having a good plan for how this will be handled will help keep everything else in check and simpler to 
design.

I think it's important to distinguish between "Rendering" a frame and the user editing a frame. I believe
both should take on a functional aspect (in order to easily support undo/redo, etc) and also to consolidate
data and associated state. The rendering process has no need to handle user input, but editing frames certainly
does.


### User input

I wonder if I could use a registration type setup for handling inputs. Each button would have a way to pass a 
function that is called whenever it is pressed for instance. This is used in many other cases (like .onClick)
and would offer a way to dynamically alter a button's function without having a mess of state handling inside each
button's code. This would make contextual UI much simpler and something I could explore.

The question with this is if I can keep state from leaking from other places. I don't want the input code to somehow
become tied to the state of the current frame or something like that, but I think as long as I design the interface
correctly the chance of that is much less than some alternatives.

In pseudo code, maybe something akin to this may be useful:

```
// Some accessible thing... where does this live?
InputManager inputman;

function doThingOne/Two/Three exist in local scope.
int foo = 10; // also exists here

// I need to read up on closures/scoping/lifetimes to appropriately
// alter local state and keep it contained.
inputman.registerButtonA(function(InputEvent event) {
		switch(event) {
			case BUTTON_DOWN:
				doThingOne();
				// Will this work like I expect?
				// (print 11)
				printf("My value: %d\n", foo+1);

			case BUTTON_HELD:
				doThingToThis(passedValue);

			case BUTTON_RELEASED:
				doThingThree();

			default:

		}
	});

```

Then whenever the application changed to a different mode the UI could be updated simply, eliminating

```cpp
if (kDown & BUTTON_A) {
	switch (currentMode) {
		case MODE_A:
			<too much stuff>
		case MODE_B:
			<too much stuff>
		case MODE_C:
			<too much stuff>
		case MODE_D:
			<too much stuff>
		case MODE_E:
			<too much stuff>
	}
}
```

or something akin to that. Being able to drop in arbitrary functions sounds interesting. There is a potential problem if two
things try to assign functionality at the same time (or expect their functionality to be active) but that sounds more like 
a mistake rather than fundamental flaw.

### Animation State

I believe for my first attempt I am going to utilize a method I have used in other projects (notably the robotic platform my company created) wherein there lies a single mutable structure that defines the application's state and everything is derived from that central store. This is also the method used by Endgame in their functionally reactive UI (https://www.endgame.com/blog/technical-blog/how-we-built-our-ui), so given previous success with this method I believe it is appropriate to give it a try here. The key to making this implementation work cleanly and remain maintainable is limiting mutability of the state. In essence, part A should not be able to directly change aspects of part B directly, even if it theoretically has vision of part B's components. It is also important to have things react as a function of the state, rather than have the state be part of the function. For instance, the following pseudo-snippet shows a basic comparison:

```cpp
// Cleaner interaction
void drawFrame(Frame frm, Destination output) {
	/* Do my frame drawing routine */
	output = frm.source; // basic idea
}

// Potentially unclear and untestable
void drawFrame() {
	/* Do my frame drawing routine */
	// What is currentFrame/output and where did they come from?
	// This is brittle and tied to the state structure as it exists 
	// when the code was written.
	output = currentFrame.source;
}

```

In the first example we have context of all of the values being used thanks to the arguments passed, and we have an idea of what is going
on when we call the function - `drawFrame(currentFrame, bottomScreen)` is clear in its meaning over `drawFrame()`. Where is the frame being drawn in the second function? What frame is it? Having a function like the first eliminates situations where you would otherwise have `drawCurrentFrameOnTopScreen()` and `drawLastFrameOnBottomScreen()` or `drawCurrentFrameToBuffer()`. 

That demonstrates how we can use the central state definition to **derive** presentation from the state, which is arguably a simpler problem than limiting the mutablility of the central state by different components. As stated above, this is an incredibly important design problem that makes or breaks this setup. So in this case I am going to limit state mutation to user inputs only.

#### State mutability

This application lives to react to user inputs. It is in many ways a dumb canvas, but even if it partakes in "smart" behaviors they are always a result of the user requesting them. As such, there should be no change to internal state outside of user input. Everything else that the system needs it should be able to derive from the results of these inputs.

 