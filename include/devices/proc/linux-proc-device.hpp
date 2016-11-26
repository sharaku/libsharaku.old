/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_DEVICES_DEVICE_ACCESSOR_H_
#define _SHARAKU_DEVICES_DEVICE_ACCESSOR_H_

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
}
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sharaku/types.h>
#include <sharaku/task.h>
#include <sharaku/thread.h>
#include <sharaku/debug.h>
#include <devices/update-api.hpp>

NAMESPACE_SHARAKU_BEGIN

#ifndef DEVICE_FILEPATH_MAXLEN
#define DEVICE_FILEPATH_MAXLEN		128
#endif
#ifndef DEVICE_BUFFER_LEN
#define DEVICE_BUFFER_LEN		32
#endif

// エラー判定しない、空白来ない、マイナス判定あり
static int sharaku_atoi(const char* str)
{
	const char *work = str;
	int	result = 0;
	int	sign = 0;

	if (*work == '-') {
		sign = 1;
		work ++;
	}

	while (1) {
		if (*work < '0' || *work > '9') {
			break;
		}
		result = result * 10 + *work - '0';
		work ++;
	}

	if (sign) {
		result = -result;
	}

	return result;
}

static unsigned int sharaku_atou(const char* str)
{
	const char *work = str;
	int	result = 0;
	int	sign = 0;

	while (1) {
		if (*work < '0' || *work > '9') {
			break;
		}
		result = result * 10 + *work - '0';
		work ++;
	}

	return result;
}

// エラー判定しない、空白来ない、マイナス判定あり
// バッファは十分大きい
static int sharaku_itoa(int i, char* str)
{
	char	work[16] = {0};
	char*	pwork = work;
	int	len = 0;

	if (i < 0) {
		*str = '-';
		str ++;
		len ++;
		i = -i;
	}
	while (1) {
		*pwork = '0' + i % 10;
		pwork ++;
		i /= 10;
		if (!i) {
			break;
		}
	}
	// 反対に入っているので反転させる
	for (;pwork != work;) {
		pwork --;
		*str = *pwork;
		str ++;
		len ++;
	}
	*str = '\0';
	
	return len;
}

// エラー判定しない、空白来ない、マイナス判定あり
// バッファは十分大きい
static int sharaku_utoa(unsigned int i, char* str)
{
	char	work[16] = {0};
	char*	pwork = work;
	int	len = 0;

	while (1) {
		*pwork = '0' + i % 10;
		pwork ++;
		i /= 10;
		if (!i) {
			break;
		}
	}
	// 反対に入っているので反転させる
	for (;pwork != work;) {
		pwork --;
		*str = *pwork;
		str ++;
		len ++;
	}
	*str = '\0';
	
	return len;
}

