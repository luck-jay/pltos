#ifndef __PLT_ASSERT_H__
#define __PLT_ASSERT_H__

#undef assert

#ifdef USE_ASSERT
	extern void assert_failed(char *file_name, int line);
	#define assert(expr)   ((expr) ? (void)0 : assert_failed(__FILE__, __LINE__))
#else
	#define assert(expr)   (void)0
#endif

#endif /* __PLT_ASSERT_H__ */
