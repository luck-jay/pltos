#ifndef __PLT_TIMER_H__
#define __PLT_TIMER_H__

#include <plt/types.h>
#include <plt/list.h>

struct timer_list {
    struct list list;
    ubase_t expires;

    void (*func)(ubase_t);
    ubase_t data;
};

void check_timer_timeout(void);
void add_timer_on(struct timer_list *timer);

#endif /* __PLT_TIMER_H__ */
