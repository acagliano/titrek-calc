
public _getKey
public _getKeyAsync
extern _kb_AnyKey
extern _kb_Scan

;sk_key_t getKey(void);
_getKey:
	call _getKeyAsync
	or a,a
	ret z
	push af
__getKeyWaitUnpress:
	call _getKeyAsync
	or a,a
	jr nz,__getKeyWaitUnpress
	pop af
	ret
_getKeyAsync:
	call _kb_Scan
	ld hl,$F50012
	ld b,7
	ld c,49
__scanloop:
	ld a,(hl)
	or a,a
	jr nz,__keyispressed
	inc hl
	inc hl
	ld a,c
	sub a,8
	ld c,a
	djnz __scanloop
	xor a,a
	ret
__keyispressed:
	ld b,8
__keybitloop:
	rrca
	jr c,__return_this_keycode
	inc c
	djnz __keybitloop
__return_this_keycode:
	ld a,c
	ret

