#ifndef LIST_H
#define LIST_H

typedef volatile struct _list {
    volatile struct _list *next, *prev;
} list;

static inline void
list_init(list *list)
{
    list->next = list;
    list->prev = list;
}

static inline int
list_empty(list *list)
{
    return list->next == list;
}

static inline void
list_insert(list *link, list *new_link)
{
    new_link->prev		= link->prev;
    new_link->next		= link;
    new_link->prev->next	= new_link;
    new_link->next->prev	= new_link;
}

static inline void
list_append(list *list1, list *new_link)
{
    list_insert((list *)list1, new_link);
}

static inline void
list_prepend(list *list1, list *new_link)
{
    list_insert(list1->next, new_link);
}

static inline void
list_remove(list *link)
{
    link->prev->next = link->next;
    link->next->prev = link->prev;
}

#define list_entry(link, type, member) \
    ((type *)((char *)(link)-(unsigned int)(&((type *)0)->member)))

#define list_head(list, type, member) \
    list_entry((list)->next, type, member)

#define list_tail(list, type, member) \
    list_entry((list)->prev, type, member)

#define list_next(elm, member) \
    list_entry((elm)->member.next, typeof(*elm), member)

#define list_for_each_entry(pos, list, member) \
    for (pos = list_head(list, typeof(*pos), member); \
    &pos->member != (list); \
    pos = list_next(pos, member))

#endif // LIST_H
