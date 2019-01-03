#include <stdint.h>

#pragma once

int tile_loader_gen_tileset(char *name, uint8_t *out);
int tile_loader_gen_map(char *name, char *layername, uint8_t *out);
