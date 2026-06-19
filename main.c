#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "include/hotel_types.h"
#include "include/linked_list.h"
#include "include/hotel_service.h"
#include "include/file_io.h"
#include "include/ui.h"

// Định nghĩa biến toàn cục
int floor_count;
int room_counts[MAX_FLOORS];
char rooms[MAX_FLOORS][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN];

int main() {
    SetConsoleOutputCP(65001);
    srand(time(NULL));
    DrawTable();
    List L;
    init(&L); // LUÔN KHỞI TẠO NGAY ĐẦU TIÊN
    date data = {0};
    int chinh, phu, choice;
    char filename[100];
    drawCustomDiagramMenu();
    choice = get_safe_int(" ", 1, 2);
    if(choice == 1) {
        if (!read_hotel_layout(FILE_LAYOUT, &floor_count, room_counts, rooms)) {
            printf("Loi: Khong doc duoc file layout hotel.\n");
            return 1;
        }
    }
    read_file(&L, choice, "files/input.txt");

    do {
        if (choice == 1) {
            export_custom_table(&L);
            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
            getchar();
        }
        if(choice == 2){
            export_table(&L);
            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
            getchar();
        }
        menu_chinh();
        chinh = get_safe_int(" ", 0, 6);

        switch(chinh) {
            case 1:
                do {
                    menu_them_xoa();
                    phu = get_safe_int(" ", 0, 5);
                    switch(phu) {
                        case 1:
                            if (input_data(&L, &data, choice) == 1) {
                                add_tail(&L, data);
                            }
                            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 2:
                            while(1){
                                export_list_table(&L);
                                int b = list_length(&L);
                                printf( BOLD_WHITE INDENT"Nhap vi tri can them (0 de quay lai, %d de them vao cuoi danh sach): " RESET, b+1);
                                int k = get_safe_int(BOLD_WHITE "" RESET, 0, b+1);
                                if(k == 0) break;
                                if (input_data(&L, &data, choice) == 1) {
                                    add_k_place(&L, data, k);
                                }
                                printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                                getchar();
                            }
                            break;
                        case 3:
                            delete_tail(&L);
                            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 4:
                            delete_more_customer(&L);
                            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 5:
                            popMinh(&L);
                            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 0: 
                            break;
                        default: printf(INDENT RED "Lua chon khong hop le, vui long chon lai!\n" RESET);
                    }
                } while(phu != 0);
                break;

            case 2:
                do {
                    menu_timkiem();
                    phu = get_safe_int(" ", 0, 2);
                    switch(phu) {
                        case 1: {
                            char CMT[15];
                            printf(INDENT "Nhap so CMT: ");
                            read_line(CMT, sizeof(CMT));
                            searchCMT(&L, CMT);
                            printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        }
                        case 2: {
                            char room[20];
                            printf(INDENT "Nhap ma phong: ");
                            read_line(room, sizeof(room));
                            searchRoom(&L, room, choice);
                            printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        }
                        case 0: break;
                        default: printf(INDENT RED "Lua chon khong hop le, vui long chon lai!\n" RESET);
                    }
                } while(phu != 0);
                break;

            case 3:
                do {
                    menu_sapxep();
                    phu = get_safe_int(" ", 0, 2);
                    switch(phu) {
                        case 1:
                            printf(INDENT BOLD_WHITE "Nhap ten file de xuat thong tin:" RESET);
                            read_line(filename, sizeof(filename));
                            sort_by_date_in(&L);
                            print_sort_by_date(&L, filename);
                            printf(INDENT GREEN"Da sap xep theo ngay vao.\n" RESET);
                            printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 2:
                            printf(INDENT BOLD_WHITE "Nhap ten file de xuat thong tin:" RESET);
                            read_line(filename, sizeof(filename));
                            sort_by_price(&L);
                            print_sort_by_price(&L, filename);
                            printf(INDENT GREEN"Da sap xep theo tien phong.\n" RESET);
                            printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 0: break;
                        default: printf(INDENT RED "Lua chon khong hop le, vui long chon lai!\n" RESET);
                    }
                } while(phu != 0);
                break;

            case 4:
                do {
                    menu_thongke();
                    phu = get_safe_int(" ", 0, 3);
                    switch(phu) {
                        case 1:
                            printf(INDENT BOLD_WHITE "Nhap ten file de xuat thong tin:" RESET);
                            read_line(filename, sizeof(filename));
                            print_file(&L, filename);
                            printf(INDENT BOLD_WHITE "Da in hoa don cua khach hang ra file:  " GREEN "%s\n" RESET, filename);
                            printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 2:
                            thongke(&L);
                            printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 3: {
                            int ngay = get_safe_int(INDENT"Nhap ngay can thong ke (1-31): ", 1, 31);
                            thongke_theo_ngay(&L, ngay);
                            printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        }
                        case 0: break;
                        default: printf(INDENT RED"Lua chon khong hop le, vui long chon lai!\n"RESET);
                    }
                } while(phu != 0);
                break;

            case 5:
                do {
                    menu_rieng();
                    phu = get_safe_int(" ", 0, 2);
                    switch(phu) {
                        case 1:
                            clear_list(&L);
                            printf(INDENT"Da khoi tao lai.\n");
                            printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 2:
                            printf(INDENT BOLD_WHITE "Nhap ten file de doc thong tin:" RESET);
                            read_line(filename, sizeof(filename));
                            read_file(&L, choice, filename);
                            printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        case 0: break;
                        default: printf(INDENT RED "Lua chon khong hop le, vui long chon lai!\n" RESET);
                    }
                } while(phu != 0);
                break;
            case 6:
                change_customer_info(&L, choice);
                printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                getchar();
                break;

            case 0:
                clear_list(&L); // Giải phóng bộ nhớ trước khi thoát
                printf(INDENT YELLOW"THOÁT CHUƠNG TRÌNH, CẢM ƠN THẦY CÔ ĐÃ LẮNG NGHE\n" RESET);
                break;

            default:
                printf(INDENT RED"Lua chon khong hop le!\n"RESET);
        }
    } while(chinh != 0);
    
    return 0;
}
// in thêm ngày ra, so ngay o