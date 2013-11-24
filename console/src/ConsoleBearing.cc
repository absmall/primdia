#include <stdlib.h>
#include <iostream>
#include <View.h>
#include <Binding.h>
#include <Bearing.h>
#include <Document.h>
#include <BearingType.h>
#include <readline/readline.h>

class ConsoleBearingType : public BearingType
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

void ConsoleBearingType::input(View *v, Binding *b) const
{
    char *line = readline(_("Please enter bearing"));

    if(line != NULL ) {
        if( *line ) {
            if (!b->setValue(v->document, new Bearing(atoi(line))))
            {
                std::cerr << _("The binding could not be set. It overconstrains an object in the diagram.") << std::endl;
            }
        }
        delete line;
    }

}
