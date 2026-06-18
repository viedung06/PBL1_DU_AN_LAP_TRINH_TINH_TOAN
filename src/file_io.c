#include <stdio.h>
#include <string.h>
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

        if (fgets(d.invoice_code, sizeof(d.invoice_code), f) == NULL) break;
        d.invoice_code[strcspn(d.invoice_code, "\r\n")] = 0;

        if (fgets(d.customer_info.name, sizeof(d.customer_info.name), f) == NULL) break;
        d.customer_info.name[strcspn(d.customer_info.name, "\r\n")] = 0;

        if (fgets(d.customer_info.identity_card, sizeof(d.customer_info.identity_card), f) == NULL) break;
        d.customer_info.identity_card[strcspn(d.customer_info.identity_card, "\r\n")] = 0;

        if (fgets(d.customer_info.gender, sizeof(d.customer_info.gender), f) == NULL) break;
        d.customer_info.gender[strcspn(d.customer_info.gender, "\r\n")] = 0;

        if (!check_room_exist(L, d.room_type, choice)) {
            printf(INDENT RED "CANH BAO:"RESET BOLD_WHITE" Phong %s khong ton tai, bo qua hoa don nay!\n" RESET, d.room_type);
            continue;
        }
        if (!check_invoiceCode_exist(L, d.invoice_code)) {
            printf(INDENT RED "CANH BAO:"RESET BOLD_WHITE" Ma hoa don %s da ton tai. bo qua hoa don nay!\n" RESET, d.invoice_code);
            continue;
      }
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
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET); return;
    } else {
        FILE *f = fopen(filename, "w");
        if(f == NULL) { 
            printf(INDENT RED "Khong mo duoc file!\n" RESET);
            return;
            }
        Node *p = L->head;
        char tien_phong_str[20], tien_phong_temp[20];
        while (p != NULL) {
            fprintf(f, "HOA DON CUA KHACH HANG %s\n", p->data.customer_info.name);
            fprintf(f, "Ngay vao: %d\nNgay ra: %d\nLoai phong: %s\nMa hoa don: %s\nTen khach hang: %s\nSo CMT: %s\nGioi tinh: %s\n", 
                    p->data.date_in, p->data.date_out, p->data.room_type, p->data.invoice_code, 
                    p->data.customer_info.name, p->data.customer_info.identity_card, p->data.customer_info.gender);
            sprintf(tien_phong_str, "%lld", p->data.tien_phong);
            add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_temp);
            fprintf(f, "Tien phong phai tra: %s VNĐ\n-------------------------\n", tien_phong_temp);
            p = p->next;
        }
        fclose(f);
    }
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