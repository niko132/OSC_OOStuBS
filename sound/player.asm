WAVSamplingRate     dw 0
WAVFileSize         dd 0
EnableDigitized     db 0


; EXPORTED FUNCTIONS

[EXTERN WAVFile]
[EXTERN WAVSize]

[GLOBAL PlayWAV]
[GLOBAL ir_sound]

[EXTERN ProgramPIT]

; FUNCTION IMPLEMENTATIONS

[SECTION .text]

ir_sound:
    cmp byte [EnableDigitized],1                  ;If it's set to 1, process next lines of code
    jne NoDigitizedSound                     ;If not, do the standard irq0 routine
   
    cmp al,0x80                                 ;If the byte taken from the memory is less than 80h,
                                                         ;turn off the speaker to prevent "unwanted" sounds,
    jb TurnOffBeeper                          ;like: ASCII strings (e.g. "WAVEfmt" signature etc).
    mov bx,[WAVSamplingRate]              ;Turn on the speaker with the WAV's sampling rate.
    call Sound_On
    jmp Sound_Done

TurnOffBeeper:
    call Sound_Off

Sound_Done:
    inc esi                                         ;Increment ESI to load the next byte

NoDigitizedSound:
    ret



;; PC speaker wave player PCSPEAKR.ASM

Sound_On:                                     ; A routine to make sounds with BX = frequency in Hz
    mov ax,0x34dd                        ; The sound lasts until NoSound is called
    mov dx,0x0012               
    cmp dx,bx               
    jnc Done1               
    div bx               
    mov bx,ax
    in al,0x61
    test al,3
    jnz A99               
    or al,3                                 ;Turn on the speaker itself
    out 0x61,al               
    mov al,0xb6
    out 0x43,al

A99:   
    mov al,bl
    out 0x42,al             
    mov al,bh
    out 0x42,al

Done1:
    ret

Sound_Off:
    in al,0x61                 
    and al,11111100b                               ;Turn off the speaker
    out 0x61,al
    ret

PlayWAV:
    mov word [WAVSamplingRate],8000
    mov esi,[WAVFile]
    call ProgramPIT                                  ;when a 6000Hz WAV file is played. This is how
                                                              ;the speaker can play the digitized sound:
                                                              ;it turns on and off very fast with the specified
                                                              ;wave sample rate.
    mov ecx,[WAVSize]                         ;Sets the loop point
    mov byte [EnableDigitized],1                       ;Tells the irq0 handler to process the routines

Play_Repeat:
    lodsb                                              ;Loads a byte from ESI to AL
    mov al, 0x10
    hlt                                                 ;Wait for IRQ to fire
    loop Play_Repeat                              ;and the whole procedure is looped ECX times
    mov byte [EnableDigitized],0                     ;Tells the irq0 handler to disable the digitized functions
    call Sound_Off                                        ;Turn the speaker off just in case
    ret
