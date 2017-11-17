# Planning
Need to have a decent idea of what I want to make so that good design decisions can be made. In short, here is what I'm hoping for in no particular order:

- [ ] Onion Skinning
- [ ] Large number of frames
- [ ] Framerate adjustment
- [ ] Export to file
- [ ] Load from file
- [ ] Video generator (external?)
- [ ] Palette and full color
- [ ] Pen types
- [ ] Actor/Sprite Support
- [ ] Basic splines?
- [ ] Smart animation
- [ ] Copy/paste frames and layers
- [ ] Animation Scrubbing (timeline guide?)
- [ ] Color flood
- [ ] Transparency
- [ ] Good stylus input
- [ ] Decent UX
- [ ] Frame compression
- [ ] Animation gallery
- [ ] Import from other project
- [ ] Good code structure
- [ ] Portable
- [ ] Web enabled (dedicated server)
- [ ] Scene Support? (Animation hierarchy)
- [ ] Undo/redo

## Onion Skinning
Onion skinning is really important in creating animations and is fun. I have a working basic version that displays the previous frame if it exists. Depending on a) if it is necessary and b) if it works well with UX it might be cool to be able to mark an alternate frame as the onion skin.

## Large number of frames
Given the large amount (~96MB) of ram available on the 3DS I don't want frame count to be an issue for us. Previous programs on much more constrained systems allowed for very large numbers of frames, and I think it presents some fun challenges that I've wanted to take a shot at. On the fly compression makes the most sense to begin eeking out more from the available memory, as well as allowing both paletted and full color (bitmap) modes. A full frame is 225kB, so the maximum theoretical number of uncompressed raw frames is a bit over 420. By limiting to a 565 RBG we can increase capacity by 33%, and by limiting to a 16 color palette we can store a raw frame in less than 38kB.

## Frame Compression
There are many existing methods of compressing this sort of data but I think I may try my hand at creating (or inadvertently replicating) a compression scheme. It sounds like a fun challenge and neat way to mess around with things.

## Framerate Adjustment
Being able to change the framerate of the animation is important, but it might also be interesting to allow variable framerates for difference scenes (if I implement them).

## Import/Export Animation
Being able to save and load animations is obviously important! I need to create a format for the file and methods for storing and loading everything.

## Video Export
Being able to generate modern video formats (or... gifs) either on device or on a computer would make it more useful for many.
Being able to send (gifs in particular) these exported files via wifi to a service would also be a plus.

## Color Options
I think supporting full color in addition to paletted modes (maybe on a scene by scene or project by project basis) would be good for both users and memory constraints. 

## Pen Types
Obviously for drawing we want a variety of pen sizes (1 px, 2px^2, 3px^2 etc). What other kinds of inputs would be good and
useful for people? Would a custom drawing type be of interest (e.g you create a mask of where pixels go when you press)?

## Actor/Sprite Support
As a later feature I think supporting animation layers for sprites or actors would be cool. Each could contain their own animation data and could be moved around the environment. Leading to...

## Basic Splines?
Or whatever they are called. Insert keyframes for locations and interpolate movement between them. This would go well with
the sprite support above.

## Smart animation
To make it a bit more special (and more complicated from a simple drawing app perspective) it might be fun to add support for
something like Bret Victors demo of animations with rules for how they interact with the environment. On a limited system with limited inputs I'm not sure how this would come to life, but it's interesting.

## Copy/Paste Frames and Layers
Definitely an important feature. Being able to copy full frames of animation as well as individual layers (when supported)
would be a nice UX feature.

## Animation Scrubbing
Being able to see and traverse an animation is important. Maybe having a small scrubbable timeline with small rendered frames would be good?

## Color flood
As part of pen types/drawing tools, color flooding is important! And make it work well...

## Transparency
For any of the fun things that I want to do to work (layers, onion skinning, etc) we need good support for transparency. This is also important when designing image storage containers.

## Good stylus input
Since the stylus can move faster than the program can update we need to have smart interpolation to draw continuous lines from frame to frame. Otherwise you get spaces in between dots because the stylus moves too quickly to be polled in time.

## Decent UX
If it's not fun and easy to use then no one (including myself) will want to use it. So it's important (and why this planning exists) to make sure that I start out with a good idea of what I am working towards and how it will all come together in the finished (or WIP) product. If UX is ignored or features are added that are detrimental to UX then time is wasted and things don't get better.

