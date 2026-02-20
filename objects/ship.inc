;-*-asm-*-
	.align 4

ship_points:
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
ship_texture_info:
	tex_info_shifted 1, 0,0,63,0,63,63	 	; front
	tex_info_shifted 1, 0,0,63,63,0,63 		; front
	tex_info_shifted 3, 0,0,63,0,63,63 		; right
	tex_info_shifted 3, 0,0,63,63,0,63 		; right
	tex_info_shifted 0, 0,0,63,0,63,63	 	; back
	tex_info_shifted 0, 0,0,63,63,0,63 		; back
	tex_info_shifted 2, 0,0,63,0,63,63	 	; left
	tex_info_shifted 2, 0,0,63,63,0,63 		; left
 ENDIF
ship_faces:
	dc.l 12

 IF TEXTURE = 100
  ttri 0, 1, 5, ship_texture_info
  ttri 0, 5, 4, ship_texture_info+_tex_info_size
  ttri 1, 2, 6, ship_texture_info+_tex_info_size*2
  ttri 1, 6, 5, ship_texture_info+_tex_info_size*3
  ttri 2, 3, 7, ship_texture_info+_tex_info_size*4
  ttri 2, 7, 6, ship_texture_info+_tex_info_size*5
  ttri 3, 0, 4, ship_texture_info+_tex_info_size*6
  ttri 3, 4, 7, ship_texture_info+_tex_info_size*7

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

	align	4
ship_normals:
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

ship_vnormals:
 dc.l  8
 VNT  147 , 147 , 147
 VNT  -147 , 147 , 147
 VNT  -147 , 147 , -147
 VNT  147 , 147 , -147
 VNT  147 , -147 , 147
 VNT  -147 , -147 , 147
 VNT  -147 , -147 , -147
 VNT  147 , -147 , -147
