#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "hotel_service.h"

void DrawTable() { 
    printf(BLUE "                                ╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("                                ║                        " GREEN BOLD "PBL1 - ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN" RESET BLUE "                      ║\n");
    printf("                                ╠══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("                                ║ " CYAN "Đề tài:" RESET " QUẢN LÝ THÔNG TIN KHÁCH SẠN TRONG 1 THÁNG                            " BLUE "║\n");
    printf("                                ╠══════════════════════════════════════╦═══════════════════════════════════════╣\n");
    printf("                                ║                " YELLOW "MÔ TẢ" RESET BLUE "                 ║         " YELLOW "SINH VIÊN THỰC HIỆN" RESET BLUE "           ║\n");
    printf("                                ╠══════════════════════════════════════╬═══════════════════════════════════════╣\n");
    printf("                                ║     Dùng Danh sách liên kết đơn để:  ║                 NHÓM 8                " BLUE "║\n");
    printf("                                ║ - Quản lý danh sách khách hàng       ║ 1. Nguyễn Việt Dũng                   " BLUE "║\n");
    printf("                                ║ - Quản lý đặt phòng/trả phòng        ║     Lớp: 25T_DT4     MSSV: 102250165  " BLUE "║\n");
    printf("                                ║ - Tính toán doanh thu hàng tháng     ║                                       " BLUE "║\n");
    printf("                                ║ - Đọc và ghi dữ liệu từ file.txt     ║ 2. Ngô Đăng Gia Bảo                   " BLUE "║\n");
    printf("                                ║ - Tìm kiếm & sắp xếp thông tin       ║     Lớp: 25T_DT4     MSSV: 102250158  " BLUE "║\n");
    printf("                                ╠══════════════════════════════════════╩═══════════════════════════════════════╣\n");
    printf("                                ║ " RED "GIẢNG VIÊN HƯỚNG DẪN: ĐỖ THỊ TUYẾT HOA" RESET BLUE "                                       ║\n");
    printf("                                ╚══════════════════════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n" BOLD_WHITE"                                                        Nhấn phím bất kỳ để tiếp tục..." RESET "\n");
    getchar();
}

void menu_chinh() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║            " GREEN "QUAN LY KHACH SAN" RESET BLUE "                   ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "Chuc nang:" RESET BLUE "                                    ║\n");
    printf("                                  ║  " BOLD_WHITE "1. Xoa va them hoa don" RESET BLUE "                        ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Tim kiem va tra cuu khach hang" RESET BLUE "             ║\n");
    printf("                                  ║  " BOLD_WHITE "3. Sap xep hoa don" RESET BLUE "                            ║\n");
    printf("                                  ║  " BOLD_WHITE "4. Thong ke va in du lieu" RESET BLUE "                     ║\n");
    printf("                                  ║  " BOLD_WHITE "5. Chuc nang khac    " RESET BLUE "                         ║\n");
    printf("                                  ║  " BOLD_WHITE "6. Sua thong tin hoa don" RESET BLUE "                      ║\n");
    printf("                                  ║  " BOLD_WHITE "0. Thoat" RESET BLUE "                                      ║\n");

    printf("                                  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE"                                  Nhap lua chon: " RESET);
}

void menu_them_xoa() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║            " GREEN "XOA VA THEM HOA DON" RESET BLUE "                 ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "1. Them hoa don vao cuoi danh sach" RESET BLUE "            ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Them nhieu hoa don vao danh sach" RESET BLUE "           ║\n");
    printf("                                  ║  " BOLD_WHITE "3. Xoa hoa don cuoi danh sach" RESET BLUE "                 ║\n");
    printf("                                  ║  " BOLD_WHITE "4. Xoa nhieu hoa don trong danh sach" RESET BLUE "          ║\n");
    printf("                                  ║  " BOLD_WHITE "5. Xoa khach hang ten Minh" RESET BLUE "                    ║\n");
    printf("                                  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("                                  ╚════════════════════════════════════════════════╝\n");
    printf("                                  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_timkiem() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║            " GREEN "TIM KIEM VA TRA CUU" RESET BLUE "                 ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "1. Tim kiem theo so CMT" RESET BLUE "                       ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Tim kiem theo phong" RESET BLUE "                        ║\n");
    printf("                                  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("                                  ╚════════════════════════════════════════════════╝\n");
    printf("                                  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_sapxep() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║               " GREEN "SAP XEP HOA DON" RESET BLUE "                  ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "1. Sap xep theo ngay vao" RESET BLUE "                      ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Sap xep theo tien phong" RESET BLUE "                    ║\n");
    printf("                                  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("                                  ╚════════════════════════════════════════════════╝\n");
    printf("                                  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_thongke() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║            " GREEN "THONG KE VA IN DU LIEU" RESET BLUE "              ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "1. In thong tin hoa don ra file" RESET BLUE "               ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Thong ke doanh thu 1 thang" RESET BLUE "                 ║\n");
    printf("                                  ║  " BOLD_WHITE "3. Thong ke doanh thu theo ngay" RESET BLUE "               ║\n");
    printf("                                  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("                                  ╚════════════════════════════════════════════════╝\n");
    printf("                                  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_rieng() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║              " GREEN "CHUC NANG DE RIENG" RESET BLUE "                ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "1. Khoi tao lai" RESET BLUE "                               ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Doc du lieu tu file" RESET BLUE "                        ║\n");
    printf("                                  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("                                  ╚════════════════════════════════════════════════╝\n");
    printf("                                  " BOLD_WHITE "Nhap lua chon: " RESET);
}
void drawCustomDiagramMenu(){
    printf(BLUE"                                ╔══════════════════════════════════════════════════════════╗\n");
    printf("                                ║  "GREEN"BAN CO MUON TAO SO DO KHACH SAN CUA RIENG BAN KHONG ?"RESET BLUE"   ║\n");
    printf("                                ╠══════════════════════════════════════════════════════════╣\n");
    printf("                                ║  " BOLD_WHITE "1. Co" RESET BLUE "                                                   ║\n");
    printf("                                ║  " BOLD_WHITE "2. Khong" RESET BLUE "                                                ║\n");
    printf("                                ╚══════════════════════════════════════════════════════════╝\n");
    printf("                                " BOLD_WHITE "Nhap lua chon (1: Co, 2: Khong): " RESET);
}

void export_list_table(List *L) { 
    if (check_empty(L)) {
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET); return;
    }
    printf(BLUE INDENT"╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf(INDENT"║%*s" GREEN BOLD "DANH SÁCH HÓA ĐƠN CỦA KHÁCH HÀNG      " RESET BLUE "%*s║\n", 37, "", 37, "");
    printf(                INDENT"╠══════╦═══════════════════════════╦══════════════╦════════════╦═════════════════╦══════════════╦════════════════╣\n");
    printf(INDENT"║ " BOLD_WHITE "%-4s" BLUE " ║ " BOLD_WHITE "%-25s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║ " BOLD_WHITE "%-14s" BLUE " ║\n" RESET, "STT", "Ho va Ten", "Ma HD", "Phong o", "Ngay vao va ra", "So CCCD", "Tien Phong");
    printf(BLUE INDENT"╠══════╬═══════════════════════════╬══════════════╬════════════╬═════════════════╬══════════════╬════════════════╣\n" RESET);
    Node *p = L->head; int stt = 1;
    char tien_str[20], tien_tmp[20];
    while (p != NULL) {
        char date_range[20];
        sprintf(date_range, "%d -> %d", p->data.date_in, p->data.date_out);
        sprintf(tien_str, "%lld", p->data.tien_phong);
        add_dot(tien_str, strlen(tien_str), tien_tmp);
        printf(BLUE INDENT "║ " RESET BOLD_WHITE"%-4d"RESET BLUE " ║ " RESET BOLD_WHITE"%-25.25s"RESET BLUE " ║ " RESET BOLD_WHITE"%-12s"RESET BLUE " ║ " RESET BOLD_WHITE"%-10s"RESET BLUE " ║ " RESET BOLD_WHITE"%-15s"BLUE " ║ " RESET BOLD_WHITE"%-12s"BLUE " ║ " RESET BOLD_WHITE"%-14s"BLUE " ║\n", stt, p->data.customer_info.name, p->data.invoice_code, p->data.room_type, date_range, p->data.customer_info.identity_card, tien_tmp);
        p = p->next; stt++;
    }
    printf(BLUE INDENT "╚══════╩═══════════════════════════╩══════════════╩════════════╩═════════════════╩══════════════╩════════════════╝\n" RESET);
}

void export_custom_table(List *L) { 
    printf(BLUE "\n╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║%*s" GREEN BOLD "SƠ ĐỒ ĐẶT PHÒNG CHI TIẾT" RESET BLUE "%*s║\n", 47, "", 48, "");
    printf(       "╠══════════╦══════════╦═══════════════╦══════════╦════════════╦════════════╦═════════════════════════╦══════════════════╣\n");
    printf("║ " BOLD_WHITE "%-8s" BLUE " ║ " BOLD_WHITE "%-8s" BLUE " ║ " BOLD_WHITE "%-13s" BLUE " ║ " BOLD_WHITE "%-8s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-23s" BLUE " ║ " BOLD_WHITE "%-16s" BLUE " ║\n" RESET, "Tang", "Ma phong", "Gia (VND)", "T.Thai", "Ngay Vao", "Ngay Ra", "Khach hang", "Tien Phong");
    printf(BLUE"╠══════════╬══════════╬═══════════════╬══════════╬════════════╬════════════╬═════════════════════════╬══════════════════╣\n" RESET);

    for (int i = 0; i < floor_count; i++) {
        for (int j = 0; j < room_counts[i]; j++) {
            char type = get_room(rooms[i][j]);
            long price = get_price(type);
            char price_str[20];
            sprintf(price_str, "%ld.000", price / 1000);
            int found_at_least_one = 0; Node *p = L->head;
            char tien_str[20], tien_tmp[20];

            while (p != NULL) {
                if (strcmp(p->data.room_type, rooms[i][j]) == 0) {
                    printf(BLUE "║ " RESET);
                    if (found_at_least_one == 0 && j == 0) {
                        char fl_txt[10]; sprintf(fl_txt, "Tang %d", i + 1);
                        printf("%-8s" BLUE " ║ " RESET, fl_txt);
                    } else printf("%-8s" BLUE " ║ " RESET, "");

                    if (found_at_least_one == 0) {
                        printf("%-8s" BLUE " ║ " RESET, rooms[i][j]);
                        printf("%-13s" BLUE " ║ " RESET, price_str);
                    } else {
                        printf("%-8s" BLUE " ║ " RESET, "");
                        printf("%-13s" BLUE " ║ " RESET, "");
                    }
                    printf(YELLOW "%-8s" BLUE " ║ " RESET, "Da dat");
                    printf("%-10d" BLUE " ║ " RESET, p->data.date_in);
                    printf("%-10d" BLUE " ║ " RESET, p->data.date_out);
                    
                    char name_tmp[24];
                    strncpy(name_tmp, p->data.customer_info.name, 23);
                    name_tmp[23] = '\0';
                    printf("%-23s" BLUE " ║ " RESET, name_tmp);
                    sprintf(tien_str, "%lld", p->data.tien_phong);
                    add_dot(tien_str, strlen(tien_str), tien_tmp);
                    printf("%-16s" BLUE " ║\n" RESET, tien_tmp);
                    found_at_least_one = 1;
                }
                p = p->next;
            }
            if (!found_at_least_one) {
                printf(BLUE "║ " RESET);
                if (j == 0) {
                    char fl_txt[10]; sprintf(fl_txt, "Tang %d", i + 1);
                    printf("%-8s" BLUE " ║ " RESET, fl_txt);
                } else printf("%-8s" BLUE " ║ " RESET, "");
                printf("%-8s" BLUE " ║ " RESET, rooms[i][j]);
                printf("%-13s" BLUE " ║ " RESET, price_str);
                printf(CYAN "%-8s" BLUE " ║ " RESET, "Trong");
                printf("%-10d" BLUE " ║ " RESET, 0);
                printf("%-10d" BLUE " ║ " RESET, 0);
                printf("%-23s" BLUE " ║ " RESET, "-");
                printf("%-16s" BLUE " ║\n" RESET, "-");
            }
        }
        if (i < floor_count - 1) printf(BLUE "╠══════════╬══════════╬═══════════════╬══════════╬════════════╬════════════╬═════════════════════════╬══════════════════╣\n" RESET);
    }
    printf(BLUE "╚══════════╩══════════╩═══════════════╩══════════╩════════════╩════════════╩═════════════════════════╩══════════════════╝\n" RESET);
}

void export_table(List *L) {
    printf(BLUE "╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n" RESET);
    printf(BLUE "║%*s" GREEN BOLD "DANH SÁCH CHI TIẾT ĐẶT PHÒNG KHÁCH SẠN" RESET BLUE "%*s║\n" RESET, 47, "", 48, "");
    printf(BLUE "╠════════════╦════════════╦════════════╦════════════════╦══════════════╦══════════════╦══════════════════════════════╦════════════════╣\n" RESET);
    printf(BLUE "║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-14s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║ " BOLD_WHITE "%-28s" BLUE " ║ " BOLD_WHITE "%-14s" BLUE " ║\n" RESET, "Tang/Lau", "Ma Phong", "Loai", "Gia (VND)", "Ngay Vao", "Ngay Ra", "Ten Khach Hang", "Tien Phong");

    char loai_phong, gia_tien[15], ma_phong[10];
    char tien_phong_str[20], tien_phong_tmp[20];

    for (int lau = 1; lau <= 3; lau++) {
        printf(BLUE "╠════════════╬════════════╬════════════╬════════════════╬══════════════╬══════════════╬══════════════════════════════╬════════════════╣\n" RESET);
        for (int phong = 1; phong <= 6; phong++) {
            if (phong == 1) { loai_phong = 'A'; strcpy(gia_tien, "400,000"); }
            else if (phong == 2 || phong == 3) { loai_phong = 'B'; strcpy(gia_tien, "300,000"); }
            else if (phong == 4 || phong == 5) { loai_phong = 'C'; strcpy(gia_tien, "200,000"); }
            else { loai_phong = 'D'; strcpy(gia_tien, "200,000"); }
            
            sprintf(ma_phong, "%c%d%02d", loai_phong, lau, phong);
            int found_customer = 0; Node *p = L->head;

            while (p != NULL) {
                if (strcmp(p->data.room_type, ma_phong) == 0) {
                    printf(BLUE "║ " RESET);
                    if (found_customer == 0 && phong == 1) {
                        char txt_lau[15]; sprintf(txt_lau, "Lau %d", lau);
                        printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_lau);
                    } else printf("%-10s" BLUE " ║ " RESET, "");

                    if (found_customer == 0) {
                        char txt_loai[15]; sprintf(txt_loai, "Loai %c", loai_phong);
                        printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, ma_phong);
                        printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_loai);
                        printf(BOLD_WHITE "%-14s" BLUE " ║ " RESET, gia_tien);
                    } else {
                        printf("%-10s" BLUE " ║ " RESET, "");
                        printf("%-10s" BLUE " ║ " RESET, "");
                        printf("%-14s" BLUE " ║ " RESET, "");
                    }
                    
                    printf(BOLD_WHITE "%-12d" BLUE " ║ " RESET, p->data.date_in); 
                    printf(BOLD_WHITE "%-12d" BLUE " ║ " RESET, p->data.date_out);
                    sprintf(tien_phong_str, "%lld", p->data.tien_phong);
                    add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_tmp);
                    char ten_tam[31];
                    strncpy(ten_tam, p->data.customer_info.name, 28);
                    ten_tam[28] = '\0';
                    printf(BOLD_WHITE "%-28s" BLUE " ║ " RESET, ten_tam);
                    printf(BOLD_WHITE "%-14s" BLUE " ║\n" RESET, tien_phong_tmp);
                    found_customer++;
                }
                p = p->next;
            }
            if (found_customer == 0) {
                printf(BLUE "║ " RESET);
                if (phong == 1) {
                    char txt_lau[15]; sprintf(txt_lau, "Lau %d", lau);
                    printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_lau);
                } else printf("%-10s" BLUE " ║ " RESET, "");
                
                char txt_loai[15]; sprintf(txt_loai, "Loai %c", loai_phong);
                printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, ma_phong);
                printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_loai);
                printf(BOLD_WHITE "%-14s" BLUE " ║ " RESET, gia_tien);
                printf("%-12d" BLUE " ║ " RESET, 0);
                printf("%-12d" BLUE " ║ " RESET, 0);
                printf(CYAN "%-28s" BLUE " ║ " RESET, "- Trong -");
                printf(CYAN "%-14s" BLUE " ║\n" RESET, "-");
            }
        }
    }
    printf(BLUE "╚════════════╩════════════╩════════════╩════════════════╩══════════════╩══════════════╩══════════════════════════════╩════════════════╝\n" RESET);
}