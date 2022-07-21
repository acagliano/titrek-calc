;user input subroutine for use with ez80 C toolchain programs
;author: Adam "beckadamtheinventor" Beckingham

section .text

public _user_input
extern _gfx_SetColor
extern _gfx_PrintChar
extern _gfx_GetTextX
extern _gfx_GetTextY
extern _gfx_SetTextXY
extern _gfx_SetTextFGColor
extern _gfx_HorizLine
extern _gfx_FillRectangle
extern _gfx_SetDraw
extern _gfx_Blit
extern _getKey
extern _kb_AnyKey

;uint8_t user_input(char *buffer, size_t length ,uint8_t flags);
_user_input:
    call _kb_AnyKey
    jq nz,_user_input
	ld hl,-11
	call $12C ;ti._frameset
	xor a,a
	sbc hl,hl
	ld (ix-3),hl
	ld (ix-4),a
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
	ld (ix-11),a
__draw:
	ld c,1
	push bc
	call _gfx_SetDraw
	ld l,0
	ex (sp),hl
	call _gfx_SetColor
	ld hl,9
__height_of_text:=$-3
	ex (sp),hl
	ld hl,320
	ld bc,(ix-7)
	or a,a
	sbc hl,bc
	push hl
	ld hl,(ix-10)
	push hl
	push bc
	call _gfx_FillRectangle
	pop bc
	pop bc
	pop bc
	ld hl,(ix-10)
	ex (sp),hl
	ld bc,(ix-7)
	push bc
	call _gfx_SetTextXY
	pop bc
	ld l,228
	ex (sp),hl
	call _gfx_SetTextFGColor
	pop bc
	ld hl,(ix+6)
	bit 0,(ix+12) ;test if we should print the string, or asterisks
	jq z,__normal_string
	ld c,'*'
__print_asterisk_loop:
	ld a,(hl)
	or a,a
	jq z,__print_overtype
	push hl
	push bc
	call __PrintChar
	pop bc
	pop hl
	inc hl
	jq __print_asterisk_loop
__normal_string:
	ld a,(hl)
	or a,a
	jq z,__print_overtype
	push hl
	ld c,a
	push bc
	call __PrintChar
	pop bc
	pop hl
	inc hl
	jq __normal_string
__print_overtype:
	ld c,$FF
	push bc
	call _gfx_SetColor
	ld l,$FF
	ex (sp),hl
	call _gfx_SetTextFGColor
	pop bc
	ld bc,0
	ld hl,__overtypes
	ld c,(ix-4)
	add hl,bc
	ld l,(hl)
	push hl
	call _gfx_GetTextY
	ld bc,8
	push bc
	add hl,bc
	push hl
	call _gfx_GetTextX
	push hl
	call _gfx_HorizLine
	pop bc
	pop bc
	pop bc
	call __PrintChar
	ld l,1
	ex (sp),hl
	call _gfx_Blit
	pop bc
__keys:
	call _getKey
	or a,a
	jq z,__keys
	push af
__wait_key_off_loop:
	call _kb_AnyKey
	jq nz,__wait_key_off_loop
	pop af
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
	ld b,(ix-4)
	ld c,38
	mlt bc
	ld hl,__keymaps
	add hl,bc
	sub a,10
	call $021DB4 ;ti.AddHLAndA
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
__return2:
	ld a,2
	jq __return
__enter:
	ld a,1
	jq __return
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
	jq nz,__decmap
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
	jq nz,__setmap
	xor a,a
	jq __setmap


__PrintChar:
	call _gfx_GetTextX
	ld bc,300
	or a,a
	sbc hl,bc
	jq c,__PrintChar_Draw
	push hl
	call _gfx_GetTextY
	ld bc,9
	add hl,bc
	ld (__PrintChar_NextLineY),hl
	ex hl,de
	ld bc,(ix-7)
	ld hl,320
	or a,a
	sbc hl,bc
	ld bc,9
	push bc,hl
	ld bc,(ix-7)
	push de,bc
	ld c,0
	push bc
	call _gfx_SetColor
	pop bc
	call _gfx_FillRectangle
	pop bc,bc,bc,bc
	pop hl
	ld bc,10
	or a,a
	sbc hl,bc
	jq c,__PrintChar_Draw
	ld hl,0
__PrintChar_NextLineY:=$-3
	ld bc,(ix-7)
	push hl,bc
	call _gfx_SetTextXY
	pop bc,bc
__PrintChar_Draw:
	pop hl
	ld (__PrintChar_return_smc),hl
	call _gfx_PrintChar
	jp _user_input
__PrintChar_return_smc:=$-3

__keymaps:
	db "#WRMH  ?!VQLG  :ZUPKFC  YTOJEB  XSNIDA"
	db "#wrmh  ?!vqlg  :zupkfc  ytojeb  xsnida"
	db "+-*/^  ;369)$@ .258(&~ 0147,][  ",$1A,"<=>}{"
__overtypes:
	db "Aa1"

