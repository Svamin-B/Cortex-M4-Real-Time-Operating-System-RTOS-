/*
 * kernel.c
 *
 *  Created on: Jun 21, 2025
 *      Author: Svamin
 */

#include "kernel.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_THREADS 12
#define DEFAULT_TIMESLICE_MS   5
extern void runFirstThread(void);
volatile uint32_t os_time_ms = 0;
//extern uint32_t *stackptr;

k_thread threadTable[MAX_THREADS];
static int      numThreadsRunning = 0;
int      currentThread     = 0;


#define STACK_POOL_SIZE   0x4000
#define THREAD_STACK_SIZE 0x0400


static uint32_t *pool_top;
static uint32_t *pool_bottom;
static uint32_t *next_free_top;


void osKernelInitialize(void)
{
    pool_top      = *(uint32_t **)0x0;
    pool_bottom   = (uint32_t *)((uintptr_t)pool_top - STACK_POOL_SIZE);
    next_free_top = pool_top;

    //set the priority of PendSV to almost the weakest
    SHPR3 |= 0xFE << 16; //shift the constant 0xFE 16 bits to set PendSV priority
    SHPR2 |= 0xFDU << 24; //Set the priority of SVC higher than PendSV

}

void SVC_Handler_Main(unsigned int *svc_args)
{
	unsigned int svc_number;
	/*
	* Stack contains:
	* r0, r1, r2, r3, r12, r14, the return address and xPSR
	* First argument (r0) is svc_args[0]
	*/
	svc_number = ((char*)svc_args[6])[-2];
	switch(svc_number)
	{
		case 17: //17 is sort of arbitrarily chosen
			printf("Success!\r\n");
			break;
		case 18:
			printf("Pass!\r\n");
			break;
		case 19:
			printf("Fail!\r\n");
			break;
		case 20:
			//Pend an interrupt to do the context switch
			_ICSR |= 1<<28;
			__asm("isb");
			break;
		case 10:
			runFirstThread();
			break;
		default: /* unknown SVC */
			break;
	}
}

uint32_t* stack_allocator(void) {
    uint32_t *new_top = (uint32_t *)((uint32_t)next_free_top - THREAD_STACK_SIZE);


    if (new_top < pool_bottom)
        return NULL;


    next_free_top = new_top;
    return next_free_top;
}

bool osCreateThread(void (*funct)(void *), void *arg)
{
	if (numThreadsRunning >= MAX_THREADS) return false;

    uint32_t *sp = stack_allocator();
    if (sp == NULL) return false;

    *(--sp) = 1<<24; //A magic number, this is xPSR
    *(--sp) = (uint32_t)funct; //the function name (PC)
    *(--sp) = 0xFFFFFFFD;
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = (uint32_t)arg;
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number
    *(--sp) = 0xA; //An arbitrary number


    threadTable[numThreadsRunning].sp    = sp;
    threadTable[numThreadsRunning].thread_function = funct;
    threadTable[numThreadsRunning].timeslice_ms  = DEFAULT_TIMESLICE_MS;
    threadTable[numThreadsRunning].runtime_ms    = DEFAULT_TIMESLICE_MS;
    ++numThreadsRunning;

    return true;
}

bool osCreateThreadWithDeadline(void (*fn)(void *), void *arg, uint32_t time_ms)
{
    if (time_ms == 0) return false;
    if (!osCreateThread(fn, arg)) return false;
    threadTable[numThreadsRunning-1].timeslice_ms = threadTable[numThreadsRunning-1].runtime_ms   = time_ms;
    return true;
}

void osKernelStart(void)
{
	if (numThreadsRunning == 0) return;

	__set_PSP((uint32_t)threadTable[0].sp);
    __asm("SVC #10");
    runFirstThread();
}

void osSched(void)
{
    threadTable[currentThread].sp = (uint32_t *)((uint32_t)__get_PSP() - 8*4);

    currentThread = (currentThread + 1) % numThreadsRunning;

    __set_PSP((uint32_t)threadTable[currentThread].sp);
}

void osYield(void)
{
	threadTable[currentThread].runtime_ms = threadTable[currentThread].timeslice_ms;
    __asm ("SVC #20");
}

