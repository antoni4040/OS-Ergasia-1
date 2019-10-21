
/* 
Create by Antonis Karvelas, sdi1600060, for the 1st project of
K22: Operating Systems.
Everything that has to do with file reading/writing is in here.
*/

#include "files.h"

//TODO: remove later
int maxLevel = 0;
int numOfItems = 0;

/*
An election manager is a struct that combines all the useful data structures
into one for ease of use.
*/
electionManager* initializeElectionManager(unsigned int numberOfUpdates)
{
    electionManager* newElectionManager = malloc(sizeof(electionManager));
    newElectionManager->numberOfUpdates = numberOfUpdates;
    return newElectionManager;
}

//TODO: remove later
void* printRBT(RBT* rbt, node* currentNode, int level) {
    // if(currentNode == NULL) printf("NULL\n");
    // else printf("l: %i %s __ \n", level, ((voter*)(currentNode->element))->IDstring);

    if(currentNode != rbt->NIL) numOfItems++;

    if(maxLevel < level && currentNode != rbt->NIL) maxLevel = level;

    if(currentNode != rbt->NIL) {
        printRBT(rbt, currentNode->leftChild, level+1);
        printRBT(rbt, currentNode->rightChild, level+1);
    }
}

/*
Given the filepath of the input file, check if it exists.
*/
bool checkInputFileExists(char* inputFile) {
    struct stat fileExists;
    if (!(stat(inputFile, &fileExists) == 0 && S_ISREG(fileExists.st_mode)))
    {
        fprintf(stderr, "Input file doesn't exist. There must be some mistake...\n");
        return false;
    }
    return true;
}

/*
Count the lines of a given opened file to get the number of voters.
It also rewinds the file back to the start for convenience.
*/
uint32_t getNumberOfVoters(FILE* input) {
    char* line = NULL;
    size_t len = 0;
    uint32_t numberOfVoters = 0;
    while (getline(&line, &len, input) != EOF) {
        numberOfVoters++;
    }
    rewind(input);
    return numberOfVoters;
}

/*
From given line of input, add voter data to all the data structures.
(Note on code: strtoul for converting to unsigned int can cause problems,
but only for large values, and I assume no one has the age of the universe
or a galactic postcode.)
*/
void insertVoterToDataStructs(electionManager* manager, char* line, bool print) {
    unsigned int age;
    gender voterGender;
    unsigned int postcode;

    char* IDstring = strtok(line, " ");
    char* name = strtok(NULL, " ");
    char* surname = strtok(NULL, " ");
    char* ageString = strtok(NULL, " ");
    char* genderString = strtok(NULL, " ");
    char* postcodeString = strtok(NULL, " ");

    age = strtoul(ageString, NULL, 10); 
    if(strcmp(genderString, "M") == 0) {
        voterGender = MALE;
    }
    else {
        voterGender = FEMALE;
    }
    postcode = strtoul(postcodeString, NULL, 10);

    voter* newVoter = initializeVoter(IDstring, name, surname, age, voterGender, postcode);
    insertToBloomFilter(manager->bloomFilter, IDstring, strlen(IDstring));
    node* newNode = initializeNode(newVoter);
    int added = RBInsert(manager->redBlackTree, newNode);
    if(print) {
        if(added == -1)
            printf("REC-WITH %s EXISTS\n", IDstring);
        else
            printf("REC-WITH %s INSERTED-IN-BF-RBT\n", IDstring);
    }
    if(added == 0) {
        node* newNodePostcode = initializeNode(newVoter);
        insertToHashTable(manager->hashTable, newNodePostcode);
    }
}

/*
Read voter data from given file and create new voters.
Update data structures with new voters.
*/
void readVotersAndUpdateStructures(FILE* input, electionManager* manager) {
    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, input) != EOF) {
        insertVoterToDataStructs(manager, line, false);
    }
}

/*
Check input file exists, try to open file, get number of voters,
create data structures, update them with voters, progress.
*/
int getVotersFromFile(char* inputFile, electionManager* manager) {
    if(!checkInputFileExists(inputFile)) {  //Check file exists.
        return 1;
    }

    FILE* input = fopen(inputFile, "r");    //Attempt to open file.
    if(input == NULL) {
        fprintf(stderr, "Problem opening input file for reading.\n");
        return 1;
    }

    uint32_t numberOfVoters = getNumberOfVoters(input);

    manager->bloomFilter = initializeBloomFilter(numberOfVoters);
    manager->redBlackTree = initializeRedBlackTree(&alphanumericCompare);
    manager->redBlackTree->freeNode = &freeNode;
    manager->redBlackTree->printNode = &printNode;
    manager->hashTable = initializeHashtable();
    
    readVotersAndUpdateStructures(input, manager);
    //TODO: remove later:
    printRBT(manager->redBlackTree, manager->redBlackTree->root, 0);
    printf("Max level %d, Items: %d\n", maxLevel, numOfItems);
    voter* spectreVoter = initializeVoter(
        "XB112605", "name", "surname", 10, MALE, 11111);
    node* spectreNode = initializeNode(spectreVoter);
    node* nodeToDelete = RBTSearch(manager->redBlackTree, spectreNode);
    RBDelete(manager->redBlackTree, nodeToDelete);
    maxLevel = 0;
    numOfItems = 0;
    printRBT(manager->redBlackTree, manager->redBlackTree->root, 0);
    printf("Max level %d, Items: %d\n", maxLevel, numOfItems);
    return 0;
}