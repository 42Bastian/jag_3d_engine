;-*-asm-*-
 IF GOURAUD = 1
****************
* gouraud lightning
*
vn_ptr		reg 14

g_ptr		reg 99
p_cnt		reg 99
v_ptr		reg 99
l_x		reg 99
l_y		reg 99
l_z		reg 99
LOOP		reg 99
vn_x		reg 99
vn_y		reg 99
vn_z		reg 99
darken		reg 99

gouraud::
	movei	#LIGHT_X,r14
	load	(r14),l_x
	load	(r14+4),l_y
	load	(r14+8),l_z

	load	(curr_object+obj_vnormals_rotated),vn_ptr
	cmpq	#0,vn_ptr
	load	(vn_ptr),p_cnt
	jump	eq,(LR)
	move	vn_ptr,g_ptr

	load	(vn_ptr),vn_x
	load	(vn_ptr+4),vn_z
 IF DARKEN = 1
	load	(vn_ptr+8),darken
 ENDIF

	move	pc,LOOP
	addq	#4,LOOP
.loop
	move	vn_x,vn_y
	addq	#8,vn_ptr
	sharq	#16,vn_x
	sharq	#16,vn_z

	imultn	l_x,vn_x
	imacn	l_y,vn_y
	imacn	l_z,vn_z
	resmac	tmp1

	sharq	#8,tmp1
	load	(vn_ptr),vn_x
	jr	pl,.limitlum
	load	(vn_ptr+4),vn_z
	moveq	#0,tmp1
.limitlum
 IF ambient <> 0
	addq	#ambient,tmp1
 ENDIF
	sat8	tmp1
 IF DARKEN = 1
	add	darken,tmp1
	load	(vn_ptr+8),darken
 ENDIF
	sat8	tmp1

	subq	#1,p_cnt
	store	tmp1,(g_ptr)
	jump	ne,(LOOP)
	addq	#8,g_ptr

	jump	(LR)
	nop

	unreg	vn_ptr,v_ptr,p_cnt,g_ptr
	unreg	l_x,vn_x
	unreg	l_y,vn_y
	unreg	l_z,vn_z
	unreg	LOOP
	unreg	darken
 ENDIF
