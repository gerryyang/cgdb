#include "queue.h"
#include "error.h"
#include "sys_util.h"

struct queue_iterator {
    struct node *node;
};

struct node {
   void *data;
   struct node *next;
};

struct queue {
   int size;
   struct node *head;
};

struct queue *queue_init(void) {
   struct queue *q = (struct queue *)xmalloc(sizeof(struct queue));
   q->size = 0;
   q->head = NULL;
   return q;
}

void queue_append(struct queue *q, void *item) {
    struct node *cur = q->head, *node;

    /* Allocate the new node */
    node = (struct node *)xmalloc(sizeof(struct node));
    node->data = item;
    node->next = NULL;

    if ( q->head == NULL )
        q->head = node;   
    else {
       while ( cur -> next != NULL )
           cur = cur -> next;
       
       cur->next = node;
    }
    q->size = q->size + 1;
}

void *queue_pop(struct queue *q) {
    struct node *cur = q->head;
    void *d;

    if ( cur == NULL )
        return (void *)NULL;

    /* Move the head of the queue forward */
    q->head = q->head->next;
    
    /* Assertion: cur is the leftover node that needs to be freed */
    d = cur->data;
    free(cur);
    cur = NULL;

    q->size = q->size - 1;

    return d;
}

void queue_free_list(struct queue *q, item_func func) {
    struct node *prev, *cur = q->head;
   
    if ( cur == NULL || (!func))
        return;

    while ( cur != NULL ) {
        prev = cur;
        cur = cur->next;
        /* Remove the previous node */
        func ( prev->data );
        free ( prev );
        prev = NULL; 
    }

    q->size = 0;
    q->head = NULL;
}

void queue_traverse_list(struct queue *q, item_func func) {
    struct node *cur = q->head;  

    if ( !func )
       return;

    while ( cur != NULL ) {
        func ( cur->data );
        cur = cur->next;
    }
}

int queue_size(struct queue *q) {
   /* This list is empty */
   if ( q->head == NULL )
       return 0;
   else
      return q->size;
}

struct queue_iterator *queue_iterator_init(void) {
   struct queue_iterator *i = (struct queue_iterator *)xmalloc(sizeof(struct queue_iterator));
   i->node = NULL;
   return i;
}

void queue_iterator_free ( struct queue_iterator *i ) {
    free ( i );
    i = NULL;
}

void queue_iterator_reset ( struct queue *q, struct queue_iterator *i ) {
    i->node = NULL;

    if ( q == NULL )
        return;

    i->node = q->head;
}

void queue_iterator_forward ( struct queue_iterator *i ) {
    if ( i->node != NULL )
        i-> node = i->node->next;

}

int queue_iterator_end_of_list ( struct queue_iterator *i ) {
    if ( i->node == NULL )
        return 1;
    else
        return 0;
}

void *queue_iterator_get_item ( struct queue_iterator *i ) {
    return i->node->data;
}
