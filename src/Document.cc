#include <list>
#include <algorithm>
#include <sstream>
#include <libxml++/libxml++.h>

#include <Node.h>
#include <Tool.h>
#include <Type.h>
#include <View.h>
#include <Point2D.h>
#include <Bearing.h>
#include <Binding.h>
#include <Distance.h>
#include <Document.h>
#include <Attribute.h>
#include <Exception.h>
#include <Interface.h>
#include <InterfaceAttribute.h>

using namespace std;

list<Document *> Document::documentList;

int Document::newDocuments = 1;

Document::Document()
{
	has_name = false;
	modified = false;
	documentList.push_back(this);
	name = makeTemporaryDocumentName();
}

Document::~Document()
{
	while(nodes.size())
	{
		delete *nodes.begin();
	}

	documentList.erase(find(documentList.begin(), documentList.end(), this));
}

const std::list<Document *> &Document::getDocuments()
{
	return documentList;
}

const std::set<Interface *> &Document::getNodes(void) const
{
	return nodes;
}

void Document::registerView(View *v)
{
	views.push_back(v);
	Document::updateDocumentList();
}

void Document::unRegisterView(View *v)
{
	views.erase(find(views.begin(), views.end(), v));
	if (views.size() == 0)
	{
		delete this;
	} else {
		Document::updateDocumentList();
	}
}

void Document::load(const std::string &name)
{
	enum State
	{
		PARSING_NOTHING,
		PARSING_DIAGRAM,
		PARSING_INTERFACES,
		PARSING_INTERFACE,
		PARSING_BINDINGS,
		PARSING_BINDING,
		PARSING_BINDING_MEMBER
	} state;

	state = PARSING_NOTHING;
	xmlpp::TextReader reader(name); 

	while (reader.read())
	{
        std::cout << "See name " << reader.get_name() << "(" << reader.get_node_type() << ")" << std::endl;
        switch(reader.get_node_type()) {
            case xmlpp::TextReader::Element:
                switch(state) {
                    case PARSING_NOTHING:
                        if( reader.get_name() == "diagram" ) {
                            state = PARSING_DIAGRAM;
                        }
                        break;
                    case PARSING_DIAGRAM:
                        if( reader.get_name() == "interfaces" ) {
                            state = PARSING_INTERFACES;
                        } else if( reader.get_name() == "bindings" ) {
                            state = PARSING_BINDINGS;
                        }
                        break;
                    case PARSING_INTERFACES:
                        if( reader.get_name() == "interface" ) {
                            state = PARSING_INTERFACE;
                        }
                        break;
                    case PARSING_BINDINGS:
                        if( reader.get_name() == "binding" ) {
                            state = PARSING_BINDING;
                        }
                        break;
                    case PARSING_BINDING:
                        if( reader.get_name() == "member" ) {
                            state = PARSING_BINDING_MEMBER;
                            reader.move_to_first_attribute();
                            do {
                                std::cout << "See attribute " << reader.get_name() << " is " << reader.get_value() << std::endl;
                            } while(reader.move_to_next_attribute());
                            reader.move_to_element();
                        }
                        break;
                    case PARSING_INTERFACE:
                    case PARSING_BINDING_MEMBER:
                    default:
                        std::cout << "Unexpected open tag" << std::endl;
                }
                break;
            case xmlpp::TextReader::EndElement:
                switch(state) {
                    case PARSING_DIAGRAM:
                        if( reader.get_name() == "diagram" ) {
                            state = PARSING_NOTHING;
                        }
                        break;
                    case PARSING_INTERFACES:
                        if( reader.get_name() == "interfaces" ) {
                            state = PARSING_DIAGRAM;
                        }
                        break;
                    case PARSING_INTERFACE:
                        if( reader.get_name() == "interface" ) {
                            state = PARSING_INTERFACES;
                        }
                        break;
                    case PARSING_BINDINGS:
                        if( reader.get_name() == "bindings" ) {
                            state = PARSING_DIAGRAM;
                        }
                        break;
                    case PARSING_BINDING:
                        if( reader.get_name() == "binding" ) {
                            state = PARSING_BINDINGS;
                        }
                        break;
                    case PARSING_BINDING_MEMBER:
                        if( reader.get_name() == "binding_member" ) {
                            state = PARSING_BINDING;
                        }
                        break;
                    default:
                        std::cout << "Unexpected close tag" << std::endl;
                }
            case xmlpp::TextReader::Text:
            case xmlpp::TextReader::SignificantWhitespace:
            case xmlpp::TextReader::Whitespace:
                break;
            default:
                std::cout << "Unexpected element type" << std::endl;
        }
	}
	this->name = name;
	has_name = true;
	modified = false;
}

void Document::save(void)
{
	if (!modified) return;

	if (!has_name) throw unset_parameter("No file name");

	write_file(name);
	modified = false;
}

void Document::saveAs(const std::string &name)
{
	write_file(name);
	this->name = name;
	has_name = true;
	modified = false;
}

void Document::saveTool(const std::string &name)
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
bool Document::isModified(void) const
{
	return modified;
}

void Document::updateDocumentList()
{
	foreach(i, documentList)
	{
		foreach(j, (*i)->views)
		{
			(*j)->updateWindowList();
		}
	}
}

const std::string &Document::getName()
{
	return name;
}

const list<View *> &Document::getViews()
{
	return views;
}

std::string Document::makeTemporaryDocumentName()
{
	std::stringstream ss;
	ss << "Untitled ";
	ss << (newDocuments++);
	return ss.str();
}

void Document::write_file(const std::string &name)
{
	// Count the primitives
	std::set<Binding *> b;
	std::map<int, Binding *> bindings;
	std::map<Binding *, int> rbindings;
	std::map<int, Interface *> interfaces;
	std::map<Interface *, int> rinterfaces;
	xmlpp::Document document;

	xmlpp::Element *diagramRoot = document.create_root_node("diagram");
	diagramRoot->set_namespace_declaration("http://smeagle.dyndns.org/primdia/diagram");

	xmlpp::Element *interfaceRoot = diagramRoot->add_child("interfaces");
	foreach(i, nodes)
	{
		int r;
        std::ostringstream ss;
		do r = rand(); while(interfaces.find(r) != interfaces.end());
        ss << r;

		xmlpp::Element *interface = interfaceRoot->add_child("interface");
		interface->set_attribute("type", (*i)->getName().c_str());
		interface->set_attribute("name", ss.str());

		interfaces[r] = *i;
		rinterfaces[*i] = r;

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
			if ((*j)->getNode() == NULL)
			{
				std::ostringstream ss;
				ss << rinterfaces[static_cast<InterfaceAttribute * const>(*j)->getInterface()];

				xmlpp::Element *member = binding->add_child("member");
				member->set_attribute("node", ss.str());
				member->set_attribute("attribute", (*j)->getName().c_str());
			}
		}
	}

	document.write_to_file_formatted(name.c_str());
}


Interface *Document::addNode(const Tool *type)
{
	Interface *ret = new Interface(this, NULL, type->name, type);
	nodes.insert(ret);

	update(Document::AddNode, NULL, ret, NULL);

	return ret;
}

void Document::update(int command, Attribute *attribute, Interface *node, Binding *binding)
{
	modified = true;
	foreach(i, views)
	{
		(*i)->update(command, attribute, node, binding);
	}
}

void Document::unregisterNode(Interface *n)
{
	nodes.erase(n);
	update(RemoveNode, NULL, n, NULL);
}
