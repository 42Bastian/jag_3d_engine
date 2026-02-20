;-*-asm-*-
	.align 4

cube_points:
	DC.L 8
	PT -32,-32,-32
	PT  32,-32,-32
	PT  32,-32, 32
	PT -32,-32, 32
	PT -32, 32,-32
	PT  32, 32,-32
	PT  32, 32, 32
	PT -32, 32, 32

 IF TEXTURE <> 0
cube_texture_info:
	tex_info_shifted 1, 0,0,63,0,63,63	 	; front
	tex_info_shifted 1, 0,0,63,63,0,63 		; front
	tex_info_shifted 3, 0,0,63,0,63,63 		; right
	tex_info_shifted 3, 0,0,63,63,0,63 		; right
	tex_info_shifted 0, 0,0,63,0,63,63	 	; back
	tex_info_shifted 0, 0,0,63,63,0,63 		; back
	tex_info_shifted 2, 0,0,63,0,63,63	 	; left
	tex_info_shifted 2, 0,0,63,63,0,63 		; left
 ENDIF
cube_faces:
	dc.l 12

 IF TEXTURE <> 0
  ttri 0, 1, 5, cube_texture_info
  ttri 0, 5, 4, cube_texture_info+_tex_info_size
  ttri 1, 2, 6, cube_texture_info+_tex_info_size*2
  ttri 1, 6, 5, cube_texture_info+_tex_info_size*3
  ttri 2, 3, 7, cube_texture_info+_tex_info_size*4
  ttri 2, 7, 6, cube_texture_info+_tex_info_size*5
  ttri 3, 0, 4, cube_texture_info+_tex_info_size*6
  ttri 3, 4, 7, cube_texture_info+_tex_info_size*7

   tri 3, 2, 1,$8f3f		; bottom
   tri 3, 1, 0,$8f3f		; bottom
   tri 4, 5, 6,$cf00		; top
   tri 4, 6, 7,$cf00		; top
 ELSE
   tri 0, 1, 5,$013f		; front
   tri 0, 5, 4,$013f		; front
   tri 1, 2, 6,$ff2f		; right
   tri 1, 6, 5,$ff2f		; right
   tri 2, 3, 7,$c93f		; back
   tri 2, 7, 6,$c93f		; back
   tri 3, 0, 4,$713f		; left
   tri 3, 4, 7,$713f		; left
   tri 3, 2, 1,$8f3f		; bottom
   tri 3, 1, 0,$8f3f		; bottom
   tri 4, 5, 6,$cf00		; top
   tri 4, 6, 7,$cf00		; top
 ENDIF
cube2_faces:
	dc.l 12
 IF TEXTURE <> 0
  tri 0, 1, 5, %1001011<<9 		; front
  tri 0, 5, 4, %1001101<<9 		; front
  tri 1, 2, 6, %1001011<<9 		; right
  tri 1, 6, 5, %1001101<<9 		; right
  tri 2, 3, 7, %1001011<<9 		; back
  tri 2, 7, 6, %1001101<<9 		; back
  tri 3, 0, 4, %1001011<<9 		; left
  tri 3, 4, 7, %1001101<<9 		; left
  tri 3, 2, 1, %1001011<<9 		; bottom
  tri 3, 1, 0, %1001101<<9 		; bottom
  tri 4, 5, 6, %1001011<<9 		; top
  tri 4, 6, 7, %1001101<<9 		; top
 ELSE
 tri 0, 1, 5,$883f		; front
 tri 0, 5, 4,$883f		; front
 tri 1, 2, 6,$882f		; right
 tri 1, 6, 5,$882f		; right
 tri 2, 3, 7,$883f		; back
 tri 2, 7, 6,$883f		; back
 tri 3, 0, 4,$883f		; left
 tri 3, 4, 7,$883f		; left
 tri 3, 2, 1,$883f		; bottom
 tri 3, 1, 0,$883f		; bottom
 tri 4, 5, 6,$8800		; top
 tri 4, 6, 7,$8800		; top
endif

	align	4
cube_normals:
cube2_normals:
	dc.l	12
 NT 0 , 0 , 256
 NT 0 , 0 , 256
 NT -256 , 0 , 0
 NT -256 , 0 , 0
 NT 0 , 0 , -256
 NT 0 , 0 , -256
 NT 256 , 0 , 0
 NT 256 , 0 , 0
 NT 0 , 256 , 0
 NT 0 , 256 , 0
 NT 0 , -256 , 0
 NT 0 , -256 , 0

cube_vnormals:
cube2_vnormals:
 dc.l  8
 VNT  147 , 147 , 147
 VNT  -147 , 147 , 147
 VNT  -147 , 147 , -147
 VNT  147 , 147 , -147
 VNT  147 , -147 , 147
 VNT  -147 , -147 , 147
 VNT  -147 , -147 , -147
 VNT  147 , -147 , -147
