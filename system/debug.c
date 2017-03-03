/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2017 Abe Takafumi
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/thread.h>
#include <sharaku/utime.h>

static struct sharaku_logger_handle	*_dblog_handle = NULL;

// ロガー機能
//   一定長のレコードを連続記録する。
//   ロガー機能を利用して、デバッグログ機能、データロガー機能を実現する。
struct sharaku_logger_handle*
sharaku_logger_init(struct sharaku_logger_create *cre_param)
{
	struct sharaku_logger_handle* hdl;
	hdl = (struct sharaku_logger_handle*)
			malloc(sizeof(struct sharaku_logger_handle)
				 + cre_param->recode_sz * cre_param->count);
	if (!hdl) {
		return NULL;
	}

	hdl->recode_sz		= cre_param->recode_sz;
	hdl->count		= cre_param->count;
	hdl->current		= 0;
	hdl->log_rotation	= cre_param->log_rotation;
	hdl->is_overlap		= 0;
	sharaku_mutex_init(&hdl->mx);
	hdl->cb_conv		= cre_param->cb_conv;

	return hdl;
}

void
sharaku_logger_add(struct sharaku_logger_handle *hdl, void *recode)
{
	uint32_t current = 0;

	if (!hdl) {
		return;
	}

	// ログローテーションしない場合、最後まで書ききったら以降書き込まない
	if (!hdl->log_rotation) {
		if (hdl->current >= hdl->count) {
			goto out;
		}
	}

	// 領域を確保する。同時実行を防ぐために排他を行う。
	// 
	sharaku_mutex_lock(&hdl->mx);
	current = hdl->current;
	hdl->current ++;
	if (hdl->log_rotation && hdl->current >= hdl->count) {
		hdl->current	= 0;
		hdl->is_overlap	= 1;
	}
	sharaku_mutex_unlock(&hdl->mx);

	// レコードをコピーする
	char	*buffer_current = hdl->buffer + current * hdl->recode_sz;
	memcpy(buffer_current, recode, hdl->recode_sz);

 out:
	return;
}

void
sharaku_logger_flush_to_file(struct sharaku_logger_handle *hdl,
				const char *filename, const char *mode)
{
	static char buffer[256];
	int i;
	FILE *fp;
	char	*buffer_current;

	if (!hdl) {
		return;
	}

	buffer_current = hdl->buffer;

	fp = fopen(filename, mode);

	// log overlapが発生している場合はcurrentよりも後ろを出力する
	if (hdl->is_overlap) {
		for (i = hdl->current; i < hdl->count; i++) {
			hdl->cb_conv(buffer, 256, buffer_current
					 + i * hdl->recode_sz);
			buffer[255] = '\0';
			fprintf(fp, "%s", buffer);
		}
	}
	// 0～currentまでを出力する
	for (i = 0; i < hdl->current; i++) {
		hdl->cb_conv(buffer, 256, buffer_current
				+ i * hdl->recode_sz);
		buffer[255] = '\0';
		fprintf(fp, "%s", buffer);
	}

	fclose(fp);
}

void
sharaku_logger_release(struct sharaku_logger_handle *hdl)
{
	if (!hdl) {
		return;
	}
	free(hdl);
}

// デバッグログ機能
//   トレースログをメモリ上に格納する。
//   出力の際に文字列のログに変換してファイルへ出力する。
static void _sharaku_dblog_convert(char *buf, size_t bufsz, void *recode)
{
	struct sharaku_db_log_recode	*log
				 = (struct sharaku_db_log_recode*)recode;

	snprintf(buf, bufsz, log->format, log->function,
			log->arg[0], log->arg[1], log->arg[2],
			log->arg[3], log->arg[4], log->arg[5]);
}

void sharaku_init_dblog(void)
{
	struct sharaku_logger_create	cre_param;
	cre_param.recode_sz	= sizeof(struct sharaku_db_log_recode);
	cre_param.count		= SHARAKU_DBLOG_COUNT;
	cre_param.log_rotation	= 1;
	cre_param.cb_conv	= _sharaku_dblog_convert;
	_dblog_handle = sharaku_logger_init(&cre_param);
}

void sharaku_finl_dblog(void)
{
	sharaku_logger_flush_to_file(_dblog_handle,
					SHARAKU_DBLOG_FILENAME, "w+");
}

void sharaku_dblog_finalize(void)
{
	sharaku_logger_flush_to_file(_dblog_handle,
					SHARAKU_DBLOG_FILENAME, "w+");
	sharaku_logger_release(_dblog_handle);
}

void _sharaku_dblog_internal(const char* format, const char* function,
				int32_t arg1, int32_t arg2, int32_t arg3,
				int32_t arg4, int32_t arg5, int32_t arg6)
{
	struct sharaku_db_log_recode	log;
	log.format	= format;
	log.function	= function;
	log.arg[0]	= arg1;
	log.arg[1]	= arg2;
	log.arg[2]	= arg3;
	log.arg[3]	= arg4;
	log.arg[4]	= arg5;
	log.arg[5]	= arg6;
	if (_dblog_handle) {
		sharaku_logger_add(_dblog_handle, &log);
	}
}

