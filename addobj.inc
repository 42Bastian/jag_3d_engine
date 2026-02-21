;-*-asm-*-
****************************************
** Add an object to the draw array

tri_array	reg 14
f_ptr		reg 99
v_ptr		reg 99
f_cnt		reg 99
LR2		reg 99
y0		reg 99
z0		reg 99
y1		reg 99
z1		reg 99
y2		reg 99
z2		reg 99
lum012		reg 99
tri_ptrs	reg 99
scale		reg 99

 IF TEXTURE <> 0
ti_ptr		reg 99
 ENDIF
 IF GOURAUD = 1
lum0		reg 99
lum1		reg 99
lum2		reg 99
vn_ptr		reg 99
NO_GOURAUD	reg 99
 ENDIF
LOOP	reg 99

proj_ptr	reg 15!

AddObjects::
	movei	#tri_array_ram,tri_array
	load	(tri_array),tri_array
	movei	#tri_ptrs_ram,tri_ptrs

SCALE	EQU 1024<<10/far_z

 IF SCALE > 32767
	FAIL "SCALE exceeds range"
 ENDIF

	movei	#SCALE,scale
	movei	#addSingleObject,tmp1

 if GOURAUD = 1
	movei	#USE_GOURAUD,tmp0
	load	(tmp0),tmp0
	movei	#use_gouraud,NO_GOURAUD
	cmpq	#0,tmp0
	jr	ne,.use_g
	nop
	movei	#no_gouraud,NO_GOURAUD
.use_g
 endif

 IF LANDSCAPE = 1
	;; plane
	movei	#obj_plane,curr_object
	load	(curr_object+obj_faces),f_ptr
	load	(curr_object+obj_facesVisible),v_ptr
 if GOURAUD = 1
	load	(curr_object+obj_vnormals_rotated),vn_ptr
 endif
	load	(curr_object+obj_projected),proj_ptr

	move	pc,LR2
	load	(f_ptr),f_cnt
	addq	#10,LR2
	jump	(tmp1)
	addq	#4,f_ptr
 ENDIF

	movei	#OBJECT_LIST,object_list
	load	(object_list),curr_object

	move	PC,LOOP
	movei	#.skip_this,LR2
	addq	#4+6,LOOP
.loop:
	loadw	(curr_object),tmp0 ; object visible?
	addqt	#4,object_list
	load	(curr_object+obj_faces),f_ptr
	shlq	#24,tmp0
	load	(curr_object+obj_facesVisible),v_ptr
	jump	ne,(LR2)
	nop
 if GOURAUD = 1
	load	(curr_object+obj_vnormals_rotated),vn_ptr
 endif
	load	(curr_object+obj_projected),proj_ptr
	load	(f_ptr),f_cnt
	jump	(tmp1)
	addq	#4,f_ptr

.skip_this
	load	(object_list),curr_object
	cmpq	#0,curr_object
	jump	ne,(LOOP)
	nop

	movei	#tri_array_ram,tmp0
	jump	(LR)
	store	tri_array,(tmp0)

skip_tri:
	subq	#1,f_cnt
	addqt	#face_size-4,f_ptr
	jump	eq,(LR2)
	nop
addSingleObject::
	load	(v_ptr),lum012	; get visible-flag and luminance
	addqt	#4,v_ptr
	cmpq	#0,lum012
	load	(f_ptr),y0
	jr	mi,skip_tri
	addq	#4,f_ptr
	load	(f_ptr),y2
	addq	#4,f_ptr
	move	y0,y1
	move	y2,tmp0
 IF TEXTURE <> 0
	load	(f_ptr),ti_ptr
	addq	#4,f_ptr
 ENDIF
	shlq	#16,y1
	shlq	#16,tmp0
	shrq	#16-3,y0
	shrq	#16,y2
	shrq	#16-3,y1
	shrq	#24,tmp0

//->	movei	#$ffffff,lum012

 IF GOURAUD = 1
	jump	(NO_GOURAUD)
	shlq	#3,y2
use_gouraud:
	shlq	#24,lum012	; base color
	move	y0,lum0
	shrq	#24,lum012
	add	vn_ptr,lum0
	move	y1,lum1
	load	(lum0),lum0
	add	vn_ptr,lum1
	move	y2,lum2
	load	(lum1),lum1
	add	vn_ptr,lum2
	add	lum012,lum0
	load	(lum2),lum2
	add	lum012,lum1
	add	lum2,lum012

	sat8	lum0
	sat8	lum1
	shlq	#16,lum0
	sat8	lum012
	shlq	#8,lum1
	or	lum0,lum012
	or	lum1,lum012
no_gouraud:
 ELSE
	shlq	#3,y2
 ENDIF
	add	proj_ptr,y0
	add	proj_ptr,y1
	load	(y0),z0
	add	proj_ptr,y2
	addq	#4,y0
	load	(y1),z1
	addq	#4,y1
	load	(y2),z2
	addq	#4,y2
	load	(y0),y0
	cmp	z0,z1
	load	(y1),y1
	jr	pl,.lz
	load	(y2),y2
	move	z1,z0
.lz	cmp	z0,z2
	move	y0,tmp2
	jr	pl,.lz1
	and	y1,tmp2
	move	z2,z0
.lz1
	and	y2,tmp2
	movei	#(max_x+10)<<16,tmp3
	jr	mi,.koxy	; all x < 0
	cmp	tmp2,tmp3
	jr	mi,.koxy
	shlq	#16,tmp2
	movei	#max_y<<16,tmp3
	jr	mi,.koxy
	cmp	tmp2,tmp3
	jr	pl,.okxy
	mult	scale,z0	; 1024*(z0+z1+z2)/3/far_z
.koxy
	subq	#1,f_cnt
	jump	eq,(LR2)
	nop
	jump	(tmp1)
.okxy:
	store	y0,(tri_array+8)
	shrq	#9,z0		; >> 16 (Fix point), then << 6 (sat16)
	shlq	#24,tmp0
	sat16	z0
	or	lum012,tmp0
	shrq	#6,z0		; back to 10bit Z array
	store	tmp0,(tri_array+4)
	shlq	#2,z0		; long pointer
	store	y1,(tri_array+12)
	add	tri_ptrs,z0
	store	y2,(tri_array+16)
	load	(z0),tmp2
	store	tri_array,(z0)

 IF TEXTURE <> 0
	store	ti_ptr,(tri_array+20)
 ENDIF

	movefa	dump0.a,tmp0
	addq	#1,tmp0
	moveta	tmp0,dump0.a

	subq	#1,f_cnt
	store	tmp2,(tri_array) ; link new triangle
	jump	ne,(tmp1)
	addqt	#_tri_size,tri_array

	jump	(LR2)
	nop

	unreg	f_ptr,v_ptr,f_cnt,tri_array, LR2, tri_ptrs
	unreg	y0,z0,y1,z1,y2,z2,proj_ptr,lum012,scale
	unreg LOOP
 IF TEXTURE <> 0
	unreg ti_ptr
 ENDIF
 IF GOURAUD = 1
	unreg	lum0,lum1,lum2,NO_GOURAUD,vn_ptr
 ENDIF
