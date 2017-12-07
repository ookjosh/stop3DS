## User menu interface

I want to implement a simple menu interface for commonly used elements. It is intended to have a uniform experience (e.g same buttons play same/similar roles for each function) and rapid interface (easy to get at commonly needed features).

### Graphics

The graphics portion consists of 6 uniform squares with an icon and optionally number printed on them. The selected square is highlighted a different color than the rest. Normally only these 6 squares are visible, but if a square is selected that has a sub menu the menu extends outward from the menu bar with their own squares. This selection is similarly highlighted. Each selection has small icons indicating buttons to press if necessary.

### Backend

I don't think I've ever made a menu before. I want the menu to be somewhat flexible in terms of layout (so positions shouldn't be hardcoded for instance) and in terms of positioning on the screen (vertical vs horizontal, etc).

```
int topMenu = 0;
int subMenu = -1;

downPressed () {
	topMenu++;
	if (topMenu > MAX) topMenu = MAX;
}

upPressed () {
	topMenu--;
	if (topMenu < MIN) topMenu = MIN;
}

rightPressed() {
	if (menuList[topMenu].hasSubMenu) {
		subMenu++;
	}
}

leftPressed() {
	if (subMenu > -1) {
		subMenu--;
	}
}

Menu 1
  |
Menu 2	-> Sub 1 -> Sub 2
  |
Menu 3
  |
Menu 4 -> Sub 1 -> Sub 2
  |
Menu 5
  |
Menu 6
```