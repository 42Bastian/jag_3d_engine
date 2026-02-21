;-*-asm-*-
****************
* draw polys

TXT_FP		equ 8
TXT_SIZE	equ 64

CLIP_TEXTURE	EQU BLIT_CLIP_A1*0
TRANS_ENABLE	EQU BLIT_DCOMPEN*0

tri_array	reg 15!
blitter		reg 14

x0		reg 99
y0		reg 99
x1		reg 99
y1		reg 99
x2		reg 99
y2		reg 99

tri_ptrs.a	reg 99
f_cnt.a		reg 99


Drawfaces::
	PUSHLR	object_list

	movei	#1023,LR
	movei	#$f02200,blitter
	moveta	LR,f_cnt.a

	movei	#tri_ptrs_ram+1023*4,tri_array

	move	pc,LR
	moveta	tri_array,tri_ptrs.a
	addq	#6,LR
.loop
	load	(tri_array),tri_array
	cmpq	#0,tri_array
	load	(tri_array+4),tmp3	// NULL pointer access !!
	load	(tri_array+8),y0
	load	(tri_array+12),y1
	load	(tri_array+16),y2
	jr	ne,triangle
	load	(tri_array+20),tmp2

.next
	movefa	tri_ptrs.a,tri_array
	movefa	f_cnt.a,tmp0
	subqt	#4,tri_array
	subq	#1,tmp0
	moveta	tri_array,tri_ptrs.a
	jump	pl,(LR)
	moveta	tmp0,f_cnt.a

	POPLR object_list

u0		reg 99
v0		reg 99
u1		reg 99
v1		reg 99
u2		reg 99
v2		reg 99
dy01		reg 99
dy02		reg 99
dy12		reg 99
dx01		reg 99
dx02		reg 99
dx12		reg 99
du02		reg 99
du12		reg 99
du01		reg object_list!

texture.a	reg 99
dv01.a		reg 99
dv02.a		reg 99
dv12.a		reg 99
ul.a		reg 99
vl.a		reg 99
luminance.a	reg 99

triangle:
	;; split x and y and sign-extend
	move	y0,x0
	move	y1,x1
	move	y2,x2
	shlq	#16,y0
	shlq	#16,y1
	shlq	#16,y2
	sharq	#16,x0
	sharq	#16,x1
	movei	#max_x,tmp0
	sharq	#16,x2
	cmp	x0,tmp0
	jr	pl,.x_ok
	cmp	x1,tmp0
	jr	pl,.x_ok
	cmp	x2,tmp0
	jump	mi,(LR)
	nop
.x_ok:
	movei	#max_y,tmp0
	sharq	#16,y0
	sharq	#16,y1
	sharq	#16,y2
	cmp	y0,tmp0	; triangle > y_max ?
	jr	cc,.y_ok
	cmp	y1,tmp0
	jr	cc,.y_ok
	cmp	y2,tmp0
	jump	cs,(LR)
.y_ok:

	cmpq	#0,tmp2		; ti ptr set?
	movei	#gtriangle,tmp0
	jump	eq,(tmp0)
	nop

 if TRANS_ENABLE <> 0 & CLIP_TEXTURE <> 0
	WAITBLITTER
 endif

 if TRANS_ENABLE <> 0
	movei	#$42424242,tmp0
	store	tmp0,(blitter+_BLIT_PATD)
	store	tmp0,(blitter+_BLIT_PATD+4)
 ENDIF
 IF CLIP_TEXTURE <> 0
	movei	#64<<16|64,tmp0
	store	tmp0,(blitter+_BLIT_A1_CLIP)
 ENDIF
	movei	#$80000000,tmp0
	shlq	#8,tmp3
	sub	tmp0,tmp3
	shrq	#8,tmp3
	moveta	tmp3,luminance.a

	load	(tmp2),tmp0
	movei	#texture,tmp1
	shrq	#16-13,tmp0
	add	tmp0,tmp1
	addq	#4,tmp2
	load	(tmp2),u0
	addq	#4,tmp2
	moveta	tmp1,texture.a
	load	(tmp2),u1
	addq	#4,tmp2
	movei	#$ffff,tmp3
	load	(tmp2),u2
	move	u0,v0
	move	u1,v1
	move	u2,v2
	and	tmp3,v0
	and	tmp3,v1
	and	tmp3,v2
	shrq	#16,u0
	shrq	#16,u1
	shrq	#16,u2
