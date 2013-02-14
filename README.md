SATNKernel
==========

SATNKernel is a port of the [TNKernel](http://www.tnkernel.com/ "TNKernel") real-time kernel version 2.6 for the Sega Saturn game console.
In addition to porting the kernel to the SH2 architecture, modifications have been made to allow running separate instances of the kernel on the master and slave CPUs. There is no SMP support included.

Estimating task stack space requirements
----------------------------------------

If an interrupt stack is not used, the absolute minimum stack size can be calculated as follows:  
>size of context (21) + size of stack expansion at exit (21) + 11 * number of used interrupt levels

If all 15 interrupt levels are used, this equals 207 words (828 bytes).

If an interrupt stack is used, this changes to:  
>size of context (21) + size of stack expansion at exit (21) + 4 * (number of used interrupt levels - 1) + 5

The minimum size of the interrupt stack is:  
>11 * (number of used interrupt levels - 1) + 6

If all 15 interrupt levels are used, this equals 103 words (412 bytes) for tasks and 160 words (640 bytes) for the interrupt stack.

This does not account for stack space used by the tasks or interrupt handlers.

---

The kernel is released under the BSD license as follows:

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
