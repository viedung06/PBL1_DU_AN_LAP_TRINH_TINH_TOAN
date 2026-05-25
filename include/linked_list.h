#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "hotel_types.h"

typedef struct Node { 
    date data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;

void init(List *L);
int check_empty(List *L);
int list_length(List *L);
void clear_list(List *L);

void add_head(List *L, date data);
void add_tail(List *L, date data);
void add_k_place(List *L, date data, int k);
void delete_head(List *L);
void delete_tail(List *L);
void delete_k_place(List *L, int k);

#endif