#include <stack>
#include <glib.h>
#include <sstream>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#include <Node.h>
#include <Tool.h>
#include <Type.h>
#include <Value.h>
#include <Binding.h>
#include <Document.h>
#include <Attribute.h>
#include <Interface.h>

using namespace std;

static stack<std::string> tags;
static Tool::BindingData b;
static int attribute_count;

// Note - not re-entrant. Should fix that sometime
static void process_start(GMarkupParseContext *context,
                          const gchar *element_name,
                          const gchar **attribute_names,
                          const gchar **attribute_values,
                          gpointer user_data,
                          GError **error)
{
	Tool *t = (Tool *)user_data;

	if (tags.size() == 0 && std::string(element_name) == std::string("tool"))
	{
		for(int i=0; attribute_names[i] != NULL; i++)
		{
			if (attribute_names[i] == std::string("description"))
			{
				t->description = std::string(attribute_values[i]);
			}
		}
	} else if (tags.top() == std::string("tool") && std::string(element_name) == std::string("nodes")) {
		// This is just a container
	} else if (tags.top() == std::string("tool") && std::string(element_name) == std::string("bindings")) {
		// This is just a container
	} else if (tags.top() == std::string("nodes") && std::string(element_name) == std::string("node")) {
		std::string name;
		std::string type;
		for(int i=0; attribute_names[i] != NULL; i++)
		{
			if (attribute_names[i] == std::string("name"))
			{
				name = attribute_values[i];
			} else if (attribute_names[i] == std::string("type")) {
				type = attribute_values[i];
			}
		}

		if (name == std::string("") || type == std::string(""))
		{
			// This should be a recoverable error
			assert(0);
			return;
		}

		t->nodes.push_back(Tool::NodeData(name, type));
	} else if (tags.top() == std::string("bindings") && std::string(element_name) == std::string("binding")) {
		std::string interface;
		for(int i=0; attribute_names[i] != NULL; i++)
		{
			if (attribute_names[i] == std::string("type"))
			{
				b.type = attribute_values[i];
			} else if (attribute_names[i] == std::string("interface")) {
				interface = attribute_values[i];
			} else if (attribute_names[i] == std::string("value")) {
				b.value = attribute_values[i];
			}
		}

		if (b.type == std::string(""))
		{
			// This should be a recoverable error
			assert(0);
			return;
		}

		if (interface != std::string(""))
		{
			t->interface.push_back(Tool::NodeData(interface, b.type));
			b.attributes.push_back(make_pair(std::string("Interface"), interface));
		}
	} else if (tags.top() == std::string("binding") && std::string(element_name) == std::string("member")) {
		std::string node;
		std::string attribute;
		for(int i=0; attribute_names[i] != NULL; i++)
		{
			if (attribute_names[i] == std::string("node"))
			{
				node = attribute_values[i];
			} else if (attribute_names[i] == std::string("attribute")) {
				attribute = attribute_values[i];
			}
		}

		if (node == std::string("") || attribute == std::string(""))
		{
			// This should be a recoverable error
			assert(0);
			return;
		}

		b.attributes.push_back(make_pair(node, attribute));
	} else {
		// This should be a recoverable error
		assert(0);
	}

	tags.push(element_name);
}

static void process_end(GMarkupParseContext *context, const gchar *element_name, gpointer user_data, GError **error)
{
	Tool *t = (Tool *)user_data;

	if (std::string(element_name) == "binding")
	{
		t->bindings.push_back(b);
		b.attributes.clear();
		b.type="";
		b.value="";
	}

	tags.pop();
}

GMarkupParser p =
{
	process_start,
	process_end,
	NULL,
	NULL,
	NULL
};

