#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "hotel_service.h"
#include "ui.h"

void clear_input_buffer() { 
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int get_safe_int(const char *prompt, int min, int max) {
    int val;
    char buf[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            if (sscanf(buf, "%d", &val) == 1 && val >= min && val <= max) {
                return val;
            }
        }
        printf(INDENT RED "Gia tri khong hop le! Vui long nhap lai trong khoang [%d, %d].\n" RESET, min, max);
        printf(INDENT BOLD_WHITE "Nhap lai: " RESET);
    }
}

void read_line(char *buf, int size) {
    if (buf == NULL || size <= 0) return;
    if(size > 500) size = 500; // Giới hạn tối đa để tránh tràn bộ nhớ
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    else clear_input_buffer();
}

void generate_invoice_code(char *code, int date_in) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(code, 11, "%02d%02d%02d%04d",
             t->tm_year % 100,
             t->tm_mon + 1,
             date_in,
             rand() % 9000 + 1000);
}

int get_max_days_current_month() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int year = t->tm_year + 1900;
    int month = t->tm_mon + 1;

    switch (month) {
        case 2:
            return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

int get_days(int in, int out){ 
    if(in == out) return 1; 
    return (out - in); 
}
char get_room(char *room_number){ return room_number[0]; }
int get_price(char type){
    if(type == 'A') return 400000;
    if(type == 'B') return 300000;
    return 200000;
}

void add_dot(char a[], int n, char b[]){ 
    if(n==0) return;
    int dots = (n - 1) / 3;
    int len = n + dots;
    int i = n - 1, j = len - 1, count = 0;
    b[len] = '\0';
    while(i >= 0){
        if(count == 3){ b[j] = '.'; j--; count = 0; }
        b[j] = a[i]; i--; j--; count++;
    }
}

void normalize_room_code(char *room) {
    char *start = room; 
    while (*start != '\0' && isspace((unsigned char)*start)) start++; 

    char *end = start + strlen(start) - 1; 
    while (end >= start && isspace((unsigned char)*end)) { 
        *end = '\0'; 
        end--; 
    }
    // kéo phần chữ bị dịch về đầu room
    if (start != room) memmove(room, start, strlen(start) + 1); 
    // Chuyển toàn bộ thành chữ in hoa
    for (char *p = room; *p != '\0'; p++) *p = toupper((unsigned char)*p);
}

int check_room_exist(List *L, char *room, int choice){ 
    normalize_room_code(room);
    if(choice == 1){
        for (int i = 0; i < floor_count; i++) {
            for (int j = 0; j < room_counts[i]; j++) {
                if (strcmp(rooms[i][j], room) == 0) return 1;
            }
        }
        return 0;
    } else {
        char loai_phong;
        char ma_phong[10];
        for (int lau = 1; lau <= 3; lau++) {
            for (int phong = 1; phong <= 6; phong++) {
                if (phong == 1) loai_phong = 'A';
                else if (phong == 2 || phong == 3) loai_phong = 'B';
                else if (phong == 4 || phong == 5) loai_phong = 'C';
                else loai_phong = 'D';
                sprintf(ma_phong, "%c%d%02d", loai_phong, lau, phong);
                if (strcmp(ma_phong, room) == 0) return 1;
            }
        }
        return 0;
    }
}

int check_room_conflict(List *L, char *room, int date_in, int date_out, int choice){ 
    if (!check_room_exist(L, room, choice)) return 0; 
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.room_type, room) == 0 && !(date_in >= p->data.date_out || date_out <= p->data.date_in)){
            return 0; 
        }
        p = p->next;
    }
    return 1; 
}

int check_invoiceCode_exist(List *L, char *invoice){ 
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.invoice_code, invoice) == 0) return 0;
        p = p->next;
    }
    return 1;
}

int check_name(char *str) {
    while (*str != '\0') {
        if (*str == ' ') { str++; continue; }
        if (!isalpha((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

int check_identity_with_different_name(List *L, char *CMT, char *name){
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.customer_info.identity_card, CMT) == 0){
            if(strcmp(p->data.customer_info.name, name) != 0) return 1; 
        }
        p = p->next;
    }
    return 0; 
}

Node* check_identity_with_different_name_node(List *L, char *CMT, char *name){
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.customer_info.identity_card, CMT) == 0){
            if(strcmp(p->data.customer_info.name, name) != 0) return p; 
        }
        p = p->next;
    }
    return NULL; 
}