;;; ----------------------------------------
//->_dx	equ 60
//->
//->	movei	#0,u0
//->	movei	#0,v0
//->	movei	#$3f<<TXT_FP,u1
//->	movei	#0,v1
//->	movei	#$3f<<TXT_FP,u2
//->	movei	#$3f<<TXT_FP,v2
//->
//->	movei	#160,x0
//->	movei	#10,y0
//->
//->	movei	#160+129,x1
//->	movei	#40,y1
//->
//->	movei	#160+129,x2
//->	movei	#10+128,y2
//->
//->	movefa	dump.a,r0
//->	cmpq	#1,r0
//->	jump	ne,(LR)
//->	nop
//->.first
;;; ----------------------------------------
	; sort y
	cmp	y0,y1
	move	x1,tmp0
	jr	pl,.no_swap1
	move	y1,tmp1

	move	u1,tmp2
	move	v1,tmp3
	move	x0,x1
	move	y0,y1
	move	u0,u1
	move	v0,v1
	move	tmp0,x0
	move	tmp1,y0
	move	tmp2,u0
	move	tmp3,v0
.no_swap1:
	cmp	y1,y2
	move	x1,tmp0
	jr	pl,.no_swap2
	move	y1,tmp1
	move	u1,tmp2
	move	v1,tmp3
	move	x2,x1
	move	y2,y1
	move	u2,u1
	move	v2,v1
	move	tmp0,x2
	move	tmp1,y2
	move	tmp2,u2
	move	tmp3,v2

.no_swap2:
	cmp	y0,y1
	move	x1,tmp0
	jr	pl,.no_swap3
	move	y1,tmp1
	move	u1,tmp2
	move	v1,tmp3
	move	x0,x1
	move	y0,y1
	move	u0,u1
	move	v0,v1

	move	tmp0,x0
	move	tmp1,y0
	move	tmp2,u0
	move	tmp3,v0
.no_swap3:

TXFP	EQU 15

	move	y2,dy02
	sub	y0,dy02
	moveq	#1,tmp0
	jump	eq,(LR)		; flat triangle
	bset	#TXFP,tmp0

	div	dy02,tmp0	; 1/dy02

	;; debug
	movefa	dump.a,r3
	addqt	#1,r3
	moveta	r3,dump.a

	move	u2,du02
	move	v2,tmp2		; dv02
	sub	u0,du02
	sub	v0,tmp2
	move	x2,dx02
	move	x2,dx12
	sub	x0,dx02
	sub	x1,dx12

	imult	tmp0,dx02	; (x2-x0)/dy02
	imult	tmp0,tmp2
	imult	tmp0,du02


	sharq	#TXFP-TXT_FP+2,tmp2
	move	y1,dy01
	moveta	tmp2,dv02.a
	sub	y0,dy01
	moveq	#0,tmp0
	jr	eq,.dy01_zero
	nop
	bset	#TXFP,tmp0
	div	dy01,tmp0	; 1/dy01
.dy01_zero:

	sharq	#TXFP-fp_rez,dx02
	sharq	#TXFP-TXT_FP+2,du02

	move	x1,dx01
	move	u1,du01
	sub	x0,dx01
	move	v1,tmp2		; dv01
	sub	u0,du01
	sub	v0,tmp2

	imult	tmp0,tmp2
	imult	tmp0,dx01	; (x2-x0)/dy02
	sharq	#TXFP-TXT_FP+2,tmp2
	imult	tmp0,du01
	move	y2,dy12
	moveta	tmp2,dv01.a
	sub	y1,dy12
	moveq	#0,tmp0
	jr	eq,.dy12_zero
	nop
	bset	#TXFP,tmp0

	div	dy12,tmp0	; 1/dy12
