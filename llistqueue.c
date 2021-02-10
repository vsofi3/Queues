/*
 * implementation for linked-list-based generic FIFO queue
 */

#include "ADTs/llistqueue.h"
#include <stdlib.h>


/* any other includes needed for the implementation */

typedef struct qnode{
	struct qnode *next;
	void *value; //value the node holds
} QNode;

typedef struct q_data{
    /* flesh out the instance specific data structure */
	long size; //number of elements in queue
	QNode *head;
	QNode *tail;
	void (*freeValue)(void *e);
} QData;
/* any other data structures needed */


static void purge(QData *qd, void(*freeV)(void *e)){
	if(freeV != NULL){
		QNode *p;
		for(p = qd->head; p != NULL; p = p->next)
			(*freeV)(p->value);
	} //DONE
}

static void freeList(QData *qd){
	//traverse list, freeing nodes in the list
	QNode *p, *q = NULL;
	for(p = qd->head; p != NULL; p = q){
		q = p->next;
		free(p);
	}
	//DONE
}

static void q_destroy(const Queue *q) {
    /* implementation of the destroy() method */
	QData *queue = (QData *)(q->self);
	//I think I need to loop through the list and delete nodes as well
	purge(queue, queue->freeValue);
	freeList(queue);
	free(queue);
	free((void *)q);
}


static void q_clear(const Queue *q) {
    /* implementation of the clear() method */
	QData *queue = (QData *)(q->self);
	//free the nodes of the linked list
	purge(queue, queue->freeValue);
	freeList(queue);
	queue->head = queue->tail = NULL;
	queue->size = 0L;
	//DONE
}

static int q_enqueue(const Queue *q, void *element) {
    /* implementation of the enqueue() method */
	QData *queue = (QData *)(q->self);
	//add element to the tail of the queue; if the queue is full, the method returns 0; else, it returns 1
	QNode *new = (QNode *)malloc(sizeof(QNode));
	int status = 0;

	if(new != NULL){
		new->value = element;
		status = 1;
		if(queue->size == 0){
			queue->head = queue->tail = new;
		} else{
			queue->tail->next = new;
			queue->tail = new;
		}
		new->next = NULL;
		queue->size++;
	}
	return status;
	//DONE
}

static int q_front(const Queue *q, void **element) {
    /* implementation of the front() method */
	//return the element at the head of the queue in void **element WITHOUT removing it from the queue; if the queue has no elements, return 0, else, return 1
	QData *queue = (QData *)(q->self);
	int status = (queue->size > 0L);
	if(status){
		*element = queue->head->value;
	}
	return status;
	//DONE
}

static int q_dequeue(const Queue *q, void **element) {
    /* implementation of the dequeue() method */
	//retrieves and REMOVES the head of the queue and returns it in the void **element
	QData *queue = (QData *)(q->self);
	int status = (queue->size > 0L);
	if(status){
		QNode *node = queue->head;
		if((queue->head = node->next) == NULL){
			queue->tail = NULL;
		}
		*element = node->value;
		queue->size--;
		free(node);
	}
	return status;
	//DONE
}

static long q_size(const Queue *q) {
    /* implementation of the size() method */
	QData *queue = (QData *)(q->self);
	return queue->size;
}

static int q_isEmpty(const Queue *q) {
	//if the list is is empty, both head and tail are NULL
	QData *queue = (QData *)(q->self);
	return (queue->size == 0L);
}

//helper function to create array of void *'s for toArray and itCreate functions
static void **genArray(QData *q){
	void **theArray = NULL;
	if(q->size > 0L){
		theArray = (void **)malloc(q->size*sizeof(void *));
		if(theArray != NULL){
			long i = 0L;
			QNode *node;
			for(node = q->head; node != NULL; node = node->next)
				theArray[i++] = node->value;
		}
	}
	return theArray;
	//DONE
} 


static void **q_toArray(const Queue *q, long *len) {
    /* implementation of the toArray() method */
	//generate array of element values on the heap
	//returns pointer to array or NULL if malloc failure
	QData *queue = (QData *)(q->self);
	void **temp = genArray(queue); //call function
	if(temp != NULL)
		*len = queue->size; //store in *len
	return temp; //void **
} //DONE?


static const Iterator *q_itCreate(const Queue *q) {
    /* implementation of the itCreate() method */
	QData *queue = (QData *)(q->self);
	const Iterator *it = NULL;
	void **temp = genArray(queue);
	if(temp != NULL){
		it = Iterator_create(queue->size, temp);
		if(it == NULL)
			free(temp);
	}
	return it; //return the iterator
	//DONE
}

static const Queue *q_create(const Queue *q);

static Queue template = {
    NULL, q_create, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size, q_isEmpty, q_toArray, q_itCreate};



static void doNothing(__attribute__((unused)) void *e){
}


//helper function for queue creation
static const Queue *newQueue(void(*freeV)(void *)){
	Queue *q = (Queue *)malloc(sizeof(Queue));
	if(q != NULL){
		QData *qd = (QData *)malloc(sizeof(QData));
		if(qd != NULL){
			qd->size = 0L;
			qd->head = NULL;
			qd->tail = NULL;
			qd->freeValue = (freeV != NULL) ? freeV : doNothing;
			*q = template;
			q->self = qd;
		}
		else{
			free(q);
			q = NULL;
		}
	}
	return q;
	//DONE
}


static const Queue *q_create(const Queue *q) {
    /* implementation of the create() method */
	QData *queue = (QData *)(q->self);
	//create a queue from a queue that already exists
	return newQueue(queue->freeValue);
}


const Queue *LListQueue(void (*freeValue)(void *e)) {
    /* implementation of the structure-specific constructor */
	return newQueue(freeValue);
}

const Queue *Queue_create(void (*freeValue)(void *e)) {
    /* implementation of the generic constructor */
	return newQueue(freeValue);
}
