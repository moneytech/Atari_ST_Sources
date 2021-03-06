;------------------------------------------------------------------------------
;                       ANIMATE SHIP SMASHER
;------------------------------------------------------------------------------
;
SMASH:
        ORI.B      #0,SMASHFLG1
        BNE        SM3
        MOVE.B     BBFLG1,D0
        BNE        SM9
        BTST       #0,STIMER+3
        BEQ        SM9
;
        LEA        SMASHPARM,A0                ;GET BB PARMS
        MOVE.W     WINDY,D0                ;GET SCREEN WINDOW YCORD
        AND.W      #$FFF0,D0
        ADD.W      #(9*16),D0                ;MOVE DOWN 7 BLOCKS
        MOVE.W     D0,YCORD(A0)                ;SET YCORD
        MOVE.W     #3-1,D3
SM1:
        JSR        FRANDOM
        MOVE.B     D0,D1
        LSL        #1,D1
        JSR        FRANDOM
        EOR.W      D1,D0
        AND        #$1FF,D0
        ADD.B      #(640-512)/2,D0
        MOVE       D0,D2
        JSR        FIND_CHAR
        CMPI.W     #236,D0                        ;OVER WATER?
        BLS        SM2
        DBRA       D3,SM1
        RTS
SM2:
        MOVE.W     D2,XCORD(A0)
        CLR.B      ANIMSEQ(A0)
        MOVE.B     #1,SMASHFLG1
        MOVE.L     STIMER,D0
        ADD.L      #$40,D0
        MOVE.L     D0,SMASHTMR
SM9:
        RTS
;
SM3:
        LEA        SMASHPARM,A0                ;GET BB PARMS
        LEA        SHIPPARM,A2
        CLR.L      D0
        CLR.L      D1
        MOVE.W     XCORD(A0),D0
        MOVE.W     YCORD(A0),D1
        CMPI.B     #2,ANIMSEQ(A0)
        BHI        SM11
        MOVE.L     STIMER,D4
        CMP.L      SMASHTMR,D4
        BLS        SM4
        MOVE.B     #3,ANIMSEQ(A0)
        BRA        SM6
SM4:
        CMPI.B     #2,ANIMSEQ(A0)
        BLT        SM5
        MOVE.B     #$FF,ANIMSEQ(A0)
SM5:
        ADDQ.B     #1,ANIMSEQ(A0)
SM6:
        CMP.W      XCORD(A2),D0           ;CHECK FOR PROXIMITY TO SHIP
        BEQ        SM8
        BHI        SM7
        ADDQ.W     #2,D0
        BRA        SM8
SM7:
        SUBQ.W     #2,D0
SM8:
        CMP.W      YCORD(A2),D1
        BEQ        SM10A
        BHI        SM10
        ADDQ.W     #2,D1
        BRA        SM10A
SM10:
        SUBQ.W     #2,D1
SM10A:
        MOVE.W     D0,D4
        MOVE.W     D1,D5
        JSR        GET_CHAR
        CMPI.W     #236,D0                 ;OVER WATER?
        BHI        SM12A                   ;NO. OVER LAND. KILL IT
        MOVE.W     D4,XCORD(A0)
        MOVE.W     D5,YCORD(A0)
        RTS
;
SM11:
        MOVE.W     D0,D2
        MOVE.W     D1,D3
        MOVE.W     #$F,D4
        ADD.W      D4,D2
        ADD.W      D4,D3
        CMP.W      XCORD(A2),D2
        BLS        SM12
        CMP.W      YCORD(A2),D3
        BLS        SM12
        MOVE.W     XCORD(A2),D2
        ADD.W      D4,D2
        CMP.W      D2,D0
        BHI        SM12
        MOVE.W     YCORD(A2),D2
        ADD.W      D4,D2
        CMP.W      D2,D1
        BHI        SM12
;
        CLR.B      SMASHFLG1                   ;COLLISION
        JMP        FTEND2                      ;SET EXPLOSION
SM12:
        CMPI.B     #3,ANIMSEQ(A0)
        BLS        SM13
SM12A:
        CLR.B      SMASHFLG1
        RTS
SM13:
        ADDQ.B     #1,ANIMSEQ(A0)
        BRA        SM6

    END
 