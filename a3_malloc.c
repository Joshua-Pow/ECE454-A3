#include "a3_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ALLOC_SIZE 4096

struct h_Node {
    int STATUS;           // defines the status of the block 0 if it is free and 1 if it is
                          // blocked
    size_t SIZE;          // defines the size of the block in bytes
    void* c_blk;          // defines the starting address of the current block
    void* n_blk;          // defines the starting address of the next block
    struct h_Node* NEXT;  // points to the next h_Node containing the data for the
                          // next block linked to this one
    struct h_Node* PREV;  // points to the previous h_Node containing the data for
                          // the previous block linked to this one
};

struct h_List {
    int usedSize;         // defines the size of the used memory in the heap
    struct h_Node* head;  // points to the first h_Node of the list
    // struct h_Node* tail;  // points to the last h_Node of the list
};

void* start;
h_List memoryList;

void h_layout(h_Node* ptr) {
    h_Node* currentNode = ptr;
    if (currentNode == NULL) {
        printf("NULL Pointer\n");
        return;
    }

    do {
        printf("--------------------\n");
        printf("NODE: %p\n", currentNode);
        printf("STATUS: %d\n", currentNode->STATUS);
        printf("SIZE: %lu\n", currentNode->SIZE);
        if (currentNode->STATUS == 1) printf("SIZE with header: %lu\n", currentNode->SIZE + sizeof(h_Node));
        printf("c_blk: %p\n", currentNode->c_blk);
        printf("n_blk: %p\n", currentNode->n_blk);
        printf("NEXT: %p\n", currentNode->NEXT);
        currentNode = currentNode->NEXT;
    } while (currentNode != NULL);
}

// Finds the best fit of size in the heap
h_Node* findBestFit(size_t size) {
    h_Node* currentNode = memoryList.head;
    h_Node* bestFit = NULL;
    size_t sizeWithHeader = size + sizeof(h_Node);

    while (currentNode != NULL) {
        if (currentNode->STATUS == 0 && currentNode->SIZE >= sizeWithHeader) {
            if (bestFit == NULL) {
                bestFit = currentNode;
            } else if (currentNode->SIZE < bestFit->SIZE) {
                bestFit = currentNode;
            }
        }
        currentNode = currentNode->NEXT;
    }

    return bestFit;
}

int m_check(void) {
    h_Node* currentNode = memoryList.head;

    // Loop through the list making sure theres no where that can be coalesced
    // If so return -1
    while (currentNode != NULL) {
        // If the current block is free
        if (currentNode->STATUS == 0) {
            // If the next block exists and previous block exist
            if (currentNode->NEXT != NULL && currentNode->PREV != NULL) {
                // If any blocks adjacent are free
                if (currentNode->NEXT->STATUS == 0 || currentNode->PREV->STATUS == 0) {
                    return -1;
                }
            } else if (currentNode->NEXT != NULL) {  // If only next block exists
                if (currentNode->NEXT->STATUS == 0) {
                    return -1;
                }
            } else if (currentNode->PREV != NULL) {  // If only previous block exists
                if (currentNode->PREV->STATUS == 0) {
                    return -1;
                }
            }
        }
        currentNode = currentNode->NEXT;
    }

    return 0;
}

void m_free(void* ptr) {
    // Starting of the block is the h_Node
    if (ptr == NULL) {
        printf("NULL Pointer\n");
        return;
    }

    printf("Free ptr: %p\n", ptr);
    printf("Freeing %p\n", ptr - sizeof(h_Node));

    h_Node* currentNode = (h_Node*)((char*)ptr - sizeof(h_Node));  // ptr is a pointer to the allocated block NOT the h_Node
    currentNode->STATUS = 0;                                       // Set current block to free since were not going to use it
    currentNode->SIZE += sizeof(h_Node);                           // Add the header size back

    // Keep track of used memory
    memoryList.usedSize -= (currentNode->SIZE + sizeof(h_Node));

    // Coalescing

    // If the next block exists and previous block exist
    if (currentNode->NEXT != NULL && currentNode->PREV != NULL) {
        // If both blocks are free
        if (currentNode->NEXT->STATUS == 0 && currentNode->PREV->STATUS == 0) {
            currentNode->PREV->SIZE += currentNode->SIZE + currentNode->NEXT->SIZE;
            currentNode->PREV->NEXT = currentNode->NEXT->NEXT;
        } else if (currentNode->NEXT->STATUS == 0) {  // If only next block is free
            currentNode->SIZE += currentNode->NEXT->SIZE;
            currentNode->NEXT = currentNode->NEXT->NEXT;
        } else if (currentNode->PREV->STATUS == 0) {  // If only previous block is free
            currentNode->PREV->SIZE += currentNode->SIZE;
            currentNode->PREV->NEXT = currentNode->NEXT;
        }
    } else if (currentNode->NEXT != NULL) {  // If only next block exists
        if (currentNode->NEXT->STATUS == 0) {
            currentNode->SIZE += currentNode->NEXT->SIZE;
            currentNode->NEXT = currentNode->NEXT->NEXT;
        }
    } else if (currentNode->PREV != NULL) {  // If only previous block exists
        if (currentNode->PREV->STATUS == 0) {
            currentNode->PREV->SIZE += currentNode->SIZE;
            currentNode->PREV->NEXT = currentNode->NEXT;
        }
    }

    if (m_check() == -1) {
        printf("Error in m_free()\n");
    }
    return;
}

