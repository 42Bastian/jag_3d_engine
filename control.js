;-*-asm-*-
	include <js/macro/joypad1.mac>
	include <js/symbols/joypad.js>

ptr		reg 14

obj_ptr		reg 99
curr_obj	reg 99
print_dec	reg 99
print_short	reg 99
joypad		reg 99
check_button	reg 99
testValue	reg 99
cam_x		reg 99
cam_y		reg 99
cam_z		reg 99

Control::
	MODULE control,MODend_irq
//->	MODULE control,MODend_poly_mmu

	PUSHLR

	movei	#OBJECT_PTR,obj_ptr
	load	(obj_ptr),tmp0
	load	(tmp0),curr_obj

	loadb	(curr_obj),r0
	addq	#1,r0
	storeb	r0,(curr_obj)

	move	curr_obj,r0
	addq	#obj_dummy,r0
	loadw	(r0),r0
	movei	#no_rotate,r1
	cmpq	#0,r0
	movei	#511<<2,r0
	jump	ne,(r1)

	move	curr_obj,r1
	addq	#obj_angle_a,r1

	loadw	(r1),r2
	addq	#16,r2
	and	r0,r2
	storew	r2,(r1)
	addqt	#2,r1

	loadw	(r1),r2
	addq	#16,r2
	and	r0,r2
	storew	r2,(r1)
	addqt	#2,r1

	loadw	(r1),r2
	addq	#16,r2
	and	r0,r2
//->	storew	r2,(r1)
no_rotate:
	movei	#joy,r0
	BL	(r0)

	movei	#LastJoy,r0
	load	(r0),r0

	movei	#no_123,r1
	movei	#$00F00000|JOY_1|JOY_2|JOY_3|JOY_A|JOY_B,joypad	 ; 1-3+Cursor
	and	r0,joypad
	movei	#checkButton,check_button
	jump	eq,(r1)
	nop
	movei	#JOY_1|JOY_2|JOY_3,r1
	and	joypad,r1
	jr	eq,no_object
******************
* check x,y,z change
	moveq	#obj_x,r4
	add	curr_obj,r4

	moveq	#0,testValue
	move	PC,LR
	jump	(check_button)
	bset	#JOY_1_BIT,testValue

	moveq	#0,testValue
	jump	(check_button)
	bset	#JOY_2_BIT,testValue

	movei	#no_123-6,LR
	moveq	#0,testValue
	jump	(check_button)
	bset	#JOY_3_BIT,testValue

no_object:
	movei	#CAMERA_X,r14
	btst	#JOY_RIGHT_BIT,joypad
	load	(r14+CAMERA_ANGLE_Y-CAMERA_X),r0
	jr	ne,turn_right
	btst	#JOY_LEFT_BIT,joypad
	jr	eq,no_turn
	nop
	addqt	#a_speed*4*2,r0
turn_right:
	subqt	#a_speed*4,r0

	shlq	#32-9-2,r0
	shrq	#32-9-2,r0
	store	r0,(r14+CAMERA_ANGLE_Y-CAMERA_X)
no_turn:

	moveq	#5,tmp0
	load	(r14+CAMERA_Y-CAMERA_X),cam_y
	btst	#JOY_DOWN_BIT,joypad
	jr	ne,up_down
	btst	#JOY_UP_BIT,joypad
	jr	eq,no_up_down
	neg	tmp0
up_down:
	moveq	#20,tmp1
	move	tmp1,tmp2
	shlq	#2,tmp2
	add	tmp0,cam_y
	cmp	tmp1,cam_y
	jr	pl,up_down_ok
	cmp	cam_y,tmp2
	move	tmp1,cam_y
up_down_ok:
	jr	pl,up_down_ok2
	nop
	move	tmp2,cam_y
up_down_ok2:
	store	cam_y,(r14+CAMERA_Y-CAMERA_X)
no_up_down:

no_123:
	movei	#LastJoy+4,r0
	load	(r0),joypad

	load	(r14+CAMERA_SPEED-CAMERA_X),r2
	btst	#JOY_A_BIT,joypad
	moveq	#4,r0
	jr	ne,forward
	btst	#JOY_B_BIT,joypad
	jr	eq,no_move
	neg	r0
forward:
	add	r0,r2
	jr	pl,.oks1
	cmpq	#-12,r2
	jr	cc,.oks
	nop
	jr	.oks
	sub	r0,r2

.oks1
	cmpq	#12,r2
	jr	mi,.oks
	nop
	moveq	#12,r2
.oks
	store	r2,(r14+CAMERA_SPEED-CAMERA_X)
no_move:
	btst	#JOY_OPTION_BIT,joypad
	movei	#OBJECT_PTR,r3
	jr	eq,.not_option
	load	(r3),r1
	addqt	#4,r1
	load	(r1),r2
	cmpq	#0,r2
	jr	ne,.nn
	nop
	movei	#OBJECT_LIST,r1
