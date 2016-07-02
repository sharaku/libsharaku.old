/*****************************************************************************/
/*!
@author	abe takafumi
@par	Copyright
2014 abe takafumi All Rights Reserved.
@file	prof.c
@brie	統計情報収集機能
	対応プラットフォーム：Windows (C++), Linux (C/C++) ...
******************************************************************************/

/******************************************************************************
include section
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sharaku/log.h>
#include <sharaku/thread.h>
#include <sharaku/utime.h>

/******************************************************************************
struct section
******************************************************************************/

/******************************************************************************
extern values section
******************************************************************************/

/******************************************************************************
static values section
******************************************************************************/
static struct sharaku_logger_handle	*_dblog_handle = NULL;

/******************************************************************************
function (logger)
******************************************************************************/
struct sharaku_logger_handle*
sharaku_logger_init(struct sharaku_logger_create *cre_param)
{
	struct sharaku_logger_handle* hdl;
	hdl = (struct sharaku_logger_handle*)
			malloc(sizeof(struct sharaku_logger_handle)
				 + cre_param->recode_sz * cre_param->count);

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
						const char *filename)
{
	static char buffer[256];
	int i;
	FILE *fp;
	char	*buffer_current = hdl->buffer;

	fp = fopen(filename, "w+");

	// log overlapが発生している場合はcurrentよりも後ろを出力する
	if (hdl->is_overlap) {
		for (i = hdl->current; i < hdl->count; i++) {
			hdl->cb_conv(buffer, 256, buffer_current + i * hdl->recode_sz);
			buffer[255] = '\0';
			fprintf(fp, "%s", buffer);
		}
	}
	// 0～currentまでを出力する
	for (i = 0; i < hdl->current; i++) {
		hdl->cb_conv(buffer, 256, buffer_current + i * hdl->recode_sz);
		buffer[255] = '\0';
		fprintf(fp, "%s", buffer);
	}

	fclose(fp);
}

void
sharaku_logger_release(struct sharaku_logger_handle *hdl)
{
	free(hdl);
}

/******************************************************************************
function (dblog)
******************************************************************************/
static void _sharaku_dblog_convert(char *buf, size_t bufsz, void *recode)
{
	struct sharaku_db_log_recode	*log
				 = (struct sharaku_db_log_recode*)recode;

	snprintf(buf, bufsz, log->format, log->function,
			log->arg[0], log->arg[1], log->arg[2],
			log->arg[3], log->arg[4], log->arg[5]);
}

void sharaku_dblog_initialize(void)
{
	struct sharaku_logger_create	cre_param;
	cre_param.recode_sz	= sizeof(struct sharaku_db_log_recode);
	cre_param.count		= SHARAKU_DBLOG_COUNT;
	cre_param.log_rotation	= 1;
	cre_param.cb_conv	= _sharaku_dblog_convert;
	_dblog_handle = sharaku_logger_init(&cre_param);
}

void sharaku_dblog_flush(void)
{
	sharaku_logger_flush_to_file(_dblog_handle, SHARAKU_DBLOG_FILENAME);
}

void sharaku_dblog_finalize(void)
{
	sharaku_logger_flush_to_file(_dblog_handle, SHARAKU_DBLOG_FILENAME);
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

/******************************************************************************
function (data logger)
******************************************************************************/
struct sharaku_datalogger_recode {
	int32_t	i;
	float	f;
	char	c[4];
};
struct sharaku_datalogger_recode_header {
	int32_t	type;
	char	title[1020];
};
struct sharaku_datalogger_context
{
	int32_t					index;
	int32_t					max_counts;
	struct sharaku_datalogger_recode	*recode;
	struct sharaku_datalogger_recode_header	*recode_header;
	struct sharaku_logger_handle		*handle;
};

static struct sharaku_datalogger_context _logger = {0};

void
sharaku_datalogger_convert(char *buf, size_t bufsz, void *recode)
{
	struct sharaku_datalogger_recode *rcd = (struct sharaku_datalogger_recode*)recode;
	uint32_t		sz;
	int			i;

	sz = 0;
	for (i = 0; i < _logger.index && sz < bufsz; i++) {
		switch (_logger.recode_header[i].type) {
		case SHARAKU_DATALOGGER_TYPE_INT32:
			sz += snprintf(buf, bufsz - sz, "%d,", rcd[i].i);
			break;
		case SHARAKU_DATALOGGER_TYPE_FLOAT:
			sz += snprintf(buf, bufsz - sz, "%f,", rcd[i].f);
			break;
		case SHARAKU_DATALOGGER_TYPE_STR4:
			sz += snprintf(buf, bufsz - sz, "%c%c%c%c,",
					 rcd[i].c[0], rcd[i].c[1],
					 rcd[i].c[2], rcd[i].c[3]);
			break;
		}
	}
	snprintf(buf, bufsz, "\n");
}

void
sharaku_datalogger_initialize(void)
{
	struct sharaku_logger_create	create;

	_logger.max_counts 	= SHARAKU_DATALOGGER_MAXCOUNT;
	_logger.index		= 0;
	_logger.max_counts	= 0;

	sharaku_datalogger_create(SHARAKU_DATALOGGER_TYPE_INT32, "time(us)");
	create.recode_sz	= sizeof(struct sharaku_datalogger_recode) * _logger.index;
	create.count		= _logger.max_counts;
	create.log_rotation	= 0;
	create.cb_conv		= sharaku_datalogger_convert;

	_logger.handle = sharaku_logger_init(&create);
}

sharaku_datalogger_handle
sharaku_datalogger_create(int32_t t, char *title)
{
	int32_t	handle = _logger.index;
	_logger.index ++;
	// 初期化後であればログ領域を拡張する
	if (_logger.max_counts != 0) {
		struct sharaku_logger_create	create;

		sharaku_logger_release(_logger.handle);
		_logger.recode_header = malloc(sizeof(struct sharaku_datalogger_recode)
						 * _logger.index);
		_logger.recode = malloc(sizeof(struct sharaku_datalogger_recode)
						 * _logger.index);

		create.recode_sz	= sizeof(struct sharaku_datalogger_recode) * _logger.index;
		create.count		= _logger.max_counts;
		create.log_rotation	= 0;
		create.cb_conv		= sharaku_datalogger_convert;

		_logger.handle = sharaku_logger_init(&create);
	}

	// mod_loggerに対してログを登録する
	_logger.recode_header[handle].type	= t;
	strncpy(_logger.recode_header[handle].title, title, 1020);
	_logger.recode_header[handle].title[1019] = '\0';

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
	sharaku_usec_t us = sharaku_get_usec();
	sharaku_datalogger_set_int32(0, us);
	sharaku_logger_add(_logger.handle, (void*)_logger.recode);
}

void
sharaku_datalogger_flush(void)
{
	sharaku_logger_flush_to_file(_logger.handle, SHARAKU_DATALOGGER_FILENAME);
}

/******************************************************************************
module
******************************************************************************/

