
[org 0x7c00]

boot:
    jmp main
    TIMES 3-($-$$) DB 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    ; Dos 4.0 EBPB 1.44MB floppy
    OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
    bytesPerSector:    dw    512
    sectPerCluster:    db    1
    reservedSectors:   dw    1
    numFAT:            db    2
    numRootDirEntries: dw    224
    numSectors:        dw    2880
    mediaType:         db    0xf0
    numFATsectors:     dw    9
    sectorsPerTrack:   dw    18
    numHeads:          dw    2
    numHiddenSectors:  dd    0
    numSectorsHuge:    dd    0
    driveNum:          db    0
    reserved:          db    0
    signature:         db    0x29
    volumeID:          dd    0x2d7e5a1a
    volumeLabel:       db    "NO NAME    "
    fileSysType:       db    "FAT12   "

main:
    cli

    xor ax, ax     ; Explicitly set DS and ES to 0
    mov ds, ax
    mov es, ax

    sti

    ;mov ax, 01h
    ;int 0x10

    mov [BOOT_DISK], dl

    mov bp, 0x7c00
    mov sp, bp

    mov ah, 0x0e
    mov al, 'H'
    int 0x10

    call read_disk

    jmp PRG_SPACE

%include "../lib/print.asm"
%include "../lib/diskread.asm"

boot_str: db 'Booting...',0

times 510-($-$$) db 0

dw 0xAA55