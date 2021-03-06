;       RECONSTRUCTED WARP ROUTINE I INADVERTANTLY DESTROYED
POINTS  = 150   ; NUMBER OF POINTS
        BSS
        ALIGN
INTIN   DS.L    1
PTSIN   DS.L    1
DIR     DS.W    1
X       DS.B    POINTS
Y       DS.B    POINTS
VX      DS.B    POINTS
VY      DS.B    POINTS
COL     DS.B    POINTS
OVX     DS.B    POINTS
OVY     DS.B    POINTS

        TEXT
        ALIGN
        DC.W    $A000              ; INIT LINE A
        MOVE.L  8(A0),INTIN
        MOVE.L  $C(A0),PTSIN
INIT:
L0001D:                                 ; INIT
        LEA     X,A6                    ; LOAD DATA BASE ADR INTO A6
        MOVE.W  #POINTS-1,D7
L00017:
        MOVE.W  #17,-(SP)               ; FETCH RANDOM #
        TRAP    #14
        ADDQ.L  #2,SP
        BTST    #4,D0
        BEQ     L0000A
        OR.B    #$F0,D0
        BRA     L0000C
L0000A:
        AND.B   #$F,D0
L0000C:
        MOVE.B  D0,(POINTS*2)(A6)       ; VX
        NEG.B   D0
        MOVE.B  D0,(POINTS*5)(A6)       ; OVX
        SWAP    D0
        BTST    #4,D0
        BEQ     L00010
        OR.B    #$F0,D0
        BRA     L00011
L00010:
        AND.B   #$F,D0
L00011:
        MOVE.B  D0,(POINTS*3)(A6)       ; VY
        NEG.B   D0
        MOVE.B  D0,(POINTS*6)(A6)       ; OVY
        MOVE.B  D7,D0
        AND.B   #$F,D0
        ADD.B   #100,D0
        MOVE.B  D0,(POINTS)(A6)         ; Y
        MOVE.B  D7,D0
        LSR.B   #4,D0
        ADD.B   #128,D0
        MOVE.B  D0,(A6)+                ; X
        DBRA    D7,L00017

        JSR     DRAWPOINTS      ; INITIAL DRAW
        MOVEA.L PTSIN,A4
        MOVEA.L INTIN,A5
        JSR     CHKKEY

        MOVE.W  #16-1,D5
EXPLOSION:
        JSR     UNDRAWPOINTS
        JSR     CALC            ; CALCULATE NEW POINTS
        JSR     DRAWPOINTS
        DBRA    D5,EXPLOSION

        LEA     X,A6                    ; REVERSE DIRECRION
        MOVE.W  #(POINTS*2)-1,D7
COPY:                                   ; COPY OV? TO V?
        MOVE.B  (POINTS*5)(A6),(POINTS*2)(A6)
        ADDQ.L  #1,A6
        DBRA    D7,COPY

        MOVE.W  #16-1,D5
IMPLOSION:
        JSR     UNDRAWPOINTS
        JSR     CALC            ; CALCULATE NEW POINTS
        JSR     DRAWPOINTS
        DBRA    D5,IMPLOSION

        JMP INIT

DRAWPOINTS:
        MOVE.W  #POINTS-1,D7
        MOVEA.L PTSIN,A4
        MOVEA.L INTIN,A5
        LEA     X,A6
        CLR.L   (A4)
PLOT1:
        CMPI.B  #199,POINTS(A6)         ; BOUNDRY CHECK Y
        BHI     NODRAW1
        MOVE.B  (A6),1(A4)              ; LOAD X
        MOVE.B  POINTS(A6),3(A4)        ; LOAD Y
        DC.W    $A002                   ; GET POINT
        MOVE.B  D0,(POINTS*4)(A6)       ; STORE COLOR
        MOVE    #1,(A5)
        DC.W    $A001                   ; PLOT
NODRAW1:
        ADDQ.L  #1,A6
        DBRA    D7,PLOT1
        RTS

UNDRAWPOINTS:
        MOVE.W  #POINTS-1,D7
        LEA     X+POINTS-1,A6
        CLR.L   (A4)
        CLR.B   (A5)
PLOT2:
        CMPI.B  #199,POINTS(A6)         ; BOUNDRY CHECK Y
        BHI     NODRAW2
        MOVE.B  (A6),1(A4)              ; LOAD X
        MOVE.B  POINTS(A6),3(A4)        ; LOAD Y
        MOVE.B  (POINTS*4)(A6),1(A5)    ; STORE COLOR
        DC.W    $A001                   ; PLOT
NODRAW2:
        SUBQ.L  #1,A6
        DBRA    D7,PLOT2
        RTS


CALC:
        LEA     X,A6
        MOVE    #POINTS-1,D7
CALC1:
        TST.B   (POINTS*2)(A6)          ; TEST VX
        BEQ     NOVX                    ; 0 - LEAVE ALONE
        BPL     POSVX                   ; OTHERWISE DECELERATE
        ADDQ.B  #1,(POINTS*2)(A6)
        BRA     ADJX
POSVX:
        SUBQ.B  #1,(POINTS*2)(A6)
ADJX:
        MOVE.B  (POINTS*2)(A6),D0       ; X=X+VX
        ADD.B   D0,(A6)
NOVX:
        TST.B   (POINTS*3)(A6)          ; TEST VY
        BEQ     NOVY                    ; 0 LEAVE ALONE
        BPL     POSVY                   ; OTHERWISE DECELERATE
        ADDQ.B  #1,(POINTS*3)(A6)
        BRA     ADJY
POSVY:
        SUBQ.B  #1,(POINTS*3)(A6)
ADJY:
        MOVE.B  (POINTS*3)(A6),D0       ; Y=Y+VY
        ADD.B   D0,POINTS(A6)
NOVY:
        ADDQ    #1,A6
        DBRA    D7,CALC1
        RTS

CHKKEY:
        MOVE.W  #2,-(A7)                ; BCONIN
        MOVE.W  #1,-(A7)
        TRAP    #13
        ADDQ.L  #4,A7
        TST.W   D0
        BEQ     NOEXIT                  ; NO KEY NO EXIT
        CLR.W   -(A7)                   ; TERM
        TRAP    #1
NOEXIT  RTS

        END
 