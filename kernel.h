/*
 * kernel.h
 *
 *  Created on: Jun 21, 2025
 *      Author: Svamin
 */


#ifndef INC_KERNEL_H_
#define INC_KERNEL_H_
#define SHPR2 *(uint32_t*)0xE000ED1C
#define SHPR3 *(uint32_t*)0xE000ED20
#define _ICSR *(uint32_t*)0xE000ED04
#define SVC_YIELD 20

#include <stdint.h>
#include <stdbool.h>

typedef struct k_thread{
	uint32_t* sp; //stack pointer
	void (*thread_function)(void*); //function pointer
	uint32_t timeslice_ms;
	uint32_t runtime_ms;
} k_thread;

void     osKernelInitialize(void);
bool     osCreateThread(void (*fn)(void *), void *arg);
void SVC_Handler_Main(unsigned int *svc_args);
uint32_t *stack_allocator(void);
void    osKernelStart(void);
void osSched(void);
void osYield(void);
bool osCreateThreadWithDeadline(void (*fn)(void *), void *arg, uint32_t time_ms);

#endif /* INC_KERNEL_H_ */
