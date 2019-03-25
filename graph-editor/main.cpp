#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

// A node contains a name and an index.
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
};

// A link contains 2 nodes that need to be
// connected, alongside the link's width
struct Link
{
	std::vector<Node> nodes;	// TODO: Probably could be pair

	sf::RectangleShape line;
	sf::Text label;

	int weight;	// Link's weight (distance)

	// Equals comparison operator overload
	bool operator==(const Link& rhs) const
	{
		return (nodes[0].name == rhs.nodes[0].name) && (nodes[1].name == rhs.nodes[1].name);
	}
};

int main() 
{
	// Window dimensions
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	// Window creation
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Graph editor");
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	sf::Font font;
	if(!font.loadFromFile("CodeSaver-Regular.otf"))
	{
		std::cout << "Failed to load font!" << std::endl;
	}

	std::vector<Node> nodes;
	std::vector<Link> links;

	// Used below to toggle drawing weight labels
	bool shouldDrawStrengths = false;

	// Used below to discard unnecessary clicks
	bool firstLeftClick = true;
	bool firstRightClick = true;
	bool firstMiddleClick = true;

	// Render loop
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

				// Set up label
				sf::Text text;
				text.setFont(font);
				text.setString(std::string(1, newNode.name));
				text.setCharacterSize(40);
				text.setFillColor(sf::Color::Black);
				text.setOrigin(11.2f, 27.0f);	// Very specific numbers so text is aligned
				text.setPosition(sf::Vector2f(localPosition));
				newNode.label = text;

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

					// Get the positions of both nodes
					sf::Vector2f firstPos = temp.nodes[0].nodeShape.getPosition();
					sf::Vector2f secondPos = temp.nodes[1].nodeShape.getPosition();

					// Calculate distance on x and y axes
					float xdist = secondPos.x - firstPos.x;
					float ydist = secondPos.y - firstPos.y;

					// Calculate hypotenuse
					float c = std::sqrt(std::pow(xdist, 2) + std::pow(ydist, 2));

					// Angle between the 2 points
					float angle = (180 / 3.14159f) * std::atan2(ydist, xdist);

					float lineWidth = 5.0f;
					temp.line = sf::RectangleShape(sf::Vector2f(c, lineWidth));	// Line dimensions
					temp.line.setPosition(firstPos);							// Starting position (at first node)
					temp.line.setOrigin(lineWidth * 0.5f, lineWidth * 0.5f);	// Origin so that the line looks centered
					temp.line.rotate(angle);									// Rotate the line so that it touches the second node
					temp.line.setFillColor(sf::Color(100, 100, 100, 255));		// Line color

					// Line strength label
					sf::Text text1;
					text1.setFont(font);
					text1.setString(std::to_string(static_cast<int>(c)));
					text1.setFillColor(sf::Color::White);
					text1.setCharacterSize(25);		
					text1.setOrigin(16.0f, 17.0f);	// Specific numbers...
					text1.setPosition(static_cast<int>(firstPos.x + xdist * 0.5f), static_cast<int>(firstPos.y + ydist * 0.5f));
					// Cast is for text sharpness

					temp.label = text1;

					// If the current link is the same as, or a
					// reversed copy from a link that already exists,
					// don't push it to the vector
					bool duplicate = false;
					for(auto& l : links)
					{
						if(temp == l)
							duplicate = true;
					}

					if(!duplicate)
						links.push_back(temp);
				}
			}

			// This is to prevent 'painting' bug, so one node is placed on each click
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				firstRightClick = true;
			}

			if(sf::Mouse::isButtonPressed(sf::Mouse::Middle) && firstMiddleClick)
			{
				shouldDrawStrengths = !shouldDrawStrengths;
			}

			// This is to prevent 'painting' bug, so one node is placed on each click
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Middle))
			{
				firstMiddleClick = true;
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

			// Erase all nodes and links
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				links.erase(links.begin(), links.end());
				nodes.erase(nodes.begin(), nodes.end());
			}
		}

		window.clear();

		for(auto& l : links)
		{
			window.draw(l.line);
		}

		if(shouldDrawStrengths)
			for(auto& l : links)
			{
				window.draw(l.label);
			}

		for(auto& n : nodes)
		{
			window.draw(n.nodeShape);
			window.draw(n.label);
		}		
		
		window.display();
	}
	
	return 0;
}