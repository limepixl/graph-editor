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

	bool selected;
};

// A link contains 2 nodes that need to be
// connected, alongside the link's width
struct Link
{
	std::vector<Node> nodes;
	sf::RectangleShape line;
	int weight;
};

int main() 
{
	// Window creation
	sf::RenderWindow window(sf::VideoMode(800, 600), "Graph editor");
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	std::vector<Node> nodes;
	std::vector<Link> links;

	// Used below to discard unnecessary clicks
	bool firstLeftClick = true;
	bool firstRightClick = true;
	while(window.isOpen())
	{
		// Event polling
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && firstLeftClick)
			{
				firstLeftClick = false;

				// Get the mouse's current position
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				
				// Create a new node
				int index = static_cast<int>(nodes.size());
				Node newNode = { (char)(65 + index), index };

				// Set the node's circle position and origin
				newNode.nodeShape.setPosition(sf::Vector2f(localPosition));
				newNode.nodeShape.setOrigin(newNode.radius, newNode.radius);
				newNode.nodeShape.setOutlineColor(sf::Color(255, 0, 0, 255));
				nodes.push_back(newNode);	// Add it to vector
			}

			// This is to prevent 'painting' bug, so one node is placed on each click
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				firstLeftClick = true;
			}

			if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && firstRightClick)
			{
				// Sum of all selected nodes
				int sum = 0; 
				firstRightClick = false;

				// Check if the mouse is clicked within the bounds
				// of a node's circle shape
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				for(auto& n : nodes)
				{					
					// The node's center coordinates
					sf::Vector2f center = n.nodeShape.getPosition();

					// Distance between node's center and cursor's position
					float distance = std::sqrt(std::pow(mousePos.x - center.x, 2) + std::pow(mousePos.y - center.y, 2));

					// If the mouse is clicked inside the circle
					if(distance <= n.radius)
					{
						n.nodeShape.setOutlineThickness(5.0f);
						n.selected = true;
					}

					if(n.selected) sum++;
				}

				// If there are already 2 selected nodes, don't select any more
				// and create a link between the 2 nodes
				if(sum == 2)
				{
					Link temp;
					for(auto& n : nodes)
					{
						if(n.selected)
						{
							temp.nodes.push_back(n);
							n.nodeShape.setOutlineThickness(0.0f);
							n.selected = false;
						}
					}

					sf::Vector2f firstPos = temp.nodes[0].nodeShape.getPosition();
					sf::Vector2f secondPos = temp.nodes[1].nodeShape.getPosition();

					float xdist = secondPos.x - firstPos.x;
					float ydist = secondPos.y - firstPos.y;

					float c = std::sqrt(std::pow(xdist, 2) + std::pow(ydist, 2));

					float angle = (180 / 3.14159f) * std::atan2(ydist, xdist);

					temp.line = sf::RectangleShape(sf::Vector2f(c, 5.0f));
					temp.line.setPosition(firstPos);
					temp.line.rotate(angle);

					links.push_back(temp);
				}
			}

			// This is to prevent 'painting' bug, so one node is placed on each click
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				firstRightClick = true;
			}

			// Print debug info
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				// Clear console window
				system("cls");

				// Print all created nodes for debugging
				std::cout << "Currently created nodes: " << std::endl;
				for(auto& n : nodes)
				{
					std::cout << n.name << std::endl;
				}

				if(!links.empty())
				{
					// Print all the created links for debugging
					std::cout << "Currently created links: " << std::endl;
					for(auto& l : links)
					{
						std::cout << l.nodes[0].name << "----" << l.nodes[1].name << std::endl;
					}
				}
			}
		}

		window.clear();

		for(auto& n : nodes)
		{
			window.draw(n.nodeShape);
		}

		for(auto& l : links)
		{
			window.draw(l.line);
		}
		
		window.display();
	}
	
	return 0;
}