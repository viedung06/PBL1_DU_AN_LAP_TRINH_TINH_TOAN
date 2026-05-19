#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// ĐỊNH NGHĨA MÀU SẮC CHO BẢNG HIỂN THỊ
// Định nghĩa các mã màu ANSI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define CYAN    "\033[1;36m"
#define WHITE_B "\033[1;37m"         
#define BOLD_WHITE "\033[1;37m"             

#define MAX_FLOORS 50
#define MAX_ROOMS_PER_FLOOR 20
#define MAX_ROOM_NAME_LEN 30

// **KHAI BÁO CẤU TRÚC DỮ LIỆU**//
typedef struct { // Thông tin khách hàng
    char name[50];
    char identity_card[15];
    char gender[10];
} info;

typedef struct { // Thông tin ngày tháng, loại phòng, mã hóa đơn và thông tin khách hàng
    int date_in;
    int date_out;
    char room_type[20];
    char invoice_code[10];
    int gia_phong;
	int tien_phong; 
    info customer_info;
} date;

// Khai báo cấu trúc của Linked List
typedef struct Node { 
    date data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;


// ============================================================================
//           NHÓM 1: CÁC THAO TÁC CƠ BẢN TRÊN DANH SÁCH LIÊN KẾT 
// ============================================================================
void init(List *L);
int check_empty(List *L);
int list_length(List *L);
void clear_list(List *L);


// ============================================================================
//           NHÓM 2: CÁC NGHIỆP VỤ THÊM / XÓA HOÁ ĐƠN
// ============================================================================
void add_head(List *L, date data);
void add_tail(List *L, date data);
void add_k_place(List *L, date data, int k);
void delete_head(List *L);
void delete_tail(List *L);
void delete_k_place(List *L, int k);
void delete_more_customer(List *L);
void popMinh(List *L);


// ============================================================================
//           NHÓM 3: KIỂM TRA ĐIỀU KIỆN RÀNG BUỘC VÀ XỬ LÝ CHUỖI 
// ============================================================================
void clear_input_buffer();
void read_line(char *buf, int size);
void normalize_room_code(char *room);
char *lastName(char *name);
void add_dot(char a[], int n, char b[]);
int check_identityCard_exist(List *L, char *cmt);
int check_invoiceCode_exist(List *L, char *invoice);
int check_room_exist(List *L, char *room, int choice);
int check_room_conflict(List *L, char *room, int date_in, int date_out, int choice);


// ============================================================================
//            NHÓM 4: TÍNH TOÁN NGHIỆP VỤ VÀ THỐNG KÊ DOANH THU 
// ============================================================================
int get_days(int in, int out);
char get_room(char *room_number);
int get_price(char type);
void thongke(List *L);
void thongke_theo_ngay(List *L, int date);


// ============================================================================
//             NHÓM 5: TÌM KIẾM VÀ SẮP XẾP DỮ LIỆU 
// ============================================================================
void searchCMT(List *L, char *CMT);
void searchRoom(List *L, char *room, int choice);
void sort_by_date_in(List *L);
void sort_by_price(List *L);


// ============================================================================
//             NHÓM 6: ĐỌC GHI FILE VÀ HIỂN THỊ GIAO DIỆN 
// ============================================================================
void DrawTable();
void menu_chinh();
void menu_them_xoa();
void menu_timkiem();
void menu_sapxep();
void menu_thongke();
void menu_rieng();
void drawCustomDiagramMenu();
void read_file(List *L, int choice);
int read_hotel_layout(const char *filename, int *floor_count, int room_counts[], char rooms[][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN]);
void print_file(List *L);
void print_sort_by_date(List *L);
void print_sort_by_price(List *L);
void export_list_table(List *L);
void export_table(List *L);
void export_custom_table(List *L);





// **KHAI BÁO CÁC HÀM CHỨC NĂNG**//
void init(List *L) { // Hàm khởi tạo danh sách liên kết
    L->head = NULL;
    L->tail = NULL;
}

int check_empty(List *L) { // Hàm kiểm tra xem danh sách có rỗng hay không
    return (L->head == NULL);
}

void add_tail(List *L, date data) { // Hàm thêm một node mới vào cuối danh sách
    Node *p = (Node*)malloc(sizeof(Node));
    p->data = data;
    p->next = NULL;
    if (check_empty(L)) {
        L->head = p;
        L->tail = p;
    } else {
        L->tail->next = p;
        L->tail = p;
    }
    printf("Da them hoa don vao cuoi danh sach.\n");
}

void add_head(List *L, date data) { // Hàm thêm một node mới vào đầu danh sách
    Node *p = (Node*)malloc(sizeof(Node));
    p->data = data;
    p->next = L->head;
    if (check_empty(L)) {
        L->head = p;
        L->tail = p;
    } else {
        L->head = p;
    }
    printf("Da them hoa don vao dau danh sach.\n");
}

void delete_head(List *L) { //Hàm xóa node ở đầu danh sách
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
    } else {
        Node *p = L->head;
        L->head = L->head->next;
        if (L->head == NULL) L->tail = NULL; 
        free(p);
    }
    printf("Da xoa hoa don o dau danh sach.\n");
}

void delete_tail(List *L) { // Hàm xóa node ở cuối danh sách
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
    } else {
        Node *p = L->head;
        if (p->next == NULL) {
            free(p);
            L->head = NULL;
            L->tail = NULL;
        } else {
            while (p->next != L->tail) {
                p = p->next;
            }
            free(L->tail);
            L->tail = p;
            p->next = NULL;
        }
    }
    printf("Da xoa hoa don cuoi danh sach.\n");
}
int list_length(List *L) { // Hàm đếm số lượng node trong danh sách
    int count = 0;
    Node *p = L->head;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}
void delete_k_place(List *L, int k) { // Hàm xóa node tại vị trí k của danh sách
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
    } else {
        if (k == 1) {
            delete_head(L);
        } else {
            Node *p = L->head;
            for (int i = 1; i < k - 1 && p != NULL; i++) {
                p = p->next;
            }
            if (p == NULL || p->next == NULL) {
                delete_tail(L);
            } else {
                Node *temp = p->next;
                p->next = temp->next;
                if (temp == L->tail) {
                    L->tail = p;
                }
                free(temp);
            }
        }
    }
    printf("Da xoa hoa don tai vi tri %d cua danh sach.\n", k);
}
void add_k_place(List *L, date data, int k) { // Hàm thêm node tại vị trí k của danh sách
    if (k == 1) {
        add_head(L, data);
    } else {
        Node *p = (Node*)malloc(sizeof(Node));
        p->data = data;
        p->next = NULL;

        Node *temp = L->head;
        for (int i = 1; i < k - 1 && temp != NULL; i++) {
            temp = temp->next;
        }
        if (temp == NULL) {
            add_tail(L, data);
        } else {
            p->next = temp->next;
            temp->next = p;
            if (p->next == NULL) {
                L->tail = p;
            }
        }
    }
    printf("Da them hoa don moi vao vi tri thu %d cua danh sach.\n", k);
}

