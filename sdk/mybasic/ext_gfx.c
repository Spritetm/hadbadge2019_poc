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
	mb_check(mb_pop_usertype(s, l, (void**)&tm));
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
	mb_check(mb_pop_usertype(s, l, (void**)&tm));
	mb_check(mb_pop_usertype(s, l, (void**)&gfx));
	mb_check(mb_pop_int(s, l, &transcol));
	mb_check(mb_attempt_close_bracket(s, l));

	if (id<0 || id>GFX_BGND_COUNT) {
		result=MB_FUNC_ERR;
	} else {
		gfx_bgnd_set_gfx(id, gfx, transcol);
		gfx_bgnd_set_map(id, tm);
	}

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
		scrollx %= gfx_bgnd_get_w(id)*8;
		scrolly %= gfx_bgnd_get_h(id)*8;
		if (scrollx<0) scrollx+=gfx_bgnd_get_w(id)*8;
		if (scrolly<0) scrolly+=gfx_bgnd_get_h(id)*8;
		gfx_bgnd_set_scroll(id, scrollx, scrolly);
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

//sprinit(max_count, tilegfx, trans_col)
static int _sprinit(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	uint8_t *gfx;
	int count, transcol;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_int(s, l, &count));
	mb_check(mb_pop_usertype(s, l, (void**)&gfx));
	mb_check(mb_pop_int(s, l, &transcol));
	mb_check(mb_attempt_close_bracket(s, l));

	mach_sprite_t *sprites=calloc(sizeof(mach_sprite_t), count+1);
	mb_assert(sprites);
	for (int i=0; i<count; i++) sprites[i].ypos=SPRITE_HIDE;
	sprites[count].ypos=SPRITE_LIST_END_MARKER;
	gfx_sprite_set_mem(sprites, gfx, transcol);
	return result;
}


//sprsettile(sprno, tileno)
static int _sprsettile(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int id, v;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_int(s, l, &id));
	mb_check(mb_pop_int(s, l, &v));
	mb_check(mb_attempt_close_bracket(s, l));

	mach_sprite_t *sprites=gfx_get_sprites();
	sprites[id].tile=v;
	return result;
}

//sprsetx(sprno, tileno)
static int _sprsetx(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int id, v;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_int(s, l, &id));
	mb_check(mb_pop_int(s, l, &v));
	mb_check(mb_attempt_close_bracket(s, l));

	mach_sprite_t *sprites=gfx_get_sprites();
	sprites[id].xpos=v;
	return result;
}

//sprsety(sprno, tileno)
static int _sprsety(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	int id, v;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_int(s, l, &id));
	mb_check(mb_pop_int(s, l, &v));
	mb_check(mb_attempt_close_bracket(s, l));

	mach_sprite_t *sprites=gfx_get_sprites();
	sprites[id].ypos=v;
	return result;
}

//tile=tilemapat(tilemap, x, y)
static int _tilemapat(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	mach_tilemap_t *tm;
	int x, y;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, (void**)&tm));
	mb_check(mb_pop_int(s, l, &x));
	mb_check(mb_pop_int(s, l, &y));
	mb_check(mb_attempt_close_bracket(s, l));

	//Wraparound
	x=x%tm->w;
	y=y%tm->h;
	if (x<0) x+=tm->w;
	if (y<0) y+=tm->h;

	mb_check(mb_push_int(s, l, tm->tiles[tm->w*y+x]));

	return result;
}

//tilemapset(tilemap, x, y, tile)
static int _tilemapset(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_assert(s && l);
	mach_tilemap_t *tm;
	int x, y, t;
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, (void**)&tm));
	mb_check(mb_pop_int(s, l, &x));
	mb_check(mb_pop_int(s, l, &y));
	mb_check(mb_pop_int(s, l, &t));
	mb_check(mb_attempt_close_bracket(s, l));

	//Wraparound
	x=x%tm->w;
	y=y%tm->h;
	if (x<0) x+=tm->w;
	if (y<0) y+=tm->h;

	tm->tiles[tm->w*y+x]=t;

	return result;
}



void mybasicext_gfx_install(struct mb_interpreter_t* bas) {
	mb_register_func(bas, "TILEMAPLOAD", _tilemapload);
	mb_register_func(bas, "TILEMAPFREE", _tilemapload);
	mb_register_func(bas, "TILEGFXLOAD", _tilegfxload);
	mb_register_func(bas, "TILEGFXFREE", _tilegfxload);
	mb_register_func(bas, "BGNDINIT", _bgndinit);
	mb_register_func(bas, "BGNDSCROLL", _bgndscroll);
	mb_register_func(bas, "WAITVBL", _waitvbl);
	mb_register_func(bas, "SPRINIT", _sprinit);
	mb_register_func(bas, "SPRSETTILE", _sprsettile);
	mb_register_func(bas, "SPRSETX", _sprsetx);
	mb_register_func(bas, "SPRSETY", _sprsety);
	mb_register_func(bas, "TILEMAPAT", _tilemapat);
	mb_register_func(bas, "TILEMAPSET", _tilemapset);
}
