bigship_normals:
  dc.l 26
  CM_NT   -14, 4093,  174 	; 0
  CM_NT -3589,-1967, -153 	; 1
  CM_NT  3518,-2040,  489 	; 2
  CM_NT   331,  -67,-4083 	; 3
  CM_NT   -25,-4095,  -82 	; 4
    CM_NT   -25,-4095,  -82 	; 4
  CM_NT    25, 4096,    3 	; 5
    CM_NT    25, 4096,    3 	; 5
  CM_NT  3006,-2741,  479 	; 6
  CM_NT  2920, 2832,  482 	; 7
  CM_NT     0, 4074,  424 	; 8
  CM_NT -2964, 2790,  453 	; 9
  CM_NT -2922,-2836,  447 	; 10
  CM_NT   104,-4073,  418 	; 11
  CM_NT     0,    0,-4096 	; 12
    CM_NT     0,    0,-4096 	; 12
      CM_NT     0,    0,-4096 	; 12
        CM_NT     0,    0,-4096 	; 12
  CM_NT    25,-4095,  -82 	; 13
    CM_NT    25,-4095,  -82 	; 13
  CM_NT   -25, 4096,    4 	; 14
    CM_NT   -25, 4096,    4 	; 14
  CM_NT    14, 4093,  174 	; 15
  CM_NT  3543,-2049, -155 	; 16
  CM_NT -3563,-1958,  500 	; 17
  CM_NT  -331,  -57,-4083 	; 18

bigship_points:
  dc.l 23
  CM_PT  96, 32,-131
  CM_PT  59, 32,-134
  CM_PT  77, -1,-132
  CM_PT  54, 21,125

  CM_PT 210,  4,-55
  CM_PT  22,  2, 39
  CM_PT 219,  4,-120
  CM_PT  53,  5,-120

  CM_PT  0,  0,207

  CM_PT  21,-33,-120
  CM_PT  52,  0,-120
  CM_PT  20, 34,-120

  CM_PT -20, 34,-120
  CM_PT -52,  0,-120
  CM_PT -21,-33,-120

  CM_PT -210,  4,-55
  CM_PT -22,  2, 39
  CM_PT -219,  4,-120
  CM_PT -53,  5,-120

  CM_PT -96, 32,-131
  CM_PT -59, 32,-134
  CM_PT -77, -1,-132
  CM_PT -54, 21,125

bigship_texture_info:
	tex_info_shifted 4, 0,0,63,0,63,63	 	; front
	tex_info_shifted 4, 0,0,63,63,0,63 		; front

bigship_faces:
	dc.l 26
; 0
    cm_tri 3,0,1,$003c
; 1
    cm_tri 3,1,2,$003c
; 2
    cm_tri 3,2,0,$003c
; 3
    cm_tri 2,1,0,$003c

; 4 right wing bottom
    cm_tri 5,7,6,$7dc
    cm_tri 5,6,4,$7dc
; 5 right wing top

  cm_ttri 4, 6, 7, bigship_texture_info
  cm_ttri 4, 7, 5, bigship_texture_info+_tex_info_size

//->    cm_tri 4,6,7,$f0dc
//->    cm_tri 4,7,5,$f0dc
; 6
    cm_tri 8,9,10,$785c
; 7
    cm_tri 8,10,11,$785c
; 8
    cm_tri 8,11,12,$785c
; 9
    cm_tri 8,12,13,$785c
; 10
    cm_tri 8,13,14,$785c
; 11
    cm_tri 8,14,9,$785c
; 12
    cm_tri 13,12,11,$785c
    cm_tri 13,11,10,$785c
    cm_tri 13,10,9,$785c
    cm_tri 13,9,14,$785c

; 13 left wing bottom
    cm_tri 17,18,16,$7dc
    cm_tri 17,16,15,$7dc
; 14 left wing up
  cm_ttri 18, 17, 15, bigship_texture_info
  cm_ttri 18, 15, 16, bigship_texture_info+_tex_info_size

//->    cm_tri 18,17,15,$f0dc
//->    cm_tri 18,15,16,$f0dc

; 15
    cm_tri 20,19,22,$003c
; 16
    cm_tri 21,20,22,$003c
; 17
    cm_tri 19,21,22,$003c
; 18
    cm_tri 21,19,20,$003c
; real f-count 26
