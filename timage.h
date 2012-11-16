#ifndef __TOBIE_IMAGE_H__
#define __TOBIE_IMAGE_H__

/**
 * timage_comp 与 timage_crop返回的错误号
 * 请定义更多错误类型
 */
typedef enum {
	TIAMGE_OK = 0,
	TIAMGE_UNKNOWN_IMAGE_TYPE
} timage_error_code_e;

/**
 * 公用数据结构
 * # position
 * # rectangle
 * # buffer
 */
typedef struct {
	int x;
	int y;
} timage_position_t;

typedef struct {
	int width;
	int height;
} timage_rectangle_t;

typedef struct {
	void *buf;
	int size;
} timage_buffer_t;

/**
 * 图片处理前后存储类型
 * 原图和目的图都可以是
 * # file
 * # blob
 */
typedef enum {
	TIAMGE_F2F = 0,
	TIAMGE_F2B,
	TIMAGE_B2F,
	TIMAGE_B2B
} timage_store_type_e;

typedef struct {
	timage_store_type_e type;
	union {
		char *from_path;
		timage_buffer_t from_buffer;
	};  

	union {
		char *to_path;
		timage_buffer_t to_buffer;
	};  
} timage_store_t;

/**
 * 缩略请求结构体
 */
typedef struct {
	char *lua;                  /* 计算脚本 */
	timage_store_t store;       /* 存储位置 */
	timage_rectangle_t rect;    /* 缩略大小 */
	int xaxis;                  /* x轴裁剪比例 0~10 */
	int yaxis;                  /* y轴裁剪比例 0~10 */
} timage_comp_req_t;

/**
 * 裁剪请求结构体
 */
typedef struct {
	char *lua;                  /* 计算脚本 */
	timage_store_t store;       /* 存储位置 */
	timage_position_t position; /* 裁剪位置 */
	timage_rectangle_t rect;    /* 裁剪大小 */
} timage_crop_req_t;

/**
 * 全局初始化函数，初始化图形库等
 * @param data: 自定义参数
 * @return 成功返回0
 */
typedef int (*timage_init)(void *data);

/**
 * 全局销毁函数
 * @param data: 自定义参数
 */
typedef void (*timage_destory)(void *data);

/**
 * 图片缩略处理函数
 * @param req: 请求参数，req中返回值内存由应用释放
 * @param data: 自定义参数
 * @return 成功返回0，否则返回错误号
 */
typedef int (*timage_comp)(timage_comp_req_t *req, void *data);

/**
 * 图片裁剪处理函数
 * @param req: 请求参数，req中返回值内存由应用释放
 * @param data: 自定义参数
 * @return 成功返回0，否则返回错误号
 */
typedef int (*timage_crop)(timage_crop_req_t *req, void *data);

/**
 * 图片处理操作接口结构体
 */
typedef struct {

	timage_init init;
	timage_destory destory;
	timage_comp comp;
	timage_crop crop;

} timage_t;

#endif

