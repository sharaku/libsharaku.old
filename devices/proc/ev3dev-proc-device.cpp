/* --
 *
 * MIT License
 * 
 * Copyright (c) 2014-2016 Abe Takafumi
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
#include <string.h>
#include <errno.h>
#include <sharaku/log.h>
#include <sharaku/prof.h>
#include <devices/proc/ev3dev-proc-device.hpp>

NAMESPACE_SHARAKU_BEGIN

int32_t
ev3dev_lego_get_device_path(char* outpath, int outpath_len,
			    const char* device_path, const char* port,
			    const char* file)
{
	static char	path[256];
	static char	port_name[64];
	DIR		*dir;
	struct dirent	*dp;

	// ディレクトリをopenする
	dir = opendir(device_path);
	if(dir == NULL) {
		sharaku_db_error("opendir error errno=%d. ", errno, 0, 0, 0, 0, 0);
		goto ng;
	}

	// ディレクトリ内の番号すべてに対してポートの検索を行う。
	// ディレクトリ内の番号とポート番号に関連はないため、検索が必要
	// 発見できたらそのpathを登録する。
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
		snprintf(path, 256, "%s%s/address", device_path, dp->d_name);
		int	fd = open(path, O_RDONLY);
		if (fd == 0) {
			continue;
		}
		if (read(fd, port_name, sizeof(port_name)) == -1) {
			close(fd);
			continue;
		}
		close(fd);

		if (strcmp(port_name, port) == 0) {
			snprintf(outpath, outpath_len, "%s%s/%s",
					device_path, dp->d_name, file);
			goto ok;
		}
	}

	sharaku_db_error("unmach.", 0, 0, 0, 0, 0, 0);
	outpath[0] = '\0';
	closedir(dir);

 ng:
	return -1;

 ok:
	return 0;
}

NAMESPACE_SHARAKU_END

