
[org 0x7c00]

boot:
    jmp main
    times 3-($-$$) DB 0x90

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
    fileSysType:       db    "FAT16   "

main:
    cli

    xor ax, ax     ; Explicitly set DS and ES to 0
    mov ds, ax
    mov es, ax

    sti

    mov [BOOT_DISK], dl

    mov bp, 0x7c00
    mov sp, bp

    call read_disk

    mov ax, 0x0013
    int 0x10

    jmp PRG_SPACE

%include "../sec1/print.asm"
%include "../sec1/diskread.asm"

times 510-($-$$) db 0

dw 0xAA55