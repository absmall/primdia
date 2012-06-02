#include <set>
#include <glib.h>
#include <sstream>
#include <libxml++/libxml++.h>
#include <stdlib.h>
#include <iostream>

#include <Node.h>
#include <Type.h>
#include <Value.h>
#include <Binding.h>
#include <NodeSet.h>
#include <Attribute.h>
#include <Interface.h>

void NodeSet::load(const std::string &name)
{
	enum State
	{
		PARSING_NOTHING,
		PARSING_DIAGRAM,
		PARSING_INTERFACE,
		PARSING_INTERFACE_NODE,
		PARSING_BINDINGS,
		PARSING_BINDINGS_MEMBER
	} state;

	state = PARSING_NOTHING;
	xmlpp::TextReader reader(name); 

	while (reader.read())
	{
		switch(state) {
			case PARSING_NOTHING:
				if( reader.get_name() == "diagram" ) {
					state = PARSING_DIAGRAM;
				} else {
					throw std::string("Parse error");
				}
				break;
			case PARSING_DIAGRAM:
			case PARSING_INTERFACE:
			case PARSING_INTERFACE_NODE:
			case PARSING_BINDINGS:
			case PARSING_BINDINGS_MEMBER:
			default:
				throw std::string("Unexpected parser state");
		}
	}
}

void NodeSet::saveAs(const std::string &name)
{
	// Count the primitives
	std::set<Binding *> b;
	std::map<int, Node *> anodes;
	std::map<int, Binding *> bindings;
	std::map<int, Interface *> interfaces;
	std::map<Node *, int> rnodes;
	std::map<Binding *, int> rbindings;
	xmlpp::Document document;

	xmlpp::Element *diagramRoot = document.create_root_node("diagram");
	diagramRoot->set_namespace_declaration("http://smeagle.dyndns.org/primdia/diagram");

	xmlpp::Element *interfaceRoot = diagramRoot->add_child("interfaces");
	foreach(i, nodes)
	{
		int r;
		do r = rand(); while(interfaces.find(r) != interfaces.end());

		xmlpp::Element *interface = interfaceRoot->add_child("interface");
		interface->set_attribute("name", (*i)->getName().c_str());

		interfaces[r] = *i;

		foreach(j, (*i)->nodes)
		{
			int r;
			do r = rand(); while(anodes.find(r) != anodes.end());
			std::ostringstream ss;
			ss << r;

			xmlpp::Element *node = interface->add_child("node");
			node->set_attribute("name", ss.str());
			node->set_attribute("type", (*j)->type->name.c_str());

			anodes[r] = *j;
			rnodes[*j] = r;
		}

		(*i)->getBindings(b);
	}

	foreach(i, b)
	{
		int r;
		do r = rand(); while(bindings.find(r) != bindings.end());

		bindings[r] = *i;
		rbindings[*i] = r;
	}

	xmlpp::Element *bindingRoot = diagramRoot->add_child("bindings");

	foreach(i, bindings)
	{
		Binding *b = i->second;

		xmlpp::Element *binding = bindingRoot->add_child("binding");
		binding->set_attribute("type", b->type->name.c_str());

		if (i->second->hasSetValue())
		{
			binding->set_attribute("value", i->second->getValue()->getString().c_str());
		}

		foreach(j, b->attributes)
		{
			if ((*j)->getNode() != NULL)
			{
				std::ostringstream ss;
				ss << rnodes[(*j)->getNode()];

				xmlpp::Element *member = binding->add_child("member");
				member->set_attribute("node", ss.str());
				member->set_attribute("attribute", (*j)->getName().c_str());
			}
		}
	}

	document.write_to_file_formatted(name.c_str());
}

void NodeSet::saveTool(const std::string &name)
{
	// Count the primitives
	std::set<Binding *> b;
	std::map<int, Node *> anodes;
	std::map<int, Binding *> bindings;
	std::map<int, Interface *> interfaces;
	std::map<Node *, int> rnodes;
	std::map<Binding *, int> rbindings;
	xmlpp::Document document;
	int attribute_count = 1;

	xmlpp::Element *toolRoot = document.create_root_node("tool");
	toolRoot->set_namespace_declaration("http://smeagle.dyndns.org/primdia/tool");
	toolRoot->set_attribute("description", name.c_str());

	foreach(i, nodes)
	{
		int r;
		do r = rand(); while(interfaces.find(r) != interfaces.end());

		interfaces[r] = *i;

		foreach(j, (*i)->nodes)
		{
			int r;
			do r = rand(); while(anodes.find(r) != anodes.end());

			anodes[r] = *j;
			rnodes[*j] = r;
		}

		(*i)->getBindings(b);
	}

	foreach(i, b)
	{
		int r;
		do r = rand(); while(bindings.find(r) != bindings.end());

		bindings[r] = *i;
		rbindings[*i] = r;
	}

	xmlpp::Element *nodeRoot = toolRoot->add_child("nodes");

	foreach(i, anodes)
	{
		std::ostringstream ss;
		ss << i->first;

		xmlpp::Element *node = nodeRoot->add_child("node");
		node->set_attribute("name", ss.str());
		node->set_attribute("type", i->second->type->name.c_str());
	}

	xmlpp::Element *bindingRoot = toolRoot->add_child("bindings");

	foreach(i, bindings)
	{
		Binding *b = i->second;

		xmlpp::Element *binding = bindingRoot->add_child("binding");
		binding->set_attribute("type", b->type->name.c_str());

		if (b->hasSetValue() || !b->hasValue())
		{
			std::ostringstream ss;
			ss << attribute_count++;
			binding->set_attribute("interface", (b->type->name+ss.str()));
		}

		if (i->second->hasSetValue())
		{
			binding->set_attribute("value", i->second->getValue()->getString().c_str());
		}

		foreach(j, b->attributes)
		{
			if ((*j)->getNode() != NULL)
			{
				std::ostringstream ss;
				ss << rnodes[(*j)->getNode()];
				xmlpp::Element *member = binding->add_child("member");
				member->set_attribute("node", ss.str());
				member->set_attribute("attribute", (*j)->getName().c_str());
			}
		}
	}

	const gchar *base = g_get_user_data_dir();
	document.write_to_file_formatted((std::string(base)+std::string("/applications/")+ std::string(PACKAGE)
				+ std::string("/tools/") + name).c_str());
}
