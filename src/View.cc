#include <View.h>
#include <Document.h>
#include <sstream>

using namespace std;

unsigned int View::newViews = 1;

View::View()
{
	document = new Document;
	name = makeTemporaryViewName();
}

View::View(Document *doc)
{
	this->document = doc;
	name = makeTemporaryViewName();
}

View::~View()
{
	document->unRegisterView(this);
}

const std::string &View::getName()
{
	return name;
}

void View::updateWindowList()
{
}

std::string View::makeTemporaryViewName()
{
	stringstream ss;
	ss << "Untitled ";
	ss << (newViews++);
	return ss.str();
}
