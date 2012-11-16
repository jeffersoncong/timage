#include <stdio.h>
#include <stdlib.h>
#include "timage.h"

int cv_timage_init(void *data)
{
	fprintf(stdout, "i am cv_timage_init\n");
	return 0;
}

void cv_timage_destory(void *data)
{
	fprintf(stdout, "i am cv_timage_destory\n");
}

int cv_timage_comp(timage_comp_req_t *req, void *data)
{
	fprintf(stdout, "i am cv_timage_comp\n");
	return 0;
}

int cv_timage_crop(timage_crop_req_t *req, void *data)
{
	fprintf(stdout, "i am cv_timage_crop\n");
	return 0;
}

#if 0
int main()
{
	timage_t *timage = (timage_t*)malloc(sizeof(timage_t));
	timage->init = cv_timage_init;
	timage->destory = cv_timage_destory;
	timage->comp = cv_timage_comp;
	timage->crop = cv_timage_crop;

	timage->init(NULL);
	timage->comp(NULL, NULL);
	timage->crop(NULL, NULL);
	timage->destory(NULL);

	return 0;
}
#endif

