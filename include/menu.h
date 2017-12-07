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
	Menu& getSubMenu();
private:

	std::string name = "Default";
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

void MenuItem::addSubMenu(Menu submen) {
	sub = true;
	subMenu = submen;
}

Menu& MenuItem::getSubMenu() {
	return subMenu;
}


#endif