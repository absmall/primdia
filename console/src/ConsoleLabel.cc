#include <readline/readline.h>
#include <iostream>
#include <stdlib.h>

#include <Label.h>
#include <Binding.h>
#include <View.h>
#include <Document.h>
#include <LabelType.h>

using namespace std;

class ConsoleLabelType : public LabelType
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

void ConsoleLabelType::input(View *v, Binding *b) const
{
	char *line = readline(_("Please enter text"));

	if (line != NULL ) {
		if (!b->setValue(v->document, new Label(line)))
		{
			cerr << _("The binding could not be set. It overconstrains an object in the diagram.") << endl;
		}
        free(line);
	}
}
