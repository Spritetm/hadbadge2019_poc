
print "Loading gfx";
tilegfx=tilegfxload("tileset.png")
framemap=tilemapload("frame.tmx", "bricks")
bgndmap=tilemapload("bgnd.tmx", "bgnd")

print "Initializing gfx";
bgndinit(0, bgndmap, tilegfx, &he3)
bgndinit(1, framemap, tilegfx, &he3)
sprInit(5, tilegfx, &he3)

'Ball sprite at index 0
sprSetTile(0, 0)
'Bat sprites at 1..3
for i = 1 to 3
	sprSetTile(i, i)
	sprSetY(i, 220)
	sprSetX(i, 200+i*8)
next

sprSetX(0, 100): sprSetY(0, 100)

ballx=0
bgndy=0
x=151

while 1
	b=buttons()
	'ToDo: The and/or/xor operations in this Basic dialect are strictly logical and can't be used
	'for bitmasking... need to fix this.
	if b = 4 then x=x-3
	if b = 8 then x=x+3

	'Clamp paddle position
	if x<75 then x=75
	if x>228 then x=228

	'Move paddle
	for i = 1 to 3
		sprSetX(i, x+i*8-12)
	next
	'Scroll background fancily
	bgndy=bgndy+0.1
	bgndscroll(0, sin(bgndy/10)*20, bgndy)
	'All done, wait for refresh.
	waitvbl()
wend


'note: need no-op at end