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

    char *key, *lastname, *firstname, *postcode;
    int age;
    char gender;

    while ((line_length = getline(&line, &len, fp)) != -1) {
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        tokenize_string(line, tokens);

        key = tokens[0];
        lastname = tokens[1];
        firstname = tokens[2];
        age = atoi(tokens[3]);
        gender = tokens[4][0];
        postcode = tokens[5];

        bloom_add(BF, tokens[0], strlen(tokens[0]));
        new_node = rbt_insert(RBT, key, lastname, firstname, age, gender, postcode);
        pcl_insert(PCL, new_node);

        // printf("Key: %s\n", key);
        // printf("Name: %s\n", lastname);
        // printf("Surname: %s\n", firstname);
        // printf("Age: %d\n", age);
        // printf("Gender: %c\n", gender);
        // printf("Postcode: %s\n", postcode);
        // printf("\n");
    }

    if (line != NULL) {
        free(line);
    }
}


RedBlackNode* find_key(BloomFilter BF, RedBlackTree RBT, char key[9]) {
    RedBlackNode *found_node;

    if (bloom_check(BF, key, strlen(key))) {  // First check if key in BF
        found_node = rbt_find_node_by_key(RBT, key);  // Then check in RBT
    } else {
        found_node = NULL;
    }

    return found_node;
}


void listen_for_commands(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL){
    size_t len = 0;
    ssize_t nread;
    char *line = NULL, *token;
    RedBlackNode *found_node;
    int updated;

    while(1){
        nread = getline(&line, &len, stdin);
        if (line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        if(!strcmp(line, "exit")) break;

        token = strtok(line, " \t");
        printf("%s\n", token);
        if(token != NULL){
            if(strcmp(token, "lbf") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    if (bloom_check(*BF, token, strlen(token))) {
                        printf("KEY %s POSSIBLY-IN REGISTRY\n", token);
                    } else {
                        printf("KEY %s Not-in-LBF\n", token);
                    }
                }
            }
            else if(strcmp(token, "lrb") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    if (rbt_find_node_by_key(*RBT, token) != NULL) {
                        printf("KEY %s FOUND-IN-RBT\n", token);
                    } else {
                        printf("KEY %s NOT-IN-RBT\n", token);
                    }
                }
            }
            else if(strcmp(token, "ins record") == 0) {
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "find") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    found_node = find_key(*BF, *RBT, token);
                    if (found_node != NULL) {
                        printf("REC-IS: %s %s %s %s %d\n", found_node->key, found_node->firstname, found_node->lastname, found_node->postcode, found_node->age);
                    } else {
                        printf("REC-WITH %s NOT-in-structs\n", token);
                    }
                }
            }
            else if(strcmp(token, "delete key") == 0) {
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "vote") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    found_node = find_key(*BF, *RBT, token);
                    if (found_node != NULL) {
                        updated = rbt_update_node_has_voted(found_node, 1);
                        if (updated) {
                            printf("REC-WITH %s SET-VOTED\n", token);
                        } else {
                            printf("REC-WITH %s ALREADY-VOTED\n", token);
                        }
                    } else {
                        printf("REC-WITH %s NOT-in-structs\n", token);
                    }
                }
            }
            else if(strcmp(token, "load fileofkeys") == 0) {
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "voted") == 0) {
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "voted postcode") == 0) {
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else if(strcmp(token, "votedperpc") == 0) {
                token = strtok(NULL, " \t");
                printf("%s\n", token);
            }
            else {
                printf("Command not recognized\n");
            }
        }
    }

    free(line);
}
