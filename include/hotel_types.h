#ifndef HOTEL_TYPES_H
#define HOTEL_TYPES_H

#define FILE_LAYOUT      "files/your_hotel.txt"
#define FILE_INPUT       "files/input.txt"
#define FILE_DATA_OUT    "files/customer_bill.txt"
#define FILE_SORT_DATE   "files/sort_by_date_in.txt"
#define FILE_SORT_PRICE  "files/sort_by_price.txt"

#define MAX_FLOORS 50
#define MAX_ROOMS_PER_FLOOR 100
#define MAX_ROOM_NAME_LEN 30

// Global variables (Được định nghĩa tại main.c)
extern int floor_count;
extern int room_counts[MAX_FLOORS];
extern char rooms[MAX_FLOORS][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN];

typedef struct {
    char name[50];
    char identity_card[15];
    char gender[10];
} info;

typedef struct {
    int date_in;
    int date_out;
    long long gia_phong;
    long long tien_phong; 
    char room_type[20];
    char invoice_code[10];
    info customer_info;
} date;

#endif