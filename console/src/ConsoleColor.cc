#include <math.h>
#include <stdlib.h>
#include <iostream>

#include <Color.h>
#include <Binding.h>
#include <View.h>
#include <Document.h>
#include <ColorType.h>
#include <readline/readline.h>

using namespace std;

class GtkColorType : public ColorType
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

void GtkColorType::input(View *v, Binding *b) const
{
    int i;
    char *line;
    int rgba[4];
    char prompt[20];
    const char *colors[] = {"red", "green", "blue", "alpha"};

    for(i=0; i < 4; i ++ ) {
        snprintf(prompt, sizeof(prompt), "Enter %s value", colors[i]);
        line = readline(prompt);
        if( line ) {
            if(!*line) {
                free(line);
                return;
            }
        }
        rgba[i] = atoi(line);
        free(line);
    }
    if (!b->setValue(v->document, new Color(rgba[0], rgba[1], rgba[2], rgba[3])))
    {
        cerr << _("The binding could not be set. It overconstrains an object in the diagram.") << endl;
    }
}
