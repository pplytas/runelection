#ifndef UTILITIES_H
#define UTILITIES_H

void check_errors(void *, char *, int);
void get_command_line_arguments(int, char *[], char *[3]);
int get_optimal_bf_size(int);
int get_lines_count(FILE *);
int get_record_info_from_tokenized_string(char *, char *[6]);
int get_record_info(char *, char *[6]);

#endif
