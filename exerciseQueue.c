#include "ADTs/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void printQueue(const Queue *q, FILE *fp){
	//check if queue is empty, then return to exit
	if(q->isEmpty(q)){
		fprintf(fp,"Empty\n");
		return;
	}
	
	long element;
	bool first = true;

	//main logic for print, enqueue, dequeue
	const Iterator *it = q->itCreate(q);

	//from head to tail
       	while(it->hasNext(it)){
		(void) it->next(it, (void **) &element);
		fprintf(fp, "%s%ld", first ? "" : " ", element);
		first = false;
	}
	fprintf(fp, "\n");
	
	it->destroy(it);
}


int main(int argc, char *argv[]){
	FILE *fd = NULL;
	char buf[BUFSIZ];

	const Queue *q = Queue_create(NULL);
	if(q == NULL){ //queue not created correctly
		fprintf(stderr, "Cannot create queue");
		return EXIT_FAILURE;
	}
	if(argc != 2){
		fprintf(stderr, "Program takes a single command-line argument");
		return EXIT_FAILURE;
	}
	if((fd = fopen(argv[1],"r")) == NULL){
		fprintf(stderr, "Cannot open file");
		return EXIT_FAILURE;
	}
	else{
	while(fgets(buf, BUFSIZ, fd) != NULL){
		if(strstr(buf,"print") != NULL){
			printQueue(q,stdout);
		}

		if(strstr(buf,"enqueue") != NULL){
			//get element
			//push onto the queue
			long element;
			char *instr;
			instr = malloc(sizeof(char*));

			sscanf(buf,"%s %ld",instr,&element);
			//add element to the tail of the queue
			q->enqueue(q, (void *)element);
			free(instr);
		}

		if(strstr(buf,"dequeue") != NULL){
			long element;
			if(q->isEmpty(q)){
				fprintf(stdout,"QueueError\n");
			}
			else{	
				//remove head of queue
				q->dequeue(q,(void **)&element);
				//print it
				fprintf(stdout,"%ld\n",element);
			}


		}
	}

	q->destroy(q); //destroy the queue
	fclose(fd); //close the file
	return EXIT_SUCCESS;
	}



}
