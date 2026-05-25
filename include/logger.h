#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include "ui.h"

// Macro dùng để in log gỡ lỗi (có thể gọi ở bất cứ đâu)
#define LOG_ERR(msg) printf(INDENT RED "[ERROR] %s\n" RESET, msg)
#define LOG_INFO(msg) printf(INDENT BLUE "[INFO] %s\n" RESET, msg)

void init_logger();

#endif