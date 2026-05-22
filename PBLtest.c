#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// ĐỊNH NGHĨA MÀU SẮC CHO PHẦN HIỂN THỊ
#define RESET   "\033[0m"            // Đặt lại màu mặc định
#define BOLD    "\033[1m"            // In đậm
#define BLUE    "\033[1;34m"         // Màu xanh dương sáng
#define GREEN   "\033[1;32m"         // Màu xanh lá sáng
#define YELLOW  "\033[1;33m"         // Màu vàng sáng
#define RED     "\033[1;31m"         // Màu đỏ sáng
#define CYAN    "\033[1;36m"         // Màu cyan sáng
#define WHITE_B "\033[1;37m"         // In đậm
#define BOLD_WHITE "\033[1;37m"      // In siêu đậm
// ĐỊNH NGHĨA KHOẢNG TRỐNG ĐỂ CĂN LỀ
#define INDENT "                                  "

// ĐỊNH NGHĨA TÊN FILE DỮ LIỆU CỦA KHÁCH SẠN
#define FILE_LAYOUT      "your_hotel.txt"
#define FILE_INPUT       "input.txt"
#define FILE_DATA_OUT    "data.txt"
#define FILE_SORT_DATE   "datein.txt"
#define FILE_SORT_PRICE  "price.txt"

// ĐỊNH NGHĨA SỐ LƯỢNG TẦNG, PHÒNG TRÊN MỖI TẦNG VÀ ĐỘ DÀI TÊN PHÒNG        
#define MAX_FLOORS 50
#define MAX_ROOMS_PER_FLOOR 100
#define MAX_ROOM_NAME_LEN 30

int floor_count;
int room_counts[MAX_FLOORS];
char rooms[MAX_FLOORS][MAX_ROOMS_PER_FLOOR][MAX_ROOM_NAME_LEN];
    
// **KHAI BÁO CẤU TRÚC DỮ LIỆU**//
typedef struct { // Thông tin khách hàng
    char name[50];
    char identity_card[15];
    char gender[10];
} info;