int input_data(List *L, date *d, int choice) {
    int booking_done = 0;

    while (!booking_done) {
        int max_days = get_max_days_current_month();
        char prompt_in[120], prompt_out[120];
        sprintf(prompt_in, INDENT"Nhap ngay vao (0 de huy, 1-%d): ", max_days);
        sprintf(prompt_out, "                                  Nhap ngay ra (0 de huy, %d-%d): ", 1, max_days);

        do {
            d->date_in = get_safe_int(prompt_in, 0, max_days);
            if (d->date_in == 0) {
                printf(INDENT YELLOW "Da huy thao tac nhap lieu!\n" RESET);
                return 0;
            }
            if (d->date_in < max_days) {
                sprintf(prompt_out, "                                  Nhap ngay ra (0 de huy, %d-%d): ", d->date_in + 1, max_days);
                d->date_out = get_safe_int(prompt_out, 0, max_days);
                if (d->date_out == 0) {
                    printf(INDENT YELLOW "Da huy thao tac nhap lieu!\n" RESET);
                    return 0;
                }
            } else {
                printf(INDENT BOLD_WHITE"Ngay vao la %d nen tu dong lay ngay ra la %d, tien phong lay gia 1 ngay"RESET, max_days, max_days);
                d->date_out = max_days;
                break;
            }
            if (get_days(d->date_in, d->date_out) <= 0) {
                printf(INDENT "Ngay ra phai sau ngay vao. Vui long nhap lai.\n");
            } else {
                break;
            }
        } while (1);

        while (1) {
            printf(INDENT "Nhap loai phong (0 de huy): ");
            read_line(d->room_type, sizeof(d->room_type));
            if (strcmp(d->room_type, "0") == 0) {
                printf(INDENT YELLOW "Da huy thao tac nhap lieu!\n" RESET);
                return 0;
            }

            int room_exists = check_room_exist(L, d->room_type, choice);
            if (!room_exists) {
                printf(INDENT RED "Phong khong ton tai. Vui long nhap lai.\n" RESET);
                continue;
            }

            int room_available = check_room_conflict(L, d->room_type, d->date_in, d->date_out, choice);
            if (!room_available) {
                printf(INDENT RED "Phong da co khach dat trong thoi gian nay.\n" RESET);
                printf(INDENT "1. Nhap lai ngay vao va ngay ra\n");
                printf(INDENT "2. Nhap lai loai phong\n");
                int sub_choice = get_safe_int(INDENT"Lua chon cua ban (1 hoac 2): ", 1, 2);
                if (sub_choice == 1) {
                    break;
                } else {
                    continue;
                }
            }

            booking_done = 1;
            break;
        }
    }

    do {
        generate_invoice_code(d->invoice_code, d->date_in);
    } while (!check_invoiceCode_exist(L, d->invoice_code));
    printf(GREEN INDENT "Da cap ma hoa don tu dong: %s\n" RESET, d->invoice_code);

    int flag_gender = 0;

    while (1) {
        printf(INDENT "Nhap ten khach hang (0 de huy): ");
        read_line(d->customer_info.name, sizeof(d->customer_info.name));
        if (strcmp(d->customer_info.name, "0") == 0) {
            printf(INDENT YELLOW "Da huy thao tac nhap lieu!\n" RESET);
            return 0;
        }
        if (!check_name(d->customer_info.name)) {
            printf(INDENT RED "Ten khach hang chi duoc chua chu cai va khoang trang. Vui long nhap lai.\n" RESET);
            continue;
        }
        if (strlen(d->customer_info.name) == 0) {
            printf(INDENT RED "Ten khach hang khong duoc de trong. Vui long nhap lai.\n" RESET);
            continue;
        }
        break;
    }

    while (1) {
        printf(INDENT "Nhap so CMT/CCCD (0 de huy): ");
        read_line(d->customer_info.identity_card, sizeof(d->customer_info.identity_card));
        if (strcmp(d->customer_info.identity_card, "0") == 0) {
            printf(INDENT YELLOW "Da huy thao tac nhap lieu!\n" RESET);
            return 0;
        }
        int identity_length = strlen(d->customer_info.identity_card);

        if (identity_length != 12) {
            printf(INDENT RED "CCCD phai gom dung 12 chu so. Vui long nhap lai\n" RESET);
            continue;
        }

        int all_digits = 1;
        for (int i = 0; i < identity_length; i++) {
            if (!isdigit((unsigned char)(d->customer_info.identity_card[i]))) {
                printf(INDENT RED "CCCD phai gom 12 chu so. Vui long nhap lai\n" RESET);
                all_digits = 0;
                break;
            }
        }

        if (!all_digits) {
            continue;
        }

        Node *check_node = check_identity_with_different_name_node(L, d->customer_info.identity_card, d->customer_info.name);
        if (check_identity_with_different_name(L, d->customer_info.identity_card, d->customer_info.name)) {
            printf(INDENT RED "Da co khach hang su dung CCCD nay voi ten: " BOLD_WHITE "%s\n" RESET, check_node->data.customer_info.name);
            printf(INDENT "1. Ghi ten khach hang la:  %s\n", check_node->data.customer_info.name);
            printf(INDENT "2. Nhap lai so CMT/CCCD\n");
            int chon = get_safe_int(INDENT"Lua chon cua ban (1 hoac 2): ", 1, 2);
            if (chon == 1) {
                strcpy(d->customer_info.name, check_node->data.customer_info.name);
                strcpy(d->customer_info.gender, check_node->data.customer_info.gender);
                flag_gender = 1;
                break;
            } else {
                continue;
            }
        } else {
            break;
        }
    }

    do {
        if (flag_gender) {
            break;
        }
        int choice_gender;
        printf(INDENT "Chon gioi tinh:\n");
        printf(INDENT "1. Nam\n");
        printf(INDENT "2. Nu\n");
        choice_gender = get_safe_int(INDENT"Nhap lua chon: ", 1, 2);
        if (choice_gender == 1) {
            strcpy(d->customer_info.gender, "Nam");
            break;
        } else if (choice_gender == 2) {
            strcpy(d->customer_info.gender, "Nu");
            break;
        }
    } while (1);

    char type = get_room(d->room_type);
    d->gia_phong = get_price(type);
    int days = get_days(d->date_in, d->date_out);
    d->tien_phong = d->gia_phong * days;
    return 1;
}

