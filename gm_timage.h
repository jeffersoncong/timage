/**
 * File:   gm_timage.h
 * Author: laidongmin@corp.netease.com
 * Date:   2012/11/16
 * Brief:  GraphicsMagick图片处理引擎接口
 *
 * Copyright (C) 2010 - All Rights Reserved
 */

#ifndef __GM_TIMAGE_H__
#define __GM_TIMAGE_H__

#include "timage.h"

int gm_timage_init(void *data);
void gm_timage_destory(void *data);
int gm_timage_comp(timage_comp_req_t *req, void *data);
int gm_timage_crop(timage_crop_req_t *req, void *data);

#endif