typedef struct { // Thông tin ngày tháng, loại phòng, mã hóa đơn và thông tin khách hàng
    int date_in;
    int date_out;
    int gia_phong;
	int tien_phong; 
    char room_type[20];
    char invoice_code[10];
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
int get_safe_int(const char *prompt, int min, int max);
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
static void merge_sort_by_date(Node **head);
static void merge_sort_by_price(Node **head);
static Node* merge_by_date(Node *left, Node *right);
static Node* merge_by_price(Node *left, Node *right);
static void split_list(Node *source, Node **front, Node **back);

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

void add_head(List *L, date data) { // Hàm thêm một node mới vào đầu danh sách
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
    } else {
        L->head = p;
    }
    printf(INDENT BLUE"Da them hoa don vao dau danh sach.\n" RESET);
}
void delete_head(List *L) { //Hàm xóa node ở đầu danh sách
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

void delete_tail(List *L) { // Hàm xóa node ở cuối danh sách
    if (check_empty(L)) {
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
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
    printf(INDENT BLUE "Da xoa hoa don cuoi danh sach.\n" RESET);
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
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
    } else {
        if (k == 1) {
            delete_head(L);
            return;
        } else {
            Node *p = L->head;
            for (int i = 1; i < k - 1 && p != NULL; i++) {
                p = p->next;
            }
            if (p == NULL || p->next == NULL) {
                delete_tail(L);
                return;
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
    printf(INDENT BLUE "Da xoa hoa don tai vi tri %d cua danh sach.\n" RESET, k);
}
void add_k_place(List *L, date data, int k) { // Hàm thêm node tại vị trí k của danh sách
    int len = list_length(L);
    if (k == 1) {
        add_head(L, data);
        return;
    } else if(k == len + 1) {
        add_tail(L, data);
        return;
    } else if (k > 1 && k <= len) {
        Node *p = (Node*)malloc(sizeof(Node));
        if(p == NULL){
            printf(INDENT RED "Khong du bo nho de cap phat node moi!\n" RESET);
            return;
        }
        p->data = data;
        Node *temp = L->head;
        for (int i = 1; i < k - 1 && temp != NULL; i++) {
            temp = temp->next;
        }
        p->next = temp->next;
        temp->next = p;
        
    }
    printf(INDENT BLUE "Da them hoa don moi vao vi tri thu %d cua danh sach.\n" RESET, k);
    return;
}

void clear_input_buffer() { // Hàm giúp xóa bỏ phần dư trong stdin sau khi dùng scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF){
    }
    
}
int get_safe_int(const char *prompt, int min, int max) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) == 1 && val>= min && val <= max) {
            clear_input_buffer();
            return val;
        } else {
            clear_input_buffer();
        }
        printf(INDENT RED "Gia tri khong hop le! Vui long nhap lai trong khoang [%d, %d].\n" RESET, min, max);
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
	return (out - in ); 
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
    if(n==0) return;
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
    tongTatCa = tongA+tongB+tongKhac;
    
    sprintf(tongA_str, "%d", tongA);
    sprintf(tongB_str, "%d", tongB);
    sprintf(tongKhac_str, "%d", tongKhac);
    sprintf(tongTatCa_str, "%d", tongTatCa);

    add_dot(tongA_str, strlen(tongA_str), tempA);
    add_dot(tongB_str, strlen(tongB_str), tempB);
    add_dot(tongKhac_str, strlen(tongKhac_str), tempKhac);
    add_dot(tongTatCa_str, strlen(tongTatCa_str), tempTatCa);

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
    tongTatCa = tongA+tongB+tongKhac;

    sprintf(tongA_str, "%d", tongA);
    sprintf(tongB_str, "%d", tongB);
    sprintf(tongKhac_str, "%d", tongKhac);
    sprintf(tongTatCa_str, "%d", tongTatCa);

    add_dot(tongA_str, strlen(tongA_str), tempA);
    add_dot(tongB_str, strlen(tongB_str), tempB);
    add_dot(tongKhac_str, strlen(tongKhac_str), tempKhac);
    add_dot(tongTatCa_str, strlen(tongTatCa_str), tempTatCa);

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
void print_file(List *L) { // Hàm in thông tin các hóa đơn ra file data.txt
    if (check_empty(L)) {
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET);
        return;
    } else {
        FILE *f = fopen(FILE_DATA_OUT, "w");
        Node *p = L->head;
        char tien_phong_str[20];
        char tien_phong_temp[20];
        while (p != NULL) {
            fprintf(f, "HOA DON CUA KHACH HANG %s\n", p->data.customer_info.name);
            fprintf(f, "Ngay vao: %d\n", p->data.date_in);
            fprintf(f, "Ngay ra: %d\n", p->data.date_out);
            fprintf(f, "Loai phong: %s\n", p->data.room_type);
            fprintf(f, "Ma hoa don: %s\n", p->data.invoice_code);
            fprintf(f, "Ten khach hang: %s\n", p->data.customer_info.name);
            fprintf(f, "So CMT: %s\n", p->data.customer_info.identity_card);
            fprintf(f, "Gioi tinh: %s\n", p->data.customer_info.gender);
            sprintf(tien_phong_str, "%d", p->data.tien_phong);
            add_dot(tien_phong_str, strlen(tien_phong_str), tien_phong_temp);
            fprintf(f, "Tien phong phai tra: %s VNĐ\n ", tien_phong_temp);
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
            if (fscanf(f, "%29s", rooms[i][j]) != 1) {
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);
    return 1;
}

int check_room_exist(List *L, char *room, int choice){ // Hàm kiểm tra phòng có tồn tại hay không

    normalize_room_code(room);

    if(choice == 1){
        for (int i = 0; i < floor_count; i++) {
            for (int j = 0; j < room_counts[i]; j++) {
                if (strcmp(rooms[i][j], room) == 0) {
                    return 1;
                }
            }
        }
        return 0;
    }

    else{
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
                    return 1;
                }
            }
        }
        return 0;
    }
}

