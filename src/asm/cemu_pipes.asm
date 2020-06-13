public _cemu_check
public _cemu_get
public _cemu_send

check_cmd = 2
send_cmd = 3
get_cmd = 4

dbgext = 0xFD0000

_cemu_check:
	xor	a,a
	ld	hl,dbgext
	ld	(hl),check_cmd
	ret

_cemu_send:
	pop	de
	pop	hl
	pop	bc
	push	bc
	push	hl
	push	de

	ld	a,send_cmd
	ld	(dbgext),a

	push	bc
	pop	hl

	ret

_cemu_get:
	pop	hl
	pop	de
	pop	bc
	push	bc
	push	de
	push	hl

	ld	a,get_cmd
	ld	(dbgext),a

	push	bc
	pop	hl

	ret