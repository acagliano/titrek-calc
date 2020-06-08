;user input subroutine for use with ez80 C toolchain programs
;author: Adam "beckadamtheinventor" Beckingham

.def _user_input
.ref _gfx_SetColor
.ref _gfx_PrintString
.ref _gfx_PrintChar
.ref _gfx_GetTextX
.ref _gfx_GetTextY
.ref _gfx_SetTextXY
.ref _gfx_SetTextFGColor
.ref _gfx_FillRectangle
.ref _gfx_SetDraw
.ref _gfx_BlitLines
.ref _kb_Scan

;bool user_input(char *buffer,size_t length);
_user_input:
	ld hl,-11
	call $12C ;ti._frameset
	xor a,a
	sbc hl,hl
	ld (ix-3),hl
	ld (ix-4),a
	ld (ix-7),hl
	ld (ix-10),hl
	ld hl,(ix+6)
	ld (hl),a
	push hl
	pop de
	inc de
	ld bc,(ix+9)
	ldir
	call _gfx_GetTextX
	ld (ix-7),hl
	call _gfx_GetTextY
	ld (ix-10),hl
	call _gfx_SetTextFGColor
	ld (ix-11),l
__draw:
	ld c,1
	push bc
	call _gfx_SetDraw
	ld l,0
	ex (sp),hl
	call _gfx_SetColor
	pop bc
	ld hl,320
	ld bc,(ix-7)
	or a,a
	sbc hl,bc
	ld de,9
	push de
	push hl
	ld hl,(ix-10)
	push hl
	push bc
	call _gfx_SetTextXY
	call _gfx_FillRectangle
	pop bc
	pop bc
	pop bc
	ld l,228
	ex (sp),hl
	call _gfx_SetTextFGColor
	ld hl,(ix+6)
	ex (sp),hl
	call _gfx_PrintString
	ld l,255
	ex (sp),hl
	call _gfx_SetTextFGColor
	ld bc,0
	ld hl,__overtypes
	ld c,(ix-4)
	add hl,bc
	ld l,(hl)
	ex (sp),hl
	call _gfx_PrintChar
	ld hl,9
	ex (sp),hl
	ld bc,(ix-10)
	push bc
	ld c,1
	push bc
	call _gfx_BlitLines
	pop bc
	pop bc
	pop bc
__keys:
	call __getkey
	cp a,56
	jq z,__delete
	cp a,15
	jq z,__exit
	cp a,54
	jq z,__nextmap
	cp a,9
	jq z,__enter
	jq c,__keys
	cp a,48
	jq z,__prevmap
	jq nc,__keys
	ld bc,0
	ld c,(ix-4)
	ld hl,__keymaps
	add hl,bc
	add hl,bc
	add hl,bc
	ld hl,(hl)
	sub a,10
	ld c,a
	add hl,bc
	ex hl,de
	ld hl,(ix-3)
	ld bc,(ix+9)
	or a,a
	inc hl
	sbc hl,bc
	jq nc,__keys
	add hl,bc
	ld (ix-3),hl
	dec hl
	ld a,(de)
	ld bc,(ix+6)
	add hl,bc
	ld (hl),a
	inc hl
	ld (hl),0
	jq __draw
__delete:
	ld hl,(ix+6)
	ld bc,(ix-3)
	ld a,(ix-1)
	or a,b
	or a,c
	jq z,__draw
	dec bc
	add hl,bc
	ld (hl),0
	ld (ix-3),bc
	jq __draw
__enter:
	ld a,1
	jr __return
__exit:
	xor a,a
	ld hl,(ix+6)
	ld (hl),a
__return:
	push af
	ld c,(ix-11)
	push bc
	call _gfx_SetTextFGColor
	pop bc
	pop af
	ld sp,ix
	pop ix
	ret
__prevmap:
	ld a,(ix-4)
	or a,a
	jr nz,__decmap
	ld a,3
__decmap:
	dec a
__setmap:
	ld (ix-4),a
	jq __draw
__nextmap:
	ld a,(ix-4)
	inc a
	cp a,3
	jr nz,__setmap
	xor a,a
	jr __setmap


__getkey:
	call __get_csc
	or a,a
	jr z,__getkey
	push af
__unpress_loop:
	call __get_csc
	or a,a
	jr nz,__unpress_loop
	pop af
	ret
__get_csc: ;my routine I used in both OpenCE and BOS lol
	call _kb_Scan
	ld hl,$F50012
	ld b,7
	ld c,49
__key_scanloop:
	ld a,(hl)
	or a,a
	jr nz,__keyispressed
	inc hl
	inc hl
	ld a,c
	sub a,8
	ld c,a
	djnz __key_scanloop
	xor a,a
	ret
__keyispressed:
	ld b,8
__keybitloop:
	rrca
	jr c,__return_this_key
	inc c
	djnz __keybitloop
__return_this_key:
	ld a,c
	ret

__keymaps:
	dl __keymap_A,__keymap_a,__keymap_1
__keymap_A:
	db "#WRMH  ?!VQLG  :ZUPKFC  YTOJEB  XSNIDA"
__keymap_a:
	db "#wrmh  ?!vqlg  :zupkfc  ytojeb  xsnida"
__keymap_1:
	db "+-*/^  ;369)$@ .258(&~ 0147,][  ",$1A,"<=>}{"
__overtypes:
	db "Aa1"
