
comm =	 -6
resp =	-16
zahl =	  0

.globl _exp10
.text
.even

_exp10:
	lea	0xfffa50,a0
	movew	#0x5412,a0@(comm)	| tentox -> fp0
	cmpiw	#0x8900,a0@(resp)	| check
	movel	a7@(4),a0@		| load arg_hi
	movel	a7@(8),a0@		| load arg_low
	movew	#0x7400,a0@(comm)	| result to d0
# wait
	.long	0x0c688900, 0xfff067f8
	movel	a0@,d0
	movel	a0@,d1
 	rts
