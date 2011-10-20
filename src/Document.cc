#include <list>
#include <algorithm>
#include <sstream>

#include <Node.h>
#include <Tool.h>
#include <View.h>
#include <Point2D.h>
#include <Bearing.h>
#include <Binding.h>
#include <Distance.h>
#include <Document.h>
#include <Exception.h>
#include <Interface.h>

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
	NodeSet::load(name);
	this->name = name;
	has_name = true;
	modified = false;
}

void Document::save(void)
{
	if (!modified) return;

	if (!has_name) throw unset_parameter("No file name");

	NodeSet::saveAs(name);
	modified = false;
}

void Document::saveAs(const std::string &name)
{
	NodeSet::saveAs(name);
	this->name = name;
	has_name = true;
	modified = false;
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
