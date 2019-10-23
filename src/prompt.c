/* 
Created by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
This is the implementation of command prompt.
*/

#include "prompt.h"

int commandPrompt(electionManager* manager) {
    char* command;
    char* line = NULL;
    char* rest;
    size_t len = 0;

    putchar('~');
    putchar(' ');
    while(getline(&line, & len, stdin) != EOF) {
        line = strtok(line, "\n");
        command = strtok_r(line, " ", &rest);

        if(command == NULL) {       // If no input is given:
            putchar('~');
            putchar(' ');
            continue;
        }

        printf("%s\n", command);

        if(strcmp(command, "exit") == 0) {
            freeRedBlackTree(manager->redBlackTree, true);
            freeHashTable(manager->hashTable);
            freeBloomFilter(manager->bloomFilter);
            break;
        }
        else if(strcmp(command, "lbf") == 0) {
            char* key = strtok_r(rest, " ", &rest);
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
            char* key = strtok_r(rest, " ", &rest);
            if(key == NULL) {
                printf("No key given. Nothing to do.\n");
            }
            else {
                voter* findVoter = (voter*)(searchVoterInRBT(manager->redBlackTree, key)->element);
                if(findVoter != NULL) {
                    printf("KEY %s FOUND-IN-RBT\n", key);
                }
                else {
                    printf("KEY %s NOT-IN-RBT\n", key);
                }
            }
        }
        else if(strcmp(command, "ins") == 0) {
            insertVoterToDataStructs(manager, rest, true);
        }
        else if(strcmp(command, "find") == 0) {
            char* key = strtok_r(rest, " ", &rest);
            if(key == NULL) {
                printf("No key given. Nothing to do.\n");
            }
            else {
                bool perhapsExists = searchBloomFilter(
                    manager->bloomFilter, key, strlen(key));
                if(perhapsExists) {
                    voter* findVoter = (voter*)(searchVoterInRBT(manager->redBlackTree, key)->element);
                    if(findVoter != NULL) {
                        printf("REC-IS: ");
                        printVoter(findVoter);
                    }
                    else {
                        printf("REC-WITH %s NOT-in-structs\n", key);
                    }
                }
                else {
                    printf("REC-WITH %s NOT-in-structs\n", key);
                }
            }
        }
        else if(strcmp(command, "delete") == 0) {
            char* key = strtok_r(rest, " ", &rest);
            if(key == NULL) {
                printf("No key given. Nothing to do.\n");
            }
            else {
                //Search red-black tree:
                node* findVoter = searchVoterInRBT(manager->redBlackTree, key);
                if(findVoter == manager->redBlackTree->NIL) {
                    printf("KEY %s NOT-in-structs\n", key);
                }
                else {
                    //Remove from red-black tree:
                    RBDelete(manager->redBlackTree, findVoter);
                    //Remove from postcode hashtable:
                    unsigned int postcode = ((voter*)(findVoter->element))->postCode;
                    RBT* postcodeRBT = searchPostCodeRBT(manager->hashTable, postcode);
                    node* findVoterInPC = searchVoterInRBT(postcodeRBT, key);
                    RBDelete(postcodeRBT, findVoterInPC);

                    //Free deleted node:
                    manager->redBlackTree->freeNode(findVoter, false);
                    postcodeRBT->freeNode(findVoterInPC, true);

                    manager->numberOfVoters -= 1;

                    //Update and check if BF needs restructuring:
                    update(manager);

                    printf("# DELETED %s FROM-structs\n", key);
                }
            }
        }
        else if(strcmp(command, "vote") == 0) {
            char* key = strtok_r(rest, " ", &rest);
            if(key == NULL) {
                printf("No key given. Nothing to do.\n");
            }
            else {
                vote(manager->redBlackTree, key);
            }
        }
        else if(strcmp(command, "load") == 0) {
            char* fileofkeys = strtok_r(rest, " ", &rest);
            if(fileofkeys == NULL) {
                printf("No file of keys given. Nothing to do.\n");
            }
            else {
                if(checkInputFileExists(fileofkeys)) {
                    voteFromFile(manager->redBlackTree, fileofkeys);
                }
            }
        }
        else if(strcmp(command, "voted") == 0) {
            char* postcodeString = strtok_r(rest, " ", &rest);
            unsigned int votes;
            if(postcodeString == NULL) {
                //Show total votes:
                votes = countVotesInRBT(manager->redBlackTree);
                printf("NUMBER %u\n", votes);
            }
            else {
                //Show total votes in postcode:
                unsigned int postcode = strtoul(postcodeString, NULL, 10);
                RBT* postcodeTree = searchPostCodeRBT(manager->hashTable, postcode);
                if(postcodeTree == NULL) {
                    printf("Postcode not found.\n");
                }
                else {
                    votes = countVotesInRBT(postcodeTree);
                    printf("IN %u VOTERS-ARE %u\n", postcode, votes);
                }
            }
        }
        else if(strcmp(command, "votedperpc") == 0) {
            postCodeStatistics(manager->hashTable);
        }
        

        putchar('~');
        putchar(' ');
    }

    free(line);
    return 0;
}