void clear_input_buffer() { // Hàm giúp xóa bỏ phần dư trong stdin sau khi dùng scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}
void clear_list(List *L) { // Hàm xóa toàn bộ danh sách liên kết và giải phóng bộ nhớ
    Node *current = L->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    L->head = NULL;
    L->tail = NULL;
}
// Hàm giúp đọc dòng (có thể có dấu cách) và loại bỏ ký tự xuống dòng
void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        // nếu chuỗi dài hơn size-1, bỏ phần dư trong stdin
        clear_input_buffer();
    }
}

// ham tim so ngay o cua khach hang
int get_days(int in, int out){
	return (out - in + 1); 
}

// ham tim loai phong
char get_room(char *room_number){
	return room_number[0];
}

// ham tim don gia cua tung loai phong
int get_price(char type){
	if(type == 'A') return 400000;
	if(type == 'B') return 300000;
    return 200000;
}
void add_dot(char a[], int n, char b[]){ // Hàm thêm dấu chấm để người dùng dễ đọc giá tiền
    int dots = (n - 1) / 3;
    int len = n + dots;
    int i = n - 1;
    int j = len - 1;
    int count = 0;
    b[len] = '\0';
    while(i >= 0){
        if(count == 3){
            b[j] = '.';
            j--;
            count = 0;
        }
        b[j] = a[i];
        i--;
        j--;
        count++;
    }
}
void thongke(List *L) { // Hàm thống kê doanh thu của khách sạn trong 1 tháng
    char tongA_str[20], tongB_str[20], tongKhac_str[20], tongTatCa_str[20];
    char tempA[20], tempB[20], tempKhac[20], tempTatCa[20];
    int tongA = 0, tongB = 0, tongKhac = 0, tongTatCa = 0;
    Node *p = L->head;
    while(p != NULL) {
        char type = get_room(p->data.room_type);
        if(type == 'A') tongA += p->data.tien_phong;
        else if(type == 'B') tongB += p->data.tien_phong;
        else tongKhac += p->data.tien_phong;
        p = p->next;
    }
    tongTatCa += tongA+tongB+tongKhac;
    
    sprintf(tongA_str, "%d", tongA);
    sprintf(tongB_str, "%d", tongB);
    sprintf(tongKhac_str, "%d", tongKhac);
    sprintf(tongTatCa_str, "%d", tongTatCa);

    add_dot(tongA_str, strlen(tongA_str), tempA);
    add_dot(tongB_str, strlen(tongB_str), tempB);
    add_dot(tongKhac_str, strlen(tongKhac_str), tempKhac);
    add_dot(tongTatCa_str, strlen(tongTatCa_str), tempTatCa);

    printf("\n" YELLOW BOLD "              BẢNG THỐNG KÊ DOANH THU" RESET "\n");

    printf(BLUE "╔══════════════════════╦═════════════════════════╗\n");
    
    printf("║" WHITE_B "     LOẠI PHÒNG       " BLUE "║" WHITE_B "     DOANH THU (VNĐ)     " BLUE "║\n");
    
    printf("╠══════════════════════╬═════════════════════════╣\n");

    printf("║ " WHITE_B "Phòng loại A        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempA);
    printf("║ " WHITE_B "Phòng loại B        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempB);
    printf("║ " WHITE_B "Các loại khác       " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempKhac);

    printf("╠══════════════════════╬═════════════════════════╣\n");

    printf("║ " GREEN   "TỔNG CỘNG           " BLUE " ║ " GREEN   "%12s" RESET GREEN   " VNĐ      " BLUE "  ║\n", tempTatCa);

    printf(BLUE "╚══════════════════════╩═════════════════════════╝\n" RESET);
    printf("\n");
}
void thongke_theo_ngay(List *L, int date) { // Hàm thống kê doanh thu của khách sạn trong 1 ngày cụ thể
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
    tongTatCa += tongA+tongB+tongKhac;

    sprintf(tongA_str, "%d", tongA);
    sprintf(tongB_str, "%d", tongB);
    sprintf(tongKhac_str, "%d", tongKhac);
    sprintf(tongTatCa_str, "%d", tongTatCa);

    add_dot(tongA_str, strlen(tongA_str), tempA);
    add_dot(tongB_str, strlen(tongB_str), tempB);
    add_dot(tongKhac_str, strlen(tongKhac_str), tempKhac);
    add_dot(tongTatCa_str, strlen(tongTatCa_str), tempTatCa);

    printf("\n" YELLOW BOLD "       BẢNG THỐNG KÊ DOANH THU NGÀY %d" RESET "\n", date);

    printf(BLUE "╔══════════════════════╦═════════════════════════╗\n");
    
    printf("║" WHITE_B "     LOẠI PHÒNG       " BLUE "║" WHITE_B "     DOANH THU (VNĐ)     " BLUE "║\n");
    
    printf("╠══════════════════════╬═════════════════════════╣\n");

    printf("║ " WHITE_B "Phòng loại A        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempA);
    printf("║ " WHITE_B "Phòng loại B        " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempB);
    printf("║ " WHITE_B "Các loại khác       " BLUE " ║ " WHITE_B "%12s" RESET " VNĐ      " BLUE "  ║\n", tempKhac);

    printf("╠══════════════════════╬═════════════════════════╣\n");

    printf("║ " GREEN   "TỔNG CỘNG           " BLUE " ║ " GREEN   "%12s" RESET GREEN   " VNĐ      " BLUE "  ║\n", tempTatCa);

    printf(BLUE "╚══════════════════════╩═════════════════════════╝\n" RESET);
    printf("\n");
}
void print_file(List *L) { // Hàm in thông tin các hóa đơn ra file data.txt
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
        return;
    } else {
        FILE *f = fopen("data.txt", "w");
        Node *p = L->head;
        while (p != NULL) {
            fprintf(f, "HOA DON CUA KHACH HANG %s\n", p->data.customer_info.name);
            fprintf(f, "Ngay vao: %d\n", p->data.date_in);
            fprintf(f, "Ngay ra: %d\n", p->data.date_out);
            fprintf(f, "Loai phong: %s\n", p->data.room_type);
            fprintf(f, "Ma hoa don: %s\n", p->data.invoice_code);
            fprintf(f, "Ten khach hang: %s\n", p->data.customer_info.name);
            fprintf(f, "So CMT: %s\n", p->data.customer_info.identity_card);
            fprintf(f, "Gioi tinh: %s\n", p->data.customer_info.gender);
            fprintf(f, "Tien phong phai tra: %d VNĐ\n ", p->data.tien_phong);
            fprintf(f, "-------------------------\n");
            p = p->next;
        }
        fclose(f);
    }
}
// Hàm loại bỏ khoảng trắng ở đầu và cuối, và chuẩn hóa chữ hoa nếu cần
void normalize_room_code(char *room) {
    
    char *start = room; // Con trỏ bắt đầu từ đầu chuỗi
    while (*start != '\0' && isspace((unsigned char)*start)) { // Bỏ qua khoảng trắng ở đầu
        start++; // Di chuyển con trỏ về phía trước cho đến khi gặp ký tự không phải là khoảng trắng hoặc kết thúc chuỗi
    }

    char *end = start + strlen(start) - 1; // Con trỏ bắt đầu từ cuối chuỗi
    while (end >= start && isspace((unsigned char)*end)) { // Bỏ qua khoảng trắng ở cuối
        *end = '\0'; // Đặt ký tự null để cắt bỏ phần khoảng trắng ở cuối
        end--; // Di chuyển con trỏ về phía trước cho đến khi gặp ký tự không phải là khoảng trắng hoặc bắt đầu chuỗi
    }
/* hàm isspace nhận một ký tự và trả về một giá trị khác 0 nếu ký tự đó là một ký tự khoảng trắng (space, tab, newline, v.v.) 
   và trả về 0 nếu không phải là ký tự khoảng trắng. 
   Việc sử dụng (unsigned char) để đảm bảo rằng giá trị được truyền vào hàm isspace nằm trong phạm vi hợp lệ của kiểu char, tránh các 
   lỗi có thể xảy ra khi char có giá trị âm.
 */
   if (start != room) {
        memmove(room, start, strlen(start) + 1); // Di chuyển phần còn lại của chuỗi về đầu mảng nếu có khoảng trắng ở đầu
    }

    for (char *p = room; *p != '\0'; p++) { // Chuẩn hóa chữ hoa
        *p = toupper((unsigned char)*p);
    }
}
int check_room_exist(List *L, char *room, int choice){ // Hàm kiểm tra xem phòng có tồn tại hay không
    normalize_room_code(room);

    int floor_count;
    int room_counts[MAX_FLOORS];
    char rooms[MAX_FLOORS][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN];
    

    if(choice == 1) {
        if (read_hotel_layout("your_hotel.txt", &floor_count, room_counts, rooms)) {
            for (int i = 0; i < floor_count; i++) {
                for (int j = 0; j < room_counts[i]; j++) {
                    if (strcmp(rooms[i][j], room) == 0) {
                        return 1; // Phòng tồn tại trong file
                }
                }
            }
        }
        return 0;
        
    }else{
    char loai_phong;
    char ma_phong[10];

    for (int lau = 1; lau <= 3; lau++) {
        for (int phong = 1; phong <= 6; phong++) {
            if (phong == 1) loai_phong = 'A';
            else if (phong == 2 || phong == 3) loai_phong = 'B';
            else if (phong == 4 || phong == 5) loai_phong = 'C';
            else loai_phong = 'D';

            sprintf(ma_phong, "%c%d%02d", loai_phong, lau, phong);
            if (strcmp(ma_phong, room) == 0) {
                return 1; // Phòng tồn tại trong mặc định
            }
        }
    }
    return 0; // Phòng không tồn tại
}
}


int check_room_conflict(List *L, char *room, int date_in, int date_out, int choice){ // Hàm kiểm tra xem phòng tồn tại và đã có khách đặt hay chưa
    if (!check_room_exist(L, room, choice)) {
        return 0; // Phòng không tồn tại -> coi như không hợp lệ
    }
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.room_type, room) == 0 && !(date_in > p->data.date_out || date_out < p->data.date_in)){
            return 0; // Xung đột ngày đặt phòng
        }
        p = p->next;
    }
    return 1; // Phòng tồn tại và không xung đột
}
// Hàm đọc sơ đồ khách sạn từ file your_hotel.txt 
int read_hotel_layout(const char *filename, int *floor_count, int room_counts[], char rooms[][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN]) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 0;
    }

    if (fscanf(f, "%d", floor_count) != 1) {
        fclose(f);
        return 0;
    }

    if (*floor_count <= 0 || *floor_count > MAX_FLOORS) {
        fclose(f);
        return 0;
    }

    for (int i = 0; i < *floor_count; i++) {
        if (fscanf(f, "%d", &room_counts[i]) != 1) {
            fclose(f);
            return 0;
        }
        if (room_counts[i] <= 0 || room_counts[i] > MAX_ROOMS_PER_FLOOR) {
            fclose(f);
            return 0;
        }
        for (int j = 0; j < room_counts[i]; j++) {
            if (fscanf(f, "%s", rooms[i][j]) != 1) {
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);
    return 1;
}

int check_identityCard_exist(List *L, char *cmt){ // Hàm kiểm tra cccd đã tồn tại hay chưa
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.customer_info.identity_card, cmt) == 0){
            return 0;
        }
        p = p->next;
    }
    return 1;
}

