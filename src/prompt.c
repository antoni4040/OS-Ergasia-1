/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is the implementation of command prompt.
*/

#include "prompt.h"

int commandPrompt(electionManager* manager) {
    char* command;
    char* line;
    size_t len = 0;

    putchar('~');
    putchar(' ');
    while(getline(&line, & len, stdin) != EOF) {
        line = strtok(line, "\n");
        command = strtok(line, " ");

        if(command == NULL) {       // If no input is given:
            putchar('~');
            putchar(' ');
            continue;
        }

        printf("%s\n", command);

        if(strcmp(command, "exit") == 0) {
            return 0;
        }
        else if(strcmp(command, "lbf") == 0) {
            char* key = strtok(NULL, " ");
            if(key == NULL) {
                printf("No key given. Nothing to do.\n");
            }
            else {
                bool perhapsExists = searchBloomFilter(
                    manager->bloomFilter, key, strlen(key));
                if(perhapsExists) {
                    printf("KEY %s POSSIBLY-IN REGISTRY\n", key);
                }
                else {
                    printf("KEY %s Not-in-LBF\n", key);
                }
            }
        }
        else if(strcmp(command, "lrb") == 0) {
            char* key = strtok(NULL, " ");
            if(key == NULL) {
                printf("No key given. Nothing to do.\n");
            }
            else {
                voter* spectreVoter = initializeVoter(key, "name", "surname", 10, MALE, 11111);
                node* spectreNode = initializeNode(spectreVoter);
                node* findInRBT = RBTSearch(manager->redBlackTree, spectreNode);
                if(findInRBT != NULL) {
                    printf("KEY %s FOUND-IN-RBT\n", key);
                }
                else {
                    printf("KEY %s NOT-IN-RBT\n", key);
                }
            }
        }

        putchar('~');
        putchar(' ');
    }
}