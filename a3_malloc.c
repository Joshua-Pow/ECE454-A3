#include "a3_malloc.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ALLOC_SIZE 4096

struct h_Node {
    int STATUS;           // defines the status of the block 0 if it is free and 1 if it is
                          // blocked
    size_t SIZE;          // defines the size of the block in bytes
    void* c_blk;          // defines the starting address of the current block
    void* n_blk;          // defines the starting address of the next block
    struct h_Node* NEXT;  // points to the next h_Node containing the data for the
                          // next block linked to this one
};

struct h_List {
    struct h_Node* head;  // points to the first h_Node of the list
    struct h_Node* tail;  // points to the last h_Node of the list
} h_list;

void *start;

h_Node* create_h_Node(int STATUS, size_t SIZE, void* c_blk, void* n_blk) {
    h_Node* newNode = (h_Node*)malloc(sizeof(h_Node));
    newNode->STATUS = STATUS;
    newNode->SIZE = SIZE;
    newNode->c_blk = c_blk;
    newNode->n_blk = n_blk;
    newNode->NEXT = NULL;
    return newNode;
}

void h_layout(h_Node* ptr) {
    h_Node* currentNode = ptr;
    if (ptr == NULL) {
        printf("NULL\n");
        return;
    }
    while (ptr->NEXT != NULL) {
        printf("--------------------\n");
        printf("STATUS: %d\n", ptr->STATUS);
        printf("SIZE: %lu\n", ptr->SIZE);
        printf("c_blk: %p\n", ptr->c_blk);
        printf("n_blk: %p\n", ptr->n_blk);
        ptr = ptr->NEXT;
    }
}

int m_init(void) { 
    start = sbrk(ALLOC_SIZE);
    if (!start){
        return -1;
    }
    h_list.head = (struct h_Node *) start;
    h_list.head->STATUS = 0;
    h_list.head->SIZE = ALLOC_SIZE - sizeof(h_Node);
    h_list.head->c_blk = start + sizeof(h_Node);
    h_list.head->n_blk = NULL;
    h_list.head->NEXT = NULL;
    return 0;
    }

void *m_malloc(size_t size) {
    struct h_Node *node = h_List->start;

    while(node != NULL) {
        if (!node->STATUS && node->SIZE >= size) {

            struct h_Node *newNode = node->c_blk + size;

            newNode->STATUS = 0;
            newNode->SIZE = node->SIZE - size - sizeof(struct h_Node);
            newNode->c_blk = newNode + sizeof(struct h_Node);
            newNode->n_blk = node->n_blk;
            newNode->NEXT = node->NEXT;

            node->STATUS = 1;
            
            return node->c_blk;
        }
        node = node->NEXT;
    }
    return NULL;
}

int main(int argc, char const* argv[]) {
    void* c_break;
    printf("0 - %p \n", c_break);
    c_break = sbrk(4096);
    printf("1 - %p \n", c_break);
    c_break = sbrk(4096);
    printf("2 - %p \n", c_break);
    c_break = sbrk(4096);
    printf("3 - %p \n", c_break);
    return 0;
}
