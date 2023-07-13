#ifndef __PLT_REF_H__
#define __PLT_REF_H__

#include "atomic.h"

struct ref {
    atomic_t refcnt;
};

static inline void init_ref(struct ref *ref)
{
	atomic_set(&ref->refcnt, 1);
}

static inline void get_ref(struct ref *ref)
{
	Atomic_Increment_u32(&ref->refcnt);
}

static inline
err_t put_ref(struct ref *ref, void (*release) (struct ref *ref))
{
	Atomic_Decrement_u32(&ref->refcnt);
	if (ref->refcnt == 0) {
		release(ref);
		return 1;
	}
	return 0;
}

static inline unsigned int ref_read(const struct ref *ref)
{
	return ref->refcnt;
}

#endif /* __PLT_REF_H__ */
