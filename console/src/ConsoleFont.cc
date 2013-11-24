#include <iostream>
#include <stdlib.h>

#include <Font.h>
#include <Binding.h>
#include <View.h>
#include <Document.h>
#include <FontType.h>
#include <readline/readline.h>

using namespace std;

class ConsoleFontType : public FontType
{
public:
	/**
	 * Get a value from the user
	 * @param v A view to be used in some
	 *          way to retrieve a value.
	 * @param b A binding to initialize with the value.
	 */
	void input(View *v, Binding *b) const;
} static instance;

void ConsoleFontType::input(View *v, Binding *b) const
{
	char *line = readline(_("Please choose font"));

	if (line != NULL)
	{
		if (!b->setValue(v->document, new Font(line)))
		{
			cerr << _("The binding could not be set. It overconstrains an object in the diagram.") << endl;
		}
        free(line);
	}
}