int check_invoiceCode_exist(List *L, char *invoice){ // Hàm kiểm tra mã hóa đơn đã tồn tại hay chưa
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.invoice_code, invoice) == 0){
            return 0;
        }
        p = p->next;
    }
    return 1;
}

// Hàm in danh sách hóa đơn của khách hàng trong terminal (bổ trợ cho hàm thêm/ xóa hóa đơn để khách hàng dễ theo dõi)
void export_list_table(List *L) { 
    if (check_empty(L)) {
        printf(RED "\nDanh sach rong! Khong co hoa don de hien thi.\n" RESET);
        return;
    }
    printf(BLUE "\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║ %*s" GREEN BOLD "DANH SÁCH HÓA ĐƠN CỦA KHÁCH HÀNG" RESET BLUE "%*s   ║\n", 22, "", 22, "");
    printf("╠══════╦═══════════════════════════╦════════════╦═════════════════╦══════════════╣\n");
    printf("║ " BOLD_WHITE "%-4s" BLUE " ║ " BOLD_WHITE "%-25s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║\n" RESET,
           "STT", "Ho va Ten", "Phong o", "Ngay vao va ra", "So CCCD");
    printf(BLUE "╠══════╬═══════════════════════════╬════════════╬═════════════════╬══════════════╣\n" RESET);

    Node *p = L->head;
    int stt = 1;

    while (p != NULL) {
        char date_range[20];
        sprintf(date_range, "%d -> %d", p->data.date_in, p->data.date_out);

        printf(BLUE "║ " RESET BOLD_WHITE"%-4d"RESET BLUE " ║ " RESET BOLD_WHITE"%-25.25s"RESET BLUE " ║ " RESET BOLD_WHITE"%-10s"RESET BLUE " ║ " RESET BOLD_WHITE"%-15s"BLUE " ║ " RESET BOLD_WHITE"%-12s"BLUE " ║\n",
               stt, 
               p->data.customer_info.name, 
               p->data.room_type, 
               date_range, 
               p->data.customer_info.identity_card);

        p = p->next;
        stt++;
    }
    printf(BLUE "╚══════╩═══════════════════════════╩════════════╩═════════════════╩══════════════╝\n" RESET);
}

