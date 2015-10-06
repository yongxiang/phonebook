#include <stdlib.h>

#include "phonebook_opt.h"

entry *findName(char lastname[], entry *pHead)
{

    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }

    return NULL;
}

entry *append(char lastName[], entry *e)
{
    e->pNext = (entry *) malloc(sizeof(entry));
    e->details = NULL;
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;
    return e;
}
entry *append_lines(char Lines[][MAX_LAST_NAME_SIZE], entry *E)
{
    E->pNext =(entry *) malloc(N_LINES * sizeof(entry));
    strcpy(E->lastName, Lines[0]);
    E = E->pNext;
    int k;
    for (k=1; k < N_LINES ; k++) {
        E->details = NULL;
        strcpy(E->lastName, Lines[k]);
        E->pNext = E +1;
        E = E->pNext;
    }
    return E;
}
