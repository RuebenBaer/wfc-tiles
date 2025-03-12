#include <stdio.h>
#include <stdlib.h>

typedef struct integer_queue_item {
	int x, y;
	struct integer_queue_item *next;
} int_q;

typedef struct integer_queue_anchor {
	int_q *q_head;
	int_q *q_tail;	
} int_q_lnk;

extern int push_to_int_q (int x, int y, int_q_lnk *queue)
{
	int_q *q_temp = (int_q*) malloc(sizeof(int_q));
	if (q_temp == NULL) {
		printf("No memory available for new int_q\n");
		return 0;
	}
	q_temp->next = NULL;
	q_temp->x = x;
	q_temp->y = y;
	
	if (queue->q_tail == NULL) {
		queue->q_tail = queue->q_head = q_temp;
		return 1;
	}
	queue->q_tail->next = q_temp;
	queue->q_tail = q_temp;
	return 1;
}

extern int pop_head_int_q (int *x, int *y, int_q_lnk *queue)
{
	if (queue->q_head == NULL) {
		printf("queue is empty\n");
		return 0;
	}
	*x = queue->q_head->x;
	*y = queue->q_head->y;
	if (queue->q_head == queue->q_tail) {
		free(queue->q_head);
		queue->q_head = queue->q_tail = NULL;
		return 1;
	}
	int_q *q_temp = queue->q_head;
	queue->q_head = queue->q_head->next;
	free(q_temp);
	return 1;
}

extern int_q_lnk* initQueue(void)
{
	int_q_lnk* queue = (int_q_lnk*) malloc(sizeof(int_q_lnk));
	if (queue) {
		queue->q_head = NULL;
		queue->q_tail = NULL;
		return queue;
	}
	return NULL;
}

extern int deleteQueue(int_q_lnk* queue)
{
	int a, b;
	if (queue) {
		while (pop_head_int_q(&a, &b, queue)){}
		free(queue);
		return 1;
	}
	return 0;
}

extern int emptyQueue(int_q_lnk* queue)
{
	int a, b;
	if (queue) {
		while (pop_head_int_q(&a, &b, queue)){}
		return 1;
	}
	return 0;
}