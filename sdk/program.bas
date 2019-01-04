
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
	if b = 1 then y=y-1
	if b = 2 then y=y+1
	if b = 4 then x=x-1
	if b = 8 then x=x+1
	bgndscroll(0, x, y)
	sprSetX(0, (sin(t)+1)*100)
	sprSetY(0, (cos(t)+1)*100)
	waitvbl()
wend


'note: need no-op at end