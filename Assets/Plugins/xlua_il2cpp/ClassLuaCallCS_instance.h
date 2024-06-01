#include <string.h>
#define TOTAL_KEYWORDS 46
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 58
unsigned int
ClassLuaCallCS_instance_hash(register const char* str, register unsigned int len)
{
    static unsigned char asso_values[] =
    {
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 30, 59, 59, 59,
      59, 25, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59,  0,  0, 59,  0,
      40,  0, 59, 15, 59, 59,  0,  0, 25, 25,
      15, 59, 59,  0, 15, 59,  5, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59,  0, 30,  5,
      59, 10,  0, 20, 59,  0, 59,  5,  0, 59,
       5, 59,  0, 59, 10,  0, 10,  0,  0, 59,
      59,  0, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
      59, 59, 59, 59, 59, 59
    };
    register int hval = len;

    switch (hval)
    {
    default:
        hval += asso_values[(unsigned char)str[7]];
        /*FALLTHROUGH*/
    case 7:
    case 6:
    case 5:
        hval += asso_values[(unsigned char)str[4]];
        /*FALLTHROUGH*/
    case 4:
    case 3:
    case 2:
    case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
    return hval;
}

unsigned int GetClassLuaCallCS_instance_MaxLength() {
    return MAX_HASH_VALUE - MIN_HASH_VALUE;
}