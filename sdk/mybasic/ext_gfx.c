#include "ext_gfx.h"
#include "driver/gfxdriver.h"
#include "driver/loader.h"
#include <assert.h>

//tilemap=tilemapload("file.tmx", "layer1")
static int _tilemapload(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	char *filename;
	char *layername;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_string(s, l, &filename));
	mb_check(mb_pop_string(s, l, &layername));
	mb_check(mb_attempt_close_bracket(s, l));

	mach_tilemap_t *tm=load_tilemap(filename, layername);
	if (!tm) result=MB_FUNC_ERR;
	mb_check(mb_push_usertype(s, l, tm));

	return result;
}

//tilemapfree(tilemap)
static int _tilemapfree(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	mach_tilemap_t *tm;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, (void**)&tm));
	mb_check(mb_attempt_close_bracket(s, l));
	free(tm);
	return result;
}

//tilegfx=tilegfxload("file.png")
static int _tilegfxload(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);

	char *filename;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_string(s, l, &filename));
	mb_check(mb_attempt_close_bracket(s, l));

	uint8_t *gfx=load_tilegfx(filename);
	if (!gfx) result=MB_FUNC_ERR;
	mb_check(mb_push_usertype(s, l, gfx));

	return result;
}

//tilegfxfree(tilegfx)
static int _tilegfxfree(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	mach_tilemap_t *tm;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &tm));
	mb_check(mb_attempt_close_bracket(s, l));
	free(tm);
	return result;
}

//bgndinit(bgndid, tilemap, tilegfx, trans_col)
static int _bgndinit(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	mach_tilemap_t *tm;
	uint8_t *gfx;
	int id, transcol;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_int(s, l, &id));
	mb_check(mb_pop_usertype(s, l, &tm));
	mb_check(mb_pop_usertype(s, l, &gfx));
	mb_check(mb_pop_int(s, l, &transcol));
	mb_check(mb_attempt_close_bracket(s, l));

	if (id<0 || id>GFX_BGND_COUNT) {
		result=MB_FUNC_ERR;
	} else {
		gfx_bgnd_set_gfx(id, gfx, transcol);
		gfx_bgnd_set_map(id, tm);
	}

	free(tm);
	return result;
}

//bgndscroll(bgndid, scrollx, scrolly)
static int _bgndscroll(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int id, scrollx, scrolly;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_int(s, l, &id));
	mb_check(mb_pop_int(s, l, &scrollx));
	mb_check(mb_pop_int(s, l, &scrolly));
	mb_check(mb_attempt_close_bracket(s, l));

	if (id<0 || id>GFX_BGND_COUNT) {
		result=MB_FUNC_ERR;
	} else {
		scrollx %= gfx_bgnd_get_w(id);
		scrolly %= gfx_bgnd_get_h(id);
		if (scrollx<0) scrollx+=gfx_bgnd_get_w(id);
		if (scrolly<0) scrolly+=gfx_bgnd_get_h(id);
		gfx_bgnd_scroll(id, scrollx, scrolly);
	}
	return result;
}

//waitvbl()
static int _waitvbl(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_attempt_close_bracket(s, l));
	gfx_wait_vbl();
	return result;
}


void mybasicext_gfx_install(struct mb_interpreter_t* bas) {
	mb_register_func(bas, "TILEMAPLOAD", _tilemapload);
	mb_register_func(bas, "TILEMAPFREE", _tilemapload);
	mb_register_func(bas, "TILEGFXLOAD", _tilegfxload);
	mb_register_func(bas, "TILEGFXFREE", _tilegfxload);
	mb_register_func(bas, "BGNDINIT", _bgndinit);
	mb_register_func(bas, "BGNDSCROLL", _bgndinit);
	mb_register_func(bas, "WAITVBL", _waitvbl);
}