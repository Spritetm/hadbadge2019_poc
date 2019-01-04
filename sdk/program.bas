
print "Hello, world!";

print "Loading tile map...";
tilemap=tilemapload("tile/level1.tmx", "fgnd")
print "Loading gfx...";
tilegfx=tilegfxload("tile/beastlands.png");
print "Setting bgnd...";
bgndinit(0, tilemap, tilegfx, &h7d);

x=0
y=0

while 1
	b=buttons()
	if (b and 1) then y--
	if (b and 2) then y++
	if (b and 4) then x--
	if (b and 8) then x++
	bgndscroll(0, x, y)
	waitvbl()
wend


'note: need no-op at end