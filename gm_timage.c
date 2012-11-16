#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <magick/api.h>
#include "timage.h"
#include "lua_timage.h"

static lua_State *L = NULL;

int gm_timage_init(void *data)
{
	InitializeMagick(NULL);
	if((L = timage_lua_init()) != 0) {
		return -1;
	}
	return 0;
}

void gm_timage_destory(void *data)
{
	DestroyMagick();
	timage_lua_destory(L);
}

int gm_timage_comp(timage_comp_req_t *req, void *data)
{
	int ret = -1;
	ExceptionInfo exception;
	ImageInfo *image_info = NULL;
	Image *image = NULL, *tmp_image = NULL;

	// init exception
	GetExceptionInfo(&exception);

	// get image info
	if((image_info = CloneImageInfo((ImageInfo *)NULL)) == NULL) {
		fprintf(stderr, "CloneImageInfo fail\n");
		goto COMP_RET;
	}
	strcpy(image_info->filename, req->store.from_path);

	// ping image
	// 得到图片原始尺寸
	if((image = PingImage(image_info, &exception)) == NULL) {
		fprintf(stderr, "PingImage fail\n");
		goto COMP_RET;
	}

	// 调用lua脚本，计算图片操作序列
	timage_rectangle_t orig_rect;
	orig_rect.width  = image->columns;
	orig_rect.height = image->rows;
	timage_action_array_t *array = NULL;

	if((ret = timage_lua_comp(L, req->lua, &orig_rect, &(req->rect), &array)) != 0) {
		fprintf(stderr, "timage_lua_comp fail, ret = %s\n", ret);
		goto COMP_RET;
	}

	image = ReadImage(image_info, &exception);

	// 依次执行图片操作序列
	int i = 0;
	RectangleInfo rectExtent;
	timage_action_t *action = NULL;
	for(i = 0; i < array->size; i++) {
		action = &(array->actions[i]);
		switch(action->action) {
			case LUA_RET_COMP:
				image = ThumbnailImage(image, action->rect.width, action->rect.height, &exception);
				break;
			case LUA_RET_CROP:
				break;
			case LUA_RET_PAD:
			//mage->background_color = {background, background, background};
				image->background_color.red = 100;
				rectExtent.x = (req->rect.width - image->columns) / 2;
				rectExtent.y = (req->rect.height - image->rows) / 2;
				rectExtent.width = req->rect.width;
				rectExtent.height = req->rect.height;

				image = ExtentImage(image, &rectExtent, &exception);
				fprintf(stdout, "padi, %d, %d\n", rectExtent.width, rectExtent.height);
				break;
			default:
				break;
		}
	}

	strcpy(image->filename, req->store.to_path);
	WriteImage(image_info, image);

	//	fprintf(stdout, "%d\n", action->action);
//	fprintf(stdout, "orig width = %d\n", image->columns);
//	fprintf(stdout, "orig height = %d\n", image->rows);
	ret = 0;

COMP_RET:
	if(image_info) DestroyImageInfo(image_info);
	if(image) DestroyImage(image);
	DestroyExceptionInfo(&exception);

	return ret;
}

int gm_timage_crop(timage_crop_req_t *req, void *data)
{
	fprintf(stdout, "i am gm_timage_crop\n");
	return 0;
}

#if 0
int main()
{
	// 图片处理回调函数
	// 这里使用GraphicsMagick处理图片
	timage_t *timage = (timage_t*)malloc(sizeof(timage_t));
	timage->init = gm_timage_init;
	timage->destory = gm_timage_destory;
	timage->comp = gm_timage_comp;
	timage->crop = gm_timage_crop;

	// 初始化
	timage->init(NULL);

	// 缩略测试
	// 填充内缩略
	timage_comp_req_t comp_req;
	comp_req.lua = "./lua/inner_pad.lua";
	comp_req.store.type = TIAMGE_F2F;
	comp_req.store.from_path = "./from.jpg";
	comp_req.store.to_path = "./to.jpg";
	comp_req.rect.width = 100;
	comp_req.rect.height = 100;
	comp_req.xaxis = 5;
	comp_req.yaxis = 5;
	timage->comp(&comp_req, NULL);

	// 销毁
	timage->destory(NULL);

	return 0;
}
#endif

