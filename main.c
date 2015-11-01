#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

entry *e_append_detail;
int append_done;
pthread_mutex_t append_done_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t pNext_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pNext_cond = PTHREAD_COND_INITIALIZER;

pthread_cond_t wait_thread_done_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t wait_thread_done_mutex = PTHREAD_MUTEX_INITIALIZER;
int is_thread_done;
#ifdef MY_OPT
void *append_detail_func()
{
    pthread_mutex_lock(&append_done_mutex);
    while(!append_done || e_append_detail->pNext != NULL) {
        pthread_mutex_unlock(&append_done_mutex);

        append_detail(e_append_detail);

        pthread_mutex_lock(&pNext_mutex);
        if(e_append_detail->pNext == NULL && !append_done) { // wait for signal to continue
            pthread_cond_wait(&pNext_cond,&pNext_mutex);
        }
        e_append_detail = e_append_detail->pNext;
        pthread_mutex_unlock(&pNext_mutex);
        pthread_mutex_lock(&append_done_mutex);
    }
    pthread_mutex_unlock(&append_done_mutex);

    pthread_mutex_lock(&wait_thread_done_mutex);
    is_thread_done = 1;
    pthread_cond_signal(&wait_thread_done_cond);
    pthread_mutex_unlock(&wait_thread_done_mutex);
    pthread_exit(NULL);
}
#endif
static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    int index;
    FILE *fp;
    int i = 0;
#ifdef MY_OPT
    char lines[N_LINES][MAX_LAST_NAME_SIZE];
#else
    char line[MAX_LAST_NAME_SIZE];
#endif
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
    e_append_detail = e;

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
#ifdef MY_OPT
    append_done = 0;
    is_thread_done = 0;
    pthread_t thread_append_detail;
    pthread_create( &thread_append_detail, NULL, append_detail_func, NULL);
    int k=0;
    while (fgets(lines[k], sizeof(lines[k]), fp)) {
        while (lines[k][i] != '\0')
            i++;
        lines[k][i-1] = '\0';
        i = 0;
        k++;
        if( k == N_LINES ) {
            k = 0;
            e = append_lines(lines, e);
        }
    }
    append_lines(lines,e);

// append done
    pthread_mutex_lock(&append_done_mutex);
    pthread_mutex_lock(&pNext_mutex);
    append_done = 1;
    pthread_cond_broadcast(&pNext_cond);
    pthread_mutex_unlock(&pNext_mutex);
    pthread_mutex_unlock(&append_done_mutex);
#else
    while (fgets(line, sizeof(line),fp)) {
        while (line[i] != '\0')
            i++;
        line[i-1] = '\0' ;
        i = 0;
        e = append(line,e);
    }
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    e = pHead;

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    e = pHead;
#ifdef MY_OPT
    assert(findName(input, e, &index) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e, &index)->lastNames[index], "zyxel"));
#else
    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);

#ifdef MY_OPT
    findName(input, e, &index);
#else
    findName(input, e);
#endif
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    /* FIXME: release all allocated entries */
    free(pHead);

#ifdef MY_OPT
    pthread_mutex_lock(&wait_thread_done_mutex);
    while(!is_thread_done) {
        pthread_cond_wait(&wait_thread_done_cond,&wait_thread_done_mutex);
    }
    pthread_mutex_unlock(&wait_thread_done_mutex);
#endif
    return 0;
}
