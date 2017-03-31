#include "unicorn.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

/*
 * REPL.C
 *
 * All the main REPL functionality of Unicorn Shell
 */

// 2^16 will be our maximum command length (if not reading from stdin)
#define COMMAND_LENGTH 65536

char command_buffer[COMMAND_LENGTH];

/* 
 * read_input 
 * returns 0 on error, 1 on success.
 */
int read_input(FILE *input, char **output) {
    int index = 0;
    if (input == stdin) {
        char *line = readline(get_prompt());

        // We encountered an EOF
        if (!line) {
            error = ENCOUNTERED_EOF;
            return 0;
        }

        if (strlen(line) != 0) {
            add_history(line);
        }

        *output = line;
    }
    else {
        int c = fgetc(input);

        while(c != EOF && c != '\n' && c != ';') {
            command_buffer[index] = c;
            index++;

            if (index >= COMMAND_LENGTH) {
                // ERROR: Command too long
                error = COMMAND_TOO_LONG;
                return 0;
            }

            c = fgetc(input);
        }

        if (c == EOF) {
            // ERROR: User entered EOF, quit.
            error = ENCOUNTERED_EOF;
            return 0;
        }

        // Done reading, add null character
        command_buffer[index] = '\0';
        *output = command_buffer;
    }

    // The first character is \0 (aka line is blank)
    if (!**output) {
        error = EMPTY_LINE;
        return 0;
    }

    return 1;
}

/* 
 * parse_input
 * Loops over each character, adding \0 where the spaces are
 * to turn the char array into an array of char arrays (i.e. args)
 * Returns num arguments
 */
int parse_input(char *buffer, int length, command* command) {   
    int numArgs = 0;    // Argument counter
    int quote = 0;      // Quote state (0 = not quote, 1 = quotes)
    int isArg = 1;      // Arument state ( 0 = not arg, 1 = is arg)
    int i = 0;          // Loop counter

    char *currToken = buffer;

    // Loops over chars in buffer and adds '\0'
    for(i = 0; i < length; i++) {
        if (buffer[i] == '"') {
            // Handling quotes
            quote = !quote;
            buffer[i] == '\0';
            numArgs += isArg;
        }
        else if (buffer[i] == ' ' && quote == 0) {
            // Handling spaces outside of quotes
            buffer[i] == '\0';
            numArgs += isArg; 
        }
        else if (buffer[i] == '<' || buffer[i] == '>') {
            // No more arguments
            isArg = 0;
        }
        // Otherwise, just continue
    }

    // Check for invalid quotes
    if (quote == 1) {
        // ERROR: Invalid quotes
    }

    // Allocate space for args
    command->arguments = malloc(sizeof(char*) * (numArgs + 1));

    // Assign tokens to command variables



    return 0;
}

/*
 * execute
 * 
 */
int execute(command* command) {
    return 0;
}
