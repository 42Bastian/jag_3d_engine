;-*-asm-*-
* rez
max_x	equ 384
max_y	equ 200
	;; note: max_y 172 if max_x = 640

IF max_x = 384
VID_MODE EQU $4c1
  ELSE
IF max_x = 640
VID_MODE EQU $2c1
ELSE
VID_MODE EQU $6c1
ENDIF
ENDIF

 IF max_x = 160
project_shift_x	  equ 8
aspect_patch_pal  equ 17
aspect_patch_ntsc equ 15
aspect_shift	  equ 4
 ENDIF

 IF max_x = 256
project_shift_x	  equ 8
aspect_patch_pal  equ 12
aspect_patch_ntsc equ 10
aspect_shift	  equ 4
 ENDIF

 IF max_x = 320
project_shift_x	  equ 7
aspect_patch_pal  equ 18
aspect_patch_ntsc equ 15
aspect_shift	  equ 4
 ENDIF

 IF max_x = 384
project_shift_x	  equ 7
aspect_patch_pal  equ 12
aspect_patch_ntsc equ 11
aspect_shift	  equ 4
 ENDIF

 IF max_x = 640
project_shift_x	  equ 6
aspect_patch_pal  equ 15
aspect_patch_ntsc equ 14
aspect_shift	  equ 4
 ENDIF

 IF max_y = 100
project_shift_y	  equ 8
 ELSE
project_shift_y	  equ 7
 ENDIF

 IF max_x < 320
max_x_txt	equ 320
 ELSE
max_x_txt	equ max_x
 ENDIF
max_y_txt	equ 5*8

op_list		equ $400

gfx_screen_size	equ (((max_x*max_y*2)+31) & ~31)*2

TxtScreen	equ $001ffff0-(max_x_txt/8)*max_y_txt
screen1		equ (TxtScreen & $fffff00) - gfx_screen_size
screen0		equ screen1+8
logo_screen	equ screen1-10*8

vde_pal		equ (PAL_VMID+PAL_HEIGHT)/2+1
y_start_pal	equ 30

vde_ntsc	equ (NTSC_VMID+NTSC_HEIGHT)/2+1
y_start_ntsc	equ 24
