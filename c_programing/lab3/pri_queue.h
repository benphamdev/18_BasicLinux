#ifndef PRI_QUEUE_H
#define PRI_QUEUE_H

// Node structure for priority queue
typedef struct node {
    int priority;        // Priority of the node
    char *data;          // Data associated with the node
    struct node *next;   // Pointer to the next node in the queue
} Node_t, *Node_ptr_t;

// Function declarations for priority queue operations
extern void PQ_insert(int priority, char *data); // Insert a node with priority and data
extern Node_ptr_t PQ_delete();                   // Delete the node with the highest priority
extern Node_ptr_t PQ_get_head();                 // Get the node at the front of the queue
extern int PQ_get_size();                        // Get the size of the priority queue

#endif /* PRI_QUEUE_H */

