
print "Hello, world!";

print "Loading tile map...";
tilemap=tilemapload("tile/level1.tmx", "fgnd")
print "Loading gfx...";
tilegfx=tilegfxload("tile/beastlands.png")
print "Setting bgnd...";
bgndinit(0, tilemap, tilegfx, &h7d)
print "Setting up sprites...";
sprInit(5, tilegfx, &h7d)
sprSetTile(0, 0)

x=0
y=0
t=0
while 1
	t=t+0.05
	b=buttons()
	print (b and 2);
	if (b and 1) <> 0 then print "Bla";
	if (b and 1) <> 0 then y=y-1
	if (b and 2) <> 0 then y=y+1
	if (b and 4) <> 0 then x=x-1
	if (b and 8) <> 0 then x=x+1
	bgndscroll(0, x, y)
	sprSetX(0, (sin(t)+1)*100)
	sprSetY(0, (cos(t)+1)*100)
	waitvbl()
wend


'note: need no-op at end