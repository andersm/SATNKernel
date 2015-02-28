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


#ifndef  _TN_PORT_CONFIG_H_
#define  _TN_PORT_CONFIG_H_

#define  TN_CHECK_PARAM       1

#define  TN_MEAS_PERFORMANCE  1

#define  USE_MUTEXES          1

#define  USE_EVENTS           1

/* Use interrupt stack */
#define  TN_INT_STACK         1

/* Vector table index used as kernel context pointer */
#define TN_KERNEL_VECTOR            106

/* System trap number used for context switch routine */
#define TN_CONTEXT_SWITCH_TRAP      107

/* Interrupt stack size, in words */
#define TN_INT_STACK_SIZE           170

#endif
