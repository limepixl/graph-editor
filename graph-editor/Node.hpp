#pragma once
#include <SFML/Graphics.hpp>

// A node contains a name and an index
struct Node
{
	char name;
	int index;

	float radius = 20.0f;

	sf::CircleShape nodeShape = sf::CircleShape(radius);
	sf::Text label;

	// Whether the node is selected to 
	// to be linked with another node
	bool selected;

	// Equals comparison operator overload
	bool operator==(const Node& rhs) const
	{
		return (index == rhs.index);
	}
};