#include "Link.hpp"

bool Link::operator==(const Link& rhs) const
{
	bool f = (first->index == rhs.first->index);
	bool s = (second->index == rhs.second->index);
	return (f && s);
}