
'Load and initialize gfx
tilegfx=tilegfxload("tileset.png")
framemap=tilemapload("frame.tmx", "bricks")
bgndmap=tilemapload("bgnd.tmx", "bgnd")
bgndinit(0, bgndmap, tilegfx, &he3)
bgndinit(1, framemap, tilegfx, &he3)
sprInit(5, tilegfx, &he3)

paddley=220
'Ball sprite at index 0
sprSetTile(0, 0)
'Bat sprites at 1..3
for i = 1 to 3
	sprSetTile(i, i)
	sprSetY(i, paddley)
next

sprSetX(0, 100): sprSetY(0, 100)

'Initial position of ball
ballx=150
bally=170
'Initial direction of ball
pi=3.14159265
balldir=pi*0.75 'between -pi and pi
ballspeed=4
'Background
bgndy=0
'Initial paddle position
paddlex=151

def ballcheckhit(map, x, y)
	'convert pixel coord to tile coord
	x=x/8
	y=y/8
	'find tile
	t=tilemapat(map, x, y)
	if (t>=8 and t<=15) then
		'We hit a brick. Nuke the tile, and its neighbour
		tilemapset(map, x, y, 39) 'set to blank tile
		if (t mod 2) = 0 then
			tilemapset(map, x+1, y, 39)
		else
			tilemapset(map, x-1, y, 39)
		endif
		return true
	endif
	if t <> 39 then
		'Probably the borders.
		return true
	endif
	return false
enddef


while 1
	b=buttons()
	'ToDo: The and/or/xor operations in this Basic dialect are strictly logical and can't be used
	'for bitmasking... need to fix this.
	if b = 4 then paddlex=paddlex-3
	if b = 8 then paddlex=paddlex+3

	'Clamp paddle position
	if paddlex<83 then paddlex=83
	if paddlex>236 then paddlex=236

	'Move paddle sprites
	for i = 1 to 3
		sprSetX(i, paddlex+(i-1)*8-12)
	next

	'Move ball, check for collisions
	ballx=ballx+sin(balldir)*ballspeed;
	bally=bally+cos(balldir)*ballspeed;
	'Check collision to top or bottom
	if balldir<-pi/2 or balldir>pi/2 then
		'ball is going up-ish
		coll=ballcheckhit(framemap, ballx+4, bally)
	else
		'ball is going down-ish
		coll=ballcheckhit(framemap, ballx+4, bally+8)
		'also check against paddle
		if bally+8>paddley and bally+8<paddley+8 then
			if ballx>paddlex-12 and ballx<paddlex+12 then
				coll=true
				dif=(ballx+4)-paddlex
				balldir=balldir+(dif*0.03)
				'todo: more or less angle dependent on where the paddle was hit
			endif
		endif
	endif
	if coll then
		if (balldir>0) then
			balldir=pi-balldir
		else
			balldir=-(pi+balldir)
		endif
	endif
	'Check for collision to left or right
	if balldir>0 then
		'Ball is going right-ish
		coll=ballcheckhit(framemap, ballx+8, bally+4)
	else
		'Ball is going left-ish
		coll=ballcheckhit(framemap, ballx, bally+4)
	endif
	if coll then
		balldir=-balldir
	endif
	'check for dead
	if bally>230 then
		'just reset for now
		ballx=150
		bally=170
		balldir=pi*0.1
	endif
	sprSetX(0, ballx): sprSetY(0, bally)

	'Scroll background fancily
	bgndy=bgndy+0.1
	bgndscroll(0, sin(bgndy/10)*20, bgndy)
	'All done, wait for refresh.
	waitvbl()
wend


'note: need no-op at end