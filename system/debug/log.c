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
function (data logger)
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
module
******************************************************************************/

