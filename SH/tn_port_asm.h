/*

    SATNKernel real-time kernel for the Sega Saturn
    Based on TNKernel version 2.7

    Copyright © 2004, 2013 Yuri Tiomkin
    Saturn version modifications copyright © 2013 Anders Montonen
    All rights reserved.

    Permission to use, copy, modify, and distribute this software in source
    and binary forms and its documentation for any purpose and without fee
    is hereby granted, provided that the above copyright notice appear
    in all copies and that both that copyright notice and this permission
    notice appear in supporting documentation.

    THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

*/

#include "tn_port_config.h"

/* Kernel context structure offsets in bytes */
#define TN_KERN_CURR_RUN_TASK       0
#define TN_KERN_NEXT_TASK_TO_RUN    4
#define TN_KERN_SYSTEM_STATE        8
#define TN_KERN_NEST_COUNT         12
#define TN_USER_SP                 32
#define TN_INT_SP                  36


/* Interrupt handler wrapper macro */
!-----------------------------------------------------------------------------
    .macro _tn_cpu_irq_isr handlerfunc:req
    ! create working space (may always go on user stack)
    mov.l   r0, @-r15
    mov.l   r1, @-r15

    ! disable interrupts
    stc     sr, r1
    mov     #0xf0, r0
    ldc     r0, sr

    mov.l   r2, @-r15

    ! increase nesting count
    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r2
    mov.l   @(r0, r2), r2

    mov.l   @(TN_KERN_NEST_COUNT, r2), r0
    add     #1, r0
#ifdef TN_INT_STACK
    cmp/eq  #1, r0
    bf/s    1f
#endif
    mov.l   r0, @(TN_KERN_NEST_COUNT, r2)
#ifdef TN_INT_STACK
    ! switch to interrupt stack if first interrupt
    mov.l   r15, @(TN_USER_SP, r2)
    mov.l   @(TN_INT_SP, r2), r15
1:
#endif

    ! re-enable interrupts
    ldc     r1, sr

    ! save rest of caller-save registers
    mov.l   r3, @-r15
    mov.l   r4, @-r15
    mov.l   r5, @-r15
    mov.l   r6, @-r15
    sts.l   pr, @-r15

    ! call ISR
    mov.l   2f, r0
    jsr     @r0
    mov.l   r7, @-r15

    ! restore caller-save registers
    mov.l   @r15+, r7
    lds.l   @r15+, pr
    mov.l   @r15+, r6
    mov.l   @r15+, r5
    mov.l   @r15+, r4
    mov.l   @r15+, r3

    ! disable interrupts
    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r1
    mov.l   @(r0, r1), r1
    stc     sr, r2
    mov     #0xf0, r0
    ldc     r0, sr

    ! decrease nesting count
    mov.l   @(TN_KERN_NEST_COUNT, r1), r0
    dt      r0
#ifdef TN_INT_STACK
    bf/s    1f
#endif
    mov.l   r0, @(TN_KERN_NEST_COUNT, r1)
#ifdef TN_INT_STACK
    ! switch to user stack if last interrupt
    mov.l   r15, @(TN_INT_SP, r1)
    mov.l   @(TN_USER_SP, r1), r15
1:
#endif
    ! re-enable interrupts
    ldc     r2, sr

    ! restore rest of state,
    ! jump to context switch routine
    mov.l   @r15+, r2
    mov.l   @r15+, r1
    mov.l   3f, r0
    jmp     @r0
    mov.l   @r15+, r0

    .align 2
2:  .long \handlerfunc
3:  .long _tn_switch_context_trap
    .endm
