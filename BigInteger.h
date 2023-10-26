#ifndef BigInteger_H_INCLUDED
#define BIGInteger_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node {
    int data;
    struct node* next;
}Node;

struct BigInteger {
    struct node* head;
    int length;
    int sign; // 1 for positive, -1 for negative, 0 for zero
};



struct node* createNode(int);

void addDigit(struct BigInteger*, int);

struct BigInteger initialize(const char*);


void reverse(struct BigInteger*);

void display(struct BigInteger);

struct BigInteger sub(struct BigInteger, struct BigInteger);

struct BigInteger add(struct BigInteger, struct BigInteger);

struct BigInteger mul(struct BigInteger a,struct BigInteger b);

struct BigInteger div1(struct BigInteger a,struct BigInteger b);

struct BigInteger mod(struct BigInteger a,struct BigInteger b);
#endif //BIG_INT_H_INCLUDED