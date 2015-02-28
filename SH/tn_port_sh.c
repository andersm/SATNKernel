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

#include "tn.h"


//----------------------------------------------------------------------------
// Processor specific routine - here for SuperH
// sizeof(void*) = sizeof(int)
//----------------------------------------------------------------------------
unsigned int * tn_stack_init(void * task_func,
                             void * stack_start,
                             void * param)
{
    unsigned int * stk;

    //-- filling register's position in the stack - for debugging only

    stk  = (unsigned int *)stack_start; //-- Load stack pointer

    *stk-- = 0L;                        //-- SR
    *stk-- = (unsigned int)task_func;   //-- PC - Entry Point
    *stk-- = 0x00000000L;               //-- R0
    *stk-- = 0x01010101L;               //-- R1
    *stk-- = 0x02020202L;               //-- R2
    *stk-- = 0x03030303L;               //-- R3
    *stk-- = (unsigned int)param;       //-- R4 - task's function argument
    *stk-- = 0x05050505L;               //-- R5
    *stk-- = 0x06060606L;               //-- R6
    *stk-- = 0x07070707L;               //-- R7
    *stk-- = 0x08080808L;               //-- R8
    *stk-- = 0x09090909L;               //-- R9
    *stk-- = 0x10101010L;               //-- R10
    *stk-- = 0x11111111L;               //-- R11
    *stk-- = 0x12121212L;               //-- R12
    *stk-- = 0x13131313L;               //-- R13
    *stk-- = 0x14141414L;               //-- R14
    *stk-- = 0x16161616L;               //-- MACL
    *stk-- = 0x17171717L;               //-- MACH
    *stk-- = 0x18181818L;               //-- GBR
    *stk   = (unsigned int)tn_task_exit;       //-- PR

    return stk;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
