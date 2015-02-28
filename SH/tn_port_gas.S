/*

    SATNKernel real-time kernel for the Sega Saturn
    Based on TNKernel version 2.7

    Copyright © 2004, 2013 Yuri Tiomkin
    Saturn version modifications copyright © 2013 Anders Montonen
    All rights reserved.

    ffs_asm() - this is the ffs algorithm devised by D.Seal and posted to
                comp.sys.arm on  16 Feb 1994.

    Interrupt context switch -  this source code is derived on code
              written by WellsK


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

#include "tn_port_asm.h"
#include "tn_port_config.h"

    .text

    ! External references

    ! Public functions declared in this file

    .global  _tn_switch_context_exit
    .global  _tn_switch_context
    .global  _tn_switch_context_trap
    .global  _tn_cpu_save_sr
    .global  _tn_cpu_restore_sr
    .global  _tn_start_exe
    .global  _tn_chk_irq_disabled
    .global  _tn_inside_int
    .global  _ffs_asm
    .global  _tn_kern_ctx_ptr
    .global  _tn_hook_vec

!-----------------------------------------------------------------------------
! Interrupts should be disabled here
!-----------------------------------------------------------------------------
_tn_start_exe:
    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r1
    mov.l   @(r0, r1), r1
    mov     #1, r0
    mov.l   r0, @(TN_KERN_SYSTEM_STATE, r1)

    ! switch to new stack pointer
    mov.l   @(TN_KERN_CURR_RUN_TASK, r1), r0
    mov.l   @r0, r15

    lds.l   @r15+, pr
    ldc.l   @r15+, gbr
    lds.l   @r15+, mach
    lds.l   @r15+, macl
    mov.l   @r15+, r14
    mov.l   @r15+, r13
    mov.l   @r15+, r12
    mov.l   @r15+, r11
    mov.l   @r15+, r10
    mov.l   @r15+, r9
    mov.l   @r15+, r8
    mov.l   @r15+, r7
    mov.l   @r15+, r6
    mov.l   @r15+, r5
    mov.l   @r15+, r4
    mov.l   @r15+, r3
    mov.l   @r15+, r2
    mov.l   @r15+, r1
    mov.l   @r15+, r0
    rte
    nop

!-----------------------------------------------------------------------------
! Interrupts should be disabled here
!-----------------------------------------------------------------------------
_tn_switch_context_exit:
    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r1
    mov.l   @(r0, r1), r1

    ! switch to new stack pointer
    mov.l   @(TN_KERN_CURR_RUN_TASK, r1), r0
    mov.l   @r0, r15

    lds.l   @r15+, pr
    ldc.l   @r15+, gbr
    lds.l   @r15+, mach
    lds.l   @r15+, macl
    mov.l   @r15+, r14
    mov.l   @r15+, r13
    mov.l   @r15+, r12
    mov.l   @r15+, r11
    mov.l   @r15+, r10
    mov.l   @r15+, r9
    mov.l   @r15+, r8
    mov.l   @r15+, r7
    mov.l   @r15+, r6
    mov.l   @r15+, r5
    mov.l   @r15+, r4
    mov.l   @r15+, r3
    mov.l   @r15+, r2
    mov.l   @r15+, r1
    mov.l   @r15+, r0
    rte
    nop

!-----------------------------------------------------------------------------
_tn_switch_context:
    trapa   #TN_CONTEXT_SWITCH_TRAP
    rts
    nop

!-----------------------------------------------------------------------------
_tn_switch_context_trap:
    ! save registers
    mov.l   r0, @-r15

    ! read saved sr
    mov.l   @(8, r15), r0

    ! returning to user level?
    tst     #0xf0, r0
    bf      2f

    ! disable interrupts
    mov     #0xf0, r0
    ldc     r0, sr

    mov.l   r1, @-r15
    mov.l   r2, @-r15

    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r1
    mov.l   @(r0, r1), r1
    mov.l   @(TN_KERN_CURR_RUN_TASK, r1), r0
    mov.l   @(TN_KERN_NEXT_TASK_TO_RUN, r1), r2
    ! exit if not switching tasks
    cmp/eq  r0, r2
    bt      1f

    mov.l   r3, @-r15
    mov.l   r4, @-r15
    mov.l   r5, @-r15
    mov.l   r6, @-r15
    mov.l   r7, @-r15
    mov.l   r8, @-r15
    mov.l   r9, @-r15
    mov.l   r10, @-r15
    mov.l   r11, @-r15
    mov.l   r12, @-r15
    mov.l   r13, @-r15
    mov.l   r14, @-r15
    sts.l   macl, @-r15
    sts.l   mach, @-r15
    stc.l   gbr, @-r15
    sts.l   pr, @-r15

    ! store SP in preempted task's TCB
    mov.l   r15, @r0
    ! tn_curr_run_task = tn_next_task_to_run
    mov.l   r2, @(TN_KERN_CURR_RUN_TASK, r1)

    ! get new task's SP
    mov.l   @r2, r15

    ! restore registers
    lds.l   @r15+, pr
    ldc.l   @r15+, gbr
    lds.l   @r15+, mach
    lds.l   @r15+, macl
    mov.l   @r15+, r14
    mov.l   @r15+, r13
    mov.l   @r15+, r12
    mov.l   @r15+, r11
    mov.l   @r15+, r10
    mov.l   @r15+, r9
    mov.l   @r15+, r8
    mov.l   @r15+, r7
    mov.l   @r15+, r6
    mov.l   @r15+, r5
    mov.l   @r15+, r4
    mov.l   @r15+, r3
1:
    mov.l   @r15+, r2
    mov.l   @r15+, r1
2:
    mov.l   @r15+, r0
    rte
    nop

!-----------------------------------------------------------------------------
_tn_cpu_save_sr:
    mov     #0xf0, r1
    stc     sr, r0
    extu.b  r1, r1
    or      r0, r1
    rts
    ldc     r1, sr

!-----------------------------------------------------------------------------
_tn_cpu_restore_sr:
    rts
    ldc     r4, sr

!-----------------------------------------------------------------------------
_tn_chk_irq_disabled:
    stc     sr, r0
    and     #0xf0, r0
    cmp/eq  #0xf0, r0
    rts
    movt    r0

!-----------------------------------------------------------------------------
_tn_inside_int:
    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r1
    mov.l   @(r0, r1), r1
    mov.l   @(TN_KERN_NEST_COUNT, r1), r0
    cmp/eq  #0, r0
    movt    r0
    rts
    xor     #1, r0

!-----------------------------------------------------------------------------
_ffs_asm:
    neg     r4, r0
    and     r4, r0
    cmp/eq  #0, r0
    bt      .Lffs_ret

    sts     macl, r2
    mov.l   ffs_hash, r1
    mul.l   r0, r1
    mova    ffs_table, r0
    sts     macl, r1
    lds     r2, macl

    shlr16  r1
    shlr8   r1
    shlr2   r1

    mov.b   @(r0, r1), r0

.Lffs_ret:
    rts
    nop

    .align 2
ffs_hash:
    .long 0x0450fbaf

ffs_table:
        /*   0   1   2   3   4   5   6   7           */
    .byte    0,  1,  2, 13,  3,  7,  0, 14  /*  0- 7 */
    .byte    4,  0,  8,  0,  0,  0,  0, 15  /*  8-15 */
    .byte   11,  5,  0,  0,  9,  0,  0, 26  /* 16-23 */
    .byte    0,  0,  0,  0,  0, 22, 28, 16  /* 24-31 */
    .byte   32, 12,  6,  0,  0,  0,  0,  0  /* 32-39 */
    .byte   10,  0,  0, 25,  0,  0, 21, 27  /* 40-47 */
    .byte   31,  0,  0,  0,  0, 24,  0, 20  /* 48-55 */
    .byte   30,  0, 23, 19, 29, 18, 17,  0  /* 56-63 */

!-----------------------------------------------------------------------------
_tn_kern_ctx_ptr:
    mov     #TN_KERNEL_VECTOR, r0
    shll2   r0
    stc     vbr, r1
    rts
    mov.l   @(r0, r1), r0

!-----------------------------------------------------------------------------
! Interrupts should be disabled here
!-----------------------------------------------------------------------------
! r4 = vector number
! r5 = pointer
! returns previous value
_tn_hook_vec:
    stc     vbr, r0
    shll2   r4
    add     r0, r4
    mov.l   @r4, r0
    rts
    mov.l   r5, @r4

!-----------------------------------------------------------------------------
!-----------------------------------------------------------------------------
!-----------------------------------------------------------------------------
