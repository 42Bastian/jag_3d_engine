;-*-asm-*-
preparePlane::
	MODULE planePrep,MODend_irq
****************************************
* Create the triangles for the landscape
*
* Done only once
****************************************
	align	4
createPlaneFaces::
 IF 1
face_ptr	reg 99
p0		reg 99
p1		reg 99
p2		reg 99
p3		reg 99
LOOPZX		reg 99
ix		reg 99
iz		reg 99

	movei	#plane_faces+4,face_ptr
	nop

	moveq	#0,p0
	moveq	#1,p1
 IF dia < 31
	moveq	#dia,p2
	moveq	#dia+1,p3
	moveq	#dia-1,iz
	moveq	#dia-1,ix
 else
	movei	#dia,p2
	movei	#dia+1,p3
	movei	#dia-1,iz
	movei	#dia-1,ix
 endif
	move	PC,LOOPZX
	addq	#4,LOOPZX
.loopxz
	storew	p1,(face_ptr)
	addq	#2,face_ptr
	storew	p3,(face_ptr)
	addq	#2,face_ptr
	storew	p2,(face_ptr)
 IF TEXTURE <> 0
	addq	#4,face_ptr
	moveq	#0,tmp0
	store	tmp0,(face_ptr)
 ENDIF
	addq	#4,face_ptr


	storew	p1,(face_ptr)
	addq	#2,face_ptr
	storew	p2,(face_ptr)
	addq	#2,face_ptr
	storew	p0,(face_ptr)
 IF TEXTURE <> 0
	addq	#4,face_ptr
	store	tmp0,(face_ptr)
 ENDIF
	addqt	#1,p0
	addqt	#1,p1
	addqt	#1,p2
	subq	#1,ix
	addqt	#1,p3
	jump	ne,(LOOPZX)
	addq	#4,face_ptr

	nop
	addqt	#1,p0
	addqt	#1,p1
	addqt	#1,p2
	subq	#1,iz
 if dia-1 < 32
	moveq	#dia-1,ix
 else
	movei	#dia-1,ix
 endif
	jump	ne,(LOOPZX)
	addqt	#1,p3

	movei	#plane_faces,tmp0
	sub	tmp0,face_ptr
	moveq	#face_size,tmp1
	div	tmp1,face_ptr
	store	face_ptr,(tmp0)

	unreg p0,p1,p2,p3,face_ptr,LOOPZX
	unreg ix,iz
 ENDIF
/* Calculate Gouraud lightning */

 IF GOURAUD = 1
l_ptr		reg 99
vn_ptr		reg 99
n_x		reg 99
n_y		reg 99
n_z		reg 99

l_x		reg 99
l_y		reg 99
l_z		reg 99

	movei	#LIGHT_X,r14	; light vector
	load	(r14),l_x
	load	(r14+4),l_y
	load	(r14+8),l_z

	moveq	#0,tmp0
	movei	#plane_vnormals+2,vn_ptr
	bset	#world_size_bits*2,tmp0
	movei	#plane_lum,l_ptr

.loop
	loadw	(vn_ptr),n_x	; n_x
	addq	#2,vn_ptr
	load	(vn_ptr),n_y  ; n_y|n_z
	addq	#6,vn_ptr
	move	n_y,n_z
	sharq	#16,n_y

	imultn	l_x,n_x		; now calculate luminance
	imacn	l_y,n_y
	imacn	l_z,n_z
	resmac	tmp1
	sharq	#8,tmp1
	sat8	tmp1

	subq	#1,tmp0
	storeb	tmp1,(l_ptr)
	jr	ne,.loop
	addq	#1,l_ptr

//->	movei	#$e00008,tmp0
//->	moveq	#31,tmp1
//->	store tmp1,(tmp0)
//->	addq	#4,tmp0
//->	movei	#plane_lum,tmp1
//->	store tmp1,(tmp0)

	unreg vn_ptr,n_x,n_y,n_z,l_x,l_y,l_z,l_ptr
 ENDIF
	jump	(LR)
	nop
	ENDMODULE planePrep

	echo "ENDE (planePrep): %HMODend_planePrep"
