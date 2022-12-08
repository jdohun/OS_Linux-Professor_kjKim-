//싱글 스레드를 이용한 경우 임계구역을 관리할 필요가 없어서 mutex_lock, mutex_unlock 과정을 포함하지 않았다.
//4개의 스레드는 임계구역을 관리해야하기 때문에 이러한 과정이 시간을 소비할뿐더러 스레드간의 교체 과정도 필요하기 때문에 시간을 오히려 더 소비한다.
//또한 싱글스레드에서 임계구역 관리를 포함했더라도 4개의 스레드보다 소비 시간이 적었기 때문에
//간단한 계산이라면 싱글스레드를 이용하는게 시간을 효율적으로 사용할 수 있다.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

pthread_cond_t console_cv;
pthread_mutex_t console_cv_lock;

double x = 0;
double y = 0;
double circle_inner = 0;
unsigned int times;

void* func_Multi(void* arg) {
    //printf("%d번 실행합니다.\n", times);
    int id = (int)arg;

    for (int i = 0; i < times; ++i) {
        pthread_mutex_lock(&console_cv_lock);
        //printf("%d번 스레드.\n", id);

        // 1 x 1 사각형을 (double)RAND_MAX 등분한 후 그 위의 한 점을 구함
        x = (double)rand() / (double)RAND_MAX;
        y = (double)rand() / (double)RAND_MAX;

        if ((x * x) + (y * y) <= 1) {   // 피타고라스 정리 : 제곱했다 침
            ++circle_inner;
        }
        
        pthread_mutex_unlock(&console_cv_lock);
    }
}

void* func_Single(void* arg) {
    //printf("%d번 실행합니다.\n", times);
    int id = (int)arg;

    for (int i = 0; i < times; ++i) {
        //printf("%d번 스레드.\n", id);

        // 1 x 1 사각형을 (double)RAND_MAX 등분한 후 그 위의 한 점을 구함
        x = (double)rand() / (double)RAND_MAX;
        y = (double)rand() / (double)RAND_MAX;

        if ((x * x) + (y * y) <= 1) {   // 피타고라스 정리 : 제곱했다 침
            ++circle_inner;
        }
    }
}

int main() {
    int choice = 0;

    while (choice != 1 && choice != 2) {
        printf("1000000번을\n 싱글스레드로 수행하려면 1.\n 4개 스레드로 수행하려면 2.\n");
        printf("1. 1000000\n"); // 백만
        printf("2. 250000\n");  // 이십오만
        scanf("%d", &choice);

        if (choice != 1 && choice != 2) {
            printf("잘못된 입력입니다. 다시 입력해주세요.\n");
        }
    }

    pthread_cond_init(&console_cv, NULL);
    pthread_mutex_init(&console_cv_lock, NULL);

    if (choice == 1) {
        times = 1000000;
        pthread_t tid;
        if (pthread_create(&tid, NULL, func_Single, (void*)100) != 0) { //1번 thread 생성
            fprintf(stderr, "thread create error\n");
            exit(1);
        }

        pthread_join(tid, NULL); //스레드 자원 회수
    }
    else if (choice == 2) {
        times = 250000;
        pthread_t tid1, tid2, tid3, tid4;

        if (pthread_create(&tid1, NULL, func_Multi, (void*)1) != 0) { //1번 thread 생성
            fprintf(stderr, "thread1 create error\n");
            exit(1);
        }

        if (pthread_create(&tid2, NULL, func_Multi, (void*)2) != 0) { //2번 thread 생성
            fprintf(stderr, "thread2 create error\n");
            exit(1);
        }

        if (pthread_create(&tid3, NULL, func_Multi, (void*)3) != 0) { //3번 thread 생성
            fprintf(stderr, "thread3 create error\n");
            exit(1);
        }

        if (pthread_create(&tid4, NULL, func_Multi, (void*)4) != 0) { //4번 thread 생성
            fprintf(stderr, "thread4 create error\n");
            exit(1);
        }

        pthread_join(tid1, NULL); //1번 스레드 자원 회수
        pthread_join(tid2, NULL); //2번 스레드 자원 회수
        pthread_join(tid3, NULL); //3번 스레드 자원 회수
        pthread_join(tid4, NULL); //4번 스레드 자원 회수
    }
    

    // 4분원이므로 4를 곱하여 PI를 계산
    if (choice == 1)
        printf("파이값은 %f 이다\n", 4 * circle_inner / times);
    else if (choice == 2)
        printf("파이값은 %f 이다\n", 4 * circle_inner / (4*times));
}