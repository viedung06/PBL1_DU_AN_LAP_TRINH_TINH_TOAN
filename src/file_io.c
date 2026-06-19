#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "file_io.h"
#include "hotel_service.h"
#include "ui.h"

int read_hotel_layout(const char *filename, int *floor_count, int room_counts[], char rooms[][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN]) {
    FILE *f = fopen(filename, "r");
    if (f == NULL){
        printf(INDENT RED "Khong mo duoc file!\n" RESET);
        return 0;
    }
    if (fscanf(f, "%d", floor_count) != 1) { fclose(f); return 0; }
    if (*floor_count <= 0 || *floor_count > MAX_FLOORS) { fclose(f); return 0; }
    for (int i = 0; i < *floor_count; i++) {
        if (fscanf(f, "%d", &room_counts[i]) != 1) { fclose(f); return 0; }
        if (room_counts[i] <= 0 || room_counts[i] > MAX_ROOMS_PER_FLOOR) { fclose(f); return 0; }
        for (int j = 0; j < room_counts[i]; j++) {
            if (fscanf(f, "%29s", rooms[i][j]) != 1) { fclose(f); return 0; }
        }
    }
    fclose(f);
    return 1;
}

void read_file(List *L, int choice, const char *filename) { 
    FILE *f = fopen(filename, "r");
    if (f == NULL) { printf(INDENT RED"Khong mo duoc file!\n"RESET); return; }
    date d;
    while (fscanf(f, "%d", &d.date_in) == 1) {
        if (fscanf(f, "%d", &d.date_out) != 1) break;
        char temp_str[100];
        if (fgets(temp_str, sizeof(temp_str), f) == NULL) break;
        
        if (fgets(d.room_type, sizeof(d.room_type), f) == NULL) break;
        d.room_type[strcspn(d.room_type, "\r\n")] = 0;

        if (fgets(d.customer_info.name, sizeof(d.customer_info.name), f) == NULL) break;
        d.customer_info.name[strcspn(d.customer_info.name, "\r\n")] = 0;

        if (fgets(d.customer_info.identity_card, sizeof(d.customer_info.identity_card), f) == NULL) break;
        d.customer_info.identity_card[strcspn(d.customer_info.identity_card, "\r\n")] = 0;

        if (fgets(d.customer_info.gender, sizeof(d.customer_info.gender), f) == NULL) break;
        d.customer_info.gender[strcspn(d.customer_info.gender, "\r\n")] = 0;
        
        int max_days = get_max_days_current_month();
        if (d.date_in < 1 || d.date_in > max_days || d.date_out < 1 || d.date_out > max_days || d.date_in >= d.date_out) {
            printf(INDENT RED "CANH BAO: Ngay vao/ra khong hop le, bo qua hoa don cua khach %s!\n" RESET, d.customer_info.name);
            continue;
        }

        if (strlen(d.customer_info.name) == 0) {
            printf(INDENT RED "CANH BAO: Ten khach hang trong, bo qua hoa don co CCCD %s!\n" RESET, d.customer_info.identity_card);
            continue;
        }
        if(!check_name(d.customer_info.name)) {
                printf(INDENT RED "CANH BAO: Ten khach hang chi duoc chua chu cai va khoang trang, bo qua hoa don co CCCD %s!\n" RESET, d.customer_info.identity_card);
                continue;
        }

        int identity_length = strlen(d.customer_info.identity_card);
        int valid_cccd = 1;
        if (identity_length != 12) valid_cccd = 0;
        for (int i = 0; i < identity_length; i++) {
            if (!isdigit((unsigned char)(d.customer_info.identity_card[i]))) valid_cccd = 0;
        }
        if (!valid_cccd) {
            printf(INDENT RED "CANH BAO: CCCD cua khach %s sai dinh dang, bo qua!\n" RESET, d.customer_info.name);
            continue;
        }

        if (!check_room_exist(L, d.room_type, choice)) {
            printf(INDENT RED "CANH BAO:"RESET BOLD_WHITE" Phong %s khong ton tai, bo qua hoa don nay!\n" RESET, d.room_type);
            continue;
        }

        if (!check_room_conflict(L, d.room_type, d.date_in, d.date_out, choice)) {
            printf(INDENT RED "CANH BAO:"RESET BOLD_WHITE" Phong %s da co khach dat trong khoang thoi gian nay, bo qua hoa don nay!\n" RESET, d.room_type);
            continue;
        }

        do {
            generate_invoice_code(d.invoice_code, d.date_in);
        } while (!check_invoiceCode_exist(L, d.invoice_code));

        char type = get_room(d.room_type);
        d.gia_phong = get_price(type);
        d.tien_phong = d.gia_phong * get_days(d.date_in, d.date_out);
        add_tail(L, d);
    }
    fclose(f);
    printf(INDENT GREEN "Da doc file %s thanh cong!\n" RESET, filename);
}