class device_accessor_rw {
 public:
	device_accessor_rw() {
		_fd = -1;
		_open_flag = O_NONBLOCK|O_RDWR;
	}
	virtual ~device_accessor_rw() {
		close();
	}
	int32_t connect(const char* proc_file) {
		struct stat buf;
		int result;
		result = stat(proc_file, &buf);
		if (result) {
			sharaku_db_error("stat error errno=%d.",
					 errno, 0, 0, 0, 0, 0);
			return result;
		}
		strncpy(_file_path, proc_file, DEVICE_FILEPATH_MAXLEN);
		return 0;
	}
	int32_t open(void) {
		if (_fd == -1) {
			_fd = ::open(_file_path, _open_flag);
		}
		if (_fd == -1) {
			sharaku_db_error("open error errno=%d.",
					 errno, 0, 0, 0, 0, 0);
			goto ng;
		}
		sharaku_db_trace("open complete fd=%d.",
				 _fd, 0, 0, 0, 0, 0);
		return 0;

	 ng:
		return -1;
	}
	int32_t close(void) {
		if (_fd != -1) {
			::close(_fd);
			_fd = -1;
		}
		return 0;
	}
	// ファイルの内容を読み込み、自身のキャッシュを更新する。
	int32_t update(void) {
		int		fd = _fd;
		int32_t		result = 0;
		if (_fd == -1) {
			fd = ::open(_file_path, O_NONBLOCK|O_RDONLY);
			if (fd == -1) {
				result = errno;
				sharaku_db_error("open error errno=%d.",
						 errno, 0, 0, 0, 0, 0);
				goto out2;
			}
			sharaku_db_trace("update open complete fd=%d.",
					 fd, 0, 0, 0, 0, 0);
		}
		if (pread(fd, _buffer, DEVICE_BUFFER_LEN, 0) == -1) {
			result = errno;
			sharaku_db_error("pread error errno=%d.",
					 errno, 0, 0, 0, 0, 0);
			goto out1;
		}
		_buffer_int32 = sharaku_atoi(_buffer);
		result = 0;
	 out1:
		if (_fd == -1) {
			::close(fd);
			sharaku_db_error("update close complete fd=%d.",
					 fd, 0, 0, 0, 0, 0);
		}
	 out2:
		return result;
	}
	int32_t commit(void) {
		int		fd = _fd;
		int32_t		result = 0;

		if (_fd == -1) {
			fd = ::open(_file_path, O_NONBLOCK|O_WRONLY);
			if (fd == -1) {
				result = errno;
				sharaku_db_error("open error errno=%d.",
						 errno, 0, 0, 0, 0, 0);
				goto out2;
			}
			sharaku_db_error("commit open complete fd=%d.",
					 fd, 0, 0, 0, 0, 0);
		}
		if (pwrite(fd, _buffer, strlen(_buffer), 0) == -1) {
			result = errno;
			sharaku_db_error("pwrite error errno=%d.",
					 errno, 0, 0, 0, 0, 0);
			goto out1;
		}
		result = 0;
	 out1:
		if (_fd == -1) {
			::close(fd);
			sharaku_db_error("commit close complete fd=%d.",
					 fd, 0, 0, 0, 0, 0);
		} else {
			// closeしない場合は、更新を行う
			fsync(fd);
		}
	 out2:
		return result;
	}
	operator const char*(void) {
		return (const char*)_buffer;
	}
	operator int32_t(void) {
		return _buffer_int32;
	}
	device_accessor_rw& operator=(const char *val) {
		_buffer_int32 = 0;
		strncpy(_buffer, val, DEVICE_BUFFER_LEN);
		return *this;
	}
	device_accessor_rw& operator=(int32_t val) {
		_buffer_int32 = val;
		sharaku_itoa(_buffer_int32, _buffer);
		return *this;
	}

 protected:
	char		_file_path[DEVICE_FILEPATH_MAXLEN];
	char		_buffer[DEVICE_BUFFER_LEN];
	int32_t		_buffer_int32;
	int		_fd;
	int		_open_flag;
};

class device_accessor_r
 : public device_accessor_rw {
 public:
	device_accessor_r() : device_accessor_rw() {
		_open_flag = O_NONBLOCK|O_RDONLY;
	}
 protected:
	int32_t commit(void) {return EACCES;}
	device_accessor_rw& operator=(const char *val) {return *this;}
	device_accessor_rw& operator=(int32_t val) {return *this;}
};

class device_accessor_w
 : public device_accessor_rw {
 public:
	device_accessor_w() : device_accessor_rw()  {
		_open_flag = O_NONBLOCK|O_WRONLY;
	}
 public:
	device_accessor_w& operator=(const char *val) {
		_buffer_int32 = 0;
		strncpy(_buffer, val, DEVICE_BUFFER_LEN);
		return *this;
	}
	device_accessor_w& operator=(int32_t val) {
		_buffer_int32 = val;
		sharaku_itoa(_buffer_int32, _buffer);
		return *this;
	}
 protected:
	int32_t update(void) {return EACCES;}
	operator const char*(void) {return NULL;}
	operator int32_t(void) {return EACCES;}
};

