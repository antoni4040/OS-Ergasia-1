/* Create by Antonis Karvelas, sdi1600060.
Here lies everything that has to do with the voters for the 1st project of
K22: Operating Systems.
*/

#define VOTED 1
#define NOT_VOTED 0

#include <stdlib.h>
#include <string.h>

typedef enum {
    MALE,
    FEMALE
} gender;

struct voter {
    char* IDstring;
    char* name;
    char* surname;
    int age;
    gender gender;
    int postCode;
    unsigned int hasVoted: 1;
} typedef voter;

voter*      initializeVoter(char* IDstring, char* name, char* surname, 
            int age, gender givenGender, int postCode);