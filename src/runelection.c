#include "runelection.h"

int main(int argc, char **argv) {
    char* inputFile = NULL;
    char* outputFile = NULL;
    unsigned int numberOfUpdates;

    // Abort if no command line arguments given:
    if(argc == 1)
    {
        printf("Oups! No command line arguments in sight...\n");
        return 1;
    }

    // Get the command line arguments:
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-i") == 0) {
            i++;
            inputFile = malloc(strlen(argv[i]) * sizeof(char) + 1);
            strcpy(inputFile, argv[i]);
            inputFile[strlen(argv[i])] = '\0';
        }
        else if(strcmp(argv[i], "-o") == 0) {
            i++;
            outputFile = malloc(strlen(argv[i]) * sizeof(char) + 1);
            strcpy(outputFile, argv[i]);
            outputFile[strlen(argv[i])] = '\0';
        }
        else if(strcmp(argv[i], "-n") == 0) {
            i++;
            numberOfUpdates = strtoul(argv[i], NULL, 10);
        }
        else {
            printf("There was a problem with the command line parameters. Aborting now...\n");
            return 1;
        }
    }

    electionManager* manager = initializeElectionManager(numberOfUpdates, outputFile);

    getVotersFromFile(inputFile, manager);

    commandPrompt(manager);

    if(inputFile != NULL)
        free(inputFile);
    if(outputFile != NULL)
        free(outputFile);
    free(manager);

    return 0;
}
