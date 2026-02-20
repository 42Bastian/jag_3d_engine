;-*-asm-*-

** record visibility for each face
**
** visible = (p1 - camera) * normal > 0
**

m_ptr		reg 14
n_ptr		reg 99
f_ptr		reg 99
v_ptr		reg 99
 IF GOURAUD = 1
NO_GOURAUD	reg 99
 ENDIF
f_counter	reg 99
x0		reg 99
y0		reg 99
pt0		reg 99
n_x		reg 99
n_y		reg 99
n_z		reg 99
l_x		reg 99
l_y		reg 99
l_z		reg 99
pt1		reg 99
pt2		reg 99
NO_LUM		reg 99
LOOP		reg 99
min_z		reg 99
rnear_z		reg 99

check_faces_visible::
	movei	#RLIGHT_X,r14
	load	(r14),l_x
	load	(r14+4),l_y
	load	(r14+8),l_z
	load	(curr_object+obj_faces),f_ptr
	load	(curr_object+obj_rotated),m_ptr
	load	(curr_object+obj_normals_rotated),n_ptr
 IF GOURAUD = 1
	movei	#USE_GOURAUD,tmp0
	load	(tmp0),tmp0
	cmpq	#0,tmp0
	load	(curr_object+obj_vnormals_rotated),NO_GOURAUD
	jr	ne,.use_g
	nop
	xor	NO_GOURAUD,NO_GOURAUD
.use_g
 ENDIF
	load	(curr_object+obj_facesVisible),v_ptr
	load	(f_ptr),f_counter
	addq	#4,f_ptr
	addq	#4+4,m_ptr	; skip counter => point to Z
	addq	#4,n_ptr	; skip counter

	movei	#(grid_size+grid_size/4)<<16,rnear_z
	load	(f_ptr),pt0
	move	PC,LOOP
	movei	#no_lum,NO_LUM
	addq	#4+6,LOOP
.loop
	addq	#4,f_ptr
	loadw	(f_ptr),pt2
	move	pt0,pt1
	addq	#3,f_ptr
	shlq	#16,pt1
	shrq	#16-3,pt0
	shrq	#16-3,pt1
	add	m_ptr,pt0
	shlq	#3,pt2
	load	(pt0),tmp0	; z0
	add	m_ptr,pt1
	add	m_ptr,pt2
	load	(pt1),tmp1	; z1
	subq	#4,pt0
	load	(pt2),tmp2	; z2
	move	tmp0,min_z
	cmp	tmp0,tmp1
	load	(pt0),x0	; x0:y0
	jr	pl,.pt1
	subq	#4,pt1

	load	(pt1),x0	; x1:y1
	move	tmp1,min_z
.pt1:
	cmp	min_z,tmp2
	subq	#4,pt2
	jr	pl,.pt2
	move	rnear_z,tmp3

	move	tmp2,min_z
	load	(pt2),x0	; x2:y2

.pt2
	add	min_z,tmp3
	move	x0,y0
	jr	mi,.skip
	bset	#31,tmp0	; set "invisible" flag

	sharq	#16,min_z
	load	(n_ptr),n_x
	addq	#4,n_ptr
	sharq	#16,x0
	load	(n_ptr),n_z
	move	n_x,n_y
	sharq	#16,n_x
	sharq	#16,n_z

	imultn	x0,n_x		; backface culling
	imacn	y0,n_y
	imacn	min_z,n_z
	resmac	tmp0

	subq	#4,n_ptr
	sharq	#31,tmp0
.skip
	addqt	#8,n_ptr
	jump	mi,(NO_LUM)
	store	tmp0,(v_ptr)	; > 0 => visible

 IF GOURAUD = 1
	cmpq	#0,NO_GOURAUD
	loadb	(f_ptr),tmp0		; get base luminance
	jr	ne,.gouraud
 ELSE
	loadb	(f_ptr),tmp0		; get base luminance
 ENDIF
	addqt	#1,v_ptr		; skip vis-flag

	imultn	l_x,n_x
	imacn	l_y,n_y
	imacn	l_z,n_z
	resmac	tmp1

	sharq	#8,tmp1
	sat8	tmp1
	add	tmp1,tmp0
	sat8	tmp0
.gouraud
 IF ambient <> 0
	addq	#ambient,tmp0
	sat8	tmp0
 ENDIF
	storeb	tmp0,(v_ptr)

	addq	#1,v_ptr
	storeb	tmp0,(v_ptr)

	addq	#1,v_ptr
	storeb	tmp0,(v_ptr)

	subqt	#3,v_ptr
no_lum
	subq	#1,f_counter
	addqt	#face_size-7,f_ptr
	addqt	#4,v_ptr
	jump	ne,(LOOP)
	load	(f_ptr),pt0


	jump	(LR)
	nop

 IF GOURAUD = 1
	unreg NO_GOURAUD
 ENDIF

	unreg m_ptr,n_ptr,f_ptr,v_ptr
	unreg f_counter,LOOP,NO_LUM
	unreg x0,y0,pt0,n_x,n_y,n_z,l_x,l_y,l_z
	unreg pt1,pt2,min_z,rnear_z