void export_custom_table(List *L) { //Hàm in sơ đồ đặt phòng theo file your_hotel.txt 
    int floor_count;
    int room_counts[MAX_FLOORS];
    char rooms[MAX_FLOORS][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN];

    if (!read_hotel_layout("your_hotel.txt", &floor_count, room_counts, rooms)) {
        printf("Khong doc duoc file your_hotel.txt hoac dinh dang file khong hop le.\n");
        return;
    }
    
    printf(BLUE "\n╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║%*s" GREEN BOLD "SƠ ĐỒ ĐẶT PHÒNG CHI TIẾT" RESET BLUE "%*s ║\n", 37, "", 38, "");
    printf("╠══════════╦══════════╦═══════════════╦══════════╦════════════╦════════════╦═════════════════════════╣\n");
    
    printf("║ " BOLD_WHITE "%-8s" BLUE " ║ " BOLD_WHITE "%-8s" BLUE " ║ " BOLD_WHITE "%-13s" BLUE " ║ " BOLD_WHITE "%-8s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-23s" BLUE " ║\n" RESET,
           "Tang", "Ma phong", "Gia (VND)", "T.Thai", "Ngay Vao", "Ngay Ra", "Khach hang");
    
    printf(BLUE"╠══════════╬══════════╬═══════════════╬══════════╬════════════╬════════════╬═════════════════════════╣\n" RESET);

    for (int i = 0; i < floor_count; i++) {
        for (int j = 0; j < room_counts[i]; j++) {
            
            char type = get_room(rooms[i][j]);
            long price = get_price(type);
            char price_str[20];
            sprintf(price_str, "%ld.000", price / 1000);

            int found_at_least_one = 0;
            Node *p = L->head;

            // Duyệt toàn bộ danh sách khách hàng để tìm tất cả người đặt phòng này
            while (p != NULL) {
        
                if (strcmp(p->data.room_type, rooms[i][j]) == 0) {
                    
                    printf(BLUE "║ " RESET);
                    
                    
                    if (found_at_least_one == 0 && j == 0) {
                        char fl_txt[10]; sprintf(fl_txt, "Tang %d", i + 1);
                        printf("%-8s" BLUE " ║ " RESET, fl_txt);
                    } else {
                        printf("%-8s" BLUE " ║ " RESET, "");
                    }

                 
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
                    printf("%-23s" BLUE " ║\n" RESET, name_tmp);

                    found_at_least_one = 1;
                }
                p = p->next;
            }

            // Nếu không có khách nào trong danh sách khớp với phòng này -> In dòng TRỐNG
            if (!found_at_least_one) {
                printf(BLUE "║ " RESET);
                if (j == 0) {
                    char fl_txt[10]; sprintf(fl_txt, "Tang %d", i + 1);
                    printf("%-8s" BLUE " ║ " RESET, fl_txt);
                } else {
                    printf("%-8s" BLUE " ║ " RESET, "");
                }
                printf("%-8s" BLUE " ║ " RESET, rooms[i][j]);
                printf("%-13s" BLUE " ║ " RESET, price_str);
                printf(CYAN "%-8s" BLUE " ║ " RESET, "Trong");
                printf("%-10d" BLUE " ║ " RESET, 0);
                printf("%-10d" BLUE " ║ " RESET, 0);
                printf("%-23s" BLUE " ║\n" RESET, "-");
            }
        }

        // Đường kẻ phân cách giữa các tầng (In sau khi xong tất cả phòng của 1 tầng)
        if (i < floor_count - 1) {
            printf(BLUE "╠══════════╬══════════╬═══════════════╬══════════╬════════════╬════════════╬═════════════════════════╣\n" RESET);
        }
    }

    // 5. Viền dưới cùng
    printf(BLUE "╚══════════╩══════════╩═══════════════╩══════════╩════════════╩════════════╩═════════════════════════╝\n" RESET);
}

// Hàm bổ trợ nhập liệu để code gọn hơn
void input_data(List *L, date *d, int choice) {
    clear_input_buffer();
    int valid;
    int booking_done = 0; // Cờ hiệu để kiểm tra toàn bộ quá trình nhập đã thành công chưa

    while (!booking_done) {
        do {
            printf("Nhap ngay vao (nam trong khoang 1-31): ");
            valid = scanf("%d", &d->date_in);
            if (valid != 1) {
                printf("Nhap sai. Vui long nhap mot so nguyen tu 1 den 31.\n");
                clear_input_buffer();
                continue;
            }

            printf("Nhap ngay ra (nam trong khoang 1-31): "); 
            valid = scanf("%d", &d->date_out);
            if (valid != 1) {
                printf("Nhap sai. Vui long nhap mot so nguyen tu 1 den 31.\n");
                clear_input_buffer();
                continue;
            }

            if(d->date_in < 1 || d->date_in > 31 || d->date_out < 1 || d->date_out > 31) {
                printf("Ngay vao va ngay ra phai nam trong khoang 1-31. Vui long nhap lai.\n");
                clear_input_buffer();
            }
            else if (get_days(d->date_in, d->date_out) <= 0) {
                printf("Ngay ra phai sau ngay vao. Vui long nhap lai.\n");
                clear_input_buffer();
            } else {
                break; 
            }
        } while(1);
        
        clear_input_buffer(); 
        while (1) {
            printf("Nhap loai phong: ");
            read_line(d->room_type, sizeof(d->room_type));

            int room_exists = check_room_exist(L, d->room_type, choice);

            if (!room_exists) {
                printf("Phong khong ton tai. Vui long nhap lai.\n");
                continue; 
            }

            int room_available = check_room_conflict(L, d->room_type, d->date_in, d->date_out, choice);

            if (!room_available) {
                int choice = 0;
                do {
                    printf("Phong da co khach dat trong thoi gian nay.\n");
                    printf("1. Nhap lai ngay vao va ngay ra\n");
                    printf("2. Nhap lai loai phong\n");
                    printf("Lua chon cua ban (1 hoac 2): ");
                    
                    if (scanf("%d", &choice) != 1) {
                        clear_input_buffer();
                        choice = 0;
                    } else {
                        clear_input_buffer();
                    }
                    
                    if (choice != 1 && choice != 2) {
                        printf("Lua chon khong hop le! Vui long nhap lai.\n");
                    }
                } while (choice != 1 && choice != 2);

                if (choice == 1) {
                    break;
                } else {
                    continue; 
                }
            }

            booking_done = 1; 
            break; 
        }
    }

    do{ // Nhập mã hóa đơn, kiểm tra hóa đơn đã có trong danh sách hay chưa
        printf("Nhap ma hoa don: ");
        read_line(d->invoice_code, sizeof(d->invoice_code));
        if(!check_invoiceCode_exist(L, d->invoice_code)){
            printf("Ma hoa don da ton tai. Vui long nhap lai.\n");
        }
    }while(!check_invoiceCode_exist(L, d->invoice_code));

    printf("Nhap ten khach hang: ");
    read_line(d->customer_info.name, sizeof(d->customer_info.name));
   
     do{ // Nhập cccd; Kiểm tra cccd đã tồn tại, có phải là số và đủ 12 ký tự hay chưa 
        int i;
        int hop_le = 1;
        printf("Nhap so CMT/CCCD: ");
        read_line(d->customer_info.identity_card,
                  sizeof(d->customer_info.identity_card));
        if (strlen(d->customer_info.identity_card) != 12) {
            hop_le = 0;
        }
        for (i = 0;
             i < strlen(d->customer_info.identity_card);
             i++) {

            if (!isdigit(d->customer_info.identity_card[i])) {
                hop_le = 0;
                break;
            }
        }
        if (!hop_le) {
            printf("CCCD phai gom dung 12 chu so. Vui long nhap lai\n");
        }
        else if (!check_identityCard_exist(L,
                 d->customer_info.identity_card)) {

            printf("CCCD da ton tai. Vui long nhap lai\n");
        }
        else {
            break;
        }

    } while(1);

    do{ // Cho phép người dùng chọn giới tính
        int choice;
        printf("Chon gioi tinh:\n");
        printf("1. Nam\n");
        printf("2. Nu\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);
        clear_input_buffer();
        if (choice == 1) {
            strcpy(d->customer_info.gender, "Nam");
            break;
        }
        else if (choice == 2) {
            strcpy(d->customer_info.gender, "Nu");
            break;
        }
        else {
            printf("Lua chon khong hop le.\n");
        }

    } while (1);
    
    //tinh tien phong
    char type = get_room(d->room_type);
    d->gia_phong = get_price(type);
    int days = get_days(d->date_in, d->date_out);
    d->tien_phong = d->gia_phong * days;
}

void read_file(List *L, int choice) { // Đọc file input.txt và thêm dữ liệu vào danh sách liên kết
    FILE *f = fopen("input.txt", "r");
    if (f == NULL) {
        printf("Khong mo duoc file!\n");
        return;
    }

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
            printf("Canh bao: Phong %s khong ton tai. Bo qua hoa don nay.\n", d.room_type);
            continue;
        }
        //tinh tien phong
        char type = get_room(d.room_type);
        d.gia_phong = get_price(type);
        int days = get_days(d.date_in, d.date_out);
        d.tien_phong = d.gia_phong * days;


        add_tail(L, d);
    }

    fclose(f);
    printf("Da doc file thanh cong!\n");
}

