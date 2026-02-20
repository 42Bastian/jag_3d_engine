;-*-asm-*-

projected	reg 14
plane_source	reg 15

cam_y		reg 99
ix		reg 99
iz		reg 99
ox		reg 99
oz		reg 99
norg		reg 99
wx		reg 99
wz		reg 99
cam_sin		reg 99
neg_cam_sin	reg 99
cam_cos		reg 99
xcenter		REG 99
ycenter		REG 99
rotated		reg 99
LOOPX		reg 99
LOOPZ		reg 99
 IF DARKEN = 1
rvn_ptr		reg 99
 ENDIF
rx		reg wx!
ry		reg wz!
rz		reg 99

x_pos.a		reg 99
z_pos.a		reg 99

createPlane::
	PUSHLR

	movefa	cam_x.a,tmp0
	movefa	cam_y.a,cam_y
	movefa	cam_z.a,tmp1
	sharq	#grid_size_bits,tmp0
	sharq	#grid_size_bits,tmp1
	subq	#radius,tmp0
	subq	#radius,tmp1
	moveta	tmp0,x_pos.a
	moveta	tmp1,z_pos.a

	movei	#(max_x>>1),xcenter
	movei	#(max_y>>1),ycenter
	movefa	cam_cos.a,cam_cos
	movefa	cam_sin.a,cam_sin
	movefa	neg_cam_sin.a,neg_cam_sin

 IF dia < 32
	moveq	#dia,iz
 else
	movei	#dia,iz
 endif
	movei	#plane_y,plane_source
	movei	#plane_projected,projected
	movei	#plane_rotated+4,rotated
 IF DARKEN = 1
	movei	#plane_vnormals_rotated+4,rvn_ptr
 ENDIF
	movefa	z_pos.a,oz
	movefa	x_pos.a,ox
._pc	move	PC,LOOPX
	addq	#.loopx-._pc,LOOPX
	move	pc,LOOPZ
	addq	#4,LOOPZ
.loopz:
 IF dia < 32
	moveq	#dia,ix
 else
	movei	#dia,ix
 endif
.loopx
	move	oz,norg
	move	ox,tmp0
	shlq	#32 - world_size_bits,norg
	shlq	#32 - world_size_bits,tmp0
	shrq	#32 - 2*world_size_bits,norg
	shrq	#32 - world_size_bits,tmp0
	add	plane_source,norg
	add	tmp0,norg

	move	ox,wx
	move	oz,wz

	shlq	#grid_size_bits,wx
	shlq	#grid_size_bits,wz

	movefa	cam_x.a,tmp0
	movefa	cam_z.a,tmp1

	sub	tmp0,wx
	sub	tmp1,wz

	addq	#4,rotated	; point to z
	movei	#$3fff,tmp1

	imultn	wx,neg_cam_sin
	imacn	wz,cam_cos
	resmac	rz
	add	tmp1,rz

	sharq	#15,rz
	movei	#1<<15,tmp3
	storew	rz,(rotated)
	jr	pl_nz,.z_ok
	store	rz,(projected)
	moveq	#2,rz
.z_ok
	div	rz,tmp3

	imultn	wx,cam_cos
	imacn	wz,cam_sin
	resmac	rx

	add	tmp1,rx
	loadb	(norg),ry
	sharq	#15,rx

	subq	#2,rotated
	sub	cam_y,ry
	move	rx,tmp1
	storew	ry,(rotated)	; 3D y
	subq	#2,rotated
	move	ry,tmp2
	storew	rx,(rotated)	; 3D x

	abs	rx
	abs	ry
	mult	tmp3,rx		; unsigned !!!
	mult	tmp3,ry		; unsigned !!!
	shrq	#7,rx
	movei	#32767-max_x/2,tmp3
	shrq	#7,ry
	cmp	rx,tmp3
	movei	#32767-max_y/2,tmp3
	jr	pl,._1
	cmp	ry,tmp3
	movei	#1000,rx
._1:	jr	pl,._2
	abs	tmp1
	movei	#1000,ry
._2:	moveq	#1,tmp1
	jr	cc,._3
	abs	tmp2
	subqt	#2,tmp1
._3:	jr	cs,._4
	imult	tmp1,rx
	neg	ry
._4:

 IF NO_ASPECT_FIX = 0
_fix_ntsc_aspect2:
 IF max_x < 640
	moveq	#aspect_patch_pal,tmp2
 ELSE
	movei	#aspect_patch_pal,tmp2
 ENDIF
	imult	tmp2,ry
 SWITCH max_x
 CASE	384
	sharq	#5,ry
 CASE 320
	sharq	#3,ry
 CASE 640
	sharq	#6,ry
 ENDS
 ENDIF

 IF DARKEN = 1
	imultn	rz,rz		; z^2
	imacn	rx,rx		; +x^2
	resmac	tmp0

	movei	#dark_dist*dark_dist,tmp2
	sub	tmp2,tmp0
	addqt	#8,rotated
	jr	mi,.no_darken
	moveq	#0,tmp2

	move	r0,r2
	sat8	r2