.dy12_zero:
	sharq	#TXFP-fp_rez,dx01
	sharq	#TXFP-TXT_FP+2,du01

	move	u2,du12
	move	v2,tmp2		; dv01
	sub	u1,du12
	sub	v1,tmp2

	shlq	#fp_rez,x0
	shlq	#fp_rez,x1
	move	x0,x2
	move	u0,u2
	move	v0,v2

	imult	tmp0,dx12	; (x2-x1)/dy12
	imult	tmp0,du12
	imult	tmp0,tmp2

	sharq	#TXFP-fp_rez,dx12
	sharq	#TXFP-TXT_FP+2,du12
	sharq	#TXFP-TXT_FP+2,tmp2
	moveta	tmp2,dv12.a

	movei	#.y0_top_ok,tmp1
	cmpq	#0,y0
	move	dx01,tmp0
	jump	pl,(tmp1)
	move	dx02,tmp1

	imult	y0,tmp0
	imult	y0,tmp1
	sub	tmp0,x0
	sub	tmp1,x2

	move	du01,tmp2
	movefa	dv01.a,tmp3

	sharq	#fp_rez-2,tmp2		; reduce FP resolution to get < 16bit
	sharq	#fp_rez-2,tmp3

	imult	y0,tmp2
	imult	y0,tmp3
	sub	tmp2,u0
	sub	tmp3,v0

	move	du02,tmp2
	movefa	dv02.a,tmp3

	sharq	#fp_rez-2,tmp2
	sharq	#fp_rez-2,tmp3

	imult	y0,tmp2
	imult	y0,tmp3
	sub	tmp2,u2
	sub	tmp3,v2

	moveq	#0,y0
.y0_top_ok:
	shlq	#fp_rez-2,u0
	shlq	#fp_rez-2,u1
	shlq	#fp_rez-2,u2

	shlq	#fp_rez-2,v0
	shlq	#fp_rez-2,v1
	shlq	#fp_rez-2,v2

	cmpq	#0,y1
	movei	#max_y,tmp0
	jr	pl,.y1_top_ok
	move	du12,tmp1

	movefa	dv12.a,tmp2
	sharq	#2,tmp1
	sharq	#2,tmp2
	imult	y1,tmp1
	imult	y1,tmp2
	shlq	#2,tmp1
	shlq	#2,tmp2
	imult	dx12,y1
	sub	tmp1,u1
	sub	y1,x1
	sub	tmp2,v1

	moveq	#0,y1
.y1_top_ok:

	unreg dy01,dy02,dy12

x1.a		reg 99
u1.a		reg 99
v1.a		reg 99

	cmp	y2,tmp0
	moveta	x1,x1.a
	jr	pl,.y_btm_ok
	cmp	y1,tmp0

	move	tmp0,y2
.y_btm_ok:
	jr	pl,.y_btm2_ok
	nop
	move	tmp0,y1
.y_btm2_ok:

HLINE		reg	99
LR2		reg	99

	moveta	u1,u1.a
	moveta	v1,v1.a

	unreg x1,u1,v1

	movei	#hline,HLINE
	sub	y0,y1
._pc1	move	pc,LR2
	jump	nn_ne,(HLINE)
	addq	#.loop_top-._pc1,LR2
	jr	.top_done
	movefa	x1.a,x0
.loop_top:
	movefa	dv01.a,tmp0
	add	du01,u0
	add	du02,u2
	movefa	dv02.a,tmp1
	add	tmp0,v0
	add	tmp1,v2

	add	dx01,x0
	subq	#1,y1
	addqt	#1,y0
	jump	ne,(HLINE)
	add	dx02,x2

	movefa	x1.a,x0
.top_done:
	movefa	u1.a,u0
	movefa	v1.a,v0

	sub	y0,y2
._pc2	move	pc,LR2
	jump	ne,(HLINE)
	addq	#.loop_btm-._pc2,LR2
	jump	(LR)
.loop_btm:
	movefa	dv12.a,tmp0
	add	du12,u0
	add	du02,u2
	movefa	dv02.a,tmp1
	add	tmp0,v0
	add	tmp1,v2

	add	dx12,x0
	subq	#1,y2
	addqt	#1,y0
	jump	eq,(LR)
	add	dx02,x2

	;; fall thru
****************
* draw H-Lines
xl	reg 99
reci	reg 99
du	reg 99
dv	reg 99

hline:
	move	x0,xl		; left
	move	x2,tmp0		; right
	sharq	#fp_rez,xl
	sharq	#fp_rez,tmp0

	moveta	u0,ul.a
	moveta	v0,vl.a

	move	u2,du
	move	v2,dv
	sub	u0,du
	sub	v0,dv

	move	tmp0,tmp2	; xr
	movei	#max_x,tmp3
	sub	xl,tmp0
	moveq	#1,reci
	jr	pl,.ok
	shlq	#31,reci

	add	tmp0,xl
	sub	tmp0,tmp2

	neg	tmp0
	neg	du
	neg	dv
	moveta	u2,ul.a
	moveta	v2,vl.a
.ok
	div	tmp0,reci

	sub	tmp2,tmp3	; right clipping
	jr	pl,.rx
	sharq	#fp_rez-2,du
	add	tmp3,tmp0
