  text
  ilabel \assempro\tos\tos.l
  gem_init
start:
        CLR.L   FILE
        CLR.L   FILE+4
        CLR.L   FILE+8
        CLR.L   FILE+12
  fsel_input path,file,status,ebutton   ; display file selector
  cmp #0,ebutton        ; no cancel ?
  beq myexit

  lea afterpath+1,a0    ; put file at end of pathneme
  lea file,a1
srch:                   ; first search for end
  cmp.b #'\',-(a0)
  bne srch
  addq.l #1,a0
tackfileon:
  move.b (a1)+,(a0)+    ; move a chr
  bne tackfileon        ; not a 0 keep going

  move #2,-(sp)         ; open prom file
  move.l #path,-(sp)
  move #$3d,-(sp)
  trap #1
  addq.l #8,sp
  tst.l d0
  bmi error
  move d0,hand

  move.l #dta,-(sp)     ; setup disk data area
  move #$1a,-(sp)
  trap #1
  addq.l #6,sp

  move #0,-(sp)         ; fetch len
  move.l #path,-(sp)
  move #$4e,-(sp)
  trap #1
  addq.l #8,sp
  tst.l d0
  bmi error

  move.l dta+26,d7      ; length of file

  move.l #buffer,-(sp)  ; load all the data
  move.l d7,-(sp)
  move hand,-(sp)
  move #$3f,-(sp)
  trap #1
  add.l #12,sp
  tst.l d0
  bmi error

  move hand,-(sp)     ; close file
  move #$3e,-(sp)
  trap #1
  addq.l #4,sp
  tst.l d0
  bmi error

  lea buffer,a0         ; buffer adr
  ADD.L A0,D7
calc:                   ; calculate the sum
  SUB.b #128,(a0)+
        CMP.L  A0,D7
        BNE CALC

  move #2,-(sp)         ; open prom file
  move.l #path,-(sp)
  move #$3d,-(sp)
  trap #1
  addq.l #8,sp
  tst.l d0
  bmi error
  move d0,hand

  move.l #buffer,-(sp)  ; WRITE all the data
  move.l dta+26,-(SP)   ; length of file
  move hand,-(sp)
  move #$40,-(sp)
  trap #1
  add.l #12,sp
  tst.l d0
  bmi error

  move hand,-(sp)     ; close file
  move #$3e,-(sp)
  trap #1
  addq.l #4,sp
  tst.l d0
  bmi error

        JMP     START
myexit:
  gem_exit
  clr.w -(sp)
  trap #1

error:
  neg.l d0              ; make error code pcdos code
  sub.l #31,d0
  form_error d0,d0
  bra start

  data
hand:
  dc.w 0
ebutton:
  dc.w 0
status:
  dc.w 0
path:
        DC.B    'D:\TEMP\*.SPL'
  ds.b 64,0
afterpath:

  bss
  align
dta:
  ds.b 44
buffer:
  ds.b 200000
FILE:
        DS.L    4,0
  end




 