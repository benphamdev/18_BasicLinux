#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"

/** @file pri_queue.c */
static Node_ptr_t head = NULL;

/**
 * Insert a Node into a priority queue.
 * @param priority The priority of the node.
 * @param data The data to be stored in the node.
 * @author YOUR NAME
 */
void PQ_insert(int priority, char *data) {
    // Create a new node
    Node_ptr_t new_node = (Node_ptr_t) malloc(sizeof(Node_t));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);  // Exit if memory allocation fails
    }

    new_node->priority = priority;
    new_node->data = data;  // Assuming data is a valid string passed by the caller
    new_node->next = NULL;

    // Special case: Insert at the beginning if the queue is empty or the new node has the highest priority
    if (head == NULL || head->priority < priority) {
        new_node->next = head;
        head = new_node;
        return;
    }

    // Find the right position in the queue to insert the node based on priority
    Node_ptr_t current = head;
    while (current->next != NULL && current->next->priority >= priority) {
        current = current->next;
    }

    // Insert the new node after the current node
    new_node->next = current->next;
    current->next = new_node;
}

/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete() {
    if (head == NULL) {
        return NULL;  // Return NULL if the queue is empty
    }

    // The node to be deleted is the head (highest priority)
    Node_ptr_t deleted_node = head;
    head = head->next;  // Move the head pointer to the next node
    deleted_node->next = NULL;  // Disconnect the deleted node from the list
    return deleted_node;
}

/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list. (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head() {
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size() {
    int size = 0;
    for (Node_ptr_t tmp = head; tmp != NULL; tmp = tmp->next, size++);
    return size;
}