int m_init(void) {
    start = sbrk(ALLOC_SIZE + sizeof(h_Node));  // heap
    if (!start) {
        return -1;
    }

    // The head of the memory list starts at the head of the heap
    memoryList.head = start;
    memoryList.head->STATUS = 0;
    memoryList.head->SIZE = ALLOC_SIZE;
    memoryList.head->c_blk = (char*)start + sizeof(h_Node);
    memoryList.head->n_blk = NULL;
    memoryList.head->NEXT = NULL;
    memoryList.head->PREV = NULL;
    return 0;
}

void* m_malloc(size_t size) {
    h_Node* node = findBestFit(size);
    if (node == NULL) {
        printf("No best fit found\n");
        return NULL;
    }

    // Represents where the new free space is going to be
    struct h_Node* newNode = (h_Node*)((char*)(node->c_blk) + size);

    newNode->STATUS = 0;
    newNode->SIZE = node->SIZE - size - sizeof(h_Node);
    newNode->c_blk = (char*)newNode + sizeof(h_Node);
    newNode->n_blk = node->n_blk;
    newNode->NEXT = node->NEXT;
    newNode->PREV = node;

    // Represents the already allocated space, but with data in it
    node->STATUS = 1;
    // node->c_blk = node + sizeof(h_Node);
    node->n_blk = newNode->c_blk;
    node->SIZE = size;
    node->NEXT = newNode;
    // node->PREV = node->PREV;

    // Keep track of used memory
    memoryList.usedSize += size + sizeof(h_Node);

    return node->c_blk;

    return NULL;
}

void* m_realloc(void* ptr, size_t size) {
    // h_Node* currentNode = (h_Node*)ptr;

    h_Node* newNode = m_malloc(size);
    if (newNode == NULL) {
        printf("Error in m_realloc()\n");
        return NULL;
    }
    m_free(ptr);

    //  Set current block to free since were not going to use it
    // currentNode->STATUS = 0;
    // currentNode->SIZE += sizeof(h_Node);  // Add the header size back

    return newNode;
}

void printSpaceUtilization() {
    printf("Space utilization: %.4f\n", (float)memoryList.usedSize / (float)ALLOC_SIZE);
}

// instructions 385755
// seconds = 0.002817
// 136 938 232 instructions/sec
void driverGood() {
    m_init();
    h_layout(memoryList.head);

    printf("\nm_malloc(1000)\n");
    void* pt1 = m_malloc(1000);  // pt1 is the pointer to the allocated block NOT the h_Node
    printSpaceUtilization();
    h_layout(memoryList.head);

    printf("\nm_malloc(1000)\n");
    void* pt2 = m_malloc(1000);
    printSpaceUtilization();
    h_layout(memoryList.head);

    printf("\nm_malloc(1000)\n");
    void* pt3 = m_malloc(1000);
    printSpaceUtilization();
    h_layout(memoryList.head);

    printf("\nfree(pt2)\n");
    m_free(pt2);
    printSpaceUtilization();
    h_layout(memoryList.head);

    printf("\nm_malloc(pt2, 3000)\n");
    pt2 = m_malloc(400);
    printSpaceUtilization();
    h_layout(memoryList.head);
    // printf("\nm_malloc(2000)\n");
    // char *pt1 = m_malloc(2000);
    // h_layout(memoryList.head);
    // printf("\nm_malloc(500)\n");
    // char *pt2 = m_malloc(500);
    // h_layout(memoryList.head);
    // printf("\nm_malloc(300)\n");
    // char *pt3 = m_malloc(300);
    // h_layout(memoryList.head);
    // printf("\nm_free(500)\n");
    // m_free(pt2);
    // h_layout(memoryList.head);
    // printf("\nm_malloc(1500)\n");
    // pt3 = m_malloc(1500);
    // h_layout(memoryList.head);
}

// instuctions = 306705
// seconds = 0.002467
// 124,323,064 instructions/sec
void driverBad() {
    // Number of operations per second executed through each driver
    //  ????
    m_init();
    h_layout(memoryList.head);

    printf("\nm_malloc(1000)\n");
    void* pt1 = m_malloc(1000);  // pt1 is the pointer to the allocated block NOT the h_Node
    printSpaceUtilization();
    h_layout(memoryList.head);

    printf("\nm_malloc(1000)\n");
    void* pt2 = m_malloc(1000);
    printSpaceUtilization();
    h_layout(memoryList.head);

    printf("\nm_malloc(4000)\n");
    void* pt3 = m_malloc(4000);
    printSpaceUtilization();
    h_layout(memoryList.head);
}

int main(int argc, char const* argv[]) {
    // void* c_break;
    // printf("0 - %p \n", c_break);
    // c_break = sbrk(4096);
    // printf("1 - %p \n", c_break);
    // c_break = sbrk(4096);
    // printf("2 - %p \n", c_break);
    // c_break = sbrk(4096);
    driverGood();
    return 0;
}
