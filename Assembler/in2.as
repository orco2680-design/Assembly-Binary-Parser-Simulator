Label1: clr r1
	mcro a5555
	add r1, r5
	.data 1,2,3,4,5
	mcroend
	.extern Label4
	mov r1, r2
Label3: jmp Label1
	.data 1,1,1
;docoumant
	inc MAT[r5][r6]
	a5555
Label2: prn #-8
	add #3 , Label4
	not r4

MAT: .mat [2][3] 1,2,3,4,5,6


