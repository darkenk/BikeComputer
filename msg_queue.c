#include "msg_queue.h"

list pending_msg_list = {
    &pending_msg_list, &pending_msg_list
};
