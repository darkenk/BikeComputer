#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "list.h"
#include <stdlib.h>
#include <util/atomic.h>

extern list pending_msg_list;

typedef struct _msg_t {
    /**
     * @brief type message type
     */
    int type;

    /**
     * @brief size in bytes of data
     */
    int size;

    /**
     * @brief data additional data
     */
    char* data;

    /**
     * @brief list internal purpose only
     */
    list link;
} msg_t;

/**
 * @brief append_msg append new message to message queue
 *
 * @param type
 * @param size - size in bytes of data
 * @param data - must be created using malloc
 */
static inline void append_msg(int type, int size, char* data) {
    msg_t* msg = (msg_t*)malloc(sizeof(msg_t));
    msg->type = type;
    msg->size = size;
    msg->data = data;
    list_append(&pending_msg_list, &msg->link);
}

/**
 * @brief get_msg gets message from queue
 * @return
 */
static inline msg_t* get_msg() {
    msg_t* ret = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (!list_empty(&pending_msg_list)) {
            ret = list_head(&pending_msg_list, msg_t, link);
            list_remove(&ret->link);
        }
    }
    return ret;
}

/**
 * @brief free_msg free allocated resources for message
 * @param msg
 */
static inline void free_msg(msg_t** msg) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if ((*msg)->data) {
            free((*msg)->data);
        }
        free(*msg);
    }
    *msg = 0;
}

#define MSG_SECOND_CHANGE   0x1
#define MSG_MILISEC_CHANGE  0x2
#define MSG_INPUT           0x3

#endif // MSG_QUEUE_H
