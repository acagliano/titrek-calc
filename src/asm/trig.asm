
.assume adl = 1
XDEF _ByteATan

_ByteATan:
; Pop return address and argument
; Push return address
    pop hl
    pop de
    push hl
;D.E is x
;returns atan(D.E)*128/pi
;if DE is negative, return -atan(-D.E)
    ld a,d
    or a
    jr nc,jp1
    sbc hl,hl
    sbc hl,de
    ex de,hl
    call jp1
    neg
    ret
jp1:
;at this point, DE is positive
;if D!=0, then we need to return 64-atan(1.0/D.E)
    inc d
    dec d
    jr z,jp2
    call InvDE
    call jp2
    cpl
    add a,65
    ret
jp2:
    ld hl,atan128LUT
    add hl,de
    ld a,(hl)
    ret
InvDE:
;Special case, D.E >= 1.0
;returns 1.0/D.E
;Note: In the case D.E=1.0, actually returns 255/256, but in this context it is fine for the atan routine. Otherwise, we'd just need to add a .db $20 to the end of the LUT, but that's just a copy of 255/256 :P
    ld hl,256
;  or a
;  sbc hl,de
;  ret z
;  add hl,de
    ld b,8
jp3:
    add.s hl,hl
    sbc.s hl,de
    jr nc,$+3
    add hl,de
    rla
    djnz jp3
    cpl
    ld e,a
    ld d,b
    ret

atan128LUT:
.db $00,$00,$00,$00,$01,$01,$01,$01,$01,$01,$02,$02,$02,$02,$02,$02
.db $03,$03,$03,$03,$03,$03,$03,$04,$04,$04,$04,$04,$04,$05,$05,$05
.db $05,$05,$05,$06,$06,$06,$06,$06,$06,$06,$07,$07,$07,$07,$07,$07
.db $08,$08,$08,$08,$08,$08,$08,$09,$09,$09,$09,$09,$09,$0a,$0a,$0a
.db $0a,$0a,$0a,$0a,$0b,$0b,$0b,$0b,$0b,$0b,$0b,$0c,$0c,$0c,$0c,$0c
.db $0c,$0c,$0d,$0d,$0d,$0d,$0d,$0d,$0d,$0e,$0e,$0e,$0e,$0e,$0e,$0e
.db $0f,$0f,$0f,$0f,$0f,$0f,$0f,$10,$10,$10,$10,$10,$10,$10,$11,$11
.db $11,$11,$11,$11,$11,$11,$12,$12,$12,$12,$12,$12,$12,$13,$13,$13
.db $13,$13,$13,$13,$13,$14,$14,$14,$14,$14,$14,$14,$14,$15,$15,$15
.db $15,$15,$15,$15,$15,$15,$16,$16,$16,$16,$16,$16,$16,$16,$17,$17
.db $17,$17,$17,$17,$17,$17,$17,$18,$18,$18,$18,$18,$18,$18,$18,$18
.db $19,$19,$19,$19,$19,$19,$19,$19,$19,$19,$1a,$1a,$1a,$1a,$1a,$1a
.db $1a,$1a,$1a,$1b,$1b,$1b,$1b,$1b,$1b,$1b,$1b,$1b,$1b,$1c,$1c,$1c
.db $1c,$1c,$1c,$1c,$1c,$1c,$1c,$1c,$1d,$1d,$1d,$1d,$1d,$1d,$1d,$1d
.db $1d,$1d,$1d,$1e,$1e,$1e,$1e,$1e,$1e,$1e,$1e,$1e,$1e,$1e,$1f,$1f
.db $1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$20,$20,$20,$20,$20,$20
