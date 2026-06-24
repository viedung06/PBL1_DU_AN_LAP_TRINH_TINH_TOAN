#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#include "ui.h"

// Input: Con trỏ List L | Output: Danh sách được cấp phát head, tail = NULL
void init(List *L) { 
    L->head = NULL;
    L->tail = NULL;
}

int check_empty(List *L) { return (L->head == NULL); }

// Input: Con trỏ List L, data | Output: Node mới được chèn vào cuối List
void add_tail(List *L, date data) { 
    Node *p = (Node*)malloc(sizeof(Node));
    if(p == NULL){
        printf(INDENT RED "Khong du bo nho de cap phat node moi!\n" RESET);
        return;
    }
    p->data = data;
    p->next = NULL;
    if (check_empty(L)) {
        L->head = p;
        L->tail = p;
    } else {
        L->tail->next = p;
        L->tail = p;
    }
    printf(INDENT BLUE"Da them hoa don vao cuoi danh sach.\n" RESET);
}

// Input: Con trỏ List L, data | Output: Node mới được chèn vào đầu List
void add_head(List *L, date data) { 
    Node *p = (Node*)malloc(sizeof(Node));
    if(p == NULL){
        printf(INDENT RED "Khong du bo nho de cap phat node moi!\n" RESET);
        return;
    }
    p->data = data;
    p->next = L->head;
    if (check_empty(L)) {
        L->head = p;
        L->tail = p;
    } else L->head = p;
    printf(INDENT BLUE"Da them hoa don vao dau danh sach.\n" RESET);
}

// Input: Con trỏ List L | Output: Node đầu tiên bị xóa, giải phóng memory
void delete_head(List *L) { 
    if (check_empty(L)) {
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET);
    } else {
        Node *p = L->head;
        L->head = L->head->next;
        if (L->head == NULL) L->tail = NULL; 
        free(p);
    }
    printf(INDENT BLUE "Da xoa hoa don o dau danh sach.\n" RESET);
}

// Input: Con trỏ List L | Output: Node cuối cùng bị xóa, cập nhật tail
void delete_tail(List *L) { 
    if (check_empty(L)) {
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
    } else {
        Node *p = L->head;
        if (p->next == NULL) {
            free(p);
            p = NULL;
            L->head = NULL;
            L->tail = NULL;
        } else {
            while (p->next != L->tail) p = p->next;
            free(L->tail);
            L->tail = p;
            p->next = NULL;
        }
    }
    printf(INDENT BLUE "Da xoa hoa don cuoi danh sach.\n" RESET);
}

int list_length(List *L) { 
    int count = 0;
    Node *p = L->head;
    while (p != NULL) { count++; p = p->next; }
    return count;
}

// Input: Con trỏ List L, vị trí k | Output: Xóa node tại vị trí k (1-based)
void delete_k_place(List *L, int k) { 
    if (check_empty(L)) {
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
    } else {
        if (k == 1) {
            delete_head(L);
            return;
        } else {
            Node *p = L->head;
            for (int i = 1; i < k - 1 && p != NULL; i++) p = p->next;
            if (p == NULL || p->next == NULL) {
                delete_tail(L);
                return;
            } else {
                Node *temp = p->next;
                p->next = temp->next;
                if (temp == L->tail) L->tail = p;
                free(temp);
                temp = NULL;
            }
        }
    }
    printf(INDENT BLUE "Da xoa hoa don tai vi tri %d cua danh sach.\n" RESET, k);
}

void add_k_place(List *L, date data, int k) { 
    int len = list_length(L);
    if (k == 1) { add_head(L, data); return; } 
    else if(k == len + 1) { add_tail(L, data); return; } 
    else if (k > 1 && k <= len) {
        Node *p = (Node*)malloc(sizeof(Node));
        if(p == NULL) return;
        p->data = data;
        Node *temp = L->head;
        for (int i = 1; i < k - 1 && temp != NULL; i++) temp = temp->next;
        p->next = temp->next;
        temp->next = p;
    }
    printf(INDENT BLUE "Da them hoa don moi vao vi tri thu %d cua danh sach.\n" RESET, k);
}

// Input: Con trỏ List L | Output: Toàn bộ danh sách bị xóa, tránh rò rỉ RAM
void clear_list(List *L) { 
    if (L == NULL || L->head == NULL) return; //Danh sach rong, khong can giai phong 
    Node *current = L->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
        temp = NULL;
    }
    L->head = NULL;
    L->tail = NULL;
}