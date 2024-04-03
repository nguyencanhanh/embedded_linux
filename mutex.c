#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    char name[20];
    int date;
    char address[15];
    int phone;
}mutex;

mutex member;
int flag_read = 0;

pthread_t thread_id1, thread_id2, thread_id3;

static void *handle_th1(void *args){
    pthread_mutex_lock(&lock1);
    while (flag_read == 0){
        printf("Enter your name from keyboard:\n");
        scanf("%s", member.name);
        printf("Enter your birthday from keyboard:\n");
        scanf("%d", &member.date);
        printf("Enter your address from keyboard:\n");
        scanf("%s", member.address);
        printf("Enter your phone from keyboard:\n");
        scanf("%d", &member.phone);
        flag_read = 1;
        pthread_mutex_unlock(&lock1);
        while(flag_read == 1 || flag_read == 2);
    }
}

static void *handle_th2(void *args){
    pthread_mutex_lock(&lock1);
    while (flag_read == 1){
        FILE *info;
        info = fopen("info_member.txt", "a");
        if (info == NULL){
            printf("Cannot open file.\n");
        }
        fprintf(info, "%s %d %s %d\n", member.name, member.date, member.address, member.phone);
        fclose(info);
        flag_read = 2;
        pthread_mutex_unlock(&lock1);
        while(flag_read == 2 || flag_read == 0);
    }
}

static void *handle_th3(void *args){
    while(flag_read == 1);
    pthread_mutex_lock(&lock1);
    while (flag_read == 2){
        FILE *info;
        char line[30];
        info = fopen("info_member.txt", "r");
        if (info == NULL){
            printf("Cannot open file.\n");
        }
        while(fgets(line, sizeof(line), info) != NULL){
            printf("%s", line);
        }
        
        fclose(info);
        flag_read = 0;
        pthread_mutex_unlock(&lock1);
        while(flag_read == 0 || flag_read == 1);
    }
}

int main(){
    int ret;

    if (ret = pthread_create(&thread_id1, NULL, &handle_th1, NULL)){
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    if (ret = pthread_create(&thread_id2, NULL, &handle_th2, NULL)){
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    if (ret = pthread_create(&thread_id3, NULL, &handle_th3, NULL)){
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    while(1);
    
    return 0;
}