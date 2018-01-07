#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

class MenuItem;

class Menu {
public:
	Menu();
	void addItem(MenuItem item);
	void removeItemAtIndex(int index);
	MenuItem& at(int index);
	int size();

	// Menu navigation
	void next();
	void prev();
	void incrementItem();
	void decrementItem();
	int index();

private:

	std::vector<MenuItem> items;

	int currentIndex = 0;

};

class MenuItem {
public:
	MenuItem(std::string nm, int* value);
	void addSubMenu(Menu sub);
	bool hasSubMenu();
	std::string& getName();
	std::string& getIcon();
	Menu& getSubMenu();

	void increment();
	void decrement();

	void setAction();

	void setIcon(std::string ic);
private:

	std::string name = "Default";
	std::string icon = "";
	bool sub = false;
	// Default is empty but is 
	// here for simplicity.
	Menu subMenu;

	// Optional value that can be bound to external things like 
	// fps, onion skin, frame, etc.
	int* itemValue = NULL;
};

Menu::Menu() {

}

void Menu::addItem(MenuItem item) {
	items.push_back(item);
}

void Menu::removeItemAtIndex(int index) {
	if (index > items.size()) return;
	items.erase(items.begin()+index);
}

MenuItem& Menu::at(int index) {
	return items.at(index);
}

int Menu::size() {
	return items.size();
}


void Menu::next() {
	currentIndex+=1;
	if (currentIndex > size()) {
		currentIndex -= 1;
	}
}

void Menu::prev() {
	currentIndex -= 1;
	if (currentIndex < 0) {
		currentIndex = 0;
	}
}

int Menu::index() {
	return currentIndex;
}

void Menu::incrementItem() {
	this->at(currentIndex).increment();
}

void Menu::decrementItem() {
	this->at(currentIndex).decrement();
}

// Pass string caption for menu item, and optionally a pointer to a
// value that can be incremented or decremented.
MenuItem::MenuItem(std::string nm, int* value=NULL) {
	name = nm;
	itemValue = value;
}

bool MenuItem::hasSubMenu() {
	return sub;
}

std::string& MenuItem::getName() {
	return name;
}

std::string& MenuItem::getIcon() {
	return icon;
}

void MenuItem::addSubMenu(Menu submen) {
	sub = true;
	subMenu = submen;
}

Menu& MenuItem::getSubMenu() {
	return subMenu;
}

void MenuItem::increment() {
	if (itemValue != NULL) {
		*itemValue += 1;
	}
}

void MenuItem::decrement() {
	if (itemValue != NULL) {
		*itemValue -= 1;
	}
}


// Sets a subtitle to the menu item. For things like
// showing the currently selected tool or current frame.
void MenuItem::setIcon(std::string ic) {
	icon = ic;
}


#endif