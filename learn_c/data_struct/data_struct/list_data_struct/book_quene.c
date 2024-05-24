#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char name;
} element_data;

typedef struct
{
    element_data data;
    struct element *next;
} element;

typedef struct
{
    element *head;
    element *rear;

} book_quene;

int init_quene(book_quene *q);
int destroy_quene(book_quene *q);
int clear_quene(book_quene *q);
int quene_empty(book_quene *q);
int quene_length(book_quene *q);
int get_head(book_quene *q, element **e);
int en_quene(book_quene *q, element *e);
int de_quene(book_quene *q);
void print_queue(book_quene *q);
void wait_quene_irq();
void en_quene_irq(book_quene *q, element *e);

static sem_t quene_sem;
static book_quene *ptest_quene = NULL;

void en_quene_irq(book_quene *q, element *e)
{
    en_quene(q, e);
    sem_post(&quene_sem);
}

void wait_quene_irq()
{
    sem_wait(&quene_sem);
}

static int clear_element_data(element_data *_data)
{
    _data->id = 0;
    _data->name = '\0';
    return 0;
}

int init_quene(book_quene *q)
{
    if (q == NULL)
        return -1;
    q->head = NULL;
    q->rear = NULL;
    sem_init(&quene_sem, 0, 0);

    return 0;
}

int destroy_quene(book_quene *q)
{
    if (q == NULL)
        return -1;
    while (q->head) {
        q->rear = q->head->next;
        free(q->head);
        q->head = q->rear;
    }
    free(q);
    return 0;
}

int clear_quene(book_quene *q)
{
    if (q == NULL || q->head == NULL)
        return -1;
    q->rear = q->head;
    while (q->rear->next) {
        clear_element_data(&q->rear->data);
        q->rear = q->rear->next;
    }
    clear_element_data(&q->rear->data);
    return 0;
}

int quene_empty(book_quene *q)
{
    if (q == NULL)
        return -1;
    return q->head == q->rear ? 1 : 0;
}

int quene_length(book_quene *q)
{
    int count = 0;
    if (q == NULL)
        return -1;
    if (q->head == NULL) {
        return count;
    }
    element *_e = NULL;
    _e = q->head;
    while (_e) {
        count++;
        _e = _e->next;
    }

    return count;
}

int get_head(book_quene *q, element **e)
{
    if (q == NULL || q->head == NULL)
        return -1;
    *e = q->head;
    return 0;
}

int en_quene(book_quene *q, element *e)
{
    if (q == NULL || e == NULL)
        return -1;
    if (q->head == NULL) {
        q->head = e;
    } else {
        q->rear->next = e;
    }
    q->rear = e;
    return 0;
}

int de_quene(book_quene *q)
{
    if (q == NULL || q->head == NULL)
        return -1;
    if (q->head == q->rear) {
        free(q->head);
        q->head = NULL;
        q->rear = NULL;
    } else {
        element *temp = NULL;
        temp = q->head->next;
        free(q->head);
        q->head = temp;
    }

    return 0;
}

void print_queue(book_quene *q)
{
    if (q == NULL || q->head == NULL)
        return -1;
    element *temp = q->head;
    while (temp != NULL) {
        printf("%d\n", temp->data.id);
        temp = temp->next;
    }
}

static int temp = 0;

static void *test_thread(void *avg)
{
    element *tempe = NULL;
    book_quene *test_quene = (struct book_quene *)avg;
    int ret = 0;
    while (1) {
#if 1
        ret = get_head(test_quene, &tempe);
        if (!ret) {
            printf("de_quene, data:%d, len:%d\n", tempe->data.id, quene_length(test_quene));
            de_quene(test_quene);
        }

        if (quene_length(test_quene) == 5) {
            print_queue(test_quene);
            break;
        }
#endif
        // printf("%d\n", temp);
        sleep(1);
    }

    return NULL;
}

static void *chang_temp(void *avg)
{
    element *test_data = NULL;
    book_quene *test_quene = (struct book_quene *)avg;
    while (1) {
        temp++;
#if 1
        test_data = (element *)malloc(sizeof(element));
        test_data->data.id = temp;
        test_data->next = NULL;
        en_quene(test_quene, test_data);
        printf("en_quene\n");
#endif

        usleep(700 * 1000);
    }

    return NULL;
}

static void *test_enquene_irq(void *para)
{
    (void)para;
    element *test_data;
    int i = 0;
    while (1) {
        test_data = (element *)malloc(sizeof(element));
        test_data->data.id = i++;
        test_data->data.name = 'a';
        en_quene_irq(ptest_quene, test_data);
        sleep(1);
    }
}

static void *process_queue(void *para)
{
    (void)para;
    element *test_data = NULL;
    while (1) {
        wait_quene_irq();
        printf("len:%d\n", quene_length(ptest_quene));
        get_head(ptest_quene, &test_data);
        printf("id:%d\n", test_data->data.id);
        de_quene(ptest_quene);
    }
}

#if 1
int main(int argc, char const *argv[])
{
#if 0
    int ret = -1;
    book_quene* test_quene = (book_quene* )malloc(sizeof(book_quene));
    ret = init_quene(test_quene);
    ret = quene_length(test_quene);
    printf("%d\n", ret);
    element* test_data = (element* )malloc(sizeof(element));
    test_data->data.id = 3;
    test_data->data.name = 'a';
    ret = en_quene(test_quene, test_data);
    element* test_data_2 = (element* )malloc(sizeof(element));
    test_data_2->data.id = 2;
    test_data_2->data.name = 'a';
    ret = en_quene(test_quene, test_data_2);
    ret = quene_length(test_quene);
    printf("%d\n", ret);
    element* temp = NULL;
    ret = get_head(test_quene, &temp);
    if (!ret)
        printf("id:%d, name:%c\n", temp->data.id, temp->data.name);
    ret = clear_quene(test_quene);
    if (!ret)
        printf("id:%d, name:%c\n", temp->data.id, temp->data.name);
    ret = destroy_quene(test_quene);
    test_quene = NULL;
    
    pid_t pid;
    pid_t pid2;
    int ret;
    book_quene *test_quene = (book_quene *)malloc(sizeof(book_quene));

    ret = init_quene(test_quene);

    ret = pthread_create(&pid, NULL, chang_temp, test_quene);
    if (ret) {
        printf("create thread failed\n");
    }

    ret = pthread_create(&pid2, NULL, test_thread, test_quene);
    if (ret) {
        printf("create thread failed\n");
    }

    pthread_join(pid2, NULL);
    pthread_exit(NULL);
    free(test_quene);
#endif
    pthread_t pid;
    pthread_t pid2;
    int ret;
    ptest_quene = (book_quene *)malloc(sizeof(book_quene));
    ret = init_quene(ptest_quene);
    if (ret) {
        printf("init_quene failed\n");
    }

    ret = pthread_create(&pid, NULL, test_enquene_irq, NULL);
    if (ret) {
        printf("create thread failed\n");
    }

    ret = pthread_create(&pid2, NULL, process_queue, NULL);
    if (ret) {
        printf("create thread failed\n");
    }

    pthread_join(pid2, NULL);
    return 0;
}
#endif
