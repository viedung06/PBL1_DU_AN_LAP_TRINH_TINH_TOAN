#ifndef FILE_IO_H
#define FILE_IO_H

#include "linked_list.h"

void read_file(List *L, int choice);
int read_hotel_layout(const char *filename, int *floor_count, int room_counts[], char rooms[][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN]);
void print_file(List *L);
void print_sort_by_date(List *L);
void print_sort_by_price(List *L);

#endif