// ev3dev lego-deviceのベース。
// 指定インターバルで情報の読込を行う。
// __update		: io_submit前処理
// __io_submit		: io_submit処理。EAGAINを返す場合はもう一度実行する。
// __endio		: io_end処理
class linux_proc_device
 : public device_update_operations
{
 public:
	linux_proc_device(uint32_t interval_ms) {
		// 変数初期化
		sharaku_init_job(&_job_interval);
		sharaku_init_job(&_job_update);
		sharaku_mutex_init(&_mutex_job_i);
		_job = &_job_update;
		_job_i = &_job_interval;
		_interval_ms	= interval_ms;
		_read_flags 	= 0;
		_write_flags 	= 0;
		_type = PROC_IOTYPE_RW;

		_prof_time_read_start = 0;
		_prof_time_write_start = 0;
	}
	virtual ~linux_proc_device() {
		sharaku_mutex_destroy(&_mutex_job_i);
	}
	void set_read_profname(char *process, char *count, char *interval) {
		strncpy(_profname_read_interval, interval, sizeof _profname_read_interval);
		strncpy(_profname_read_count, count, sizeof _profname_read_count);
		strncpy(_profname_read_process, process, sizeof _profname_read_process);
		sharaku_prof_init(&_prof_read_interval, _profname_read_interval);
		sharaku_prof_init(&_prof_read_count, _profname_read_count);
		sharaku_prof_init(&_prof_read_process, _profname_read_process);
		debug_code(sharaku_prof_regist(&_prof_read_interval));
		debug_code(sharaku_prof_regist(&_prof_read_count));
		debug_code(sharaku_prof_regist(&_prof_read_process));
	}
	void set_write_profname(char *process, char *count, char *interval) {
		strncpy(_profname_write_process, process, sizeof _profname_write_process);
		strncpy(_profname_write_count, count, sizeof _profname_write_count);
		strncpy(_profname_write_interval, interval, sizeof _profname_write_interval);
		sharaku_prof_init(&_prof_write_process, _profname_write_process);
		sharaku_prof_init(&_prof_write_count, _profname_write_count);
		sharaku_prof_init(&_prof_write_interval, _profname_write_interval);
		debug_code(sharaku_prof_regist(&_prof_write_process));
		debug_code(sharaku_prof_regist(&_prof_write_count));
		debug_code(sharaku_prof_regist(&_prof_write_interval));
	}
	virtual int32_t start(void) {
		sharaku_mutex_lock(&_mutex_job_i);
		if (_job_i) {
			_job_i = NULL;
			_type = PROC_IOTYPE_RW;
			sharaku_async_message(&_job_interval,
					linux_proc_device::update_device);
		}
		sharaku_mutex_unlock(&_mutex_job_i);
		return 0;
	}
	virtual void start_update(void) {
		sharaku_mutex_lock(&_mutex_job_i);
		if (_job_i) {
			_job_i = NULL;
			_type = PROC_IOTYPE_READ;
			sharaku_async_message(&_job_interval,
					linux_proc_device::start_update_device);
		}
		sharaku_mutex_unlock(&_mutex_job_i);
	}
	virtual void start_commit(void) {
		sharaku_mutex_lock(&_mutex_job_i);
		if (_job_i) {
			_job_i = NULL;
			_type = PROC_IOTYPE_WRITE;
			sharaku_async_message(&_job_interval,
					linux_proc_device::start_commit_device);
		}
		sharaku_mutex_unlock(&_mutex_job_i);
	}
	virtual int32_t is_ioend(void) {
		int32_t ret;
		ret = (_job) ? 1 : 0;
		return ret;
	}
	virtual int32_t stop(void) {
		sharaku_cancel_message(&_job_update);
		return 0;
	}
	virtual int32_t set_interval(uint32_t interval) {
		_interval_ms = interval;
		stop();
		start();
		return 0;
	}
	virtual void set_read_flag(uint32_t flag) {
		_read_flags |= flag;
	}
	virtual void set_write_flag(uint32_t flag) {
		_write_flags |= flag;
	}
	virtual uint32_t get_read_flag(void) {
		return _read_flags;
	}
	virtual uint32_t get_write_flag(void) {
		return _write_flags;
	}

 protected:
	static void update_device(struct sharaku_job* job);
	static void start_update_device(struct sharaku_job* job);
	static void start_commit_device(struct sharaku_job* job);
	static void io_submit(struct sharaku_job* job);
	static void io_end(struct sharaku_job* job);

 protected:
	enum PROC_IOTYPE {
		PROC_IOTYPE_READ,
		PROC_IOTYPE_WRITE,
		PROC_IOTYPE_RW
	};
	virtual void __update(void) = 0;
	virtual void __commit(void) = 0;
	virtual int32_t __io_submit(void) = 0;
	virtual void __io_end(PROC_IOTYPE type) = 0;

 protected:
	sharaku_job		_job_interval;
	sharaku_job		_job_update;
	sharaku_job*		_job;
	sharaku_job*		_job_i;
	sharaku_mutex_t		_mutex_job_i;
	uint32_t		_interval_ms;
	uint32_t		_write_flags;
	uint32_t		_read_flags;
	PROC_IOTYPE		_type;

	// profile
	sharaku_usec_t		_prof_time_read_start;
	sharaku_usec_t		_prof_time_write_start;
	sharaku_prof_t		_prof_read_process;
	sharaku_prof_t		_prof_read_count;
	sharaku_prof_t		_prof_read_interval;
	sharaku_prof_t		_prof_write_process;
	sharaku_prof_t		_prof_write_count;
	sharaku_prof_t		_prof_write_interval;
	char			_profname_read_process[64];
	char			_profname_read_count[64];
	char			_profname_read_interval[64];
	char			_profname_write_process[64];
	char			_profname_write_count[64];
	char			_profname_write_interval[64];

 private:
	linux_proc_device() {}
};

