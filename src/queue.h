#ifndef __INT_QUEUE_H
#define __INT_QUEUE_H


typedef struct integer_queue_anchor int_q_lnk;

extern int push_to_int_q (int x, int y, int_q_lnk *queue);
extern int pop_head_int_q (int *x, int *y, int_q_lnk *queue);

extern int_q_lnk* initQueue(void);
extern int deleteQueue(int_q_lnk* queue);


#endif /*  __INT_QUEUE_H */