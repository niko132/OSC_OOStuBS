; EXPORTED FUNCTIONS

[GLOBAL reset_sb]
[GLOBAL programm_sb]
[GLOBAL ack_int_sb]
[GLOBAL single_mode_sb]
[GLOBAL make_int]

; FUNCTION IMPLEMENTATIONS

[SECTION .text]

reset_sb:
    push   rbp
	mov    rbp, rsp
	mov    rdx, rdi

    mov dx, 0x226
    mov al, 1
    out dx, al      ; reset port

    mov cx, 10
delay:
    loop delay      ; just delay a bit here

    mov dx, 0x226
    mov al, 0
    out dx, al      ; reset port

    mov dx, 0x22C
    mov al, 0xD1
    out dx, al      ; turn the speaker on

	pop    rbp
	ret

programm_sb:
    push   rbp
	mov    rbp, rsp
	mov    rdx, rdi

    mov dx, 0x22C
    mov al, 0x40
    out dx, al      ; set the time constant

    mov dx, 0x22C
    mov al, 233     ; was 131
    out dx, al      ; mono @ 8000Hz

    mov dx, 0x22C
    mov al, 0x14
    out dx, al      ; single mode

    mov dx, 0x22C
    mov al, 0xFF
    out dx, al      ; LOW BYTE OF COUNT (-1)

    mov dx, 0x22C
    mov al, 0x1F
    out dx, al      ; HIGH BYTE OF COUNT (-1)

	pop    rbp
	ret

ack_int_sb:
    push   rbp
	mov    rbp, rsp
	mov    rdx, rdi

    in al, 0x22E

	pop    rbp
	ret

single_mode_sb:
    push   rbp
	mov    rbp, rsp
	mov    rdx, rdi

    mov dx, 0x22C
    mov al, 0x14
    out dx, al      ; single mode

    mov dx, 0x22C
    mov al, 0xFF
    out dx, al      ; LOW BYTE OF COUNT (-1)

    mov dx, 0x22C
    mov al, 0x1F
    out dx, al      ; HIGH BYTE OF COUNT (-1)

	pop    rbp
	ret

make_int:
    push   rbp
	mov    rbp, rsp
	mov    rdx, rdi

    int 37

	pop    rbp
	ret
