#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// A node contains a name and an index.
struct Node
{
	char name;
	int index;

	float radius = 20.0f;
	sf::CircleShape nodeShape = sf::CircleShape(radius);
};

// A link contains 2 nodes that need to be
// connected, alongside the link's width
struct Link
{
	Node first;
	Node second;

	int weight;
};

int main() 
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Graph editor");

	std::vector<Node> nodes;
	std::vector<Link> links;

	bool firstClick = true;

	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && firstClick)
			{
				firstClick = false;

				// Get the mouse's current position
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				
				Node newNode = { (char)(65 + nodes.size()), nodes.size() };

				newNode.nodeShape.setPosition(sf::Vector2f(localPosition));
				newNode.nodeShape.setOrigin(newNode.radius, newNode.radius);
				nodes.push_back(newNode);

				std::cout << "Currently created nodes: " << std::endl;
				for(auto& n : nodes)
				{
					std::cout << n.name << std::endl;
				}

				std::cout << "Currently created links: " << "\nBitch ne si go implementnal toa seuste" << std::endl;
			}

			// This is to prevent 'painting' bug, so one node is placed on each click
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				firstClick = true;
			}
		}

		window.clear();

		for(auto& n : nodes)
		{
			window.draw(n.nodeShape);
		}
		
		window.display();
	}
	
	return 0;
}