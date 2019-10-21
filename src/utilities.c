#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"


void check_errors(void *response, char *error, int will_exit) {
    if (response == NULL){
        perror(error);
        if (will_exit) {
            exit(EXIT_FAILURE);
        }
    }
}


void get_command_line_arguments(int argc, char *argv[], char *arguments[3]) {
    for (int i = 1; i < (argc - 1); i++) {
        if ((strcmp(argv[i], "-i") == 0) && ((strcmp(argv[i+1], "-o") != 0)) && ((strcmp(argv[i+1], "-n") != 0))) {
            arguments[0] = argv[i+1];
        } else if (strcmp(argv[i], "-o") == 0 && ((strcmp(argv[i+1], "-i") != 0)) && ((strcmp(argv[i+1], "-n") != 0))) {
            arguments[1] = argv[i+1];
        } else if (strcmp(argv[i], "-n") == 0 && ((strcmp(argv[i+1], "-i") != 0)) && ((strcmp(argv[i+1], "-o") != 0))) {
            arguments[2] = argv[i+1];
        }
    }
}


int get_lines_count(FILE *fp) {
    int lines_count = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_length;

    while ((line_length = getline(&line, &len, fp)) != -1) {
        if(line_length > 0) {      // Count only non empty lines
            lines_count++;
        }
    }

    fseek(fp, 0, SEEK_SET);     // Move the file pointer to the start.

    if (line != NULL) {
        free(line);
    }

    return lines_count;
}


int get_record_info_from_tokenized_string(char *tokenized_string, char *array[6]) {
    int i;
    for (i = 0; i < 6; i++) {
        array[i] = tokenized_string;
        tokenized_string = strtok(NULL, " ");
        if (tokenized_string == NULL && i < 5) return 0;    // Some info missing
    }
    return 1;
}


int get_record_info(char *string, char *array[6]) {
    char *tokenized_string = strtok(string, " ");
    return get_record_info_from_tokenized_string(tokenized_string, array);
}
