
extern _ti_Open
extern _ti_Close
extern _ti_OpenVar
extern _ti_Write
extern _ti_GetDataPtr
extern _ti_GetSize
extern _gfx_End

public _update_program

_InsertMem     := $020514
_DelMem        := $020590
_asm_prgm_size := $D0118C
_userMem       := $D1A881
_EnoughMem     := $02051C

;void update_program(void);
_update_program:
	ld a,($E30018)
	cp a,$2D ;OS 16bpp
	call nz,_gfx_End
	ld hl,mode_r
	push hl
	ld hl,program_name
	push hl
	call _ti_Open
	pop bc
	pop bc
	or a,a
	ret z
	ld c,a
	push bc
	call _ti_GetDataPtr
	ld (dataptr),hl
	call _ti_GetSize
	ld (datasize),hl
	call _ti_Close
	pop bc
	ld hl,jump_data
	ld de,jump_data_loc
	ld bc,jump_data_len
	push de
	ldir
	ret
	
jump_data_loc:=$E30800
jump_data:
	or a,a
	sbc hl,hl
	ld de,(_asm_prgm_size)
	ld (_asm_prgm_size),hl
	ld hl,_userMem
	call _DelMem
	call _EnoughMem
	ret c
	ld hl,0
datasize:=$-3
	ld (_asm_prgm_size),hl
	push hl
	ld de,_userMem
	push de
	call _InsertMem
	pop de
	pop bc
	push de
	ld hl,0
dataptr:=$-3
	ldir
	ret
jump_data_len:=$-jump_data

mode_r:
	db "r",0
mode_w:
	db "w",0
program_name:
	db "TITREK",0