## Frame compression
One of the most exciting things for me to mess with. Both on-the-fly compressiong (realtime) and tighter storage compression (e.g saving to disk) are both things that I'm excited to try tackling.

## Animation Gallery
Having a menu gallery to view/load your animations would be awesome. Doubly so if the currently highlighted one played a preview of the animation.

## Import from another project
If you have assets in another project (e.g sprites, layers, palettes, etc) then it would be good to allow importing from them. Alternately if might be interesting to have a shared store of assets that you can save to for global use in your animations.

## Good code structure
If this isn't readable and maintainable then it will be buggy and never get worked on because it isn't fun.

## Portable
While we are tied in some ways to idiocincracies of the 3DS and its software stack, I want this to be as portable to other platforms as possible.

## Web enabled
Having some amount of web capability, even if it's just interacting with a local server on the network, would be fun. Maybe exporting animation data or touch information or whatever seems interesting.

## Animation Hierarchy
This is an important thing to flesh out relatively early on. How do I want projects structured for the user? Should be project look like `Project -> Animation(s) -> Scene(s) -> Frame(s) -> Layer(s)`? Should a project be allowed to contain more than one animation?

### Frames and layers
Frames should support layers to some degree, even if it's just foreground vs background. Or maybe object vs background. That way animators don't have to worry about redrawing things over and over. Is there a reason to support more than one foreground layer? If we do an object vs background setup we might want some form of Z-ordering for the objects so things can overlap in controllable ways. We could also have `background -> object -> near` layer scheme, where the object layer has z ordering internally and the near layer always overlaps everything.

In order for any of the z-ordering to work (or an object layer to work in this scheme) we essentially require supporting individual sprites/actors as above so that they can be manipulated in ways other than drawing (moved, transformed, etc).

I should make this flexible and not lock into any one scheme, so in theory it will be possible to have many layers. However, for normal use I think I will limit it to just 2 (background and object) with maybe a menu option in the future to create a project with a 3rd layer?

### Scenes
I think scenes are important for any large animation. Being able to break the animation up into smaller parts not only helps with frame limits and memory consumption but also general user organizational ability. Scenes should be able to be previewed independently as well as in the context of the full animation.

### Animations
Should we support projects with more than one animation? Is that even a valid use case? The only reason I would think to have a 'project' encompassing multiple animations is a shared asset pool. Projects could be the top level containers for related assets. But why impose this on people to have to make (for example episodic) animations in the same project in order to use existing assets?

### Assets
I like the idea of being able to export shared assets to a bundle file that is accessible from other animations. Sprites make sense to allow as well as backgrounds. I think if we have good support for this we don't need a top level 'project' thing.

## Undo/Redo
Definitely an important feature. I'm not sure off the top of my head how this will be implemented but it's important and an expected feature.


---

# UI

The 3DS has two screens so it's important to leverage that fact when designing the UI.

## Top Screen
The top screen is inherently less interactive, but also physically larger (extra 40 pixels on each side). I think it should be primarily used for previewing animations and help/information. We could have a simple setup like this:

```
 ___________________________
| up|					| a | Sidebars show little help icons for button mapping.
|dwn|	Animation		| b |
|lft|	Preview 		| x |
|rgt|					| y |
|___|___________________|___|
```

Where the user could have the preview looping constantly or toggled off. It might also be a nice feature to have the ability to put an arbitrary frame on the top (or even take a photo!!!) for reference or something like that.

A scrub bar makes sense on the top screen, but the actual scrubbing part would have to come from a button press or bottom screen touch.

## Bottom Screen

The bottom screen is where all the action is so it's important that it only has what is necessary to make an animator's job easy. I want the space to be as clean as possible so that the full drawing area is accessible.

---

# Priority Features
What I need to implement to get the ball rolling.

* Animation Hierarchy
  * Animation -> Scene -> Frame -> Layer (X)
    * Transparency ()
    * Paletted Color ()
    * Background + Object layer ()
* Drawing functions
  * Abstraction from hardware
  * Add support for top screen buffer too.
* Sprite Support
  * Z indexing
  * Individual animation?
  * Individual framerate
* Start UI elements
  * Toggle Debug Screen option
  * Keybinding display

