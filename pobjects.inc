;-*-asm-*-
******************
* diamant
	dphrase
obj_diamant:
	dc.w	0		; visible
	dc.w	-500,60,2100	; position: x,y,z
	dc.w	0		; spare
	dc.w	64*8,0,0		; angle: a,b,c
	dc.l	diamant_points
	dc.l	diamant_faces
	dc.l	diamant_normals
	dc.l	diamant_vnormals
	dc.l	diamant_rotated
	dc.l	diamant_normals_rotated
	dc.l	diamant_vnormals_rotated
	dc.l	diamant_projected
	dc.l	diamant_visible

	include "objects/diamant.inc"
******************
* prisma
	dphrase
obj_prisma:
	dc.w	0		; visible
	dc.w	-400,60,2100	; position: x,y,z
	dc.w	0		; spare
	dc.w	20*8,120*8,0		; angle: a,b,c
	dc.l	prisma_points
	dc.l	prisma_faces
	dc.l	prisma_normals
	dc.l	prisma_vnormals
	dc.l	prisma_rotated
	dc.l	prisma_normals_rotated
	dc.l	prisma_vnormals_rotated
	dc.l	prisma_projected
	dc.l	prisma_visible

	include "objects/prisma.inc"
******************
* kugel
	dphrase
obj_kugel:
	dc.w	0		; visible
	dc.w	-100,70,2000		; position: x,y,z
	dc.w	0		; spare
	dc.w 	0,0,0		; angle: a,b,c
	dc.l	kugel_points
	dc.l	kugel_faces
	dc.l	kugel_normals
	dc.l	kugel_vnormals
	dc.l	kugel_rotated
	dc.l	kugel_normals_rotated
	dc.l	kugel_vnormals_rotated
	dc.l	kugel_projected
	dc.l	kugel_visible

	include "objects/bigball.inc"
******************
* CUBE
	dphrase
obj_cube:
	dc.w	0		; visible
	dc.w	-310, 80, 2080	; position: x,y,z
	dc.w	0		; spare
	dc.w	0,0,00		; angle: a,b,c
	dc.l	cube_points
	dc.l	cube_faces
	dc.l	cube_normals
	dc.l	cube_vnormals
	dc.l	cube_rotated
	dc.l	cube_normals_rotated
	dc.l	cube_vnormals_rotated
	dc.l	cube_projected
	dc.l	cube_visible

IF 0
	set@	0
	REPT 40
obj_cube@:
	dc.w	0		; visible
	dc.w	-200-(@ >> 2)*40,40+30*(@&3),2900+110*(@&1)	; position: x,y,z
	dc.w	0		; spare
	dc.w	0,0,00		; angle: a,b,c
	dc.l	cube_points
	dc.l	cube_faces
	dc.l	cube_normals
	dc.l	cube_vnormals
	dc.l	cube@_rotated
	dc.l	cube@_normals_rotated
	dc.l	cube@_vnormals_rotated
	dc.l	cube@_projected
	dc.l	cube@_visible
	inc@
	ENDR
 ENDIF
obj_cube2:
	dc.w	0		; visible
	dc.w	-50,100,2300	; position: x,y,z
	dc.w	0		; spare
	dc.w	0,0,0		; angle: a,b,c
	dc.l	cube_points
	dc.l	cube2_faces
	dc.l	cube2_normals
	dc.l	cube2_vnormals
	dc.l	cube2_rotated
	dc.l	cube2_normals_rotated
	dc.l	cube2_vnormals_rotated
	dc.l	cube2_projected
	dc.l	cube2_visible

	include "objects/cube.inc"
******************
	dphrase
obj_torus:
	dc.w	0		; visible
	dc.w	-600,80,2100		; position: x,y,z
	dc.w	0		; spare
	dc.w	0,0,0		; angle: a,b,c
	dc.l	torus_points
	dc.l	torus_faces
	dc.l	torus_normals
	dc.l	torus_vnormals
	dc.l	torus_rotated
	dc.l	torus_normals_rotated
	dc.l	torus_vnormals_rotated
	dc.l	torus_projected
	dc.l	torus_visible

	dphrase
