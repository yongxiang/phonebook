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
    E->pNext = NULL;
    return E;
}

void append_detail(entry *E)
{
    int i;
    for ( i=0; i<N_LINES; i++) {
        E->details[i] = (details *) malloc(sizeof(details));
        strcpy(E->details[i]->firstName , "abcabc" );
        strcpy(E->details[i]->email , "xxx@xxx.xx");
        strcpy(E->details[i]->phone , "0987654321");
        strcpy(E->details[i]->cell , "0987654321");
        strcpy(E->details[i]->addr1 , "123");
        strcpy(E->details[i]->addr2 , "456");
        strcpy(E->details[i]->city ,"XX");
        strcpy(E->details[i]->state,"Y");
        strcpy(E->details[i]->zip,"0000");
    }

    return ;
}
