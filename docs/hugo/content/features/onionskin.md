+++
title = "Onion Skinning"
+++

## Overview

Onion skinning is really important in creating animations and is fun. It derives from old hand animation techniques which used a transparent sheet to allow the animators to see the previous frame. On computers this is an alpha compositing problem.


### Progress
I have a working basic version that displays the previous frame if it exists. Depending on a) if it is necessary and b) if it works well with UX it might be cool to be able to mark an alternate frame as the onion skin.

### Layers

Similar to onion skinning (in terms of technical approach) are layers in each frame. We need to be able to composite multiple layers in order to have a foreground/background/onion skin etc. 

In order to do this in software (which I am doing here - often you can get this in some form of hardware acceleration) you need to only copy pixels that are not "transparent" for each layer, iterating over your layers from back to front.