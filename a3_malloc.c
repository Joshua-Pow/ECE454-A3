#include "a3_malloc.h"

#include <stdio.h>
#include <unistd.h>

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
};

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

int m_init(void) { return 0; }

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
