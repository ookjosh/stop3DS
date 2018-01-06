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

private:

	std::vector<MenuItem> items;

};

class MenuItem {
public:
	MenuItem(std::string nm);
	void addSubMenu(Menu sub);
	bool hasSubMenu();
	std::string& getName();
	std::string& getIcon();
	Menu& getSubMenu();

	void performAction();

	void setAction();

	void setIcon(std::string ic);
private:

	std::string name = "Default";
	std::string icon = "";
	bool sub = false;
	// Default is empty but is 
	// here for simplicity.
	Menu subMenu;
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

MenuItem::MenuItem(std::string nm) {
	name = nm;
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

void MenuItem::performAction() {

}

// Sets a subtitle to the menu item. For things like
// showing the currently selected tool or current frame.
void MenuItem::setIcon(std::string ic) {
	icon = ic;
}


#endif