void searchCMT(List *L, char *CMT){
	if(check_empty(L)){ printf(INDENT YELLOW"DANH SACH RONG!\n" RESET); return; }
    int check = 0; Node *p = L->head;
    char tien_phong_str[20], tien_phong_temp[20];
    while(p != NULL){
    	if(strcmp(p->data.customer_info.identity_card, CMT) == 0){
    		char date_str[60]; 
            sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
            sprintf(tien_phong_str, "%lld", p->data.tien_phong);
            add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_temp);
            printf(BLUE INDENT "╔═════════════════╦════════════════════════════════════════════════════╗\n" RESET);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50.50s" BLUE " ║\n" RESET, "Ho va ten", p->data.customer_info.name);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Gioi tinh", p->data.customer_info.gender);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "So CMT", p->data.customer_info.identity_card);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ngay vao va ra", date_str);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Loai phong", p->data.room_type);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ma hoa don", p->data.invoice_code);
            printf(BLUE INDENT "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Tien phong", tien_phong_temp); 
            printf(BLUE INDENT "╚═════════════════╩════════════════════════════════════════════════════╝\n" RESET);
            printf("\n");
            check = 1;
		}
		p = p->next;
	}
	if (!check) printf(INDENT RED "Khong co khach hang co so CMT vua nhap!\n" RESET);
}

