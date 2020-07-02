
extern _ti_Open
extern _ti_Close
extern _ti_OpenVar
extern _ti_Write
extern _ti_GetDataPtr
extern _ti_GetSize
extern _gfx_End

public _update_program

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
	ld de,(ti.asm_prgm_size)
	ld (ti.asm_prgm_size),hl
	ld hl,ti.userMem
	call ti.DelMem
	call ti.EnoughMem
	ret c
	ld hl,0
datasize:=$-3
	ld (ti.asm_prgm_size),hl
	push hl
	ld de,ti.userMem
	push de
	call ti.InsertMem
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
