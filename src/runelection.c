#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities.h"
#include "election_utilities.h"


int main(int argc, char *argv[]) {
    char *cl_arguments[3] = {NULL, NULL, NULL};
    char *inputfile, *outfile;
    int numofupdates;

    BloomFilter BF;
    RedBlackTree RBT;
    PostCodeList PCL;

    int lines_count;
    FILE *infile;

    // Get command line arguments
    get_command_line_arguments(argc, argv, cl_arguments);
    inputfile = cl_arguments[0];
    outfile = cl_arguments[1];
    if (cl_arguments[2] != NULL) {
        numofupdates = atoi(cl_arguments[2]);
    } else {
        numofupdates = 5;       // Default value
    }

    // Init data structures
    rbt_init(&RBT);
    pcl_init(&PCL);

    if (inputfile != NULL) {
        infile = fopen(inputfile, "r");
        check_errors(infile, "fopen", 1);

        lines_count = get_lines_count(infile);          // Get number of records in inputfile
        bloom_init(&BF, lines_count, numofupdates);                       // Init BF

        insert_records(&BF, &RBT, &PCL, infile);        // Insert records to data structures

        fclose(infile);
    } else {
        bloom_init(&BF, 307, numofupdates);
    }

    // Print data structures
    // bloom_print(BF);
    // pcl_print(PCL);
    rbt_print(RBT);

    listen_for_commands(&BF, &RBT, &PCL);       // Listen for user input

    if (outfile != NULL) {
        write_new_registry(RBT, outfile);               // Store new registry in outfile
    }

    // Free allocated memory of data structures
    pcl_free(PCL);
    rbt_free(RBT);
    bloom_free(BF);

    return 0;
}