.nn
	store	r1,(r3)
.not_option:
	or	r1,r1
	btst	#JOY_0_BIT,joypad
	movei	#USE_GOURAUD,r1
	jr	eq,.not_0
	load	(r1),r2
	not	r2
	store	r2,(r1)
.not_0:
	btst	#JOY_HASH_BIT,joypad
	movei	#USE_PHRASE,r1
	jr	eq,.not_hash
	load	(r1),r2
	not	r2
	store	r2,(r1)
.not_hash:

.exit
;;; ----- output values -----

	addq	#obj_x,curr_obj
	movei	#PrintDEC_YX,print_dec
	movei	#printShort,print_short

	movei	#2<<16|19,r1
	BL	(print_short)
	movei	#2<<16|(19+9),r1
	BL	(print_short)
	movei	#2<<16|(19+18),r1
	BL	(print_short)

	addq	#2,print_short
	movei	#CAMERA_X+2,curr_obj
	movei	#1<<16|1,r1
	loadw	(curr_obj),r0
	BL	(print_short)
	addq	#2,curr_obj
	movei	#1<<16|(1+7),r1
	loadw	(curr_obj),r0
	BL	(print_short)
	addq	#2,curr_obj
	movei	#1<<16|(1+14),r1
	loadw	(curr_obj),r0
	BL	(print_short)
	addq	#2,curr_obj
	movei	#1<<16|(1+21),r1
	loadw	(curr_obj),r0
	shrq	#2,r0
	BL	(print_short)
	addq	#2,curr_obj
	movei	#1<<16|(1+28),r1
	loadw	(curr_obj),r0
	BL	(print_short)

;; ------------------------------
	movei	#CAMERA_X,r14
	load	(r14+CAMERA_ANGLE_Y-CAMERA_X),r0
	load	(r14+CAMERA_X-CAMERA_X),cam_x
	load	(r14+CAMERA_Z-CAMERA_X),cam_z
	load	(r14+CAMERA_SPEED-CAMERA_X),r4
	movei	#SinTab,r15
	move	r4,r5
	load	(r15+r0),r1		; cos(camera_angle)|sin(camera_angle)
	move	r1,r0
	shrq	#16,r1

	imult	r0,r4		; dx
	imult	r1,r5		; dz
	sharq	#15,r4
	sharq	#15,r5
	movei	#grid_size*world_size-1,r3
	movei	#grid_size*world_size/2,r2

	sub	r4,cam_x
	add	r5,cam_z
	add	r2,cam_x
	add	r2,cam_z
	and	r3,cam_x
	and	r3,cam_z
	sub	r2,cam_x
	sub	r2,cam_z
	store	cam_x,(r14+CAMERA_X-CAMERA_X)
	store	cam_z,(r14+CAMERA_Z-CAMERA_X)

 IF 0
	moveq	#8,r4
	shlq	#1,r4
	imult	r4,r0		; dx
	imult	r4,r1		; dz
	sharq	#15,r0
	sharq	#15,r1

	sub	r0,cam_x
	add	r1,cam_z
	add	r2,cam_x
	add	r2,cam_z
	and	r3,cam_x
	and	r3,cam_z
	sub	r2,cam_x
	sub	r2,cam_z
	movei	#obj_ship+obj_x,r0
	load	(r14+CAMERA_Y-CAMERA_X),cam_y
	storew	cam_x,(r0)
	addq	#2,r0
	storew	cam_y,(r0)
	addq	#2,r0
	storew	cam_z,(r0)
	load	(r14+CAMERA_ANGLE_Y-CAMERA_X),cam_y
	addq	#4+2,r0
	loadw	(r0),r1
//->	neg	cam_y
	move	cam_y,r1
	addq	#32,r1
	addq	#32,r1
	shlq	#32-9,r1
	shrq	#32-9,r1
	storew	r1,(r0)
 ENDIF
;; ------------------------------

	POPLR

printShort::
	loadw	(curr_obj),r0
	addq	#2,curr_obj
	shlq	#16,r0
	jump	(print_dec)
	sharq	#16,r0

checkButton::
	addqt	#6,LR
	and	joypad,testValue
	loadw	(r4),r5
	jr	eq,.no_button
	btst	#JOY_UP_BIT,joypad
	jr	ne,.up
	btst	#JOY_DOWN_BIT,joypad
	jr	eq,.no_chg
	nop
	subq	#8,r5
.up
	addq	#4,r5
.no_chg:
	storew	r5,(r4)
.no_button
	jump	(LR)
	addq	#2,r4

joy:
	JOYPAD1 99
	jump	(LR)
	nop
	ENDMODULE control

	unreg obj_ptr, curr_obj, print_dec, print_short
	unreg check_button, testValue, cam_x, cam_y,cam_z
	unreg joypad,ptr

	echo "ENDE (control): %HMODend_control"
