#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include "Log.h"


//��fail_cond�����������򷵻�ret_value
#define CHK_FAIL_RETURN(fail_cond, ret_value) \
    if ((fail_cond)) return (ret_value);

//��fail_cond�����������򷵻�ret_value��ͬʱ����msg_fmt��ʽ���������Ϣ
#define CHK_FAIL_MSG_RETURN(fail_cond, ret_value, msg_fmt, ...) \
    if ((fail_cond)) { \
        LOG_ERROR((msg_fmt), __VA_ARGS__); \
        return (ret_value); \
    }


#endif