#include <stdio.h>

//Define/Declare structure a_test1 and its array container as:
struct a_test1 {
 short a;
 int x;
 short b;
} table1[10];
struct a_test1 *p1, *q1;
// a-Print the sizeof each member type in struct a_test1 (here short and int)
// b-Print the size of table1[0] and then the sizeof the whole table1
// c-Print the alignment and address of table1[0]
// d-In a loop, print the aligned addresses of the previous and the current elements in table1 (starting with table1[1]),
// and then the difference between their addresses showing the length of the previous element in the array

// int main() {
//     printf("sizeof(p1->a) (short): %d\nsizeof(p1->x) (int): %d\nsizeof(p1-?b) (short): %d\n\n", sizeof(p1->a), sizeof(p1->x), sizeof(p1->b));
//     printf("sizeof(table1[0]): %d\nsizeof(table1): %d\n\n", sizeof(table1[0]), sizeof(table1));

//     printf("The address of item 0 is %p\n", &table1[0]);
//     for (int i = 1; i < 10; i++){
//         printf("The difference between address %d and address %d is %p\n\n", i-1, i, ((int)&table1[i] - (int)&table1[i-1]));
//         printf("The address of item %d is %p\n", i, &table1[i]);
//     }

// }


//Optimization: Add a new structure a_test2 and its array container as:
struct a_test2 {
 short a;
 short b;
 int x;
} table2[10];
 struct a_test2 *p2, *q2;
//Apply the same instructions above (a-d) on the new a_test2 struct,
//save the results in proper tables and describe about them in your report
// your optimized structure will be compared with its initial format

// int main() {
//     printf("sizeof(p2->a) (short): %d\nsizeof(p2->x) (int): %d\nsizeof(p2->b) (short): %d\n\n", sizeof(p2->a), sizeof(p2->x), sizeof(p2->b));
//     printf("sizeof(table1[0]): %d\nsizeof(table1): %d\n\n", sizeof(table1[0]), sizeof(table1));

//     printf("The address of item 0 is %p\n", &table2[0]);
//     for (int i = 1; i < 10; i++){
//         printf("The difference between address %d and address %d is %p\n\n", i-1, i, ((int)&table2[i] - (int)&table2[i-1]));
//         printf("The address of item %d is %p\n", i, &table2[i]);
//     }

// }



//define a new structure: a-test3 as below and apply the same instructions above (a-d) on it
struct a_test3 {
 char c;
 short s;
 int k;
 long l;
 float f;
 double d; } table3[10];
struct a_test3 *p3, *q3;
// Try to optimize it, and save the results in proper tables and describe about them in your report
// your optimized structure will be compared with its initial format

int main() {
    printf("sizeof(p3->c) (char): %d\nsizeof(p3->s) (short): %d\nsizeof(p3->k) (int): %d\nsizeof(p3->l) (long): %d\nsizeof(p3->f) (float): %d\nsizeof(p3->d) (double): %d\n\n", sizeof(p3->c), sizeof(p3->s), sizeof(p3->k), sizeof(p3->l), sizeof(p3->f), sizeof(p3->d));
    printf("sizeof(table4[0]): %d\nsizeof(table4): %d\n\n", sizeof(table3[0]), sizeof(table3));

    printf("The address of item 0 is %p\n", &table3[0]);
    for (int i = 1; i < 10; i++){
        printf("The difference between address %d and address %d is %p\n\n", i-1, i, ((int)&table3[i] - (int)&table3[i-1]));
        printf("The address of item %d is %p\n", i, &table3[i]);
    }

}
struct a_test4 {
 char c;
 char bufferChar;
 short s;
 short bufferString;
 int k;
 float f;
 long l;
 double d; } table4[10];
struct a_test4 *p4, *q4;

// int main() {
//     printf("sizeof(p3->c) (char): %d\nsizeof(p3->s) (short): %d\nsizeof(p3->k) (int): %d\nsizeof(p3->l) (long): %d\nsizeof(p3->f) (float): %d\nsizeof(p3->d) (double): %d\n\n", sizeof(p4->c), sizeof(p4->s), sizeof(p4->k), sizeof(p4->l), sizeof(p4->f), sizeof(p4->d));
//     printf("sizeof(table4[0]): %d\nsizeof(table4): %d\n\n", sizeof(table4[0]), sizeof(table4));

//     printf("The address of item 0 is %p\n", &table4[0]);
//     for (int i = 1; i < 10; i++){
//         printf("The difference between address %d and address %d is %p\n\n", i-1, i, ((int)&table4[i] - (int)&table4[i-1]));
//         printf("The address of item %d is %p\n", i, &table4[i]);
//     }

// }