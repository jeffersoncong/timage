#include "lua_timage.h"

static int new_actions(lua_State *L)
{
	int n = luaL_checkint(L, 1);
	size_t nbytes = sizeof(timage_action_array_t) + (n - 1)* sizeof(timage_action_t);
	timage_action_array_t *array = 
		(timage_action_array_t*)lua_newuserdata(L, nbytes);
	array->size = n;
	return 1;
}

static int set_actions(lua_State *L)
{
	timage_action_array_t *array = (timage_action_array_t*)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	luaL_argcheck(L, array!= NULL, 1,  "`array' expected");
	luaL_argcheck(L, 1 <= index && index <= array->size, 2,
			"index out of range" );
	timage_action_t *action = &array->actions[index - 1];

	/**
	 * LUA_RET_COMP(1), width, height
	 *   eg 1,100,200
	 * LUA_RET_CROP(2), width, height, p.x, p.y
	 *   eg 2,100,200,50,50
	 * LUA_RET_PAD(3)
	 *   eg 3
	 */
	size_t len;
	const char *value = luaL_checklstring(L, 3, &len);
	if(value == NULL || len <= 0) {
		return 0;
	}
	if(value[0] < '1' || value[0] > '3') {
		return 0;
	}

	int act = LUA_RET_NONE, width, height, px, py;
	switch(value[0]) {
		case '1':
			action->action = LUA_RET_COMP;
			sscanf(value, "%d,%d,%d", &act, &width, &height);
			break;
		case '2':
			action->action = LUA_RET_CROP;
			sscanf(value, "%d,%d,%d,%d,%d", &act, &width, &height, &px, &py);
			action->position.x = px;
			action->position.y = py;
			break;
		case '3':
			action->action = LUA_RET_PAD;
			sscanf(value, "%d,%d,%d", &act, &width, &height);
			break;
		default:
			return 0;
	}

	action->rect.width = width;
	action->rect.height = height;
	return 0;
}

static int get_size(lua_State *L)
{
	timage_action_array_t *array = (timage_action_array_t*)lua_touserdata(L, 1);
	luaL_argcheck(L, array != NULL, 1,  "`array' expected");
	lua_pushnumber(L, array->size);
	return 1;
}

static int get_actions(lua_State *L)
{
	timage_action_array_t *array = (timage_action_array_t*)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	luaL_argcheck(L, array != NULL, 1,  "'array' expected");
	luaL_argcheck(L, 1 <= index && index <= array->size, 2,
			"index out of range" );

	char buffer[256];
	timage_action_t *action = &array->actions[index];
	switch(action->action) {
		case LUA_RET_COMP:
			sprintf(buffer, "%d,%d,%d", action->action, action->rect.width,
					action->rect.height);
			break;
		case LUA_RET_CROP:
			sprintf(buffer, "%d,%d,%d,%d,%d", action->action, action->rect.width,
					action->rect.height, action->position.x, action->position.y);
			break;
		case LUA_RET_PAD:
			sprintf(buffer, "%d,%d,%d", action->action, action->rect.width,
					action->rect.height);
			break;
		default:
			buffer[0] = '\0';
	}
	lua_pushstring(L, buffer);
	return 1;
}

lua_State* timage_lua_init()
{
	lua_State *L = lua_open();
	if(L) luaL_openlibs(L);

	static const struct luaL_reg actionslib [] = {
		{"new", new_actions},
		{"set", set_actions},
		{NULL, NULL}
	};

	luaL_openlib(L, "actions", actionslib, 0);
	return L;
}

void timage_lua_destory(lua_State *L)
{
	lua_close(L);
}

int timage_lua_comp(lua_State *L, char *path, timage_rectangle_t *orig_rect,
		timage_rectangle_t *req_rect, timage_action_array_t **array)
{
	if(luaL_loadfile(L, path) != 0) {
		fprintf(stderr, "luaL_loadfile fail, %s\n", lua_tostring(L, -1));
		return -1;
	}

	if(lua_pcall(L, 0, 0, 0) != 0) {
		fprintf(stderr, "lua_pcall fail, %s\n", lua_tostring(L, -1));
		return -1;
	}

	// 指定lua处理函数
	// 传4个参数
	lua_getglobal(L, "timage_main");
	lua_pushnumber(L, orig_rect->width);
	lua_pushnumber(L, orig_rect->height);
	lua_pushnumber(L, req_rect->width);
	lua_pushnumber(L, req_rect->height);

	// 执行lua处理函数
	// 弹出返回值
	lua_call(L, 4, 1);
	timage_action_array_t *actions = lua_touserdata(L, -1);
	lua_pop(L, 1);

	if(actions == NULL) return -1;
	*array = actions;
	return 0;

	// 依次支持图片操作序列
#if 0
	int i = 0;
	timage_action_t *action = NULL;
	for(i = 0; i < actions->size; i++) {
		action = &(actions->actions[i]);
		switch(action->action) {
			case LUA_RET_COMP:
				image = ThumbnailImage(image, action->rect.width, action->rect.height, &exception);
				break;
			case LUA_RET_CROP:
				break;
			case LUA_RET_PAD:
				break;
			default:
				break;
		}

	//	fprintf(stdout, "%d\n", action->action);
	}
#endif
}

#if 0
int main()
{
	// 初始化LUA环境
	lua_State *L = timage_lua_init();
	if(!L) return -1;

	// 通过原图尺寸和请求尺寸计算图片处理操作序列
	timage_rectangle_t orig_rect;
	timage_rectangle_t req_rect;
	orig_rect.width  = 1024;
	orig_rect.height = 768;
	req_rect.width  = 100;
	req_rect.height = 100;
	timage_action_array_t *array = NULL;
	char *path = "./inner.lua"
	if(timage_lua_comp(path, &orig_rect, &req_rect, &array) != 0) {
		fprintf(stderr, "timage_lua_comp fail\n");
		goto RET;
	}

	int i = 0;
	timage_action_t *action;
	for(i = 0; i < array->size; i++) {
		// TBD
	}

RET:
	timage_lua_destory(L);
	return 0;
}
#endif

