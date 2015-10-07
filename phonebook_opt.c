#include <stdlib.h>

#include "phonebook_opt.h"

entry *findName(char lastname[], entry *pHead, int *index)
{
    int i;
    while (pHead != NULL) {
        for (i=0; i<N_LINES; i++) {
            if (strcasecmp(lastname,pHead->lastNames[i]) == 0) {
                *index=i;
                return pHead;
            }
        }
        pHead = pHead->pNext;
    }

    return NULL;
}
entry *append_lines(char Lines[][MAX_LAST_NAME_SIZE], entry *E)
{
    E->pNext =(entry *) malloc(N_LINES * sizeof(entry));
    int i;
    for ( i=0; i<N_LINES; i++) {
        strcpy(E->lastNames[i],Lines[i]);
        E->details[i] = NULL;
    }
    E = E->pNext;
    return E;
}
