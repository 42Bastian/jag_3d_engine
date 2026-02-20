;;; -*-asm-*-

	;; object definition
	RSRESET
	RSW	obj_visible		; object completely visible.
	RSW	obj_x
	RSW	obj_y
	RSW	obj_z
	RSW	obj_dummy		; current angles
	RSW	obj_angle_a
	RSW	obj_angle_b
	RSW	obj_angle_c
	RSL	obj_points		; address of original points
	RSL	obj_faces		; address of faces
	RSL	obj_normals		; original face normals
	RSL	obj_vnormals		; original vertex normals
	RSL	obj_rotated		; address rotated vertexes
	RSL	obj_normals_rotated	; rotated face normals
	RSL	obj_vnormals_rotated	; rotated vertex normals
	RSL	obj_projected		; final projectd points (incl. moving)
	RSL	obj_facesVisible	; list of visible faces and luminance
_3dobject_size equ RSCOUNT

	echo "3D objsize: %D _3dobject_size"

obj_angle	equ obj_dummy

	RSRESET
	RSL	pts_nPoints
	RSW	pts_dummy
	RSW	pts_x
	RSW	pts_y
	RSW	pts_z
pts_size	equ RSCOUNT

	;; triangle description

	RSRESET
	RSW	face_pt0
	RSW	face_pt1
	RSW	fave_pt2
	RSW	face_color
 IF TEXTURE <> 0
	RSL	face_ti_ptr	;texture info, == 0 => no texture
 ENDIF
face_size	equ RSCOUNT

//->	RSRESET
//->	RSW	tface_pt0
//->	RSW	tface_pt1
//->	RSW	tfave_pt2
//->	RSW	tface_color
//->	RSW	uv0
//->	RSW	uv1
//->	RSW	uv2
//->	RSW	tex_idx
//->tface_size	equ RSCOUNT

	RSRESET
	RSL	proj_z_org
	RSW	proj_x
	RSW	proj_y
proj_size	equ RSCOUNT

	RSRESET
	RSW	_3d_x
	RSW	_3d_y
	RSW	_3d_z
	RSW	_3d_dummy
_3d_size	equ RSCOUNT

	;; projected triangle for render list
	RSRESET
	RSL	tri_next
	RSB	tri_visible	; == 0 => visible
	RSB	tri_lum,3	; vertex (Gouraud) or triangle luminance
	RSL	tri_xy0		; projected x,y coordinate
	RSL	tri_xy1
	RSL	tri_xy2
 IF TEXTURE <> 0
	RSL	tri_ti_ptr	; texture info (== 0 => flat)
 ENDIF
_tri_size	equ RSCOUNT

	;; Texture info
	RSRESET
	RSW	ti_texure	; index
	RSW	ti_spare
	RSW	ti_u0		; u/v (TXT_FP fixpoint resolution)
	RSW	ti_v0
	RSW	ti_u1
	RSW	ti_v1
	RSW	ti_u2
	RSW	ti_v2
_tex_info_size 	EQU RSCOUNT

;;; ----------------------------------------
;;; macros to fill data objects
;;; ----------------------------------------

	macro face ; p1,p2,p3,p4,col
	dc.w \0,\1,\2,\4
	dc.w \2,\3,\0,\4
	endm

	macro tri			; p1,p2,p3,col
	dc.w \0,\1,\2
	dc.w \3
 IF TEXTURE <> 0
	dc.l 0			; dummy
 ENDIF
	endm

	macro ttri ; p1,p2,p3,texture
	dc.w \0,\1,\2
 IF TEXTURE <> 0
	dc.w $80
	dc.l \3			; texture data
 ELSE
	dc.w $8830
 ENDIF
	endm

 IF TEXTURE <> 0
	macro	tex_info
	dc.w	\0,0		; texture index
	dc.w	\1,\2		; u0/v0
	dc.w	\3,\4		; u1/v1
	dc.w	\5,\6		; u2/v2
	endm

	macro	tex_info_shifted
	dc.w	\0,0		; texture index
	dc.w	\1<<TXT_FP,\2<<TXT_FP		; u0/v0
	dc.w	\3<<TXT_FP,\4<<TXT_FP		; u1/v1
	dc.w	\5<<TXT_FP,\6<<TXT_FP		; u2/v2
	endm
 ENDIF
	;; vertex (half size)
	macro PT
 ifvar \3
	dc.w \0/2 ,\1/2 ,\2/2 ,\3
 else
	dc.w \0/2 ,\1/2 ,\2/2 ,0
 endif
	endm

	;; vertex (original size)
	macro _PT
	dc.w  \0 ,\1 ,\2 ,0
	endm

	;; normal
	macro NT
	dc.w  \0 ,\1 ,\2 ,0
	endm

	;; vertex noram
	macro VNT
	dc.w (\0) ,(\1) ,(\2) ,0
	endm
