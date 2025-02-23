#include <stdio.h>
#include <stdlib.h>

typedef struct integer_queue {
	int x, y;
	struct integer_queue *next;
}int_q;

int_q *q_head = NULL;
int_q *q_tail = NULL;

extern int push_to_int_q (int x, int y)
{
	int_q *q_temp = (int_q*) malloc(sizeof(int_q));
	if (q_temp == NULL) {
		printf("No memory available for new int_q\n");
		return 0;
	}
	q_temp->next = NULL;
	q_temp->x = x;
	q_temp->y = y;
	
	if (q_tail == NULL) {
		q_tail = q_head = q_temp;
		return 1;
	}
	q_tail->next = q_temp;
	q_tail = q_temp;
	return 1;
}

extern int pop_head_int_q (int *x, int *y)
{
	if (q_head == NULL) {
		printf("queue is empty\n");
		return 0;
	}
	*x = q_head->x;
	*y = q_head->y;
	if (q_head == q_tail) {
		free(q_head);
		q_head = q_tail = NULL;
		return 1;
	}
	int_q *q_temp = q_head;
	q_head = q_head->next;
	free(q_temp);
	return 1;
}