//ham tim kiem khach hanh theo so CMT
void searchCMT(List *L, char *CMT){
	if(check_empty(L)){
        printf("Danh sach rong!\n");
        return;
    }
    Node *p = L->head;
    while(p != NULL){
    	if(strcmp(p->data.customer_info.identity_card, CMT) == 0){
    		char date_str[60]; 
            sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
            printf(BLUE "╔═════════════════╦════════════════════════════════════════════════════╗\n" RESET);

            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50.50s" BLUE " ║\n" RESET, "Ho va ten", p->data.customer_info.name);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Gioi tinh", p->data.customer_info.gender);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "So CMT", p->data.customer_info.identity_card);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ngay vao va ra", date_str);

            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Loai phong", p->data.room_type);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ma hoa don", p->data.invoice_code);

            printf(BLUE "╚═════════════════╩════════════════════════════════════════════════════╝\n" RESET);
            printf("\n");
            return;
		}
		p = p->next;
	}
	printf("Khong co khach hang co so CMT vua nhap");
}
//ham tim kiem khach hang theo phong
void searchRoom(List *L, char *room, int choice){
	if(check_empty(L)){
        printf("Danh sach rong!\n");
        return;
    }
    if(!check_room_exist(L, room, choice)){
        printf("Phong khong ton tai!\n");
        return;
    }
    Node *p = L->head;
    printf(GREEN "\nTHONG TIN KHACH HANG DA DAT PHONG %s:\n" RESET, room);
    while(p != NULL){
    	if(strcmp(p->data.room_type, room) == 0){
            char date_str[60]; 
            sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
            printf(BLUE "╔═════════════════╦════════════════════════════════════════════════════╗\n" RESET);

            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50.50s" BLUE " ║\n" RESET, "Ho va ten", p->data.customer_info.name);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Gioi tinh", p->data.customer_info.gender);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "So CMT", p->data.customer_info.identity_card);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ngay vao va ra", date_str);

            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Loai phong", p->data.room_type);
            printf(BLUE "║ " BOLD_WHITE "%-15s" BLUE " ║ " BOLD_WHITE "%-50s" BLUE " ║\n" RESET, "Ma hoa don", p->data.invoice_code);

            printf(BLUE "╚═════════════════╩════════════════════════════════════════════════════╝\n" RESET);
            printf("\n");
		}
		p = p->next;
	}
}

