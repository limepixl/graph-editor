#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Node/Node.hpp"

// A link contains 2 nodes that are
// connected, alongside the link's width
struct Link
{
	Node* first;
	Node* second;

	sf::RectangleShape line;
	sf::Text label;

	int weight;	// Link's weight (distance)

	// Equals comparison operator overload
	bool operator==(const Link& rhs) const;
};
