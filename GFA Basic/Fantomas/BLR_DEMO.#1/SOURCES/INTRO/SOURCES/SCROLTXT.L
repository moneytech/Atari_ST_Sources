 movea.l 4(sp),a0       ; a0 = adresse buffer caractere
 movea.l 8(sp),a1       ; a1 = ecran travail

 movea.l a0,a2          ; Sauvegarde de a0
 movea.l a1,a3          ; Et de a1

 moveq #14,d0           ; 15 lignes

scroll:
 roxl   (a2)+           ; Scroll datas...
 roxl 152(a3)
 roxl 144(a3)
 roxl 136(a3)
 roxl 128(a3)
 roxl 120(a3)
 roxl 112(a3)
 roxl 104(a3)           ; Scrolle tous les 1ers
 roxl 96(a3)            ; Plans d'une ligne !
 roxl 88(a3)
 roxl 80(a3)
 roxl 72(a3)
 roxl 64(a3)
 roxl 56(a3)
 roxl 48(a3)
 roxl 40(a3)
 roxl 32(a3)
 roxl 24(a3)
 roxl 16(a3)
 roxl 8(a3)
 roxl (a3)
 lea 160(a3),a3        ;ligne suivante ...
 dbra d0,scroll

 moveq #14,d0          ; 15 lignes

scroll1:
 roxl (a0)+             ;scroll datas...
 roxl 152(a1)
 roxl 144(a1)
 roxl 136(a1)
 roxl 128(a1)
 roxl 120(a1)
 roxl 112(a1)
 roxl 104(a1)           ;scrolle tous les 1ers
 roxl 96(a1)            ;plans d'une ligne !
 roxl 88(a1)
 roxl 80(a1)
 roxl 72(a1)
 roxl 64(a1)
 roxl 56(a1)
 roxl 48(a1)
 roxl 40(a1)
 roxl 32(a1)
 roxl 24(a1)
 roxl 16(a1)
 roxl 8(a1)
 roxl (a1)
 lea 160(a1),a1
 dbra d0,scroll1
 rts

 end
 