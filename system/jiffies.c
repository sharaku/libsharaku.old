/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#include <sharaku/jiffies.h>

//
// 1msごとにカウントアップするカウンタ。
// sharakuの周期をとるのに使用している。
//
// カウントアップは以下の契機とする。
// - Linuxの場合は1msごとのタイマー
// - iTron/Toppersの場合は1msごとの周期タスク

uint64_t volatile jiffies;

