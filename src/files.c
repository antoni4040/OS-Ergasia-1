
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
void* printRBT(node* currentNode, int level) {
    // if(currentNode == NULL) printf("NULL\n");
    // else printf("l: %i %s __ \n", level, ((voter*)(currentNode->element))->IDstring);

    if(currentNode != NULL) numOfItems++;

    if(maxLevel < level && currentNode != NULL) maxLevel = level;

    if(currentNode != NULL) {
        printRBT(currentNode->leftChild, level+1);
        printRBT(currentNode->rightChild, level+1);
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
Read voter data from given file and create new voters.
Update data structures with new voters.
(Note on code: strtoul for converting to unsigned int can cause problems,
but only for large values, and I assume no one has the age of the universe
or a galactic postcode.)
*/
void readVotersAndUpdateStructures(FILE* input, BF* bf, RBT* rbt) {
    char* IDstring;
    char* name;
    char* surname;
    char* ageString;
    char* genderString;
    char* postcodeString;

    unsigned int age;
    gender voterGender;
    unsigned int postcode;

    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, input) != EOF) {
        IDstring = strtok(line, " ");
        name = strtok(NULL, " ");
        surname = strtok(NULL, " ");
        ageString = strtok(NULL, " ");
        genderString = strtok(NULL, " ");
        postcodeString = strtok(NULL, " ");

        age = strtoul(ageString, NULL, 10); 
        if(strcmp(genderString, "M") == 0) {
            voterGender = MALE;
        }
        else {
            voterGender = FEMALE;
        }
        postcode = strtoul(postcodeString, NULL, 10);

        voter* newVoter = initializeVoter(IDstring, name, surname, age, voterGender, postcode);
        insertToBloomFilter(bf, IDstring, strlen(IDstring));
        node* newNode = initializeNode(newVoter);
        RBTInsert(rbt, newNode);
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
    
    readVotersAndUpdateStructures(input, manager->bloomFilter, manager->redBlackTree);
    //TODO: remove later:
    printRBT(manager->redBlackTree->root, 0);
    printf("Max level %d\n, Items: %d\n", maxLevel, numOfItems);
    return 0;
}