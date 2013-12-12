#include <iostream>

#include <ConsoleView.h>

ConsoleView::ConsoleView(Document *document) :d(document), View(document)
{
}

ConsoleView::~ConsoleView()
{
}

void ConsoleView::update(int command, Attribute *attribute, Interface *node, Binding *binding)
{
    std::cout << "Update!" << std::endl;
}