Tool::Tool(const std::string &basename, const std::string &filename)
{
	filebuf fb;
	try
	{
		// Initialize headers
		name = filename;
        icon = basename + std::string("images/") + filename + std::string(".png");

		GError *err = NULL;
		char buffer[100];
		GMarkupParseContext *pc = g_markup_parse_context_new(&p, (GMarkupParseFlags)0, this, NULL);

		fb.open((basename+std::string("tools/")+filename).c_str(), ios::in);
		attribute_count = 0;
		istream input(&fb);

		while(!input.eof())
		{
			input.read(buffer, 100);
			g_markup_parse_context_parse(pc, buffer, input.gcount(), &err);
		}
		g_markup_parse_context_free(pc);

		// Clean up from any invalid XML files
		if( !tags.empty() ) {
			stringstream ss;
			ss << "Missing close tag \"";
			ss << tags.top();
			ss << "\"";
			while(!tags.empty()) tags.pop();
			throw std::runtime_error(ss.str());
		}

		fb.close();
	} catch (ios_base::failure e) {
		// Read error occured
		cout << "IO error occured" << endl;
	} catch (const std::runtime_error &e) {
		cout << "Runtime error parsing " << filename << ": " << e.what() << std::endl;
	}
}

bool Tool::save(const std::string &filename, const Document *doc)
{
	gchar *s;
	const gchar *base = g_get_user_data_dir();
	string path = std::string(base) + std::string("/applications/") + std::string(PACKAGE_NAME) + std::string("/tools/");
	
	// Make sure the parent directory exist
	g_mkdir_with_parents( path.c_str(), 0755 );
	
	ofstream out(( path + filename).c_str());

	if( !out.good() ) {
		// See if the directory exists
		std::cerr << "Could not output to " << path << filename << std::endl;

		return false;
	}

	out << "<?xml version=\"1.0\"?>" << endl;

	s = g_markup_printf_escaped("<tool description=\"%s\">", filename.c_str());
	out << s << endl;
	g_free(s);

	// Count the primitives
	set<Binding *> b;
	map<int, Node *> nodes;
	map<int, Binding *> bindings;
	map<int, Interface *> interfaces;
	map<Node *, int> rnodes;
	map<Binding *, int> rbindings;

	foreach(i, doc->getNodes())
	{
		int r;

		do
		{
			r = rand();
		} while(interfaces.find(r) != interfaces.end());

		interfaces[r] = *i;

		foreach(j, (*i)->nodes)
		{
			int r;

			do
			{
				r = rand();
			} while(nodes.find(r) != nodes.end());

			nodes[r] = *j;
			rnodes[*j] = r;
		}

		(*i)->getBindings(b);
	}

	foreach(i, b)
	{
		int r;

		do
		{
			r = rand();
		} while(bindings.find(r) != bindings.end());

		bindings[r] = *i;
		rbindings[*i] = r;
	}

	out << "\t<nodes>" << endl;
	foreach(i, nodes)
	{
		stringstream ss1,ss2;
		ss1 << i->first;
		ss2 << i->second->type->name;
		s = g_markup_printf_escaped("\t\t<node name=\"%s\" type=\"%s\"/>", ss1.str().c_str(), ss2.str().c_str());
		out << s <<  endl;
		g_free(s);
	}
	out << "\t</nodes>" << endl;

	out << "\t<bindings>" << endl;
	foreach(i, bindings)
	{
		Binding *b = i->second;

		out << "\t\t<binding";

		s = g_markup_printf_escaped(" type=\"%s\"", b->type->name.c_str());
		out << s;
		g_free(s);

		if (b->hasSetValue() || !b->hasValue())
		{
			stringstream ss;
			ss << attribute_count++;
			s = g_markup_printf_escaped(" interface=\"%s\"", (b->type->name+ss.str()).c_str());
			out << s;
			g_free(s);
		}

		if (i->second->hasSetValue())
		{
			s = g_markup_printf_escaped(" value=\"%s\"", i->second->getValue()->getString().c_str());
			out << s;
			g_free(s);
		}

		out << ">" << endl;

		foreach(j, b->attributes)
		{
			if ((*j)->getNode() != NULL)
			{
				stringstream ss1,ss2;
				ss1 << rnodes[(*j)->getNode()];
				ss2 << (*j)->getName();
				s = g_markup_printf_escaped("\t\t\t<member node=\"%s\" attribute=\"%s\"/>",
				ss1.str().c_str(), ss2.str().c_str());
				out << s << endl;
				g_free(s);
			}
		}

		out << "\t\t</binding>" << endl;
	}
	out << "\t</bindings>" << endl;
	out << "</tool>" << endl;
	out.close();

	return true;
}

const std::string &Tool::getName()
{
	return name;
}
