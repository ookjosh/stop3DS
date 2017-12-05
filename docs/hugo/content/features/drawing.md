+++
title = "Drawing"
+++

## Overview

As a drawing application, user input is a first class citizen. Unfortunately, I am currently forced to utilize a polling method for input that is tied to the application's FPS. Ideally (if I had absolute control over the entire system) I would have user inputs have the highest priority and utilize either a rapid interrupt or high frequency polling scheme. However, as I am focused on other aspects of the application for now (and because performance is good enough for the time being) I am working around this limitation.

One common issue with inputs is that when you move the stylus quickly it can move faster (more pixels) than the polling can keep up with, which would lead to trails of specks rather than smooth lines. To make up for this I am utilizing simple linear interpolation to fill in any gaps that may exist from rapid input.

### Progress

I started off with an incredibly bloated and naive attempt at linear interpolation to see what would happen. It performed poorly (it didn't always draw straight lines!) but was a fine starting point while I was working on other parts of the application. Then I moved to the commonly quoted "LERP" and it works has much better performance and is much more concise.  

#### Linear Interpolation

As of commit `5405bdc5179bbccd01aca7139425e38cef068cfe` I have finally actually implemented a real 'LERP' for user input. And my it makes a huge difference. Before then I had messed around with a very obtuse (intentionally naive we'll say) implementation that was both flawed and bloated. These images compare drawing with the two methods:

{{%expand "Old LERP" %}}
![Old thing](/stop3DS/img/old_lerp.png)

.{{% /expand%}}

{{%expand "Better LERP" %}}
![New lerp](/stop3DS/img/new_lerp.png)

.{{% /expand%}}

You can see that in the first image (both drawn with a trackpad - drawing quality was doomed from the start) that there is a marked difference in the ability to keep a continuous line between points. It is especially pronounced in fast diagonal movements, where the old method would actually produce small lines that looked like `\` and `/` coming off the line.

In the future I may look to more advanced interpolations (bilinear, bezier curves) if I'm not happy with the quality of the drawings, but for now this is a great and simple improvement.