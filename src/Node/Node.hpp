#pragma once
#include <SFML/Graphics.hpp>

struct Node
{
	int index;

	sf::Vector2f position;
	sf::Text label;

	// Whether the node is selected to 
	// to be linked with another node
	bool selected;

	// Whether the node has been
	// visited. Used for traversal
	bool visited = false;

	// Equals comparison operator overload
	bool operator==(const Node& rhs) const
	{
		return (index == rhs.index);
	}
};