.no_darken
	store	tmp2,(rvn_ptr)
	addq	#4,rvn_ptr
 ELSE
	addqt	#8,rotated
 ENDIF
	add	ycenter,ry
	add	xcenter,rx

	shlq	#16,ry
	shlq	#16,rx
	shrq	#16,ry
	addqt	#1,ox
	or	rx,ry
	subq	#1,ix
	store	ry,(projected+4)	; save Xscreen/Yscreen
	jump	ne,(LOOPX)
	addqt	#8,projected

	subq	#1,iz
	addqt	#1,oz
	jump	ne,(LOOPZ)
	movefa	x_pos.a,ox

	unreg xcenter, ycenter
	unreg projected,plane_source,cam_y
	unreg rotated
 IF DARKEN = 1
	unreg rvn_ptr
 ENDIF
	unreg rx,ry,rz,norg
****************************************
** rotate normals
****************************************
n_ptr		reg 14
rn_ptr		reg 15
n_x		reg 99
n_y		reg 99
n_z		reg 99
z_pos		reg 99
x		reg 99

	movefa	z_pos.a,z_pos

 IF dia < 33
	moveq	#dia-1,iz
 else
	movei	#dia-1,iz
 endif
	movei	#plane_normals,n_ptr
	movei	#plane_normals_rotated+4,rn_ptr
	movei	#$3fff,tmp3
._pc0	move	PC,LOOPX
	addq	#.loopx0-._pc0,LOOPX
	move	pc,LOOPZ
	addq	#4,LOOPZ
.loopz0:
	move	z_pos,oz
	shlq	#32-world_size_bits,oz
	shrq	#32-2*world_size_bits-4,oz
	movefa	x_pos.a,x
 IF dia < 33
	moveq	#dia-1,ix
 else
	movei	#dia-1,ix
 endif
.loopx0
	move	x,ox
	shlq	#32 - world_size_bits,ox
	shrq	#32 - world_size_bits-4,ox
	add	oz,ox

	load	(n_ptr+ox),n_x	; 0:x
	addq	#4,ox
	load	(n_ptr+ox),n_y  ; n_y|n_z
	addq	#4,ox
	move	n_y,n_z
	sharq	#16,n_y

	imultn	n_x,cam_cos
	imacn	n_z,cam_sin
	resmac	tmp0

	imultn	n_x,neg_cam_sin
	imacn	n_z,cam_cos
	resmac	tmp1

	add	tmp3,tmp0
	add	tmp3,tmp1

	sharq	#15,tmp0
	sharq	#15,tmp1
	shlq	#16,n_y
	shlq	#16,tmp0
	shrq	#16,n_y
	shlq	#16,tmp1
	or	n_y,tmp0

	store	tmp0,(rn_ptr)
	addq	#4,rn_ptr
	store	tmp1,(rn_ptr)
	addq	#4,rn_ptr

	load	(n_ptr+ox),n_x
	addq	#4,ox
	load	(n_ptr+ox),n_y
	move	n_y,n_z
	sharq	#16,n_y

	imultn	n_x,neg_cam_sin
	imacn	n_z,cam_cos
	resmac	tmp1

	imultn	n_x,cam_cos
	imacn	n_z,cam_sin
	resmac	tmp0

	add	tmp3,tmp1
	add	tmp3,tmp0
	shlq	#16,n_y
	sharq	#15,tmp0
	shrq	#16,n_y
	sharq	#15,tmp1
	shlq	#16,tmp0
	shlq	#16,tmp1
	or	n_y,tmp0

	store	tmp0,(rn_ptr)
	addq	#4,rn_ptr
	store	tmp1,(rn_ptr)

	subq	#1,ix
	addqt	#1,x
	jump	ne,(LOOPX)
	addq	#4,rn_ptr

	subq	#1,iz
	addqt	#1,z_pos
	jump	ne,(LOOPZ)
	nop

	unreg rn_ptr,n_ptr,n_x,n_y,n_z

 IF GOURAUD = 1
****************************************
** Calculate per vertex luminance
** XXX: Can be done once for all vertexes then only copy
**      the ones needed unless LIGHT vector changes.
****************************************
vn_ptr		reg 14
rvn_ptr		reg 15
n_x		reg 99
n_y		reg 99
n_z		reg 99

l_x		reg 99
l_y		reg 99
l_z		reg 99

planeCalcLuminance::
	movei	#.no_gouraud,r1
	movei	#USE_GOURAUD,r0
	load	(r0),r0
	movei	#LIGHT_X,r14	; light vector
	cmpq	#0,r0
	load	(r14),l_x
	jump	eq,(r1)
	load	(r14+4),l_y
	load	(r14+8),l_z

	movefa	z_pos.a,z_pos

 IF dia < 32
	moveq	#dia,iz
 else
	movei	#dia,iz
 endif
	;; storage for vertex luminance
	movei	#plane_vnormals_rotated,rvn_ptr
	;; base vertex normals
	movei	#plane_vnormals,vn_ptr

