+++
title = "Drawing"
+++

## Overview

### Progress

#### Linear Interpolation

As of commit `5405bdc5179bbccd01aca7139425e38cef068cfe` I have finally actually implemented a real 'LERP' for user input. And my it makes a huge difference. Before then I had messed around with a very obtuse (intentionally naive we'll say) implementation that was both flawed and bloated. These images compare drawing with the two methods:

{{%expand "Old LERP" %}}
![Old thing](/static/img/old_lerp.png)

.{{% /expand%}}

{{%expand "Better LERP" %}}
![New lerp](/static/img/new_lerp.png)

.{{% /expand%}}