/**
 * File:   lua_timage.h
 * Author: laidongmin@corp.netease.com
 * Date:   2012/11/15
 * Brief:  将原图尺寸和请求尺寸传递给LUA脚本，计算出图片处理操作序列
 *         对LUA操作代码进行了封装
 *
 * Copyright (C) 2010 - All Rights Reserved
 */

#ifndef __LUA_TIMAGE_H__
#define __LUA_TIMAGE_H__

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "timage.h"

/**
 * 图片处理操作类型
 */
typedef enum {
	LUA_RET_NONE = 0,
	LUA_RET_COMP = 1,  /* 缩略 */
	LUA_RET_CROP = 2,  /* 裁剪 */
	LUA_RET_PAD = 3    /* 填充 */
} timage_action_e;

/**
 * 图片处理操作参数
 */
typedef struct {
	timage_action_e action;     /* 缩略、裁剪、填充 */
	timage_position_t position; /* 裁剪位置 */
	timage_rectangle_t rect;    /* 缩略、裁剪、填充的大小 */
} timage_action_t;

/**
 * 图片处理操作序列
 * 原始图片可能需要进行多次操作才能得到最终处理后的结果
 * 例如：1)先进行重点区域识别对图片进行裁剪; 
 *       2)根据请求对图片进行缩略
 *       3)对缩略后的图片进行裁剪或填充
 */
typedef struct {
	int size;
	timage_action_t actions[1];
} timage_action_array_t;

/**
 * 初始化lua环境
 */
lua_State* timage_lua_init();

/**
 * 退出lua环境
 */
void timage_lua_destory(lua_State *L);

/**
 * 使用lua脚本对根据原图尺寸和请求尺寸，得到最终操作序列
 * @param path lua脚本路径
 * @param orig_rect 原图尺寸
 * @param req_rect 请求尺寸
 * @param array[out] 计算得到的操作序列
 */
int timage_lua_comp(lua_State *L, char *path, timage_rectangle_t *orig_rect,
		timage_rectangle_t *req_rect, timage_action_array_t **array);

#endif

