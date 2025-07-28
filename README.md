# Cortex-M4 Real-Time Operating System (RTOS)

This repository contains a minimal, fully working real-time operating system (RTOS) built from scratch for the ARM Cortex-M4 (STM32F401RE).

Features

- Manual stack and register setup for threads 
- Custom system call (SVC) framework with SVC handler 
- Modular OS kernel and stack allocator 
- Cooperative multitasking with round-robin scheduling 
- Preemptive multitasking using SysTick + PendSV 
- Support for passing user-defined arguments to threads
- Mixed cooperative + preemptive thread execution
- Built-in thread timing and time slice management

📁 Directory Structure
```
Core/
├── Inc/
│ ├── kernel.h # Kernel struct and function definitions
│ ├── main.h # Contains the common defines of the application.
├── Src/
│ ├── kernel.c # Core OS logic: thread mgmt, scheduling
│ ├── main.c # User threads and OS startup code
│ ├── asmDump.s # Assembly for PendSV and runFirstThread
│ ├── stm32f4xx_it.c # SysTick and interrupt handlers
```
