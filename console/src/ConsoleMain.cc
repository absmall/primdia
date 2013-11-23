#include <iostream>
#include <string>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <Primitive.h>

using namespace std;

bool help(char *args);

bool quit(char *args)
{
	return false;
}

struct command
{
	const char *name;
	bool (*parser)(char *args);
} commands[] = {
	{ "help", help },
	{ "exit", quit },
	{ "quit", quit }
};

bool help(char *args)
{
    int i;
    printf("Available commands:\n");
    for(i = 0; i < sizeof(commands)/sizeof(command); i ++ ) {
        printf("%s\n", commands[i]);
    }
    return true;
}

bool process_command(char *command)
{
	int i;
	int length;

	for( i = 0; i < sizeof(commands)/sizeof(commands[0]); i ++ ) {
		length = strlen(commands[i].name);
		if( !strncmp(commands[i].name, command, length)
				&& (isspace( command[ length ])
		         || command[ length ] == 0)) {
			return commands[ i ].parser( command + length );
		}
	}

	// Command not found
	printf("Unknown command: %s\n", command);

	return true;
}

void process_commands()
{
	bool done = false;
	char *response;
	while( !done ) {
		response = readline("primdia>");
		if( response ) {
			if (process_command (response)) {
                if( *response ) {
                    add_history(response);
                }
            } else {
                done = true;
            }
			free( response );
		} else {
			done = true;
			printf("\n");
		}
	}
}

int main(int argc, char *argv[])
{
#if ENABLE_NLS
	setlocale(LC_ALL, "");
	textdomain (PACKAGE_NAME);
	bindtextdomain (PACKAGE_NAME, LOCALEDIR);
#endif

	try
	{
		Primitive::InitializeAll();

		// Enter reading loop
		process_commands();
	} catch(string message) {
		cout << message << endl;
	}

	return 0;
}
