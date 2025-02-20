#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>  // Include this for strcmp
#include "pri_queue.h"

/* DO NOT MODIFY THIS FILE */

int main(int argc, char** argv) {
    Node_ptr_t head;

    // Assert the priority queue is initially empty
    assert(PQ_get_size() == 0);
    fprintf(stderr, "First assert worked!\n");

    // Insert first node with priority 0
    PQ_insert(0, "first node");
    assert(PQ_get_size() == 1);
    fprintf(stderr, "Second assert worked!\n");

    // Get the head of the queue and assert its validity
    head = PQ_get_head();
    assert(head != NULL);
    fprintf(stderr, "Third assert worked!\n");

    // Correct the string comparison using strcmp
    assert(strcmp(head->data, "first node") == 0);
    fprintf(stderr, "Fourth assert worked!\n");
    assert(head->priority == 0);
    fprintf(stderr, "Fifth assert worked!\n");

    // Insert second node with priority 5
    PQ_insert(5, "abc");
    head = PQ_get_head();
    assert(head->priority == 5);
    fprintf(stderr, "Sixth assert worked!\n");
    assert(head->next->priority == 0);
    fprintf(stderr, "Seventh assert worked!\n");

    // Insert third node with priority 3
    PQ_insert(3, "def");
    head = PQ_get_head();
    assert(head->priority == 5);
    fprintf(stderr, "Eighth assert worked!\n");
    assert(head->next->priority == 3);
    fprintf(stderr, "Ninth assert worked!\n");
    assert(head->next->next->priority == 0);
    fprintf(stderr, "Tenth assert worked!\n");

    // Insert fourth node with priority 7
    PQ_insert(7, "hij");
    head = PQ_get_head();
    assert(head->priority == 7);
    fprintf(stderr, "11th assert worked!\n");
    assert(head->next->priority == 5);
    fprintf(stderr, "12th assert worked!\n");
    assert(head->next->next->priority == 3);
    fprintf(stderr, "13th assert worked!\n");
    assert(head->next->next->next->priority == 0);
    fprintf(stderr, "14th assert worked!\n");

    // Insert fifth node with priority 2
    PQ_insert(2, "pqr");
    head = PQ_get_head();
    assert(head->priority == 7);
    fprintf(stderr, "15th assert worked!\n");
    assert(head->next->priority == 5);
    fprintf(stderr, "16th assert worked!\n");
    assert(head->next->next->priority == 3);
    fprintf(stderr, "17th assert worked!\n");
    assert(head->next->next->next->priority == 2);
    fprintf(stderr, "18th assert worked!\n");
    assert(head->next->next->next->next->priority == 0);
    fprintf(stderr, "19th assert worked!\n");

    return (EXIT_SUCCESS);
}

