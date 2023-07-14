/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define xPortPendSVHandler                       PendSV_Handler
#define vPortSVCHandler                          SVC_Handler

#ifdef CONFIG_RTOS_PREEMPTION
#define configUSE_PREEMPTION    1
#else
#define configUSE_PREEMPTION    0
#endif

#ifdef CONFIG_RTOS_PORT_OPTIMISED_TASK_SELECTION
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#else
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#endif

#ifdef CONFIG_RTOS_TICKLESS_IDLE
#define configUSE_TICKLESS_IDLE   1
#else
#define configUSE_TICKLESS_IDLE    0
#endif

#ifdef CONFIG_RTOS_IDLE_HOOK
#define configUSE_IDLE_HOOK   1
#else
#define configUSE_IDLE_HOOK    0
#endif

#ifdef CONFIG_RTOS_MALLOC_FAILED_HOOK
#define configUSE_MALLOC_FAILED_HOOK   1
#else
#define configUSE_MALLOC_FAILED_HOOK    0
#endif

#ifdef CONFIG_RTOS_DAEMON_TASK_STARTUP_HOOK
#define configUSE_DAEMON_TASK_STARTUP_HOOK   1
#endif

#ifdef CONFIG_RTOS_SB_COMPLETED_CALLBACK
#define configUSE_SB_COMPLETED_CALLBACK   1
#else
#define configUSE_SB_COMPLETED_CALLBACK    0
#endif

#ifdef CONFIG_RTOS_TICK_HOOK
#define configUSE_TICK_HOOK   CONFIG_RTOS_TICK_HOOK
#else
#define configUSE_TICK_HOOK    0
#endif

#define configCPU_CLOCK_HZ      CONFIG_RTOS_CPU_CLOCK_HZ

#if CONFIG_RTOS_SYSTICK_CLOCK_HZ > 0
#define configSYSTICK_CLOCK_HZ  CONFIG_RTOS_SYSTICK_CLOCK_HZ
#endif
#define configTICK_RATE_HZ      SYSTEM_FREQ

#if (CONFIG_RTOS_PORT_OPTIMISED_TASK_SELECTION) && \
    (CONFIG_RTOS_MAX_PRIORITIES > 32)
#error "When CONFIG_RTOS_PORT_OPTIMISED_TASK_SELECTION is set to yes, \
        CONFIG_RTOS_MAX_PRIORITIES cannot be greater than 32"
#endif
#define configMAX_PRIORITIES      CONFIG_RTOS_MAX_PRIORITIES
#define configMINIMAL_STACK_SIZE  CONFIG_RTOS_MINIMAL_STACK_SIZE
#define configMAX_TASK_NAME_LEN   CONFIG_RTOS_MAX_TASK_NAME_LEN

#ifdef CONFIG_RTOS_TRACE_FACILITY
#define configUSE_TRACE_FACILITY  1
#endif

#ifdef CONFIG_RTOS_STATS_FORMATTING_FUNCTIONS
#define configUSE_STATS_FORMATTING_FUNCTIONS  1
#endif

#ifdef CONFIG_RTOS_16_BIT_TICKS
#define configUSE_16_BIT_TICKS   1
#else
#define configUSE_16_BIT_TICKS    0
#endif

#ifdef CONFIG_RTOS_IDLE_SHOULD_YIELD
#define configIDLE_SHOULD_YIELD   1
#else
#define configIDLE_SHOULD_YIELD    0
#endif

#ifdef CONFIG_RTOS_TASK_NOTIFICATIONS
#define configUSE_TASK_NOTIFICATIONS   1
#else
#define configUSE_TASK_NOTIFICATIONS    0
#endif

#define configTASK_NOTIFICATION_ARRAY_ENTRIES CONFIG_RTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

#ifdef CONFIG_RTOS_MUTEXES
#define configUSE_MUTEXES   1
#else
#define configUSE_MUTEXES    0
#endif

#ifdef CONFIG_RTOS_RECURSIVE_MUTEXES
#define configUSE_RECURSIVE_MUTEXES   1
#else
#define configUSE_RECURSIVE_MUTEXES    0
#endif

#ifdef CONFIG_RTOS_COUNTING_SEMAPHORES
#define configUSE_COUNTING_SEMAPHORES   1
#else
#define configUSE_COUNTING_SEMAPHORES    0
#endif

#ifdef CONFIG_RTOS_ALTERNATIVE_API
#define configUSE_ALTERNATIVE_API   1
#else
#define configUSE_ALTERNATIVE_API    0
#endif

