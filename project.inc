;-*-asm-*-
****************
* 3D->2D
*          (x'+x_pos)*256
* x_proj = ---------------
*           z'+z_pos
*
*          (y'+y_pos)*256
* y_proj = ---------------
*           z'+z_pos
****************
m_ptr		reg 14


m_cnt		reg 99
x1		reg 99
y1		reg 99
z1		reg 99
x2		reg 99
y2		reg 99
z2		reg 99
xcenter		REG 99
ycenter		REG 99
proj_ptr	reg 99
LOOP		reg 99
mask		reg 99
 IF DARKEN = 1
rvn_ptr		reg 99
 ENDIF
reci		reg 99
DARK_DIST	reg 99
aspectFix	reg 99
sign_x		reg 99
sign_y		reg 99
_1000		reg 99
PROJ_REZ	equ 15

project_object::
	load	(curr_object+obj_rotated),m_ptr
	load	(m_ptr),m_cnt
	moveq	#(max_x>>1)>>4,xcenter
	addq	#4,m_ptr
	moveq	#(max_y>>1)/4,ycenter
	shlq	#4,xcenter
	moveq	#0,_1000
	shlq	#2,ycenter
	bset	#10,_1000

 IF NO_ASPECT_FIX = 0
_fix_ntsc_aspect:
 IF max_x = 640
	movei	#aspect_patch_pal,r2
//->	movei	#aspect_patch_ntsc,aspectFix
 ELSE
	moveq	#aspect_patch_pal,r2
//->	moveq	#aspect_patch_ntsc,aspectFix
 ENDIF
 ENDIF
	move	tmp2,aspectFix
	load	(m_ptr),x1
	movei	#dark_dist*dark_dist,DARK_DIST
	load	(m_ptr+4),z1

 IF DARKEN = 1
	load	(curr_object+obj_vnormals_rotated),rvn_ptr
	addq	#4+12,rvn_ptr
 ENDIF
	load	(curr_object+obj_projected),proj_ptr
	move	pc,LOOP
	movei	#$0000ffff,mask
	addq	#4+6,LOOP
.loop
	sharq	#16,z1
	addqt	#8,m_ptr
	movei	#1<<PROJ_REZ,reci
	jr	pl_ne,.ok_z
	store	z1,(proj_ptr)	; save original Z
	moveq	#2,z1
.ok_z
	div	z1,reci

	move	x1,y1
	sharq	#16,x1
	shlq	#16,y1
	addqt	#4,proj_ptr
	sharq	#16,y1

 IF DARKEN = 1
	imultn	z1,z1
	imacn	x1,x1
	resmac	tmp1

	sub	DARK_DIST,tmp1
	moveq	#0,tmp0
	jr	mi,.no_darken
	neg	tmp1

	move	tmp1,tmp0

.no_darken
	store	tmp0,(rvn_ptr)
	addq	#16,rvn_ptr
 ENDIF

	load	(m_ptr),x2
	move	x1,sign_x
	move	y1,sign_y
	load	(m_ptr+4),z2
	abs	x1
	abs	y1
	mult	reci,x1
	mult	reci,y1
	shrq	#7,x1
	shrq	#7,y1
	movei	#32767-max_x/2,tmp0
	cmp	x1,tmp0
	movei	#32767-max_y/2,tmp1
	jr	pl,._1
	cmp	y1,tmp1
	move	_1000,x1
._1	jr	pl,._2
	abs	sign_x
	move	_1000,y1
._2
	moveq	#1,tmp0
	jr	cc,._3
	abs	sign_y
	subqt	#2,tmp0
._3
	jr	cs,._4
	imult	tmp0,x1
	neg	y1
._4

 IF NO_ASPECT_FIX = 0
	imult	aspectFix,y1
 IF max_x = 384
	sharq	#5,y1
 ENDIF
 IF max_x = 320
	sharq	#3,y1
 ENDIF
 IF max_x = 640
	sharq	#6,y1
 ENDIF
 ENDIF
	add	xcenter,x1
	add	ycenter,y1
	shlq	#16,x1
	and	mask,y1
	move	z2,z1
	or	x1,y1
	move	x2,x1

	store	y1,(proj_ptr)	; save Xscreen/Yscreen
	subq	#1,m_cnt
	move	y2,y1
	jump	nz,(LOOP)
	addqt	#4,proj_ptr

	jump	(LR)
	nop

	UNREG m_cnt,m_ptr,LOOP
	UNREG xcenter,ycenter
	UNREG x1,y1,z1
	UNREG x2,y2,z2
	UNREG proj_ptr,mask
 IF DARKEN = 1
	UNREG rvn_ptr
 ENDIF
	UNREG reci
	UNREG DARK_DIST
	UNREG aspectFix
	unreg sign_x,sign_y
	unreg _1000
