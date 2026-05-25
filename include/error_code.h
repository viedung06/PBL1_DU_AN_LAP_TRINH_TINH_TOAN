#ifndef ERROR_CODE_H
#define ERROR_CODE_H

// Định nghĩa mã lỗi dùng để mở rộng debug sau này
typedef enum {
    ERR_SUCCESS = 0,
    ERR_NULL_PTR = 1,
    ERR_MEM_ALLOC = 2,
    ERR_FILE_NOT_FOUND = 3,
    ERR_INVALID_DATA = 4
} ErrorCode;

#endif