void searchRoom(List *L, char *room, int choice){
	if(check_empty(L)){ printf(INDENT YELLOW "Danh sach rong!\n" RESET); return; }
    if(!check_room_exist(L, room, choice)){ printf(INDENT RED "Phong khong ton tai!\n" RESET); return; }
    int check = 0; Node *p = L->head;
    char tien_phong_str[20], tien_phong_temp[20];
    printf(GREEN "\n                                           THONG TIN KHACH HANG DA DAT PHONG %s:\n" RESET, room);
    while(p != NULL){
    	if(strcmp(p->data.room_type, room) == 0){
            char date_str[60]; 
            sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
            sprintf(tien_phong_str, "%lld", p->data.tien_phong);
            add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_temp);
            printf(BLUE INDENT "╔═════════════════╦════════════════════════════════════════════════════╗\n" RESET);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50.50s" BLUE " ║\n" RESET, "Ho va ten", p->data.customer_info.name);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Gioi tinh", p->data.customer_info.gender);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "So CMT", p->data.customer_info.identity_card);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ngay vao va ra", date_str);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Loai phong", p->data.room_type);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ma hoa don", p->data.invoice_code);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Tien phong", tien_phong_temp);
            printf(BLUE INDENT"╚═════════════════╩════════════════════════════════════════════════════╝\n" RESET);
            printf("\n");
            check = 1;
		}
		p = p->next;
	}
	if (!check) printf(INDENT RED "Khong co khach hang o phong %s!\n" RESET, room);
}