void print_file(List *L, const char *filename) { 
    if (check_empty(L)) {
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET); 
        return;
    }

    printf("\n");
    export_list_table(L);

    char invoice_search[15];
    Node *p = NULL;
    int found = 0;

    while (1) {
        printf(INDENT BOLD_WHITE "Nhap ma hoa don can in (hoac nhap '0' de thoat): " RESET);
        read_line(invoice_search, sizeof(invoice_search));

        if (strcmp(invoice_search, "0") == 0) {
            printf(INDENT YELLOW "Da huy thao tac in hoa don.\n" RESET);
            return; 
        }

        p = L->head;
        found = 0;
        while (p != NULL) {
            if (strcmp(p->data.invoice_code, invoice_search) == 0) {
                found = 1;
                break; 
            }
            p = p->next;
        }

        if (found) {
            break; 
        } else {
            printf(INDENT RED "Loi: Khong tim thay hoa don mang ma '%s'. Vui long thu lai!\n" RESET, invoice_search);
        }
    }

    FILE *f = fopen(filename, "w");
    if(f == NULL) { 
        printf(INDENT RED "Khong mo duoc file!\n" RESET);
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char tien_phong_str[20], tien_phong_temp[20];
    sprintf(tien_phong_str, "%lld", p->data.tien_phong);
    add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_temp);
    
    int days = get_days(p->data.date_in, p->data.date_out);

    fprintf(f, "==============================================================\n");
    fprintf(f, "                            HOTEL                             \n");
    fprintf(f, "           He thong quan ly khach san chuyen nghiep           \n");
    fprintf(f, "==============================================================\n");
    fprintf(f, "                     HOA DON THANH TOAN                       \n");
    fprintf(f, "--------------------------------------------------------------\n");
    fprintf(f, "Ma hoa don : %s\n", p->data.invoice_code);
    fprintf(f, "Ngay in    : %02d/%02d/%d - %02d:%02d:%02d\n", 
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, 
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(f, "--------------------------------------------------------------\n");
    fprintf(f, "Khach hang : %s\n", p->data.customer_info.name);
    fprintf(f, "CMT/CCCD   : %s\n", p->data.customer_info.identity_card);
    fprintf(f, "Gioi tinh  : %s\n", p->data.customer_info.gender);
    fprintf(f, "--------------------------------------------------------------\n");
    fprintf(f, "Phong      : %-10s | Loai phong : %c\n", p->data.room_type, p->data.room_type[0]);
    fprintf(f, "Ngay vao   : %-10d | Ngay ra    : %d\n", p->data.date_in, p->data.date_out);
    fprintf(f, "So ngay o  : %-10d | Don gia    : %lld VND/ngay\n", days, p->data.gia_phong);
    fprintf(f, "--------------------------------------------------------------\n");
    fprintf(f, "TONG TIEN PHAI TRA: %s VND\n", tien_phong_temp);
    fprintf(f, "==============================================================\n");
    fprintf(f, "       CAM ON QUY KHACH DA SU DUNG DICH VU CUA CHUNG TOI!     \n");
    fprintf(f, "                         HEN GAP LAI!                         \n");
    fprintf(f, "==============================================================\n");

    fclose(f);
    printf(INDENT GREEN "Da xuat thanh cong hoa don [%s] ra file: %s\n" RESET, invoice_search, filename);
}

void print_sort_by_date(List *L, const char *filename) { 
    FILE *f = fopen(filename, "w");
    if (f == NULL) { printf(INDENT RED "Khong mo duoc file!\n" RESET); return; }
    char tien_phong_str[20], tien_phong_str_tmp[20];
    fprintf(f, "==============================================================================================================\n");
    fprintf(f, "                  DANH SACH SAP XEP THEO NGAY VAO\n");
    fprintf(f, "==============================================================================================================\n");
    fprintf(f, "%-5s %-25s %-10s %-15s %-15s %-15s %-15s\n", "STT", "TEN KHACH", "PHONG", "NGAY VAO", "NGAY RA", "SO NGAY O", "TIEN PHONG");
    fprintf(f, "--------------------------------------------------------------------------------------------------------------\n");
    Node *p = L->head; int stt = 1;
    while (p != NULL) {
        sprintf(tien_phong_str, "%lld", p->data.tien_phong);
        add_dot(tien_phong_str,strlen(tien_phong_str),tien_phong_str_tmp);
        fprintf(f, "%-5d %-25s %-10s %-15d %-15d %-15d %s\n", stt, p->data.customer_info.name, p->data.room_type, p->data.date_in, p->data.date_out, get_days(p->data.date_in, p->data.date_out), tien_phong_str_tmp);
        p = p->next; stt++;
    }
    fprintf(f, "==============================================================================================================\n");
    fclose(f);
    printf(INDENT GREEN "Da xuat file %s\n" RESET, filename);
}

void print_sort_by_price(List *L, const char *filename) { 
    FILE *f = fopen(filename, "w");
    if (f == NULL) { printf(INDENT RED "Khong mo duoc file!\n" RESET); return; }
    char tien_phong_str[20], tien_phong_str_tmp[20];
    fprintf(f, "==============================================================================================================\n");
    fprintf(f, "           DANH SACH SAP XEP THEO TIEN PHONG\n");
    fprintf(f, "==============================================================================================================\n");
    fprintf(f, "%-5s %-25s %-10s %-15s %-15s %-15s %-15s\n", "STT", "TEN KHACH", "PHONG", "NGAY VAO", "NGAY RA", "SO NGAY O", "TIEN PHONG");
    fprintf(f, "--------------------------------------------------------------------------------------------------------------\n");
    Node *p = L->head; int stt = 1;
    while (p != NULL) {
        sprintf(tien_phong_str, "%lld", p->data.tien_phong);
        add_dot(tien_phong_str,strlen(tien_phong_str),tien_phong_str_tmp);
        fprintf(f, "%-5d %-25s %-10s %-15d %-15d %-15d %s\n", stt, p->data.customer_info.name, p->data.room_type, p->data.date_in, p->data.date_out, get_days(p->data.date_in, p->data.date_out), tien_phong_str_tmp);
        p = p->next; stt++;
    }
    fprintf(f, "==============================================================================================================\n");
    fclose(f);
    printf(INDENT GREEN "Da xuat file %s\n" RESET, filename);
}