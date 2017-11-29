#ifndef PROPERTY_EPILOGUE_H__lib_ubik_
#define PROPERTY_EPILOGUE_H__lib_ubik_

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef	EXCLUDE_HRTICK
	#define EXCLUDE_HRTICK                                 0
#endif

#ifndef	EXCLUDE_HRTICK_TICKISR_DELAY_CHECK
	#define EXCLUDE_HRTICK_TICKISR_DELAY_CHECK             1
#endif

#ifndef	EXCLUDE_STIMER
	#define EXCLUDE_STIMER                                 0
#endif

#ifndef	EXCLUDE_TICK_HOOKFUNC
	#define EXCLUDE_TICK_HOOKFUNC                          0
#endif

////

#ifndef	TASK_NAME_SIZE_MAX
	#define TASK_NAME_SIZE_MAX                   16
#endif

#ifndef	TASK_PRIORITY_MAX
	#define TASK_PRIORITY_MAX                    9
#endif

#ifndef	TASK_STACK_DEPTH_DEFAULT
	#define TASK_STACK_DEPTH_DEFAULT             128
#endif

#ifndef	TASK_STACK_DEPTH_MIN
	#define TASK_STACK_DEPTH_MIN                 64
#endif

#ifndef	TASK_MAXWAITSIGOBJ_MAX
	#define TASK_MAXWAITSIGOBJ_MAX               8
#endif

#ifndef	IDLETASK_SPINWAIT_INTERVALTICK
	#define IDLETASK_SPINWAIT_INTERVALTICK       10
#endif

#ifndef	MSGQ_MSGSIZE_MAX
	#define MSGQ_MSGSIZE_MAX                     16
#endif

#ifndef	EXCLUDE_KERNEL_MONITORING
	#define EXCLUDE_KERNEL_MONITORING            0
#endif

#ifndef	EXCLUDE_TASK_PRIORITY_INHERITANCE
	#define EXCLUDE_TASK_PRIORITY_INHERITANCE    0
#endif

#ifndef	EXCLUDE_TASK_DEADLOCK_CHECK
	#define EXCLUDE_TASK_DEADLOCK_CHECK          0
#endif

#ifndef	EXCLUDE_TASK_MONITORING
	#define EXCLUDE_TASK_MONITORING              0
#endif

#ifndef	EXCLUDE_IDLETASK_GARBAGECOLLECT
	#define EXCLUDE_IDLETASK_GARBAGECOLLECT      0
#endif

#ifndef	EXCLUDE_IDLETASK_HOOKFUNC
	#define EXCLUDE_IDLETASK_HOOKFUNC            0
#endif

#ifndef	EXCLUDE_CONDV
	#define EXCLUDE_CONDV                        0
#endif

#ifndef	EXCLUDE_IOBUF
	#define EXCLUDE_IOBUF                        1
#endif

#ifndef	EXCLUDE_MSGQ
	#define EXCLUDE_MSGQ                         0
#endif

#ifndef	EXCLUDE_SEM
	#define EXCLUDE_SEM                          0
#endif

#ifndef	EXCLUDE_SIGNAL
	#define EXCLUDE_SIGNAL                       0
#endif

////

#if (1 != EXCLUDE_TASK_MONITORING)
	#if (1 == EXCLUDE_SIGNAL)
		#error Task monitoring function needs signal.
	#endif
#endif

#if (1 != EXCLUDE_IOBUF)
	#if (1 == EXCLUDE_CONDV)
		#error Iobuf needs condv.
	#endif
#endif

#if (1 != EXCLUDE_STIMER)
	#if (1 == EXCLUDE_SEM)
		#error Stimer needs sem.
	#endif
	#if (1 == EXCLUDE_SIGNAL)
		#error Stimer needs signal.
	#endif
#endif

#if  (1 != EXCLUDE_HRTICK_TICKISR_DELAY_CHECK)
	#if (1 == EXCLUDE_HRTICK)
		#error HrTick tickisr delay feature nees HrTick.
	#endif
#endif


#ifdef	__cplusplus
}
#endif

#endif /* PROPERTY_EPILOGUE_H__lib_ubik_ */
