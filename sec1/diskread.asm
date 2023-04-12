
PRG_SPACE equ 0x8000

read_disk:

	mov ah, 0x02
	mov bx, PRG_SPACE
	mov al, 128
	mov dl, [BOOT_DISK]
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02

	int 0x13

	;jc DiskReadFailed

	ret

BOOT_DISK:
	db 0

DiskReadErrorString:
	db 'Disk Read Failed',0

DiskReadFailed:
	mov ax, DiskReadErrorString
	call print

	jmp $