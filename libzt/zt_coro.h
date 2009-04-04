#ifndef _zt_coroutine_h_
#define _zt_coroutine_h_

#include <libzt/zt.h>
#include <libzt/zt_except.h>
#include <libzt/zt_assert.h>


BEGIN_C_DECLS

#define USE_UCONTEXT

#if defined(USE_UCONTEXT)
#include <libzt/ucontext/portableucontext.h>
# define zt_context_t	ucontext_t
#else
#include <setjmp.h>
# define zt_context_t	jmp_buf
#endif

typedef struct zt_coro_ctx zt_coro_ctx;

typedef struct zt_coro {
	struct zt_coro		* caller;
	struct zt_coro		* target;
	void				*(* func)(zt_coro_ctx *, void *);	
	size_t                size;
	void				* data;
	struct except_Frame	* except_stack;
	zt_context_t		  ctx;
} zt_coro;

struct zt_coro_ctx {
	zt_coro				  main;
	zt_coro				* current;
	zt_coro				* helper;
};

#define ZT_CORO_MIN_STACK_SIZE (32 * 1024) /* 32k stack */
/* #define ZT_CORO_MIN_STACK_SIZE ((128 * 1024)) */

extern char     * zt_coro_except_exit;

int zt_coro_init_ctx(zt_coro_ctx *ctx);
zt_coro *zt_coro_create(zt_coro_ctx *ctx, void *(*func)(zt_coro_ctx *, void *), zt_coro *co, size_t size);
void zt_coro_delete(zt_coro_ctx *ctx, zt_coro *co);
void *zt_coro_call(zt_coro_ctx *ctx, zt_coro *co, void *data);
void *zt_coro_yield(zt_coro_ctx *ctx, void *data);
void zt_coro_exit_to(zt_coro_ctx *ctx, zt_coro *co, void *data) NORETURN;
void zt_coro_exit(zt_coro_ctx *ctx, void * data) NORETURN;
zt_coro *zt_coro_get_current(zt_coro_ctx *ctx);
int zt_coro_stack_left(zt_coro_ctx *ctx);

END_C_DECLS
#endif /* _zt_coroutine_h_ */