#ifdef RTOS_CHECK_FOR_STACK_OVERFLOW1
#define configCHECK_FOR_STACK_OVERFLOW 1
#else
#ifdef RTOS_CHECK_FOR_STACK_OVERFLOW2
#define configCHECK_FOR_STACK_OVERFLOW 2
#else
#define configCHECK_FOR_STACK_OVERFLOW 0
#endif
#endif

#define configQUEUE_REGISTRY_SIZE  CONFIG_RTOS_QUEUE_REGISTRY_SIZE

#ifdef CONFIG_RTOS_QUEUE_SETS
#define configUSE_QUEUE_SETS   1
#else
#define configUSE_QUEUE_SETS    0
#endif

#ifdef CONFIG_RTOS_TIME_SLICING
#define configUSE_TIME_SLICING   1
#else
#define configUSE_TIME_SLICING    0
#endif

#ifdef CONFIG_RTOS_NEWLIB_REENTRANT
#define configUSE_NEWLIB_REENTRANT  1
#endif

#ifdef CONFIG_RTOS_ENABLE_BACKWARD_COMPATIBILITY
#define configENABLE_BACKWARD_COMPATIBILITY   1
#else
#define configENABLE_BACKWARD_COMPATIBILITY    0
#endif

#if CONFIG_RTOS_NUM_THREAD_LOCAL_STORAGE_POINTERS > 0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS  CONFIG_RTOS_NUM_THREAD_LOCAL_STORAGE_POINTERS
#endif

#ifdef CONFIG_RTOS_MINI_LIST_ITEM
#define configUSE_MINI_LIST_ITEM   1
#else
#define configUSE_MINI_LIST_ITEM    0
#endif

#ifdef CONFIG_RTOS_SUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION   1
#else
#define configSUPPORT_STATIC_ALLOCATION    0
#endif

#ifdef CONFIG_RTOS_SUPPORT_DYNAMIC_ALLOCATION
#define configSUPPORT_DYNAMIC_ALLOCATION   1
#else
#define configSUPPORT_DYNAMIC_ALLOCATION    0
#endif

#define configTOTAL_HEAP_SIZE   CONFIG_RTOS_TOTAL_HEAP_SIZE

#ifdef CONFIG_RTOS_APPLICATION_ALLOCATED_HEAP
#define configAPPLICATION_ALLOCATED_HEAP  1
#endif

#ifdef CONFIG_RTOS_STACK_ALLOCATION_FROM_SEPARATE_HEAP
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP  1
#endif

#ifdef CONFIG_RTOS_GENERATE_RUN_TIME_STATS
#define configGENERATE_RUN_TIME_STATS  1
#endif

#ifdef CONFIG_RTOS_CO_ROUTINES
#define configUSE_CO_ROUTINES   1
#else
#define configUSE_CO_ROUTINES    0
#endif

#ifdef CONFIG_RTOS_MAX_CO_ROUTINE_PRIORITIES
#define configMAX_CO_ROUTINE_PRIORITIES CONFIG_RTOS_MAX_CO_ROUTINE_PRIORITIES
#endif

#ifdef CONFIG_RTOS_TIMERS
#define configUSE_TIMERS   1
#else
#define configUSE_TIMERS    0
#endif
#ifdef CONFIG_RTOS_TIMER_TASK_PRIORITY
#define configTIMER_TASK_PRIORITY CONFIG_RTOS_TIMER_TASK_PRIORITY
#endif
#ifdef CONFIG_RTOS_TIMER_QUEUE_LENGTH
#define configTIMER_QUEUE_LENGTH CONFIG_RTOS_TIMER_QUEUE_LENGTH
#endif
#ifdef CONFIG_RTOS_TIMER_TASK_STACK_DEPTH
#define configTIMER_TASK_STACK_DEPTH RTOS_TIMER_TASK_STACK_DEPTH
#endif

#if CONFIG_RTOS_KERNEL_INTERRUPT_PRIORITY > 0
#define configKERNEL_INTERRUPT_PRIORITY  CONFIG_RTOS_KERNEL_INTERRUPT_PRIORITY
#endif

#if CONFIG_RTOS_MAX_SYSCALL_INTERRUPT_PRIORITY > 0
#define configMAX_SYSCALL_INTERRUPT_PRIORITY   CONFIG_RTOS_MAX_SYSCALL_INTERRUPT_PRIORITY
#endif

#ifdef CONFIG_RTOS_INCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 1
#endif

#ifdef CONFIG_RTOS_HEAP_CLEAR_MEMORY_ON_FREE
#define configHEAP_CLEAR_MEMORY_ON_FREE   1
#else
#define configHEAP_CLEAR_MEMORY_ON_FREE    0
#endif

#endif /* FREERTOS_CONFIG_H */
