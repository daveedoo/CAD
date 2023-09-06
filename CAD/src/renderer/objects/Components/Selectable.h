#pragma once
#include <string>

struct Selectable
{
	std::string name;
	bool selected;

	Selectable(std::string name, bool selected = false)
		: name(name), selected(selected) {}
};