void thongke(List *L) { 
    char tempA[20], tempB[20], tempKhac[20], tempTatCa[20];
    char tongA_str[20], tongB_str[20], tongKhac_str[20], tongTatCa_str[20];
    long long tongA = 0, tongB = 0, tongKhac = 0, tongTatCa = 0;
    Node *p = L->head;
    while(p != NULL) {
        char type = get_room(p->data.room_type);
        if(type == 'A') tongA += p->data.tien_phong;
        else if(type == 'B') tongB += p->data.tien_phong;
        else tongKhac += p->data.tien_phong;
        p = p->next;
    }
    tongTatCa = tongA+tongB+tongKhac;
    
    sprintf(tongA_str, "%lld", tongA); sprintf(tongB_str, "%lld", tongB);
    sprintf(tongKhac_str, "%lld", tongKhac); sprintf(tongTatCa_str, "%lld", tongTatCa);

    add_dot(tongA_str, strlen(tongA_str), tempA); add_dot(tongB_str, strlen(tongB_str), tempB);
    add_dot(tongKhac_str, strlen(tongKhac_str), tempKhac); add_dot(tongTatCa_str, strlen(tongTatCa_str), tempTatCa);

    printf("\n" YELLOW BOLD INDENT"              BẢNG THỐNG KÊ DOANH THU" RESET "\n");
    printf(BLUE INDENT"╔══════════════════════╦═════════════════════════╗\n");
    printf(INDENT"║" WHITE_B "     LOẠI PHÒNG       " BLUE "║" WHITE_B "     DOANH THU (VNĐ)     " BLUE "║\n");
    printf(INDENT"╠══════════════════════╬═════════════════════════╣\n");
    printf(INDENT"║ " WHITE_B "Phòng loại A        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempA);
    printf(INDENT"║ " WHITE_B "Phòng loại B        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempB);
    printf(INDENT"║ " WHITE_B "Các loại khác       " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempKhac);
    printf(INDENT"╠══════════════════════╬═════════════════════════╣\n");
    printf(INDENT"║ " GREEN   "TỔNG CỘNG           " BLUE " ║ " GREEN   "%12s" RESET GREEN   " VNĐ      " BLUE "  ║\n", tempTatCa);
    printf(BLUE INDENT"╚══════════════════════╩═════════════════════════╝\n" RESET);
    printf("\n");
}

void thongke_theo_ngay(List *L, int date) { 
    char tempA[20], tempB[20], tempKhac[20], tempTatCa[20];
    char tongA_str[20], tongB_str[20], tongKhac_str[20], tongTatCa_str[20];
    int tongA = 0, tongB = 0, tongKhac = 0, tongTatCa = 0;
    Node *p = L->head;
    while(p != NULL) {
        char type = get_room(p->data.room_type);
        if(type == 'A' && p->data.date_in <= date && p->data.date_out >= date) tongA += get_price(type);
        else if(type == 'B' && p->data.date_in <= date && p->data.date_out >= date) tongB += get_price(type);
        else if(type != 'A' && type != 'B' && p->data.date_in <= date && p->data.date_out >= date) tongKhac += get_price(type);
        p = p->next;
    }
    tongTatCa = tongA+tongB+tongKhac;

    sprintf(tongA_str, "%d", tongA); sprintf(tongB_str, "%d", tongB);
    sprintf(tongKhac_str, "%d", tongKhac); sprintf(tongTatCa_str, "%d", tongTatCa);

    add_dot(tongA_str, strlen(tongA_str), tempA); add_dot(tongB_str, strlen(tongB_str), tempB);
    add_dot(tongKhac_str, strlen(tongKhac_str), tempKhac); add_dot(tongTatCa_str, strlen(tongTatCa_str), tempTatCa);

    printf("\n" YELLOW BOLD INDENT"       BẢNG THỐNG KÊ DOANH THU NGÀY %d" RESET "\n", date);
    printf(BLUE INDENT"╔══════════════════════╦═════════════════════════╗\n");
    printf(INDENT"║" WHITE_B "     LOẠI PHÒNG       " BLUE "║" WHITE_B "     DOANH THU (VNĐ)     " BLUE "║\n");
    printf(INDENT"╠══════════════════════╬═════════════════════════╣\n");
    printf(INDENT"║ " WHITE_B "Phòng loại A        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempA);
    printf(INDENT"║ " WHITE_B "Phòng loại B        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempB);
    printf(INDENT"║ " WHITE_B "Các loại khác       " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempKhac);
    printf(INDENT"╠══════════════════════╬═════════════════════════╣\n");
    printf(INDENT"║ " GREEN   "TỔNG CỘNG           " BLUE " ║ " GREEN   "%12s" RESET GREEN   " VNĐ      " BLUE "  ║\n", tempTatCa);
    printf(BLUE INDENT"╚══════════════════════╩═════════════════════════╝\n" RESET);
    printf("\n");
}

char *lastName(char *name) { 
    int len = strlen(name);
    while (len > 0 && isspace((unsigned char)name[len - 1])) { name[len - 1] = '\0'; len--; }
    char *last = strrchr(name, ' ');
    if (last != NULL)  return last + 1; 
    return name; 
} 

void popMinh(List *L) {
    if (check_empty(L)) { printf(INDENT YELLOW "Danh sach rong!\n" RESET); return; }
    int found = 0;
    Node *current = L->head, *prev = NULL;
    while (current != NULL) {
        if (strcmp(lastName(current->data.customer_info.name), "Minh") == 0) {
            Node *del = current;
            if (del == L->head) { 
                L->head = L->head->next;
                current = L->head; 
                if (L->head == NULL) L->tail = NULL;
            } else { 
                prev->next = del->next;
                current = del->next; 
                if (del == L->tail) L->tail = prev;
            }
            free(del);
            found = 1;
        } else { prev = current; current = current->next; }
    }
    if (found) printf(INDENT GREEN "Da xoa tat ca khach hang co ten Minh.\n" RESET);
    else printf(INDENT RED "Khong co khach hang ten Minh.\n" RESET);
}

void delete_more_customer(List *L){ 
    if (check_empty(L)) { printf(INDENT YELLOW "Danh sach rong!\n" RESET); return; }
    int n=1;
    while(n){
        export_list_table(L);
        int a = list_length(L);
        n = get_safe_int(BOLD_WHITE INDENT"Nhap vi tri khach hang can xoa (0 de dung): "RESET, 0, a);
        if(n == 0) break;
        delete_k_place(L, n);
    }
}

// ====================== PHẦN MERGE SORT ====================== //
// Input: Con trỏ node head | Output: Chia List làm 2 nửa
static void split_list(Node *source, Node **frontRef, Node **backRef) {
    Node *slow = source;
    Node *fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) { slow = slow->next; fast = fast->next; }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

static Node *merge_date_in_desc(Node *a, Node *b) {
    Node dummy; // Sử dụng Dummy Node để tránh phải rẽ nhánh (if-else) cho phần tử đầu tiên
    Node *tail = &dummy;
    dummy.next = NULL;

    // Trộn 2 danh sách cho đến khi 1 trong 2 bị rỗng
    while (a != NULL && b != NULL) {
        if (a->data.date_in >= b->data.date_in) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next; // Tiến tail về phía trước
    }

    // Nối phần còn dư của danh sách a (nếu có)
    if (a != NULL) {
        tail->next = a;
    }
    // Nối phần còn dư của danh sách b (nếu có)
    if (b != NULL) {
        tail->next = b;
    }

    return dummy.next; // Bỏ qua node giả, trả về node thật đầu tiên
}

static Node *merge_price_desc(Node *a, Node *b) {
    Node dummy; 
    Node *tail = &dummy;
    dummy.next = NULL;

    while (a != NULL && b != NULL) {
        if (a->data.tien_phong >= b->data.tien_phong) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    if (a != NULL) tail->next = a;
    if (b != NULL) tail->next = b;

    return dummy.next;
}

static void merge_sort_by_date_in(Node **headRef) {
    Node *head = *headRef;
    if (head == NULL || head->next == NULL) return;
    Node *a; Node *b;
    split_list(head, &a, &b);
    merge_sort_by_date_in(&a);
    merge_sort_by_date_in(&b);
    *headRef = merge_date_in_desc(a, b);
}

static void merge_sort_by_price(Node **headRef) {
    Node *head = *headRef;
    if (head == NULL || head->next == NULL) return;
    Node *a; Node *b;
    split_list(head, &a, &b);
    merge_sort_by_price(&a);
    merge_sort_by_price(&b);
    *headRef = merge_price_desc(a, b);
}

void sort_by_date_in(List *L) {
    if (check_empty(L)) { printf(INDENT YELLOW"DANH SACH RONG!\n" RESET); return; }
    merge_sort_by_date_in(&L->head);
    L->tail = L->head;
    while (L->tail != NULL && L->tail->next != NULL) L->tail = L->tail->next;
}

void sort_by_price(List *L) {
    if (check_empty(L)) { printf(INDENT YELLOW "Danh sach rong!\n" RESET); return; }
    merge_sort_by_price(&L->head);
    L->tail = L->head;
    while (L->tail != NULL && L->tail->next != NULL) L->tail = L->tail->next;
}
void change_customer_info(List *L, int choice) {
    if (check_empty(L)) {
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
        return;
    }
    export_list_table(L);
    char invoice_code[20];
    while (1) {
        printf(INDENT BOLD_WHITE "Nhap ma hoa don cua khach hang can thay doi thong tin: " RESET);
        read_line(invoice_code, sizeof(invoice_code));
        int invoice_len = strlen(invoice_code);
        if (invoice_len != 10) {
            printf(INDENT RED "Ma hoa don phai gom 10 ky tu. Vui long nhap lai.\n" RESET);
            continue;
        }
        int valid = 1;
        for (int i = 0; i < invoice_len; i++) {
            if (!isdigit((unsigned char)invoice_code[i])) { valid = 0; break; }
        }
        if (!valid) { printf(INDENT RED "Ma hoa don phai gom 10 chu so. Vui long nhap lai.\n" RESET); continue; }
        break;
    }

    Node *p = L->head;
    int pos = 1;
    int found = 0;
    while (p != NULL) {
        if (strcmp(p->data.invoice_code, invoice_code) == 0) { found = 1; break; }
        p = p->next; pos++;
    }
    if (!found) {
        printf(INDENT RED "Khong tim thay khach hang co ma hoa don %s!\n" RESET, invoice_code);
        return;
    }

    printf(INDENT BOLD_WHITE "Thong tin khach hang truoc khi thay doi:\n" RESET);
    char tien_phong_str[20], tien_phong_temp[20];
    char date_str[60]; 
    sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
    sprintf(tien_phong_str, "%lld", p->data.tien_phong);
    add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_temp);
            printf(BLUE INDENT "╔═════════════════╦════════════════════════════════════════════════════╗\n" RESET);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50.50s" BLUE " ║\n" RESET, "Ho va ten", p->data.customer_info.name);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Gioi tinh", p->data.customer_info.gender);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "So CMT", p->data.customer_info.identity_card);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ngay vao va ra", date_str);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Loai phong", p->data.room_type);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ma hoa don", p->data.invoice_code);
            printf(BLUE INDENT"║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Tien phong", tien_phong_temp);
            printf(BLUE INDENT"╚═════════════════╩════════════════════════════════════════════════════╝\n" RESET);
            printf("\n");

    date old_data = p->data;
    delete_k_place(L, pos);

    date newd;
    printf(INDENT BOLD_WHITE "Nhap thong tin khach hang moi:\n" RESET);
    if (input_data(L, &newd, choice) == 1) {
        add_k_place(L, newd, pos);
    } else {
        add_k_place(L, old_data, pos);
    }
}