#ifndef HOTEL_SERVICE_H
#define HOTEL_SERVICE_H

#include "linked_list.h"

void clear_input_buffer();
int get_safe_int(const char *prompt, int min, int max);
void read_line(char *buf, int size);
void generate_invoice_code(char *code, int date_in);
void normalize_room_code(char *room);
char *lastName(char *name);
void add_dot(char a[], int n, char b[]);
int check_invoiceCode_exist(List *L, char *invoice);
int check_room_exist(List *L, char *room, int choice);
int check_room_conflict(List *L, char *room, int date_in, int date_out, int choice);
int check_name(char *str);
int check_identity_with_different_name(List *L, char *CMT, char *name);
Node* check_identity_with_different_name_node(List *L, char *CMT, char *name);

int get_days(int in, int out);
char get_room(char *room_number);
int get_price(char type);
int get_max_days_current_month();


void thongke(List *L);
void thongke_theo_ngay(List *L, int date);
void searchCMT(List *L, char *CMT);
void searchRoom(List *L, char *room, int choice);
void sort_by_date_in(List *L);
void sort_by_price(List *L);

void input_data(List *L, date *d, int choice);
void delete_more_customer(List *L);
void popMinh(List *L);
void change_customer_info(List *L, int choice);

#endif