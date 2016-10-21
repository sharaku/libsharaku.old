/*
 * Copyright Abe Takafumi All Rights Reserved, 2014-2016
 * Author Abe Takafumi
 *
 */

#ifndef SHARAKU_MODULES_MOD_DEVUPDATE_H_
#define SHARAKU_MODULES_MOD_DEVUPDATE_H_

#include <stdint.h>
#include <sharaku/task.h>
#include <sharaku/list.h>
#include <devices/update-api.hpp>

NAMESPACE_SHARAKU_BEGIN

#define SHARAKU_MOD_DEVUPDATE_MAX	16
// �o�^���ꂽ�f�o�C�X��update, commit����B
// �f�o�C�X�̃L�b�N�͈�x�ɍs���B
// ���̌�A�S�f�o�C�X��update, commit����������܂ő҂����킹��B
class mod_devupdate
 : public update_operations
{
 public:
	mod_devupdate();

	operator update_operations*() {
		return (update_operations *)this;
	}

	// Update�̓o�^
	void register_update(device_update_operations* update);

	virtual int32_t	pre_update(const float &interval, uint32_t retry_cnt);
	virtual int32_t	post_update(const float &interval, uint32_t retry_cnt);

 private:
	device_update_operations	*_dev_list[SHARAKU_MOD_DEVUPDATE_MAX];
	uint32_t			_devindex;
};

NAMESPACE_SHARAKU_END

#endif // SHARAKU_MODULES_MOD_UPDATE_H_
