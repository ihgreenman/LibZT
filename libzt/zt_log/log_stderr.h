/*
 * stderr.h        Zerotao logging to stderr
 *
 * Copyright (C) 2000-2002, 2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: stderr.h,v 1.2 2003/06/09 13:42:12 jshiffer Exp $
 *
 */

#ifndef _LOG_STDERR_H_
#define _LOG_STDERR_H_

#include <libzt/zt_log.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

extern log_ty *
log_stderr ( unsigned int );

#ifdef __cplusplus
}
#endif
#endif  /* _LOG_STDERR_H_ */
