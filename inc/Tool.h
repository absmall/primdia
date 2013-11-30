#ifndef __TOOL_H__
#define __TOOL_H__

#include <config/common.h>

#include <map>
#include <list>
#include <iostream>

class Tool
{
public:
	Tool(const std::string &basename, const std::string &filename);


	static bool save(const std::string &filename, const Document *doc);

	const std::string &getName();

	struct NodeData
	{
		NodeData(std::string name, std::string type)
			:name(name), type(type){}
		std::string name;
		std::string type;
	};

	struct BindingData
	{
		std::list<std::pair<std::string,std::string> > attributes;
		std::string type;
		std::string value;
	};

	std::string name;
	std::string icon;
	std::string description;
	std::list<NodeData> nodes;
	std::list<NodeData> aggregates;
	std::list<NodeData> interface;
	std::list<BindingData> bindings;
};

#endif /* __TOOL_H__ */
