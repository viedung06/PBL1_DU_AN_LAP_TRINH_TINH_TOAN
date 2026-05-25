#ifndef UI_H
#define UI_H

#include "linked_list.h"

// ĐỊNH NGHĨA MÀU SẮC VÀ CĂN LỀ
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define CYAN    "\033[1;36m"
#define WHITE_B "\033[1;37m"
#define BOLD_WHITE "\033[1;37m"
#define INDENT "                                  "

void DrawTable();
void menu_chinh();
void menu_them_xoa();
void menu_timkiem();
void menu_sapxep();
void menu_thongke();
void menu_rieng();
void drawCustomDiagramMenu();

void export_list_table(List *L);
void export_custom_table(List *L);
void export_table(List *L);

#endif