//�̱� �����带 �̿��� ��� �Ӱ豸���� ������ �ʿ䰡 ��� mutex_lock, mutex_unlock ������ �������� �ʾҴ�.
//4���� ������� �Ӱ豸���� �����ؾ��ϱ� ������ �̷��� ������ �ð��� �Һ��һӴ��� �����尣�� ��ü ������ �ʿ��ϱ� ������ �ð��� ������ �� �Һ��Ѵ�.
//���� �̱۽����忡�� �Ӱ豸�� ������ �����ߴ��� 4���� �����庸�� �Һ� �ð��� ������ ������
//������ ����̶�� �̱۽����带 �̿��ϴ°� �ð��� ȿ�������� ����� �� �ִ�.

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
    //printf("%d�� �����մϴ�.\n", times);
    int id = (int)arg;

    for (int i = 0; i < times; ++i) {
        pthread_mutex_lock(&console_cv_lock);
        //printf("%d�� ������.\n", id);

        // 1 x 1 �簢���� (double)RAND_MAX ����� �� �� ���� �� ���� ����
        x = (double)rand() / (double)RAND_MAX;
        y = (double)rand() / (double)RAND_MAX;

        if ((x * x) + (y * y) <= 1) {   // ��Ÿ��� ���� : �����ߴ� ħ
            ++circle_inner;
        }
        
        pthread_mutex_unlock(&console_cv_lock);
    }
}

void* func_Single(void* arg) {
    //printf("%d�� �����մϴ�.\n", times);
    int id = (int)arg;

    for (int i = 0; i < times; ++i) {
        //printf("%d�� ������.\n", id);

        // 1 x 1 �簢���� (double)RAND_MAX ����� �� �� ���� �� ���� ����
        x = (double)rand() / (double)RAND_MAX;
        y = (double)rand() / (double)RAND_MAX;

        if ((x * x) + (y * y) <= 1) {   // ��Ÿ��� ���� : �����ߴ� ħ
            ++circle_inner;
        }
    }
}

int main() {
    int choice = 0;

    while (choice != 1 && choice != 2) {
        printf("1000000����\n �̱۽������ �����Ϸ��� 1.\n 4�� ������� �����Ϸ��� 2.\n");
        printf("1. 1000000\n"); // �鸸
        printf("2. 250000\n");  // �̽ʿ���
        scanf("%d", &choice);

        if (choice != 1 && choice != 2) {
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
        }
    }

    pthread_cond_init(&console_cv, NULL);
    pthread_mutex_init(&console_cv_lock, NULL);

    if (choice == 1) {
        times = 1000000;
        pthread_t tid;
        if (pthread_create(&tid, NULL, func_Single, (void*)100) != 0) { //1�� thread ����
            fprintf(stderr, "thread create error\n");
            exit(1);
        }

        pthread_join(tid, NULL); //������ �ڿ� ȸ��
    }
    else if (choice == 2) {
        times = 250000;
        pthread_t tid1, tid2, tid3, tid4;

        if (pthread_create(&tid1, NULL, func_Multi, (void*)1) != 0) { //1�� thread ����
            fprintf(stderr, "thread1 create error\n");
            exit(1);
        }

        if (pthread_create(&tid2, NULL, func_Multi, (void*)2) != 0) { //2�� thread ����
            fprintf(stderr, "thread2 create error\n");
            exit(1);
        }

        if (pthread_create(&tid3, NULL, func_Multi, (void*)3) != 0) { //3�� thread ����
            fprintf(stderr, "thread3 create error\n");
            exit(1);
        }

        if (pthread_create(&tid4, NULL, func_Multi, (void*)4) != 0) { //4�� thread ����
            fprintf(stderr, "thread4 create error\n");
            exit(1);
        }

        pthread_join(tid1, NULL); //1�� ������ �ڿ� ȸ��
        pthread_join(tid2, NULL); //2�� ������ �ڿ� ȸ��
        pthread_join(tid3, NULL); //3�� ������ �ڿ� ȸ��
        pthread_join(tid4, NULL); //4�� ������ �ڿ� ȸ��
    }
    

    // 4�п��̹Ƿ� 4�� ���Ͽ� PI�� ���
    if (choice == 1)
        printf("���̰��� %f �̴�\n", 4 * circle_inner / times);
    else if (choice == 2)
        printf("���̰��� %f �̴�\n", 4 * circle_inner / (4*times));
}