#define DEVICE_IO_SUBMIT_BEGIN				\
	protected:					\
	int32_t __io_submit(void) {			\
		int32_t result = 0;			\
		if (!_read_flags && !_write_flags) { goto endio; }

#define DEVICE_READ_SUBMIT(FLAG, NAME)			\
		else if (_read_flags & FLAG) {		\
			result = NAME.update();		\
			if (result == 0 || result == EOPNOTSUPP) {	\
				_read_flags = _read_flags & (~(FLAG));	\
				result = EAGAIN;		\
			}					\
			goto endio;				\
		}

#define DEVICE_WRITE_SUBMIT(FLAG, NAME)			\
		else if (_write_flags & FLAG) {		\
			result = NAME.commit();		\
			if (result == 0) {		\
				_write_flags = _write_flags & (~(FLAG)); \
				result = EAGAIN;	\
			}				\
			goto endio;			\
		}

#define DEVICE_IO_SUBMIT_END			\
		else {}				\
	 endio:					\
		return result;			\
	}

#define DEVICE_IO_READ				\
	if (type == PROC_IOTYPE_READ || type == PROC_IOTYPE_RW) {
#define DEVICE_IO_WRITE				\
	} if (type == PROC_IOTYPE_WRITE || type == PROC_IOTYPE_RW) {
#define DEVICE_IO_END				\
	}
#define DEVICE_PROC_SET_READ_PROFNAME(fmt, ...)						\
	{										\
		char	prof_interval[64];						\
		char	prof_count[64];							\
		char	prof_process[64];						\
		char	*work;								\
		snprintf(prof_interval, sizeof prof_interval / 2, fmt, ## __VA_ARGS__);	\
		work = strstr(prof_interval, "\n");					\
		if (work) {								\
			*work = '>';							\
		}									\
		memcpy(prof_count, prof_interval, sizeof prof_count);			\
		memcpy(prof_process, prof_interval, sizeof prof_process);		\
		strncat(prof_interval, "::read::interval", sizeof prof_interval);	\
		strncat(prof_count, "::read::count", sizeof prof_count);		\
		strncat(prof_process, "::read::process", sizeof prof_process);		\
		linux_proc_device::set_read_profname(prof_process, prof_count, prof_interval);	\
	}
#define DEVICE_PROC_SET_WRITE_PROFNAME(fmt, ...)					\
	{										\
		char	prof_interval[64];						\
		char	prof_count[64];							\
		char	prof_process[64];						\
		char	*work;								\
		snprintf(prof_interval, sizeof prof_interval / 2, fmt, ## __VA_ARGS__);	\
		work = strstr(prof_interval, "\n");					\
		if (work) {								\
			*work = '>';							\
		}									\
		memcpy(prof_count, prof_interval, sizeof prof_count);			\
		memcpy(prof_process, prof_interval, sizeof prof_process);		\
		strncat(prof_interval, "::write::interval", sizeof prof_interval);	\
		strncat(prof_count, "::write::count", sizeof prof_count);		\
		strncat(prof_process, "::write::process", sizeof prof_process);		\
		linux_proc_device::set_write_profname(prof_process, prof_count, prof_interval);	\
	}



NAMESPACE_SHARAKU_END

#endif /* _SHARAKU_DEVICES_DEVICE_ACCESSOR_H_ */
