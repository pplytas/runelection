#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"


int get_prime_number(int number) {
    int i, flag = 1;
    int min_prime;

    while(flag == 1) {
        flag = 0;
        for(i = 2; i < (number / 2); i++) {
            if(number % i == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            min_prime = number;
        }

        number++;
    }

    return min_prime;
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


void tokenize_string(char *string, char *array[6]) {
    int i;
    char *token_ptr;

    i = 0;
    token_ptr = strtok(string, " ");
    while (token_ptr != NULL) {
        array[i] = token_ptr;
        token_ptr = strtok(NULL, " ");
        i++;
    }
}


void insert_records(FILE *fp, RedBlackTree *RBT) {
    int lines_count = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_length;
    char *tokens[6];

    while ((line_length = getline(&line, &len, fp)) != -1) {
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        tokenize_string(line, tokens);

        rbt_insert(RBT, tokens[0]);

        // printf("Key: %s\n", tokens[0]);
        // printf("Name: %s\n", tokens[1]);
        // printf("Surname: %s\n", tokens[2]);
        // printf("Age: %s\n", tokens[3]);
        // printf("Gender: %s\n", tokens[4]);
        // printf("Postal Code: %s\n", tokens[5]);
        // printf("\n");
    }

    if (line != NULL) {
        free(line);
    }
}
