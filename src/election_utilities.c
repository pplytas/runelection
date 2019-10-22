#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "election_utilities.h"
#include "utilities.h"
#include "stack.h"


void refill_bf(BloomFilter *BF, RedBlackTree RBT) {
    Stack S;
    RedBlackNode *rbt_node;

    stack_init(&S);
    if (RBT.root != NULL) {
        stack_push(&S, RBT.root);
    }
    while (!stack_is_empty(S)) {
        rbt_node = stack_pop(&S);

        if (rbt_node->right != NULL) {
            stack_push(&S, rbt_node->right);
        }
        if (rbt_node->left != NULL) {
            stack_push(&S, rbt_node->left);
        }

        bloom_add(BF, rbt_node->key);
    }
}


int insert_key(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL, char *record_info[6], int insert_from_file) {
    RedBlackNode *new_node;
    int needs_rebuild;

    char *key = record_info[0];
    char *lastname = record_info[1];
    char *firstname = record_info[2];
    int age = atoi(record_info[3]);
    char gender = record_info[4][0];
    int postcode = atoi(record_info[5]);

    bloom_add(BF, key);
    new_node = rbt_insert(RBT, key, lastname, firstname, age, gender, postcode);
    if (new_node == NULL) return 0;  // key already exists

    if (!insert_from_file) {    // If the records are inserted from the input file then BF already has the appropriate size, no need to rebuild
        needs_rebuild = bloom_increase_updates_count_and_check(BF);
        if (needs_rebuild) {
            bloom_print(*BF);
            printf("%d\n", RBT->count);
            bloom_reinit(BF, RBT->count);
            bloom_print(*BF);
            refill_bf(BF, *RBT);
            bloom_print(*BF);
        }
    }

    pcl_insert(PCL, new_node);
    return 1;  // key inserted successfully
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
            response = insert_key(BF, RBT, PCL, record_info, 1);
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


RedBlackNode* find_key(BloomFilter BF, RedBlackTree RBT, char *key) {
    RedBlackNode *found_node;

    if (bloom_check(BF, key)) {  // First check if key in BF
        found_node = rbt_find_node_by_key(RBT, key);  // Then check in RBT
    } else {
        found_node = NULL;
    }

    return found_node;
}

int vote_key(BloomFilter BF, RedBlackTree *RBT, PostCodeList PCL, char *key) {
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


int delete_key(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL, char *key) {
    int needs_rebuild;
    RedBlackNode *found_node = rbt_find_node_by_key(*RBT, key);

    if (found_node == NULL) return 0;   // key not found

    pcl_remove(PCL, found_node->postcode, found_node->key);

    rbt_delete_node(RBT, found_node);
    rbt_print(*RBT);

    needs_rebuild = bloom_increase_updates_count_and_check(BF);
    if (needs_rebuild) {
        bloom_print(*BF);
        printf("%d\n", RBT->count);
        bloom_reinit(BF, RBT->count);
        bloom_print(*BF);
        refill_bf(BF, *RBT);
        bloom_print(*BF);
    }

    return 1;   // key deleted successfully
}


void listen_for_commands(BloomFilter *BF, RedBlackTree *RBT, PostCodeList *PCL){
    size_t len = 0;
    ssize_t nread;
    char *line = NULL, *token;
    char *record_info[6];
    RedBlackNode *found_rbt_node;
    PostCodeNode *found_pcl_node;
    int response, postcode;

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
                    if (bloom_check(*BF, token)) {
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
                    response = insert_key(BF, RBT, PCL, record_info, 0);
                    if (response) {
                        printf("# REC-WITH %s INSERTED-IN-BF-RBT\n", record_info[0]);
                    } else {
                        printf("- REC-WITH %s EXISTS\n", record_info[0]);
                    }
                } else {
                    printf("One or more keys are missing\n");
                }
            }
            else if(strcmp(token, "find") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    found_rbt_node = find_key(*BF, *RBT, token);
                    if (found_rbt_node != NULL) {
                        printf("# REC-IS: %s %s %s %d %c %d\n", found_rbt_node->key, found_rbt_node->firstname, found_rbt_node->lastname, found_rbt_node->age, found_rbt_node->gender, found_rbt_node->postcode);
                    } else {
                        printf("# REC-WITH %s NOT-in-structs\n", token);
                    }
                }
            }
            else if(strcmp(token, "delete") == 0) {
                token = strtok(NULL, " \t");
                if (token != NULL) {
                    response = delete_key(BF, RBT, PCL, token);
                    if (response) {
                        printf("# DELETED %s FROM-structs\n", token);
                    } else {
                        printf("- KEY %s NOT-in-structs\n", token);
                    }
                }
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
                    postcode = atoi(token);
                    if (postcode > 0) {
                        found_pcl_node = pcl_find_node_by_postcode(*PCL, postcode);
                        if (found_pcl_node != NULL) {
                            printf("# IN %d VOTERS-ARE %d\n", postcode, (found_pcl_node->VL).have_voted_count);
                        } else {
                            printf("# IN %d VOTERS-ARE 0\n", postcode);
                        }
                    } else {
                        printf("Invalid postcode format (only digits allowed)\n");
                    }
                }
            }
            else if(strcmp(token, "votedperpc") == 0) {
                pcl_print_voted_per_postcode(*PCL);
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
    if (fp == NULL) return;

    stack_init(&S);

    if (RBT.root != NULL) {
        stack_push(&S, RBT.root);
    }
    while (!stack_is_empty(S)) {
        rbt_node = stack_pop(&S);

        if (rbt_node->right != NULL) {
            stack_push(&S, rbt_node->right);
        }
        if (rbt_node->left != NULL) {
            stack_push(&S, rbt_node->left);
        }

        fprintf(fp, "%s  %s %s %d  %c %d\n", rbt_node->key, rbt_node->lastname, rbt_node->firstname, rbt_node->age, rbt_node->gender, rbt_node->postcode);
    }

    fclose(fp);
}
