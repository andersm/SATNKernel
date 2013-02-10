/*

    SATNKernel real-time kernel for the Sega Saturn
    Based on TNKernel version 2.6

    Copyright © 2004, 2010 Yuri Tiomkin
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

TN_KERN_CTX kernelcontext;

//----------------------------------------------------------------------------
// Timer tick handler - must be called periodically from timer interrupt
//----------------------------------------------------------------------------
void tick_handler(void)
{
    tn_tick_int_processing();
}

//----------------------------------------------------------------------------
// Main function
//----------------------------------------------------------------------------
int main(void)
{
    // disable interrupts

    // perform hardware initialization

    // start kernel - does not return
    tn_start_system(&kernelcontext);

    return 1;
}

//----------------------------------------------------------------------------
// Application initialization callback - create and init OS objects here
//----------------------------------------------------------------------------
void tn_app_init(void)
{
}

//----------------------------------------------------------------------------
// CPU-specific function that enables interrupts
//----------------------------------------------------------------------------
void tn_cpu_int_enable(void)
{
}