// データロガー機能
//   登録されたデータレコードを記録していく
union sharaku_datalogger_recode {
	int32_t	i;
	float	f;
	char	c[4];
};
struct sharaku_datalogger_recode_header {
	int32_t	type;
	char	title[252];
};
struct sharaku_datalogger_context {
	int32_t					index;
	int32_t					max_counts;
	union sharaku_datalogger_recode		*recode;
	struct sharaku_datalogger_recode_header	*recode_header;
	struct sharaku_logger_handle		*handle;
};

static struct sharaku_datalogger_context _logger = {0};

void
sharaku_datalogger_convert(char *buf, size_t bufsz, void *recode)
{
	union sharaku_datalogger_recode *rcd
				 = (union sharaku_datalogger_recode*)recode;
	uint32_t		sz;
	int			i;

	sz = 0;
	for (i = 0; i < _logger.index && sz < bufsz; i++) {
		switch (_logger.recode_header[i].type) {
		case SHARAKU_DATALOGGER_TYPE_INT32:
			sz += snprintf(buf + sz, bufsz - sz,
					"%d,", (int)rcd[i].i);
			break;
		case SHARAKU_DATALOGGER_TYPE_FLOAT:
			sz += snprintf(buf + sz, bufsz - sz,
					"%f,", (float)rcd[i].f);
			break;
		case SHARAKU_DATALOGGER_TYPE_STR4:
			sz += snprintf(buf + sz, bufsz - sz, "%c%c%c%c,",
					 rcd[i].c[0], rcd[i].c[1],
					 rcd[i].c[2], rcd[i].c[3]);
			break;
		default:
			sz += snprintf(buf, bufsz - sz, "error,");
			break;
		}
	}
	snprintf(buf + sz, bufsz - sz, "\n");
}

void
sharaku_datalogger_initialize(void)
{
	_logger.max_counts 	= SHARAKU_DATALOGGER_MAXCOUNT;
}

sharaku_datalogger_handle
sharaku_datalogger_create(int32_t t, char *title)
{
	int32_t	handle = _logger.index;

	_logger.index ++;
	_logger.recode_header = realloc(_logger.recode_header,
				sizeof(struct sharaku_datalogger_recode_header)
						 * _logger.index);
	_logger.recode = realloc(_logger.recode,
				sizeof(union sharaku_datalogger_recode)
						 * _logger.index);

	// 登録した情報を更新する
	memset(_logger.recode, 0,
		sizeof(union sharaku_datalogger_recode) * _logger.index);
	_logger.recode_header[handle].type	= t;
	strncpy(_logger.recode_header[handle].title,
		 title, sizeof(_logger.recode_header[handle].title));

	// 初期化後であればログ領域を拡張する
	// 前の領域をいったん破棄して新規に作成する。
	if (_logger.max_counts != 0) {
		struct sharaku_logger_create	create;

		_logger.max_counts 	= SHARAKU_DATALOGGER_MAXCOUNT;
		create.recode_sz	= sizeof(union sharaku_datalogger_recode) * _logger.index;
		create.count		= _logger.max_counts;
		create.log_rotation	= 0;
		create.cb_conv		= sharaku_datalogger_convert;

		sharaku_logger_release(_logger.handle);
		_logger.handle = sharaku_logger_init(&create);
	}


	return handle;
}

void
sharaku_datalogger_set_int32(sharaku_datalogger_handle handle, int32_t val)
{
	if (handle >= _logger.index) {
		return;
	}
	if (_logger.recode_header[handle].type == SHARAKU_DATALOGGER_TYPE_INT32) {
		_logger.recode[handle].i = val;
	}
}

void
sharaku_datalogger_set_float(sharaku_datalogger_handle handle, float val)
{
	if (handle >= _logger.index) {
		return;
	}
	if (_logger.recode_header[handle].type == SHARAKU_DATALOGGER_TYPE_FLOAT) {
		_logger.recode[handle].f = val;
	}
}

void
sharaku_datalogger_set_string(sharaku_datalogger_handle handle, char *val)
{
	int i;

	if (handle >= _logger.index) {
		return;
	}
	if (_logger.recode_header[handle].type == SHARAKU_DATALOGGER_TYPE_STR4) {
		for (i = 0; i < 4; i++) {
			if (*val != '\0') {
				_logger.recode[handle].c[i] = *val;
				val++;
			} else {
				_logger.recode[handle].c[i] = '\0';
			}
		}
	}
}

void
sharaku_datalogger_commit(void)
{
	if (_logger.recode && _logger.handle) {
		sharaku_logger_add(_logger.handle, (void*)_logger.recode);
		memset(_logger.recode, 0,
			sizeof(union sharaku_datalogger_recode) * _logger.index);
	}
}

void
sharaku_datalogger_flush(void)
{
	int32_t i;
	FILE *fp;
	fp = fopen(SHARAKU_DATALOGGER_FILENAME, "w+");
	for (i = 0; i < _logger.index; i++) {
		fprintf(fp, "%s,", _logger.recode_header[i].title);
	}
	fprintf(fp, "\n");
	fclose(fp);
	sharaku_logger_flush_to_file(_logger.handle, SHARAKU_DATALOGGER_FILENAME, "a+");
}