.rx
	sharq	#fp_rez-2,dv
	movei	#.lx,tmp1

	shrq	#16,reci	; reduce divider to 15bit
	imult	reci,du
	imult	reci,dv

	sharq	#15-TXT_FP,du	; 16bit fraction
	sharq	#15-TXT_FP,dv

	cmpq	#0,xl		; left clip
	move	y0,tmp2
	jump	pl,(tmp1)
	shlq	#16,tmp2

	move	du,tmp3
	sharq	#TXT_FP-3,tmp3
	imult	xl,tmp3
	movefa	ul.a,tmp1
	shlq	#3,tmp3
	sub	tmp3,tmp1
	move	dv,tmp3
	moveta	tmp1,ul.a

	sharq	#TXT_FP-3,tmp3
	imult	xl,tmp3
	movefa	vl.a,tmp1
	shlq	#3,tmp3
	sub	tmp3,tmp1
	add	xl,tmp0		; reduce count
	moveq	#0,xl
	moveta	tmp1,vl.a
.lx
	or	tmp2,xl		; y:x

	cmpq	#0,tmp0
	move	dv,tmp1
	jr	nn_ne,.len_ok
	move	du,tmp2
	jump	(LR2)
.len_ok
	bset	#16,tmp0

	shrq	#16,tmp1	; int(dv)
	shrq	#16,tmp2	; int(du)
	shlq	#16,tmp1
	shlq	#16,du
	or	tmp1,tmp2
	shlq	#16,dv
	shrq	#16,du
	movefa	vl.a,tmp1
	or	dv,du		; FINC
	move	tmp2,dv		; INC
	movefa	ul.a,tmp2
	shrq	#TXT_FP+fp_rez-2,tmp1
	shlq	#16,tmp1
	shrq	#TXT_FP+fp_rez-2,tmp2
	or	tmp2,tmp1	; A1_PIXEL => texture

	movefa	ul.a,tmp2
	movefa	vl.a,tmp3
	shlq	#16+2,tmp2
	shlq	#16+2,tmp3
	shrq	#16,tmp2
	or	tmp3,tmp2	; A1_FPIXEL
.waitblit:
	load	(blitter+$38),tmp3 ; wait for previous blit
	btst	#0,tmp3
	jr	z,.waitblit
	nop
.direct
	store	xl,(blitter+_BLIT_A2_PIXEL)
	store	tmp1,(blitter+_BLIT_A1_PIXEL)

	movefa	texture.a,tmp1
	store	tmp2,(blitter+_BLIT_A1_FPIXEL)
	store	tmp1,(blitter+_BLIT_A1_BASE)

	movei	#BLIT_PITCH1|BLIT_PIXEL16|BLIT_WID64|BLIT_XADDINC,tmp1
	movei	#BLIT_PITCH2|BLIT_PIXEL16|BLIT_WIDTH|BLIT_XADDPIX,tmp2
	store	tmp1,(blitter+_BLIT_A1_FLAGS)
	store	tmp2,(blitter+_BLIT_A2_FLAGS)

	movefa	screen0.a,tmp1
	store	tmp1,(blitter+_BLIT_A2_BASE)
	store	dv,(blitter+_BLIT_A1_INC)
	movefa	luminance.a,tmp1
	store	du,(blitter+_BLIT_A1_FINC)

	store	tmp1,(blitter+_BLIT_IINC)

	movei	#TRANS_ENABLE|CLIP_TEXTURE|BLIT_SRCEN|BLIT_LFU_REPLACE|BLIT_DSTA2|BLIT_SRCSHADE|BLIT_GOURZ,tmp3
	movei	#BLIT_SRCEN|BLIT_LFU_REPLACE|BLIT_DSTA2|BLIT_SRCSHADE|BLIT_GOURZ,tmp3
	store	tmp0,(blitter+_BLIT_COUNT)

	jump	(LR2)
	store	tmp3,(blitter+_BLIT_CMD)

	unreg	dx01,dx02,dx12,xl
	unreg	HLINE,LR2,reci,du,dv
	unreg	dv01.a,dv12.a,dv02.a,u2,v2,u0,v0
	unreg 	ul.a,vl.a,x1.a
	unreg	texture.a
	unreg	du01,du02,du12

	include "draw2.inc"

	unreg	tri_array, x0,x2, blitter
	unreg	tri_ptrs.a,y0,y1,y2,f_cnt.a
	unreg	x0,y0,y1,x2,y2