int check_room_conflict(List *L, char *room, int date_in, int date_out, int choice){ // Hàm kiểm tra xem phòng tồn tại và đã có khách đặt hay chưa
    if (!check_room_exist(L, room, choice)) {
        return 0; // Phòng không tồn tại -> coi như không hợp lệ
    }
    Node *p = L->head;
    while(p != NULL){
        if(strcmp(p->data.room_type, room) == 0 && !(date_in >= p->data.date_out || date_out <= p->data.date_in)){
            return 0; // Xung đột ngày đặt phòng
        }
        p = p->next;
    }
    return 1; // Phòng tồn tại và không xung đột
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
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET);
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
    // clear_input_buffer(); tạm thời để như này để test xem nhập liệu có lỗi không, nếu không thì xóa dòng này
    int valid;
    int booking_done = 0; // Cờ hiệu để kiểm tra toàn bộ quá trình nhập đã thành công chưa

    while (!booking_done) {
        do {
            d->date_in = get_safe_int(INDENT"Nhap ngay vao (nam trong khoang 1-31): ", 1, 31);
            d->date_out = get_safe_int(INDENT"Nhap ngay ra (nam trong khoang 1-31): ", 1, 31);

            if (get_days(d->date_in, d->date_out) <= 0) {
                printf(INDENT "Ngay ra phai sau ngay vao. Vui long nhap lai.\n");
            } else {
                break;
            }
        } while (1);
        
        while (1) {
            printf(INDENT "Nhap loai phong: ");
            read_line(d->room_type, sizeof(d->room_type));

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
                int choice = get_safe_int("Lua chon cua ban (1 hoac 2): ", 1, 2);

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

    do{ // Nhập mã hóa đơn, kiểm tra hóa đơn đã có trong danh sách hay chưa, có phải là số và đủ 8 ký tự hay chưa
        printf(INDENT "Nhap ma hoa don: ");
        read_line(d->invoice_code, sizeof(d->invoice_code));
        int invoice_len = strlen(d->invoice_code);
        if (invoice_len != 8) {
            printf(INDENT RED"Ma hoa don phai gom 8 ky tu. Vui long nhap lai.\n" RESET);
            continue;
        }

        int valid_invoice = 1;
        for (int i = 0; i < invoice_len; i++) {
            if (!isdigit((unsigned char)(d->invoice_code[i]))) {
                printf(INDENT RED "Ma hoa don phai gom 8 chu so. Vui long nhap lai.\n" RESET);
                valid_invoice = 0;
                break;
            }
        }
        if (!valid_invoice) {
            continue;
        }

        if (!check_invoiceCode_exist(L, d->invoice_code)) {
            printf(INDENT RED "Ma hoa don da ton tai. Vui long nhap lai.\n" RESET);
            continue;
        }else {
            break;
        }
    } while (1);
    
    do{ // Nhập tên khách hàng, không được để trống
        printf(INDENT "Nhap ten khach hang: ");
        read_line(d->customer_info.name, sizeof(d->customer_info.name));
        if (strlen(d->customer_info.name) == 0) {
            printf(INDENT RED "Ten khach hang khong duoc de trong. Vui long nhap lai.\n" RESET);
        } else {
            break;
        }
    } while (1);
   
     do{ // Nhập cccd; Kiểm tra cccd đã tồn tại, có phải là số và đủ 12 ký tự hay chưa 
        int i;
        int hop_le = 1;
        printf(INDENT "Nhap so CMT/CCCD: ");
        read_line(d->customer_info.identity_card,sizeof(d->customer_info.identity_card));
        int identity_length = strlen(d->customer_info.identity_card);
        if (identity_length != 12) {
            hop_le = 0;
        }
        for (i = 0;i < identity_length;i++) {
            if (!isdigit((unsigned char)(d->customer_info.identity_card[i]))) {
                hop_le = 0;
                break;
            }
        }
        if (!hop_le) {
            printf(INDENT RED "CCCD phai gom dung 12 chu so. Vui long nhap lai\n" RESET);
        }
        else if (!check_identityCard_exist(L, d->customer_info.identity_card)) {
            printf(INDENT RED "CCCD da ton tai. Vui long nhap lai\n" RESET);
        }
        else {
            break;
        }

    } while(1);

    do { // Cho phép người dùng chọn giới tính
        int choice;
        printf(INDENT "Chon gioi tinh:\n");
        printf(INDENT "1. Nam\n");
        printf(INDENT "2. Nu\n");
        choice = get_safe_int(INDENT"Nhap lua chon: ", 1, 2);
        if (choice == 1) {
            strcpy(d->customer_info.gender, "Nam");
            break;
        } else if (choice == 2) {
            strcpy(d->customer_info.gender, "Nu");
            break;
        }
    } while (1);
    
    //tinh tien phong
    char type = get_room(d->room_type);
    d->gia_phong = get_price(type);
    int days = get_days(d->date_in, d->date_out);
    d->tien_phong = d->gia_phong * days;
}

void read_file(List *L, int choice) { // Đọc file input.txt và thêm dữ liệu vào danh sách liên kết
    FILE *f = fopen(FILE_INPUT, "r");
    if (f == NULL) {
        printf(INDENT "Khong mo duoc file!\n");
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
            printf(INDENT RED "CANH BAO:"RESET BOLD_WHITE" Phong %s khong ton tai, bo qua hoa don nay!\n" RESET, d.room_type);
            continue;
        }
        if (!check_invoiceCode_exist(L, d.invoice_code)) {
            printf(INDENT RED "CANH BAO:"RESET BOLD_WHITE" Ma hoa don %s da ton tai. bo qua hoa don nay!\n" RESET, d.invoice_code);
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
    printf(INDENT GREEN "Da doc file %s thanh cong!\n" RESET, FILE_INPUT);
}

//ham tim kiem khach hanh theo so CMT
void searchCMT(List *L, char *CMT){
	if(check_empty(L)){
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET);
        return;
    }
    Node *p = L->head;
    char tien_phong_str[20];
    char tien_phong_temp[20];
    while(p != NULL){
    	if(strcmp(p->data.customer_info.identity_card, CMT) == 0){
    		char date_str[60]; 
            sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
            sprintf(tien_phong_str, "%d", p->data.tien_phong);
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
            return;
		}
		p = p->next;
	}
	printf(INDENT RED "Khong co khach hang co so CMT vua nhap!\n" RESET);
}
//ham tim kiem khach hang theo phong
void searchRoom(List *L, char *room, int choice){
	if(check_empty(L)){
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
        return;
    }
    if(!check_room_exist(L, room, choice)){
        printf(INDENT RED "Phong khong ton tai!\n" RESET);
        return;
    }
    Node *p = L->head;
    char tien_phong_str[20];
    char tien_phong_temp[20];
    printf(GREEN "\n                                              THONG TIN KHACH HANG DA DAT PHONG %s:\n" RESET, room);
    while(p != NULL){
    	if(strcmp(p->data.room_type, room) == 0){
            char date_str[60]; 
            sprintf(date_str, "%d -> %d", p->data.date_in, p->data.date_out);
            sprintf(tien_phong_str, "%d", p->data.tien_phong);
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
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
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
    if (found) printf(INDENT GREEN "Da xoa tat ca khach hang co ten Minh.\n" RESET);
    else printf(INDENT RED "Khong co khach hang ten Minh.\n" RESET);
}
// Hàm hỗ trợ tách danh sách liên kết thành hai nửa
static void split_list(Node *source, Node **frontRef, Node **backRef) {
    Node *slow;
    Node *fast;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

// Hàm hợp nhất hai danh sách đã được sắp xếp giảm dần theo date_in
static Node *merge_date_in_desc(Node *a, Node *b) {
    Node *result = NULL;

    if (a == NULL)
        return b;
    if (b == NULL)
        return a;

    if (a->data.date_in >= b->data.date_in) {
        result = a;
        result->next = merge_date_in_desc(a->next, b);
    } else {
        result = b;
        result->next = merge_date_in_desc(a, b->next);
    }
    return result;
}

// Hàm hợp nhất hai danh sách đã được sắp xếp giảm dần theo tien_phong
static Node *merge_price_desc(Node *a, Node *b) {
    Node *result = NULL;

    if (a == NULL)
        return b;
    if (b == NULL)
        return a;

    if (a->data.tien_phong >= b->data.tien_phong) {
        result = a;
        result->next = merge_price_desc(a->next, b);
    } else {
        result = b;
        result->next = merge_price_desc(a, b->next);
    }
    return result;
}

// Merge sort cho danh sách liên kết theo date_in giảm dần
static void merge_sort_by_date_in(Node **headRef) {
    Node *head = *headRef;
    if (head == NULL || head->next == NULL)
        return;

    Node *a;
    Node *b;

    split_list(head, &a, &b);
    merge_sort_by_date_in(&a);
    merge_sort_by_date_in(&b);

    *headRef = merge_date_in_desc(a, b);
}

// Merge sort cho danh sách liên kết theo tien_phong giảm dần
static void merge_sort_by_price(Node **headRef) {
    Node *head = *headRef;
    if (head == NULL || head->next == NULL)
        return;

    Node *a;
    Node *b;

    split_list(head, &a, &b);
    merge_sort_by_price(&a);
    merge_sort_by_price(&b);

    *headRef = merge_price_desc(a, b);
}

// Ham sắp xếp thông tin khách hàng theo ngày vào 
void sort_by_date_in(List *L) {
    if (check_empty(L)) {
        printf(INDENT YELLOW"DANH SACH RONG!\n" RESET);
        return;
    }

    merge_sort_by_date_in(&L->head);

    // Cập nhật con trỏ tail
    L->tail = L->head;
    while (L->tail != NULL && L->tail->next != NULL) {
        L->tail = L->tail->next;
    }
}
void print_sort_by_date(List *L) { // Hàm in thông tin đã sắp xếp theo ngày vào ra file data
    FILE *f = fopen(FILE_SORT_DATE, "w");

    if (f == NULL) {
        printf(INDENT RED "Khong mo duoc file!\n" RESET);
        return;
    }
    char tien_phong_str[20];
    char tien_phong_str_tmp[20];

    fprintf(f, "======================================================================\n");
    fprintf(f, "                  DANH SACH SAP XEP THEO NGAY VAO\n");
    fprintf(f, "======================================================================\n");

    fprintf(f, "%-5s %-25s %-10s %-15s %-15s\n",
            "STT", "TEN KHACH", "PHONG", "NGAY VAO", "TIEN PHONG");

    fprintf(f, "----------------------------------------------------------------------\n");

    Node *p = L->head;
    int stt = 1;

    while (p != NULL) {
        sprintf(tien_phong_str, "%d", p->data.tien_phong);
        add_dot(tien_phong_str,strlen(tien_phong_str),tien_phong_str_tmp);
        fprintf(f, "%-5d %-25s %-10s %-15d %s\n",
                stt,
                p->data.customer_info.name,
                p->data.room_type,
                p->data.date_in,
                tien_phong_str_tmp);

        p = p->next;
        stt++;
    }

    fprintf(f, "======================================================================\n");

    fclose(f);

    printf(INDENT GREEN "Da xuat file %s\n" RESET, FILE_SORT_DATE);
}
void sort_by_price(List *L) { // Ham sắp xếp thông tin khách hàng theo tiền phòng
    if (check_empty(L)) {
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
        return;
    }

    merge_sort_by_price(&L->head);

    // Cập nhật con trỏ tail
    L->tail = L->head;
    while (L->tail != NULL && L->tail->next != NULL) {
        L->tail = L->tail->next;
    }
}
void print_sort_by_price(List *L) { // Hàm in thông tin đã sắp xếp theo tiền phòng ra file data
    FILE *f = fopen(FILE_SORT_PRICE, "w");

    if (f == NULL) {
        printf(INDENT RED "Khong mo duoc file!\n" RESET);
        return;
    }
    char tien_phong_str[20];
    char tien_phong_str_tmp[20];

    fprintf(f, "======================================================================\n");
    fprintf(f, "           DANH SACH SAP XEP THEO TIEN PHONG\n");
    fprintf(f, "======================================================================\n");

    fprintf(f, "%-5s %-25s %-10s %-15s %-15s\n",
            "STT", "TEN KHACH", "PHONG", "NGAY VAO", "TIEN PHONG");

    fprintf(f, "----------------------------------------------------------------------\n");

    Node *p = L->head;
    int stt = 1;

    while (p != NULL) {
        sprintf(tien_phong_str, "%d", p->data.tien_phong);
        add_dot(tien_phong_str,strlen(tien_phong_str),tien_phong_str_tmp);
        fprintf(f, "%-5d %-25s %-10s %-15d %s\n",
                stt,
                p->data.customer_info.name,
                p->data.room_type,
                p->data.date_in,
                tien_phong_str_tmp);

        p = p->next;
        stt++;
    }

    fprintf(f, "======================================================================\n");

    fclose(f);

    printf(INDENT GREEN "Da xuat file %s\n" RESET, FILE_SORT_PRICE);
}
void delete_more_customer(List *L){ // Hàm xóa nhiều khách hàng theo vị trí nhập từ bàn phím
    if (check_empty(L)) {
        printf(INDENT YELLOW "Danh sach rong!\n" RESET);
        return;
    }
    Node *p = L->head;
    int n=1;
    while(n){
        export_list_table(L);
        int a = list_length(L);
        n = get_safe_int("Nhap vi tri khach hang can xoa (0 de dung): ", 0, a);
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
printf(BLUE "                                ╔══════════════════════════════════════════════════════════════════════════════╗\n");
    printf("                                ║                        " GREEN BOLD "PBL1 - ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN" RESET BLUE "                      ║\n");
    printf("                                ╠══════════════════════════════════════════════════════════════════════════════╣\n");
    printf("                                ║ " CYAN "Đề tài:" RESET " QUẢN LÝ THÔNG TIN KHÁCH SẠN TRONG 1 THÁNG                            " BLUE "║\n");
    printf("                                ╠══════════════════════════════════════╦═══════════════════════════════════════╣\n");
    printf("                                ║                " YELLOW "MÔ TẢ" RESET BLUE "                 ║         " YELLOW "SINH VIÊN THỰC HIỆN" RESET BLUE "           ║\n");
    printf("                                ╠══════════════════════════════════════╬═══════════════════════════════════════╣\n");

    printf("                                ║     Dùng Danh sách liên kết đơn để:  ║                  NHÓM 8               " BLUE "║\n");
    printf("                                ║ - Quản lý danh sách khách hàng       ║ 1. Nguyễn Việt Dũng                   " BLUE "║\n");
    printf("                                ║ - Quản lý đặt phòng/trả phòng        ║     Lớp: 25T_DT4     MSSV: 102250165  " BLUE "║\n");
    printf("                                ║ - Tính toán doanh thu hàng tháng     ║                                       " BLUE "║\n");
    printf("                                ║ - Đọc và ghi dữ liệu từ file.txt     ║ 2. Ngô Đăng Gia Bảo                   " BLUE "║\n");
    printf("                                ║ - Tìm kiếm & sắp xếp thông tin       ║     Lớp: 25T_DT4     MSSV: 102250158  " BLUE "║\n");

    printf("                                ╠══════════════════════════════════════╩═══════════════════════════════════════╣\n");
    printf("                                ║ " RED "GIẢNG VIÊN HƯỚNG DẪN: ĐỖ THỊ TUYẾT HOA" RESET BLUE "                                       ║\n");
    printf("                                ╚══════════════════════════════════════════════════════════════════════════════╝\n" RESET);

    printf("\n" BOLD_WHITE"                                                        Nhấn phím bất kỳ để tiếp tục..." RESET);
    printf("\n");
    getchar();
}

// **HÀM IN MENU**//
void menu_chinh() {
    printf(BLUE"\n                                  ╔════════════════════════════════════════════════╗\n");
    printf("                                  ║            " GREEN "QUAN LY KHACH SAN" RESET BLUE "                   ║\n");
    printf("                                  ╠════════════════════════════════════════════════╣\n");
    printf("                                  ║  " BOLD_WHITE "Chuc nang:" RESET BLUE "                                    ║\n");
    printf("                                  ║  " BOLD_WHITE "1. Xoa va them hoa don" RESET BLUE "                        ║\n");
    printf("                                  ║  " BOLD_WHITE "2. Tim kiem va tra cuu khach hang" RESET BLUE "             ║\n");
    printf("                                  ║  " BOLD_WHITE "3. Sap xep hoa don" RESET BLUE "                            ║\n");
    printf("                                  ║  " BOLD_WHITE "4. Thong ke va in du lieu" RESET BLUE "                     ║\n");
    printf("                                  ║  " BOLD_WHITE "5. Chuc nang de rieng" RESET BLUE "                         ║\n");
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
int main() {
    DrawTable();
    List L;
    init(&L); // LUÔN KHỞI TẠO NGAY ĐẦU TIÊN
    date data;
    int chinh, phu, choice;
    drawCustomDiagramMenu();
    choice = get_safe_int(" ", 1, 2);
    if(choice ==1){
        if (!read_hotel_layout("FILE_LAYOUT", &floor_count, room_counts, rooms)){
        printf(INDENT "Khong doc duoc file %s\n", FILE_LAYOUT);
        }
    }
    read_file(&L, choice); 

do {
    if (choice == 1) {
        export_custom_table(&L);
        printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
        getchar();
    }else{
        export_table(&L);
        printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
        getchar();
    }
    menu_chinh();
    chinh = get_safe_int(" ", 0, 5);

    switch(chinh) {

        case 1:
            do {
                menu_them_xoa();
                phu = get_safe_int(" ", 0, 6);

                switch(phu) {
                    case 1:
                        input_data(&L, &data, choice);
                        add_tail(&L, data);
                        printf(INDENT BOLD_WHITE "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 2:
                        while(1){
                            export_list_table(&L);
                            int b = list_length(&L);
                            printf( BOLD_WHITE "Nhap vi tri can them (0 de quay lai, %d de them vao cuoi danh sach): " RESET, b+1);
                            int k = get_safe_int(BOLD_WHITE "" RESET, 0, b+1);
                            if(k == 0) break;
                            input_data(&L, &data, choice);
                            add_k_place(&L, data, k);
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
                    default:
                        printf(INDENT "Lua chon khong hop le, vui long chon lai!\n");
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
                    case 0:
                        break;
                    }
                    default:
                        printf(INDENT "Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 3:
            do {
                menu_sapxep();
                phu = get_safe_int(" ", 0, 2);

                switch(phu) {
                    case 1:
                        sort_by_date_in(&L);
                        print_sort_by_date(&L);
                        printf(INDENT "Da sap xep theo ngay vao.\n");
                        printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 2:
                        sort_by_price(&L);
                        print_sort_by_price(&L);
                        printf(INDENT "Da sap xep theo tien phong.\n");
                        printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 0:
                        break;
                    default:
                        printf(INDENT "Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 4:
            do {
                menu_thongke();
                phu = get_safe_int(" ", 0, 3);

                switch(phu) {
                    case 1:
                        print_file(&L);
                        printf(INDENT BOLD_WHITE"Da in tat ca hoa don ra file %s\n"RESET, FILE_DATA_OUT);
                        printf(BOLD_WHITE INDENT "An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;

                    case 2:
                        thongke(&L);
                        printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 3:    
                        int ngay = get_safe_int(INDENT"Nhap ngay can thong ke (1-31): ", 1, 31);
                        thongke_theo_ngay(&L, ngay);
                        printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 0:
                    default:
                        printf(INDENT RED"Lua chon khong hop le, vui long chon lai!\n"RESET);
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
                        read_file(&L, choice);
                        printf(BOLD_WHITE INDENT"An phim bat ky de tiep tuc..." RESET);
                        getchar();
                        break;
                    case 0:
                        break;
                    default:
                        printf(INDENT"Lua chon khong hop le, vui long chon lai!\n");
                }

            } while(phu != 0);
            break;

        case 0:
            clear_list(&L); // Giải phóng bộ nhớ trước khi thoát
            printf(INDENT YELLOW"THOÁT CHUƠNG TRÌNH, CẢM ƠN CÔ ĐÃ LẮNG NGHE\n" RESET);
            break;

        default:
            printf(INDENT RED"Lua chon khong hop le!\n"RESET);
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
    kiểm tra phần add_more_customer xem có lỗi gì không
    bug nhập 13 số mới chạy
*/
// tạo thư viện riêng mystring.h để xử lý xâu (tách họ tên,...)
// Đối với yêu cầu sắp xếp, tự động in hóa đơn ra file sau khi sắp xếp để dễ kiểm tra kết quả    DONE

