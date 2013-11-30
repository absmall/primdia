#ifndef __VIEW_H__
#define __VIEW_H__

#include <config/common.h>

#include <set>
#include <map>

class View
{
public:
	View();
	View(Document *document);
	virtual ~View()=0;
	virtual void updateWindowList();
	virtual void update(int command, Attribute *attribute, Interface *node, Binding *binding)=0;
	const std::string &getName();
	Document *document;
	std::set<Interface *> selection;

protected:
	std::string name;
	static unsigned int newViews;

private:
	static std::string makeTemporaryViewName();
};

#endif /* __VIEW_H__ */
