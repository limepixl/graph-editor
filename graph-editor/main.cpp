#include <iostream>
#include <vector>
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
};

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

	// Vectors to contain created nodes and links
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
				sf::CircleShape& circle = newNode.nodeShape;
				circle.setPosition(sf::Vector2f(localPosition));
				circle.setOrigin(newNode.radius, newNode.radius);
				circle.setOutlineColor(sf::Color(255, 0, 0, 255));

				// Set up label
				sf::Text text;
				text.setFont(font);
				text.setString(std::string(1, newNode.name));
				text.setCharacterSize(40);
				text.setFillColor(sf::Color::Black);
				text.setOrigin(11.2f, 27.0f);	// Very specific numbers so text is aligned
				text.setPosition(sf::Vector2f(localPosition));
				newNode.label = text;

				nodes.push_back(newNode);	// Push it to vector
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
					Link link;

					// Find the 2 selected nodes
					for(auto& n : nodes)
					{
						if(n.selected)
						{
							n.selected = false;
							link.nodes.push_back(n);
							n.nodeShape.setOutlineThickness(0.0f);	// So the node appears unselected
						}
					}

					// Get the positions of both nodes
					sf::Vector2f firstPos = link.nodes[0].nodeShape.getPosition();
					sf::Vector2f secondPos = link.nodes[1].nodeShape.getPosition();

					// Calculate distance on x and y axes
					float xdist = secondPos.x - firstPos.x;
					float ydist = secondPos.y - firstPos.y;

					// Calculate hypotenuse
					float c = std::sqrt(std::pow(xdist, 2) + std::pow(ydist, 2));

					// Angle between the 2 points (converted to degrees)
					float angle = (180.0f / 3.14159f) * std::atan2(ydist, xdist);

					float lineWidth = 5.0f;
					link.line = sf::RectangleShape(sf::Vector2f(c, lineWidth));	// Line dimensions
					link.line.setPosition(firstPos);							// Starting position (at first node)
					link.line.setOrigin(lineWidth * 0.5f, lineWidth * 0.5f);	// Origin so that the line looks centered
					link.line.rotate(angle);									// Rotate the line so that it touches the second node
					link.line.setFillColor(sf::Color(100, 100, 100, 255));		// Line color (grey-ish)

					// Line strength label
					sf::Text text;
					text.setFont(font);
					text.setString(std::to_string(static_cast<int>(c)));
					text.setFillColor(sf::Color::White);
					text.setCharacterSize(25);		
					text.setOrigin(16.0f, 17.0f);	// Specific numbers...
					text.setPosition(std::floor(firstPos.x + xdist * 0.5f), std::floor(firstPos.y + ydist * 0.5f));

					link.label = text;

					// If the current link is the same as, or a
					// reversed copy from a link that already exists,
					// don't push it to the vector
					bool duplicate = false;
					for(auto& l : links)
					{
						if(link == l)
							duplicate = true;
					}

					if(!duplicate)
						links.push_back(link);
				}
			}

			// This is to prevent 'painting' bug, so one node is placed on each click
			if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				firstRightClick = true;
			}

			if(sf::Mouse::isButtonPressed(sf::Mouse::Middle) && firstMiddleClick)
			{
				firstMiddleClick = false;

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

				// No point in printing if no links are present
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