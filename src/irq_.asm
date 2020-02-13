.include "hdr.asm"

; irq code must be located in bank 0.
.bank 0
.section ".irq_handler" semifree

; Clear the i flag to enable irq.
; By default, the pvsneslib init disable irq.
enable_irq:
    cli ; Clear the i flag.
    rtl ; Return long because called from C.

; Main internal irq handler.
; as declared in SNESNATIVEVECTOR.
IRQHandler:
    ; Reset the m and x flag of the status register.
    ; This set Acc, X & Y registers in 16-bit mode.
    rep #$30

    ; Push the bank register.
    ; Will switch to bss section (7e) before calling the user C irq handler.
    ; Also, will switch to the bank 0 to ack/clear the TIMEUP MMIO register.
    phb

    ; Push the direct register.
    ; Will move the memory window from the general purpose virtual registers to the dedicated irq virtual registers.
    ; see pvsneslib crt0_snes.asm.
    phd

    ; Push the Acc, X & Y registers.
    ; General purpose.
    pha
    phx
    phy

    ; Set data bank register to bss section as we enter user C code.
    pea $7e7e
    plb
    plb

    ; Move the memory window from the general purpose virtual registers to the dedicated irq virtual registers.
    ; see pvsneslib crt0_snes.asm.
    ; All direct memory access are now offset by tcc__registers_irq bytes.
    ; As we enter irq, we will not overwrite the existing general purpose virtual registers but use the dedicated ones.
    lda #tcc__registers_irq
    tad

    ; Jump (long) to the C code
    ; call_irq_handler is basically a function pointer call.
    ; For such function, the WLA assembler generate code writing into the r10 virtual register.
    ; Thanks to the previous 'tad' instruction, we do not overwrite existing value in the general purpose virtual r10 register.
    jsl call_irq_handler

    ; Set data bank register to 0 in order to read the TIMEUP MMIO register.
    pea $0000
    plb
    plb

    ; Clear the irq flag.
    ; This MMIO register will be cleared on read.
    lda $4211

    ; Pop
    ply
    plx
    pla
    pld
    plb

    ; Return from interrupt.
    rti

.ends
