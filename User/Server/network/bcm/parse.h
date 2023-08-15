/*
 * $Id$
 * $Copyright: Copyright 2016 Broadcom Limited.
 * This program is the proprietary software of Broadcom Limited
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * Defines and types relating to parsing command lines.
 */


#ifndef _DIAG_ARGS_H
#define _DIAG_ARGS_H


#define ARGS_CNT        10            /* Max argv entries */
#define ARGS_BUFFER     1024          /* # bytes total for arguments */

/*
 * Define:      PARSE_VARIABLE_PREFIX1/PREFIX2/SYSTEM
 * Purpose:     Defines the character that indicate a variable reference,
 *              and the character the indicates a system variable.
 */
#define PARSE_VARIABLE_PREFIX1  '@'     /* Deprecated */
#define PARSE_VARIABLE_PREFIX2  '$'
#define PARSE_VARIABLE_SYSTEM   '_'

/*
 * Typedef:     parse_key_t
 * Purpose:     Type used as FIRST entry in structures passed to
 *              parse_lookup.
 */
typedef char *parse_key_t;

/*
 * Define:      PARSE_ENTRIES
 * Purpose:     Determine the number of elements in a parse table.
 */
#define PARSE_ENTRIES(_p)       COUNTOF(_p)

/*
 * Typedef:     args_t
 * Purpose:     argc/argv (sorta) structure for parsing arguments.
 *
 * If the macro ARG_GET is used to consume parameters, unsued parameters
 * may be passed to lower levels by simply passing the args_t struct.
 */
typedef struct args_s {
    parse_key_t a_cmd;                  /* Initial string */
    char        *a_argv[ARGS_CNT];      /* argv pointers */
    char        a_buffer[ARGS_BUFFER];  /* Split up buffer */
    int         a_argc;                 /* Parsed arg counter */
    int         a_arg;                  /* Pointer to NEXT arg */
} args_t;

#define ARG_CMD(_a)     (_a)->a_cmd

#define _ARG_CUR(_a)     \
    ((_a)->a_argv[(_a)->a_arg])

#define ARG_CUR(_a)     \
    (((_a)->a_arg >= (_a)->a_argc) ? NULL : _ARG_CUR(_a))

#define _ARG_GET(_a)     \
    ((_a)->a_argv[(_a)->a_arg++])

#define ARG_GET(_a)     \
    (((_a)->a_arg >= (_a)->a_argc) ? NULL : _ARG_GET(_a))

#define ARG_NEXT(_a)    (_a)->a_arg++
#define ARG_PREV(_a)    (_a)->a_arg--

#define ARG_DISCARD(_a) \
    ((_a)->a_arg = (_a)->a_argc)

/*
 * Macro:       ARG_CNT
 * Purpose:     Return the number of unconsumed arguments.
 */
#define ARG_CNT(_a)     ((_a)->a_argc - (_a)->a_arg)


/* parse.c exported routines */

extern  char    *(*parse_user_var_get)(char *varname);

extern  void    parse_args_copy(args_t *dst, args_t *src);
extern  int     parse_cmp(const char *p, const char *s, const char term);
extern  int     diag_parse_args(const char *s, char **s_ret, args_t *);
extern  const void *parse_lookup(const char *, const void *tbl, int size,
                                 int cnt);
/* variable.c exported routines */

extern  void    *var_push_scope(void);
extern  void    var_pop_scope(void *);
extern  char    *var_get(const char *);
extern  int     var_set(const char *v_name, char *v_value, int local,
                        int system);
extern  int     var_set_integer(const char *v_name, int v_value, int local,
                                int system);
extern  int     var_unset(const char *v_name, int local, int global,
                          int system);
#define SCAN_DEPTH      10

typedef struct scan_s {
    const char          *scan_stack[SCAN_DEPTH];
    const char          *scan_ptr[SCAN_DEPTH];
    int                 scan_tos;
} scan_t;

#endif /* _DIAG_ARGS_H */
