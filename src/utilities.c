#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"
#include "stack.h"


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


int insert_key(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL, char *record_info[6]) {
    char *key, *lastname, *firstname, *postcode;
    int age;
    char gender;
    RedBlackNode *new_node;

    key = record_info[0];
    lastname = record_info[1];
    firstname = record_info[2];
    age = atoi(record_info[3]);
    gender = record_info[4][0];
    postcode = record_info[5];

    // printf("Key: %s\n", key);
    // printf("Lastname: %s\n", lastname);
    // printf("Firstname: %s\n", firstname);
    // printf("Age: %d\n", age);
    // printf("Gender: %c\n", gender);
    // printf("Postcode: %s\n", postcode);
    // printf("\n");

    bloom_add(BF, key, strlen(key));

    new_node = rbt_insert(RBT, key, lastname, firstname, age, gender, postcode);
    if (new_node == NULL) {
        return 0;       // key already exists
    }

    pcl_insert(PCL, new_node);
    return 1;           // key inserted successfully
}


void insert_records(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL, FILE *fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t line_length;
    char *record_info[6];
    int response;

    while ((line_length = getline(&line, &len, fp)) != -1) {
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        response = get_record_info(line, record_info);
        if (response) {
            response = insert_key(BF, RBT, PCL, record_info);
            if (response) {
                printf("# REC-WITH %s INSERTED-IN-BF-RBT\n", record_info[0]);
            } else {
                printf("- REC-WITH %s EXISTS\n", record_info[0]);
            }
        }
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

int vote_key(BloomFilter BF, RedBlackTree *RBT, PostCodeList PCL, char key[9]) {
    RedBlackNode *found_node;
    PostCodeNode *found_pcl_node;
    int updated;

    found_node = find_key(BF, *RBT, key);
    if (found_node == NULL) {
        return -1;      // key not found
    }

    updated = rbt_update_has_voted(RBT, found_node, 1);
    if (!updated) {
        return 0;       // key already voted
    }

    found_pcl_node = pcl_find_node_by_postcode(PCL, found_node->postcode);
    vl_increase_have_voted_count(&(found_pcl_node->VL), 1);
    return 1;           // key voted successfully
}


void load_fileofkeys(BloomFilter BF, RedBlackTree *RBT, PostCodeList PCL, char *filepath) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_length;
    int response;

    fp = fopen(filepath, "r");
    check_errors(fp, "fopen", 0);
    if (fp == NULL) return;

    while ((line_length = getline(&line, &len, fp)) != -1) {
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }

        response = vote_key(BF, RBT, PCL, line);
        if (response == 1) {
            printf("# REC-WITH %s SET-VOTED\n", line);
        } else if (response == 0) {
            printf("# REC-WITH %s ALREADY-VOTED\n", line);
        } else {
            printf("- REC-WITH %s NOT-in-structs\n", line);
        }
    }

    if (line != NULL) {
        free(line);
    }

    fclose(fp);
}


void listen_for_commands(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL){
    size_t len = 0;
    ssize_t nread;
    char *line = NULL, *token;
    char *record_info[6];
    RedBlackNode *found_rbt_node;
    PostCodeNode *found_pcl_node;
    int response;

    while(1){
        nread = getline(&line, &len, stdin);
        if (line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }

        if(strcmp(line, "exit") == 0) break;

        token = strtok(line, " \t");
        if(token != NULL){
            if(strcmp(token, "lbf") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    if (bloom_check(*BF, token, strlen(token))) {
                        printf("# KEY %s POSSIBLY-IN REGISTRY\n", token);
                    } else {
                        printf("# KEY %s Not-in-LBF\n", token);
                    }
                }
            }
            else if(strcmp(token, "lrb") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    if (rbt_find_node_by_key(*RBT, token) != NULL) {
                        printf("# KEY %s FOUND-IN-RBT\n", token);
                    } else {
                        printf("# KEY %s NOT-IN-RBT\n", token);
                    }
                }
            }
            else if(strcmp(token, "ins") == 0) {
                token = strtok(NULL, " \t");
                response = get_record_info_from_tokenized_string(token, record_info);
                if (response) {
                    response = insert_key(BF, RBT, PCL, record_info);
                    if (response) {
                        printf("# REC-WITH %s INSERTED-IN-BF-RBT\n", record_info[0]);
                    } else {
                        printf("- REC-WITH %s EXISTS\n", record_info[0]);
                    }
                }
            }
            else if(strcmp(token, "find") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    found_rbt_node = find_key(*BF, *RBT, token);
                    if (found_rbt_node != NULL) {
                        printf("# REC-IS: %s %s %s %s %d\n", found_rbt_node->key, found_rbt_node->firstname, found_rbt_node->lastname, found_rbt_node->postcode, found_rbt_node->age);
                    } else {
                        printf("# REC-WITH %s NOT-in-structs\n", token);
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
                    response = vote_key(*BF, RBT, *PCL, token);
                    if (response == 1) {
                        printf("# REC-WITH %s SET-VOTED\n", token);
                    } else if (response == 0) {
                        printf("# REC-WITH %s ALREADY-VOTED\n", token);
                    } else {
                        printf("- REC-WITH %s NOT-in-structs\n", token);
                    }
                }
            }
            else if(strcmp(token, "load") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    load_fileofkeys(*BF, RBT, *PCL, token);
                }
            }
            else if(strcmp(token, "voted") == 0) {
                token = strtok(NULL, " \t");
                if (token == NULL) {
                    printf("# NUMBER %d\n", RBT->have_voted_count);
                } else {
                    found_pcl_node = pcl_find_node_by_postcode(*PCL, token);
                    if (found_pcl_node != NULL) {
                        printf("# IN %s VOTERS-ARE %d\n", token, (found_pcl_node->VL).have_voted_count);
                    } else {
                        printf("# IN %s VOTERS-ARE 0\n", token);
                    }
                }
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


void write_new_registry(RedBlackTree RBT, char *filepath) {
    FILE *fp;
    RedBlackNode *rbt_node;
    Stack S;

    fp = fopen(filepath, "w");
    check_errors(fp, "fopen", 0);

    stack_init(&S);
    stack_push(&S, RBT.root);
    while (!stack_is_empty(S)) {
        rbt_node = stack_pop(&S);

        if (rbt_node->left != NULL) {
            stack_push(&S, rbt_node->left);
        }
        if (rbt_node->right != NULL) {
            stack_push(&S, rbt_node->right);
        }

        fprintf(fp, "%s  %s %s %d  %c %s\n", rbt_node->key, rbt_node->lastname, rbt_node->firstname, rbt_node->age, rbt_node->gender, rbt_node->postcode);
    }

    fclose(fp);
}
