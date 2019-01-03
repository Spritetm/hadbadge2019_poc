#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <zlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdint.h>
#include <string.h>
#include "mach_defines.h"

///---- gfx loader ----

static int conv_to_8bit(int r, int g, int b) {
	int ret=0;
	if (r&0x80) ret|=0x80;
	if (r&0x40) ret|=0x40;
	if (r&0x20) ret|=0x20;
	if (g&0x80) ret|=0x10;
	if (g&0x40) ret|=0x08;
	if (g&0x20) ret|=0x04;
	if (b&0x80) ret|=0x02;
	if (b&0x40) ret|=0x01;
	return ret;
}

//Generate tileset from png image
int tile_loader_gen_tileset(char *name, uint8_t *out) {
	FILE *f=NULL;
	int ret=-1;
	gdImagePtr im=NULL;
	f=fopen(name, "r");
	if (!f) goto err;
	im=gdImageCreateFromPng(f);
	if (im==NULL) goto err;
	int h=gdImageSY(im)/8;
	int w=gdImageSX(im)/8;
	ret=w*h*64;
	if (out!=NULL) {
		//Actually read the file
		for (int y=0; y<h; y++) {
			for (int x=0; x<w; x++) {
				for (int yy=0; yy<8; yy++) {
					for (int xx=0; xx<8; xx++) {
						int c=gdImageGetTrueColorPixel(im, x*8+xx, y*8+yy);
						int r=gdTrueColorGetRed(c);
						int g=gdTrueColorGetGreen(c);
						int b=gdTrueColorGetBlue(c);
						*out++=conv_to_8bit(r, g, b);
					}
				}
			}
		}
	}
	gdImageDestroy(im);
	fclose(f);
	return ret;
err:
	fprintf(stderr, "Error generating tileset for %s\n", name);
	if (im) gdImageDestroy(im);
	if (f) fclose(f);
	return -1;
}

// ---- map loader -----

static xmlNodePtr findNodeByName(xmlNodePtr parent, const xmlChar *name) {
	if (!parent) return NULL;
	xmlNodePtr node=parent->children;
	while (node) {
		if (!xmlStrcmp(node->name, name)) {
			return node;
		}
		node=node->next;
	}
	return NULL;
}

static int write_map(xmlDoc *doc, xmlNode *layer, uint8_t *out, char *designator) {
	char *mh=xmlGetProp(layer, "height");
	char *mw=xmlGetProp(layer, "width");
	if (mh==NULL || mw==NULL) goto err;
	int h=atoi(mh);
	int w=atoi(mw);
	int ret=sizeof(mach_tilemap_t)+(w*h*sizeof(uint16_t));
	if (out!=NULL) {
		mach_tilemap_t *map=(mach_tilemap_t*)out;
		map->w=w;
		map->h=h;
		//Actually read data
		xmlNode *data=findNodeByName(layer, "data");
		if (data==NULL) goto err;
		char *menc=xmlGetProp(data, "encoding");
		if (menc==NULL || strcmp(menc, "csv")!=0) {
			fprintf(stderr, "%s: Unsupported encoding in map layer data; only support csv.\n", designator);
			goto err;
		}
		char *csv=xmlNodeListGetString(doc, data->xmlChildrenNode, 1);
		char *p=csv;
		for (int i=0; i<w*h; i++) {
			int tile=atoi(p)-1; //csv is base-1 for some reason; we want base-0.
			if (tile==-1) tile=0xffff; //tile not filled in.
			map->tiles[i]=tile;
			p=strchr(p, ',');
			if (p!=NULL && *p==',') {
				p++; 
			} else if (i!=(h*w)-1) {
				printf("%s, %d of %d map entries: csv ends\n", designator, i, h*w);
				goto err;
			}
		}
		xmlFree(csv);
	}
	return ret;
err:
	fprintf(stderr, "Error parsing layer in %s\n", designator);
	return -1;
}


int tile_loader_gen_map(char *name, char *layername, uint8_t *out) {
	xmlDoc *doc=NULL;
	xmlNode *root=NULL;
	doc=xmlReadFile(name, NULL, 0);
	if (doc==NULL) goto err;
	root=xmlDocGetRootElement(doc);

	//Find maps and parse them
	int ret=-1;
	xmlNode *child=root->children;
	while (child!=NULL) {
		if (!xmlStrcmp(child->name, "layer")) {
			char *mn=xmlGetProp(child, "name");
			if (strlen(layername)==0 || strcmp(mn, layername)==0) {
				ret=write_map(doc, child, out, name);
				break;
			}
		}
		child=child->next;
	}

	xmlFreeDoc(doc);
	return ret;
err:
	fprintf(stderr, "Couldn't parse tmx file %s\n", name);
	xmlFreeDoc(doc);
	return 0;
}

