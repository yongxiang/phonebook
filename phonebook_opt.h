#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define N_LINES 127

typedef struct _details {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} details;

typedef struct __PHONE_BOOK_ENTRY {
    char lastNames[N_LINES][MAX_LAST_NAME_SIZE];

    struct _details *details[N_LINES];

    struct __PHONE_BOOK_ENTRY *pNext;
} entry;
entry *findName(char lastname[], entry *pHead, int* index);
entry *append_lines(char lastName[][MAX_LAST_NAME_SIZE], entry *e);
entry *append_detail(entry *e);
#endif
