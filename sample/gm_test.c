#include <stdio.h>
#include <stdlib.h>
#include "../timage.h"
#include "../gm_timage.h"

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

	// 缩略请求类型：填充内缩略
	timage_comp_req_t comp_req;
	comp_req.lua = "./lua/inner_pad.lua";
	comp_req.store.type = TIAMGE_F2F;
	comp_req.store.from_path = "./from.jpg";
	comp_req.store.to_path = "./to.jpg";
	comp_req.rect.width = 100;
	comp_req.rect.height = 100;
	comp_req.xaxis = 5;
	comp_req.yaxis = 5;

	// 执行缩略请求
	fprintf(stdout, "start image comp, inner&pad lua scrpit is %s\n", comp_req.lua);
	int ret = 0;
	if((ret = timage->comp(&comp_req, NULL)) != 0) {
		fprintf(stderr, "image comp fail, ret = %d\n", ret);
	}
	fprintf(stdout, "image comp success\n");

	// 销毁
	timage->destory(NULL);

	return 0;
}

