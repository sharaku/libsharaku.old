/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2015 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _SHARAKU_DEBUG_H
#define _SHARAKU_DEBUG_H

#include <stdint.h>
#include <sharaku/list.h>
#include <sharaku/thread.h>

#ifdef DEBUG_CODE_ENABLE
#define debug_code(x) x
#else
#define debug_code(x)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// ログレベル
enum {
	SHARAKU_LOG_LV_SYSTEM,
	SHARAKU_LOG_LV_TRACE,
	SHARAKU_LOG_LV_INFO,
	SHARAKU_LOG_LV_ERROR,
};
#ifndef SHARAKU_DBLOG_LEVEL 
#define SHARAKU_DBLOG_LEVEL SHARAKU_LOG_LV_INFO
#endif

// 2のべき乗であること
#ifndef SHARAKU_DBLOG_COUNT
#define SHARAKU_DBLOG_COUNT 0x8000
#endif

#ifndef SHARAKU_DBLOG_FILENAME
#define SHARAKU_DBLOG_FILENAME "./sharaku_db_log.log"
#endif

#ifndef SHARAKU_DATALOGGER_MAXCOUNT
#define SHARAKU_DATALOGGER_MAXCOUNT 0x4000
#endif
#ifndef SHARAKU_DATALOGGER_FILENAME
#define SHARAKU_DATALOGGER_FILENAME "./sharaku_dev_logger.csv"
#endif

#define SHARAKU_LOG_NUMtoSTR(x) _SHARAKU_LOG_NUMtoSTR(x)
#define _SHARAKU_LOG_NUMtoSTR(x) #x

// ログ採取
#define sharaku_db_trace(str, a1, a2, a3, a4, a5, a6) \
	sharaku_dblog_internal(SHARAKU_LOG_LV_TRACE, str, a1, a2, a3, a4, a5, a6)
#define sharaku_db_info(str, a1, a2, a3, a4, a5, a6) \
	sharaku_dblog_internal(SHARAKU_LOG_LV_INFO, str, a1, a2, a3, a4, a5, a6)
#define sharaku_db_error(str, a1, a2, a3, a4, a5, a6) \
	sharaku_dblog_internal(SHARAKU_LOG_LV_ERROR, str, a1, a2, a3, a4, a5, a6)

/******************************************************************************
logger機能
******************************************************************************/
struct sharaku_logger_handle;
typedef void (*sharaku_logger_convert)(char *buf, size_t bufsz, void *recode);

// logger createパラメータ
struct sharaku_logger_create {
	size_t			recode_sz;
	uint32_t		count;
	uint32_t		log_rotation:1;
	sharaku_logger_convert	cb_conv;
};

// loggerハンドル
struct sharaku_logger_handle {
	size_t			recode_sz;	// レコードサイズ
	uint32_t		count;		// レコード数
	uint32_t		current;	// カレント位置
	uint32_t		log_rotation:1;	// ログをローテーションさせる
	uint32_t		is_overlap:1;	// ログオーバーライト状態
	sharaku_mutex_t		mx;		// 排他用mutex
	sharaku_logger_convert	cb_conv;	// log変換コールバック
	char			buffer[0];	// ログバッファ
};

// loggerをcreateする
extern struct sharaku_logger_handle* sharaku_logger_init(struct sharaku_logger_create *cre_param);

// 情報を登録する
extern void sharaku_logger_add(struct sharaku_logger_handle *hdl, void *recode);

// 情報をファイルへflushする
extern void sharaku_logger_flush_to_file(struct sharaku_logger_handle *hdl, const char *filename, const char *mode);

// loggerをreleaseする
extern void sharaku_logger_release(struct sharaku_logger_handle *hdl);

/******************************************************************************
trace log機能
******************************************************************************/
// size 16byte
struct sharaku_db_log_recode
{
	const char*	format;
	const char*	function;
	int32_t		arg[6];
};

extern void sharaku_init_dblog(void);
extern void sharaku_finl_dblog(void);
extern void _sharaku_dblog_internal(const char* format, const char* function, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);

// ログ採取マクロ(直接使用禁止)

#if defined(__GNUC__)
// gccコンパイラ
#define sharaku_dblog_internal(lv, str, a1, a2, a3, a4, a5, a6)	\
	if (SHARAKU_DBLOG_LEVEL <= lv)		\
		_sharaku_dblog_internal(__FILE__"(" SHARAKU_LOG_NUMtoSTR(__LINE__) ")  %s(): "str"\n", __PRETTY_FUNCTION__, a1, a2, a3, a4, a5, a6)
#else
// その他コンパイラ
#define sharaku_dblog_internal(lv, str, a1, a2, a3, a4, a5, a6)	\
	if (SHARAKU_DBLOG_LEVEL <= lv)		\
		_sharaku_dblog_internal(__FILE__"(" SHARAKU_LOG_NUMtoSTR(__LINE__) ")  %s(): "str"\n", __func__, a1, a2, a3, a4, a5, a6)
#endif

/******************************************************************************
data logger機能
******************************************************************************/
#define SHARAKU_DATALOGGER_TYPE_INT32	1
#define SHARAKU_DATALOGGER_TYPE_FLOAT	2
#define SHARAKU_DATALOGGER_TYPE_STR4	3
typedef int32_t	sharaku_datalogger_handle;

extern void sharaku_datalogger_initialize(void);
extern sharaku_datalogger_handle sharaku_datalogger_create(int32_t t, char *title);
extern void sharaku_datalogger_set_int32(sharaku_datalogger_handle handle, int32_t val);
extern void sharaku_datalogger_set_float(sharaku_datalogger_handle handle, float val);
extern void sharaku_datalogger_set_string(sharaku_datalogger_handle handle, char *val);
extern void sharaku_datalogger_commit(void);
extern void sharaku_datalogger_flush(void);

#if defined(__cplusplus)
}
#endif

#endif /* _SHARAKU_LOG_H_ */