obj_torus2:
	dc.w	0		; visible
	dc.w	-500,80,1900	; position: x,y,z
	dc.w	0		; spare
	dc.w	128*4,0,0		; angle: a,b,c
	dc.l	torus_points
	dc.l	torus_faces
	dc.l	torus_normals
	dc.l	torus_vnormals
	dc.l	torus2_rotated
	dc.l	torus2_normals_rotated
	dc.l	torus2_vnormals_rotated
	dc.l	torus2_projected
	dc.l	torus2_visible

	include "objects/torus.inc"

obj_tunnel:
	dc.w	0		; visible
	dc.w	-200,4,2300	; position: x,y,z
	dc.w	1		; spare
	dc.w	192*8,0,120*8		; angle: a,b,c
	dc.l	tunnel_points
	dc.l	tunnel_faces
	dc.l	tunnel_normals
	dc.l	tunnel_vnormals
	dc.l	tunnel_rotated
	dc.l	tunnel_normals_rotated
	dc.l	tunnel_vnormals_rotated
	dc.l	tunnel_projected
	dc.l	tunnel_visible

	include "objects/tunnel.inc"

	dphrase
obj_ship:
	dc.w	0		; visible
	dc.w	-66,30,2480	; position: x,y,z
	dc.w	1		; spare
	dc.w	0,64,0		; angle: a,b,c
	dc.l	ship_points
	dc.l	ship_faces
	dc.l	ship_normals
	dc.l	ship_vnormals
	dc.l	ship_rotated
	dc.l	ship_normals_rotated
	dc.l	ship_vnormals_rotated
	dc.l	ship_projected
	dc.l	ship_visible

	include "objects/ship.inc"

	dphrase
obj_bomb:
	dc.w	0		; visible
	dc.w	-460,40,2800	; position: x,y,z
	dc.w	0		; spare
	dc.w	64,1600,0		; angle: a,b,c
	dc.l	bomb_points
	dc.l	bomb_faces
	dc.l	bomb_normals
	dc.l	0
	dc.l	bomb_rotated
	dc.l	bomb_normals_rotated
	dc.l	0
	dc.l	bomb_projected
	dc.l	bomb_visible

	include "objects/cm_bomb.inc"

	dphrase
obj_bigship:
	dc.w	0		; visible
	dc.w	-150,40,3000	; position: x,y,z
	dc.w	0		; spare
	dc.w	64,0,64		; angle: a,b,c
	dc.l	bigship_points
	dc.l	bigship_faces
	dc.l	bigship_normals
	dc.l	0
	dc.l	bigship_rotated
	dc.l	bigship_normals_rotated
	dc.l	0
	dc.l	bigship_projected
	dc.l	bigship_visible

	include "objects/cm_bigship.inc"

	dphrase
obj_entry1:
	dc.w	0		; visible
//->	dc.w	-150,40,2300	; position: x,y,z
	dc.w	-100,70,2000		; position: x,y,z
	dc.w	0		; spare
	dc.w	0,0,0		; angle: a,b,c
	dc.l	entry1_points
	dc.l	entry1_faces
	dc.l	entry1_normals
	dc.l	0
	dc.l	entry1_rotated
	dc.l	entry1_normals_rotated
	dc.l	0
	dc.l	entry1_projected
	dc.l	entry1_visible

	include "objects/entry1.shp"

	dphrase
obj_entry7:
	dc.w	0		; visible
//->	dc.w	-150,40,2300	; position: x,y,z
	dc.w	-100,70,2000		; position: x,y,z
	dc.w	0		; spare
	dc.w	0,0,0		; angle: a,b,c
	dc.l	entry7_points
	dc.l	entry7_faces
	dc.l	entry7_normals
	dc.l	0
	dc.l	entry7_rotated
	dc.l	entry7_normals_rotated
	dc.l	0
	dc.l	entry7_projected
	dc.l	entry7_visible

	include "objects/entry7.shp"