char *lastName(char *name) { // Hàm lấy tên cuối cùng từ chuỗi họ tên
    int len = strlen(name);
    while (len > 0 && isspace((unsigned char)name[len - 1])) {
        name[len - 1] = '\0';
        len--;
    }
    char *last = strrchr(name, ' ');
    if (last != NULL)  return last + 1; 
    return name; 
}
//ham xoa khach hang co ten Minh
void popMinh(List *L) {
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
        return;
    }
    int found = 0;
    Node *current = L->head;
    Node *prev = NULL;
    while (current != NULL) {
        if (strcmp(lastName(current->data.customer_info.name), "Minh") == 0) {
            Node *del = current;
            if (del == L->head) { 
                L->head = L->head->next;
                current = L->head; // Cập nhật current sang node tiếp theo
                if (L->head == NULL) L->tail = NULL;
            } else { // Xóa giữa hoặc cuối
                prev->next = del->next;
                current = del->next; // Cập nhật current mà không làm mất liên kết
                if (del == L->tail) {
                    L->tail = prev;
                }
            }
            free(del);
            found = 1;
        } else {
            prev = current;
            current = current->next;
        }
    }
    if (found) printf("Da xoa tat ca khach hang co ten Minh.\n");
    else printf("Khong co khach hang ten Minh.\n");
}
// Ham sắp xếp thông tin khách hàng theo ngày vào 
void sort_by_date_in(List *L) {
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
        return;
    }

    for (Node *i = L->head; i != NULL; i = i->next) {
        for (Node *j = i->next; j != NULL; j = j->next) {
            if (i->data.date_in > j->data.date_in) {
                date temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}   
void print_sort_by_date(List *L) { // Hàm in thông tin đã sắp xếp theo ngày vào ra file data
    FILE *f = fopen("datein.txt", "w");

    if (f == NULL) {
        printf("Khong mo duoc file!\n");
        return;
    }

    fprintf(f, "===============================================================\n");
    fprintf(f, "            DANH SACH SAP XEP THEO NGAY VAO\n");
    fprintf(f, "===============================================================\n");

    fprintf(f, "%-5s %-25s %-10s %-15s %-15s\n",
            "STT", "TEN KHACH", "PHONG", "NGAY VAO", "TIEN PHONG");

    fprintf(f, "---------------------------------------------------------------\n");

    Node *p = L->head;
    int stt = 1;

    while (p != NULL) {
        fprintf(f, "%-5d %-25s %-10s %-15d %-15d\n",
                stt,
                p->data.customer_info.name,
                p->data.room_type,
                p->data.date_in,
                p->data.tien_phong);

        p = p->next;
        stt++;
    }

    fprintf(f, "===============================================================\n");

    fclose(f);

    printf("Da xuat file datein.txt\n");
}
void sort_by_price(List *L) { // Ham sắp xếp thông tin khách hàng theo tiền phòng
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
        return;
    }

    for (Node *i = L->head; i != NULL; i = i->next) {
        for (Node *j = i->next; j != NULL; j = j->next) {
            if (i->data.tien_phong < j->data.tien_phong) {
                date temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}
void print_sort_by_price(List *L) { // Hàm in thông tin đã sắp xếp theo tiền phòng ra file data
    FILE *f = fopen("price.txt", "w");

    if (f == NULL) {
        printf("Khong mo duoc file!\n");
        return;
    }

    fprintf(f, "===============================================================\n");
    fprintf(f, "           DANH SACH SAP XEP THEO TIEN PHONG\n");
    fprintf(f, "===============================================================\n");

    fprintf(f, "%-5s %-25s %-10s %-15s %-15s\n",
            "STT", "TEN KHACH", "PHONG", "NGAY VAO", "TIEN PHONG");

    fprintf(f, "---------------------------------------------------------------\n");

    Node *p = L->head;
    int stt = 1;

    while (p != NULL) {
        fprintf(f, "%-5d %-25s %-10s %-15d %-15d\n",
                stt,
                p->data.customer_info.name,
                p->data.room_type,
                p->data.date_in,
                p->data.tien_phong);

        p = p->next;
        stt++;
    }

    fprintf(f, "===============================================================\n");

    fclose(f);

    printf("Da xuat file price.txt\n");
}
void delete_more_customer(List *L){ // Hàm xóa nhiều khách hàng theo vị trí nhập từ bàn phím
    if (check_empty(L)) {
        printf("Danh sach rong!\n");
        return;
    }
    Node *p = L->head;
    int n=1;
    while(n){
        export_list_table(L);
        int a = list_length(L);
        printf("Nhap vi tri khach hang can xoa (0 de dung): ");
        while(1){
            if(scanf("%d", &n) != 1 || n < 0 || n > a){
                printf("Nhap sai. Vui long nhap mot so nguyen duong nam trong khoang [0, %d] (hoac 0 de dung): ", a);
                clear_input_buffer();
            }
            else{
                clear_input_buffer();
                break;
            }
        }
        if(n == 0) break;
        delete_k_place(L, n);
    }
}
//    HÀM IN RA BẢNG THỐNG KÊ LOẠI PHÒNG GỒM CÁC CỘT: LOẠI PHÒNG, TỔNG DOANH THU
void export_table(List *L) {
    
    printf(BLUE "╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n" RESET);
    
    printf(BLUE "║ %*s"GREEN BOLD "DANH SÁCH CHI TIẾT ĐẶT PHÒNG KHÁCH SẠN"RESET BLUE"%*s║\n", 38, "", 39, "");
    
    printf(BLUE "╠════════════╦════════════╦════════════╦════════════════╦══════════════╦══════════════╦══════════════════════════════╣\n" RESET);
    
    printf(BLUE "║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-10s" BLUE " ║ " BOLD_WHITE "%-14s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║ " BOLD_WHITE "%-12s" BLUE " ║ " BOLD_WHITE "%-28s" BLUE " ║\n" RESET,
           "Tang/Lau", "Ma Phong", "Loai", "Gia (VND)", "Ngay Vao", "Ngay Ra", "Ten Khach Hang");

    char loai_phong;
    char gia_tien[15];
    char ma_phong[10];

    for (int lau = 1; lau <= 3; lau++) {
        printf(BLUE "╠════════════╬════════════╬════════════╬════════════════╬══════════════╬══════════════╬══════════════════════════════╣\n" RESET);

        for (int phong = 1; phong <= 6; phong++) {
            // Xác định loại phòng và giá (Giữ nguyên logic cũ)
            if (phong == 1) { loai_phong = 'A'; strcpy(gia_tien, "400,000"); }
            else if (phong == 2 || phong == 3) { loai_phong = 'B'; strcpy(gia_tien, "300,000"); }
            else if (phong == 4 || phong == 5) { loai_phong = 'C'; strcpy(gia_tien, "200,000"); }
            else { loai_phong = 'D'; strcpy(gia_tien, "200,000"); }
            
            sprintf(ma_phong, "%c%d%02d", loai_phong, lau, phong);

            int found_customer = 0;
            Node *p = L->head;

            // Duyệt toàn bộ danh sách để tìm TẤT CẢ khách hàng đặt phòng này
            while (p != NULL) {
                if (strcmp(p->data.room_type, ma_phong) == 0) {
                    printf(BLUE "║ " RESET);
                    
                    // Cột Tầng/Lầu (Chỉ in ở dòng đầu tiên của phòng đó)
                    if (found_customer == 0 && phong == 1) {
                        char txt_lau[15]; sprintf(txt_lau, "Lau %d", lau);
                        printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_lau);
                    } else {
                        printf("%-10s" BLUE " ║ " RESET, "");
                    }

                    // Các cột thông tin phòng (Chỉ in ở dòng đầu tiên của khách đầu tiên trong phòng đó)
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
                    
                    char ten_tam[31];
                    strncpy(ten_tam, p->data.customer_info.name, 28);
                    ten_tam[28] = '\0';
                    printf(BOLD_WHITE "%-28s" BLUE " ║\n" RESET, ten_tam);

                    found_customer++;
                }
                p = p->next;
            }

            // Nếu phòng trống, in 1 dòng trống
            if (found_customer == 0) {
                printf(BLUE "║ " RESET);
                if (phong == 1) {
                    char txt_lau[15]; sprintf(txt_lau, "Lau %d", lau);
                    printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_lau);
                } else {
                    printf("%-10s" BLUE " ║ " RESET, "");
                }
                
                char txt_loai[15]; sprintf(txt_loai, "Loai %c", loai_phong);
                printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, ma_phong);
                printf(BOLD_WHITE "%-10s" BLUE " ║ " RESET, txt_loai);
                printf(BOLD_WHITE "%-14s" BLUE " ║ " RESET, gia_tien);
                printf("%-12d" BLUE " ║ " RESET, 0);
                printf("%-12d" BLUE " ║ " RESET, 0);
                printf(CYAN "%-28s" BLUE " ║\n" RESET, "- Trong -");
            }
        }
    }
    printf(BLUE "╚════════════╩════════════╩════════════╩════════════════╩══════════════╩══════════════╩══════════════════════════════╝\n" RESET);
}


void DrawTable() { // Hàm vẽ bảng giới thiệu đề tài và thành viên nhóm trong terminal
    printf(BLUE "╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                       " GREEN BOLD "PBL1 - ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN" RESET BLUE "                       ║\n");
    printf("╠══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║ " CYAN "Đề tài:" RESET " QUẢN LÝ THÔNG TIN KHÁCH SẠN TRONG 1 THÁNG                            " BLUE "║\n");
    printf("╠══════════════════════════════════════╦═══════════════════════════════════════╣\n");
    printf("║                " YELLOW "MÔ TẢ" RESET BLUE "                 ║         " YELLOW "SINH VIÊN THỰC HIỆN" RESET BLUE "           ║\n");
    printf("╠══════════════════════════════════════╬═══════════════════════════════════════╣\n");

    printf("║     Dùng Danh sách liên kết đơn để:  ║                  NHÓM 8               " BLUE "║\n");
    printf("║ - Quản lý danh sách khách hàng       ║ 1. Nguyễn Việt Dũng                   " BLUE "║\n");
    printf("║ - Quản lý đặt phòng/trả phòng        ║    Lớp: 25T_DT4     MSSV: 102250165   " BLUE "║\n");
    printf("║ - Tính toán doanh thu hàng tháng     ║                                       " BLUE "║\n");
    printf("║ - Đọc và ghi dữ liệu từ file.txt     ║ 2. Ngô Đăng Gia Bảo                   " BLUE "║\n");
    printf("║ - Tìm kiếm & sắp xếp thông tin       ║    Lớp: 25T_DT4     MSSV: 102250158   " BLUE "║\n");

    printf("╠══════════════════════════════════════╩═══════════════════════════════════════╣\n");
    printf("║ " RED "GIẢNG VIÊN HƯỚNG DẪN: ĐỖ THỊ TUYẾT HOA" RESET BLUE "                                       ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════════╝\n" RESET);

    printf("\n" BOLD_WHITE " Nhấn phím bất kỳ để tiếp tục..." RESET);
    printf("\n");
    getchar(); 
}

// **HÀM IN MENU**//
void menu_chinh() {
    printf(BLUE"\n  ╔════════════════════════════════════════════════╗\n");
    printf("  ║            " GREEN "QUAN LY KHACH SAN" RESET BLUE "                   ║\n");
    printf("  ╠════════════════════════════════════════════════╣\n");
    printf("  ║  " BOLD_WHITE "Chuc nang:" RESET BLUE "                                    ║\n");
    printf("  ║  " BOLD_WHITE "1. Xoa va them hoa don" RESET BLUE "                        ║\n");
    printf("  ║  " BOLD_WHITE "2. Tim kiem va tra cuu khach hang" RESET BLUE "             ║\n");
    printf("  ║  " BOLD_WHITE "3. Sap xep hoa don" RESET BLUE "                            ║\n");
    printf("  ║  " BOLD_WHITE "4. Thong ke va in du lieu" RESET BLUE "                     ║\n");
    printf("  ║  " BOLD_WHITE "5. Chuc nang de rieng" RESET BLUE "                         ║\n");
    printf("  ║  " BOLD_WHITE "0. Thoat" RESET BLUE "                                      ║\n");

    printf("  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_them_xoa() {
    printf(BLUE"\n  ╔════════════════════════════════════════════════╗\n");
    printf("  ║            " BOLD_WHITE "XOA VA THEM HOA DON" RESET BLUE "                 ║\n");
    printf("  ╠════════════════════════════════════════════════╣\n");
    printf("  ║  " BOLD_WHITE "1. Them hoa don vao cuoi danh sach" RESET BLUE "            ║\n");
    printf("  ║  " BOLD_WHITE "2. Them hoa don tai vi tri k" RESET BLUE "                  ║\n");
    printf("  ║  " BOLD_WHITE "3. Xoa hoa don cuoi danh sach" RESET BLUE "                 ║\n");
    printf("  ║  " BOLD_WHITE "4. Xoa hoa don tai vi tri k" RESET BLUE "                   ║\n");
    printf("  ║  " BOLD_WHITE "5. Xoa nhieu khach hang theo vi tri" RESET BLUE "           ║\n");
    printf("  ║  " BOLD_WHITE "6. Xoa khach hang ten Minh" RESET BLUE "                    ║\n");
    printf("  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_timkiem() {
    printf(BLUE"\n  ╔════════════════════════════════════════════════╗\n");
    printf("  ║            " BOLD_WHITE "TIM KIEM VA TRA CUU" RESET BLUE "                 ║\n");
    printf("  ╠════════════════════════════════════════════════╣\n");
    printf("  ║  " BOLD_WHITE "1. Tim kiem theo so CMT" RESET BLUE "                       ║\n");
    printf("  ║  " BOLD_WHITE "2. Tim kiem theo phong" RESET BLUE "                        ║\n");
    printf("  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_sapxep() {
    printf(BLUE"\n  ╔════════════════════════════════════════════════╗\n");
    printf("  ║               " BOLD_WHITE "SAP XEP HOA DON" RESET BLUE "                  ║\n");
    printf("  ╠════════════════════════════════════════════════╣\n");
    printf("  ║  " BOLD_WHITE "1. Sap xep theo ngay vao" RESET BLUE "                      ║\n");
    printf("  ║  " BOLD_WHITE "2. Sap xep theo tien phong" RESET BLUE "                    ║\n");
    printf("  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_thongke() {
    printf(BLUE"\n  ╔════════════════════════════════════════════════╗\n");
    printf("  ║            " BOLD_WHITE "THONG KE VA IN DU LIEU" RESET BLUE "              ║\n");
    printf("  ╠════════════════════════════════════════════════╣\n");
    printf("  ║  " BOLD_WHITE "1. In thong tin hoa don ra file" RESET BLUE "               ║\n");
    printf("  ║  " BOLD_WHITE "2. Thong ke doanh thu 1 thang" RESET BLUE "                 ║\n");
    printf("  ║  " BOLD_WHITE "3. Thong ke doanh thu theo ngay" RESET BLUE "               ║\n");
    printf("  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE "Nhap lua chon: " RESET);
}

void menu_rieng() {
    printf(BLUE"\n  ╔════════════════════════════════════════════════╗\n");
    printf("  ║              " BOLD_WHITE "CHUC NANG DE RIENG" RESET BLUE "                ║\n");
    printf("  ╠════════════════════════════════════════════════╣\n");
    printf("  ║  " BOLD_WHITE "1. Khoi tao lai" RESET BLUE "                               ║\n");
    printf("  ║  " BOLD_WHITE "2. Doc du lieu tu file" RESET BLUE "                        ║\n");
    printf("  ║  " BOLD_WHITE "0. Quay lai" RESET BLUE "                                   ║\n");
    printf("  ╚════════════════════════════════════════════════╝\n");
    printf("  " BOLD_WHITE "Nhap lua chon: " RESET);
}
void drawCustomDiagramMenu(){
    printf(BLUE"╔══════════════════════════════════════════════════════════╗\n");
    printf("║  "GREEN"BAN CO MUON TAO SO DO KHACH SAN CUA RIENG BAN KHONG ?"RESET BLUE"   ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║  " BOLD_WHITE "1. Co" RESET BLUE "                                                   ║\n");
    printf("║  " BOLD_WHITE "Khac 1. Khong" RESET BLUE "                                           ║\n");
    printf("╚══════════════════════════════════════════════════════════╝"RESET"\n");
}
int main() {
    DrawTable();
    int k;
    List L;
    init(&L); // LUÔN KHỞI TẠO NGAY ĐẦU TIÊN
    date data;
    int chinh, phu, choice;
    drawCustomDiagramMenu();
     while (1) {
        if (scanf("%d", &choice) == 1) {
            break; 
        } else {
            printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
            while (getchar() != '\n'); 
        }
    }
    read_file(&L, choice); 

do {
    if (choice == 1) {
        export_custom_table(&L);
        clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
        getchar();
    }else{
        export_table(&L);
        clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
        getchar();
    }
    menu_chinh();
    while (1) {
        if (scanf("%d", &chinh) == 1) {
            break; 
        } else {
            printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
            while (getchar() != '\n'); 
        }
    }

    switch(chinh) {

        case 1:
            do {
                menu_them_xoa();
                while (1) {
                    if (scanf("%d", &phu) == 1) {
                        break;
                    } else {
                        printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
                        while (getchar() != '\n');
                    }
                }
                clear_input_buffer();  // Tiêu thụ newline

                switch(phu) {
                    case 1:
                        input_data(&L, &data, choice);
                        add_tail(&L, data);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 2:
                        export_list_table(&L);
                        int b = list_length(&L);
                        printf("Nhap vi tri can them: ");
                        while(1){
                            if (scanf("%d", &k) == 1 && k >= 1 && k <= b) {
                                clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                                break;
                            } else {
                                printf("Loi! Ban phai nhap so tu 1 den %d. Thu lai nhe!\n", b);
                                clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                            }
                        }
                        getchar();
                        input_data(&L, &data, choice);
                        add_k_place(&L, data, k);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 3:
                        delete_tail(&L);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 4:
                        int c = list_length(&L);
                        export_list_table(&L);
                        printf("Nhap vi tri can xoa: ");
                        while(1){
                            if (scanf("%d", &k) == 1 && k >= 1 && k <= c) {
                                clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                                break;
                            } else {
                                printf("Loi! Ban phai nhap so tu 1 den %d. Thu lai nhe!\n", c);
                                clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                            }
                        }
                        delete_k_place(&L, k);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 5:
                        delete_more_customer(&L);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 6:
                        popMinh(&L);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    default:
                        printf("Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 2:
            do {
                menu_timkiem();
                while (1) {
                    if (scanf("%d", &phu) == 1) {
                        break;
                    } else {
                        printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
                        while (getchar() != '\n');
                    }
                }
                clear_input_buffer();  // Tiêu thụ newline

                switch(phu) {
                    case 1: {
                        char CMT[15];
                        printf("Nhap so CMT: ");
                        read_line(CMT, sizeof(CMT));
                        searchCMT(&L, CMT);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    }

                    case 2: {
                        char room[20];
                        printf("Nhap ma phong: ");
                        scanf("%s", room);
                        searchRoom(&L, room, choice);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    }
                    default:
                        printf("Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 3:
            do {
                menu_sapxep();
                while (1) {
                    if (scanf("%d", &phu) == 1) {
                        break;
                    } else {
                        printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
                        while (getchar() != '\n');
                    }
                }
                clear_input_buffer();  // Tiêu thụ newline

                switch(phu) {
                    case 1:
                        sort_by_date_in(&L);
                        print_sort_by_date(&L);
                        printf("Da sap xep theo ngay vao.\n");
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 2:
                        sort_by_price(&L);
                        print_sort_by_price(&L);
                        printf("Da sap xep theo tien phong.\n");
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    default:
                        printf("Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 4:
            do {
                menu_thongke();
                while (1) {
                    if (scanf("%d", &phu) == 1) {
                        break;
                    } else {
                        printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
                        while (getchar() != '\n');
                    }
                }
                clear_input_buffer(); // Tiêu thụ newline sau khi đọc số

                switch(phu) {
                    case 1:
                        print_file(&L);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 2:
                        thongke(&L);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 3:
                        int ngay;
                        printf(BOLD_WHITE "Nhap ngay can thong ke (1-31): " RESET);
                        while(1){
                            if (scanf("%d", &ngay) == 1 && ngay >= 1 && ngay <= 31) {
                            clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                            thongke_theo_ngay(&L, ngay);
                            printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                            getchar();
                            break;
                        } else {
                            printf("Loi! Ban phai nhap so tu 1 den 31. Thu lai nhe!\n");
                            clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                          }
                        }
                        
                        break;
                    default:
                        printf("Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 5:
            do {
                menu_rieng();
                while (1) {
                    if (scanf("%d", &phu) == 1) {
                        break;
                    } else {
                        printf("Loi! Ban phai nhap so. Thu lai nhe!\n");
                        while (getchar() != '\n');
                    }
                }
                clear_input_buffer(); // Tiêu thụ newline sau khi đọc số
                switch(phu) {
                    case 1:
                        clear_list(&L);
                        printf("Da khoi tao lai.\n");
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 2:
                        read_file(&L, choice);
                        printf(BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    default:
                        printf("Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 0:
            clear_list(&L); // Giải phóng bộ nhớ trước khi thoát
            printf("Thoat chuong trinh.\n");
            break;

        default:
            printf("Lua chon khong hop le!\n");
    }

} while(chinh != 0);
    return 0;
}
// Chia nhỏ chức năng để riêng
// Delay                                      DONE 
// Báo lỗi khi nhập ngày sai                  DONE
// Thông báo trùng phòng                      DONE
/* Trả về thông báo nếu input vô lý
     Identity card                            DONE
     + identity_card phải là số và có độ dài 12 ký tự
     + nếu bị trung sẽ báo lỗi và yêu cầu nhập lại
     Gender                                   DONE
     + Cho người dùng chọn giới tính từ một danh sách có sẵn (Nam, Nữ) để tránh lỗi nhập liệu
     Invoice code                             DONE
     + Không được trùng với các mã hóa đơn đã tồn tại trong danh sách. Nếu trùng sẽ báo lỗi và yêu cầu nhập lại
    Room type                                 
     + Nếu trùng phòng đã có khách đặt sẽ báo lỗi và yêu cầu chọn phòng khác
     + Nếu nhập loại phòng không tồn tại sẽ báo lỗi và yêu cầu nhập lại
    
    Thêm thống kê doanh thu theo ngày       DONE
    Sửa check_room_exist                    DONE
    Cho 2 lựa chọn khi chọn phòng trùng ngày với khách hàng khác
    + Đổi ngày
    + Đổi phòng
    Sắp xếp in tên và tiền phòng
    Sắp xếp in tên và ngày vào
    
*/
// tạo thư viện riêng mystring.h để xử lý xâu (tách họ tên,...)
// Đối với yêu cầu sắp xếp, tự động in hóa đơn ra file sau khi sắp xếp để dễ kiểm tra kết quả    DONE

