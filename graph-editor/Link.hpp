#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

// A link contains 2 nodes that are
// connected, alongside the link's width
struct Link
{
	std::vector<Node> nodes;

	sf::RectangleShape line;
	sf::Text label;

	int weight;	// Link's weight (distance)

	// Equals comparison operator overload
	bool operator==(const Link& rhs) const
	{
		bool first = nodes[0].name == rhs.nodes[0].name;
		bool second = nodes[1].name == rhs.nodes[1].name;
		return (first && second);
	}
};
