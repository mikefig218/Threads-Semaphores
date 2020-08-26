/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

//A TCB structure containing the thread context and additional info
struct tcb
{
	int thread_id;
	int thread_priority;
	ucontext_t *thread_context;
};

typedef struct tcb tcb;

//Semaphore structure. Uses a FIFO queue which makes it a STRONG Semaphore
struct sem_t
{
	int count;
	struct Queue *q;
};

typedef struct sem_t sem_t;

//A structure symbolizing a node in the linked list representing the queue
struct QNode
{
    tcb *tcb; //The thread control block associated with the node
    struct QNode *next; //Pointer to next node in the queue
};
typedef struct QNode QNode;

//Simple Queue data structure made up of QNodes
//Implemented using a linked list of QNodes
struct Queue
{
	struct QNode *front, *rear; //Pointers to the head and tail of the queue
};
typedef struct Queue Queue;

struct messageNode {
  char *message;     // copy of the message
  int len;          // length of the message
  int sender;       // TID of sender thread
  int receiver;     // TID of receiver thread
  struct messageNode *next; // pointer to next node
};

typedef struct mbox {
  struct messageNode *msg;       // message queue
  sem_t *mbox_sem;
} mbox;


struct QNode *newNode(tcb *tcb);
struct Queue *createQueue();
void enQueue(struct Queue *q, tcb *tcb);
struct QNode *deQueue(struct Queue *q);
void t_yield();
void t_init();
void t_create(void (*fct)(int), int id, int pri);
void t_shutdown();
void t_terminate();
int sem_init(sem_t **sp, int sem_count);
void sem_wait(sem_t *s);
void sem_signal(sem_t *s);
void sem_destroy(sem_t **s);
int mbox_create(mbox **mb);
void mbox_destroy(mbox **mb);
void mbox_deposit(mbox *mb, char *msg, int len);
void mbox_withdraw(mbox *mb, char *msg, int *len);
void send(int tid, char *msg, int len);
void receive(int *tid, char *msg, int *len);
