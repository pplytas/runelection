#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"


int get_optimal_bf_size(int number) {
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


void insert_records(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL, FILE *fp) {
    int lines_count = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_length;
    char *tokens[6];
    RedBlackNode *new_node;

    while ((line_length = getline(&line, &len, fp)) != -1) {
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        tokenize_string(line, tokens);

        bloom_add(BF, tokens[0], strlen(tokens[0]));
        new_node = rbt_insert(RBT, tokens[0], tokens[5]);
        pcl_insert(PCL, new_node);

        // printf("Key: %s\n", tokens[0]);
        // printf("Name: %s\n", tokens[1]);
        // printf("Surname: %s\n", tokens[2]);
        // printf("Age: %s\n", tokens[3]);
        // printf("Gender: %s\n", tokens[4]);
        // printf("Postcode: %s\n", tokens[5]);
        // printf("\n");
    }

    if (line != NULL) {
        free(line);
    }
}


void listen_for_commands(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL){
    size_t len = 0;
    ssize_t nread;
    char *line = NULL, *token;
    RedBlackNode *found_node;

    while(1){
        nread = getline(&line, &len, stdin);
        if (line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        if(!strcmp(line, "exit")) break;

        token = strtok(line, " \t");
        printf("%s\n", token);
        if(token != NULL){
            if(strcmp(token, "lbf") == 0){
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    if (bloom_check(*BF, token, strlen(token))) {
                        printf("KEY %s POSSIBLY-IN REGISTRY\n", token);
                    } else {
                        printf("KEY %s Not-in-LBF\n", token);
                    }
                }
            }
            else if(strcmp(token, "lrb") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
                if (token != NULL) {
                    found_node = rbt_find_node_by_key(*RBT, token);
                    if (found_node != NULL) {
                        printf("KEY %s FOUND-IN-RBT\n", token);
                    } else {
                        printf("KEY %s NOT-IN-RBT\n", token);
                    }
                }
            }
            else if(strcmp(token, "ins record") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "find key") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "delete key") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "vote key") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "load fileofkeys") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "voted") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "voted postcode") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "votedperpc") == 0){
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else{
                printf("Command not recognized\n");
            }
        }
    }

    free(line);
}