._pc0	move	PC,LOOPX
	addq	#.loopx0-._pc0,LOOPX
	move	pc,LOOPZ
	addq	#4,LOOPZ
.loopz0:
	move	z_pos,oz
	shlq	#32-world_size_bits,oz
	movefa	x_pos.a,x
	shrq	#32-2*world_size_bits-3,oz
	move	x,ox
 IF dia < 32
	moveq	#dia,ix
 else
	movei	#dia,ix
 endif
.loopx0:
	shlq	#32 - world_size_bits,ox
	shrq	#32 - world_size_bits-3,ox
	add	oz,ox

	load	(vn_ptr+ox),n_x	; 0:x
	addq	#4,ox
	load	(vn_ptr+ox),n_y  ; n_y|n_z
	move	n_y,n_z
	sharq	#16,n_y

	imultn	l_x,n_x		; now calculate luminance
	imacn	l_y,n_y
	imacn	l_z,n_z
	resmac	tmp1

 IF DARKEN = 1
	sharq	#8,tmp1
	load	(rvn_ptr),tmp0	; darken
	jr	pl,.limitlum
	nop
	moveq	#0,tmp1
.limitlum:
	add	tmp0,tmp1
	addqt	#1,x
	jr	pl,.limitlum2
	nop
	moveq	#0,tmp1
.limitlum2:
 ELSE
	sharq	#8,tmp1
	addqt	#1,x
	jr	pl,.limitlum
	nop
	moveq	#0,tmp1
.limitlum
 ENDIF
 IF ambient <> 0
	addq	#ambient,tmp1
 ENDIF
	sat8	tmp1
	subq	#1,ix
	store	tmp1,(rvn_ptr)	; just store it, vnormals not needed
	addqt	#8,rvn_ptr
	jump	ne,(LOOPX)
	move	x,ox

	subq	#1,iz
	jump	ne,(LOOPZ)
	addq	#1,z_pos

.no_gouraud

	unreg rvn_ptr,vn_ptr,n_x,n_y,n_z,l_x,l_y,l_z
 ENDIF
	unreg cam_sin,cam_cos,neg_cam_sin
****************************************
** set triangle colors
****************************************

face_ptr	reg 99
face_ptr2	reg 99
plane_colors	reg 99

planeSetColor::
 IF TEXTURE <> 0
	movei	#plane_faces+4+face_ti_ptr,face_ptr
 ELSE
	movei	#plane_faces+4+face_color,face_ptr
 ENDIF
	moveq	#face_size,face_ptr2
	movefa	z_pos.a,z_pos
	add	face_ptr,face_ptr2

 IF dia < 33
	moveq	#dia-1,iz
 else
	movei	#dia-1,iz
 endif
	move	z_pos,oz
	movei	#plane_col,plane_colors
._pc:
	move	PC,LOOPX
	addq	#.loopx1-._pc,LOOPX
	move	PC,LOOPZ
	addq	#4,LOOPZ
.loopz1
	movefa	x_pos.a,x
	shlq	#32-world_size_bits,oz
	shrq	#32-2*world_size_bits-1,oz
 IF dia < 33
	moveq	#dia-1,ix
 else
	movei	#dia-1,ix
 endif
.loopx1
	move	x,ox
	shlq	#32-world_size_bits,ox
	shrq	#32-world_size_bits-1,ox
	add	oz,ox
	add	plane_colors,ox
	loadw	(ox),tmp1
 IF TEXTURE <> 0
	moveq	#0,tmp2
	move	tmp1,tmp3
	shrq	#8,tmp3
	movei	#$7f,tmp0
	jr	ne,.no_sea
	cmp	tmp3,tmp0
	movei	#sea_tex_0,tmp2
	jr	.tx_done
	move	tmp2,tmp3
.no_sea:
	jr	ne,.no_tx
	moveq	#0,tmp3

	movei	#grass_tex_0,tmp2
	move	tmp2,tmp3
.tx_done:
	addq	#_tex_info_size,tmp3
.no_tx:
	store	tmp2,(face_ptr)
	store	tmp3,(face_ptr2)
	subqt	#2,face_ptr
	subqt	#2,face_ptr2
 ENDIF
	storew	tmp1,(face_ptr)
 IF TEXTURE <> 0
	addqt	#face_size*2+2,face_ptr
 ELSE
	addqt	#face_size*2,face_ptr
 ENDIF

	addqt	#1,x
	subq	#1,ix
	storew	tmp1,(face_ptr2)
	jump	ne,(LOOPX)
 IF TEXTURE <> 0
	addqt	#face_size*2+2,face_ptr2
 ELSE
	addqt	#face_size*2,face_ptr2
 ENDIF

	subq	#1,iz
	addqt	#1,z_pos
	jump	ne,(LOOPZ)
	move	z_pos,oz

	POPLR

	unreg face_ptr,face_ptr2
	unreg x,ox,oz,ix,iz,z_pos,wz,wx
	unreg LOOPX,LOOPZ
	unreg z_pos.a,x_pos.a
	unreg plane_colors
