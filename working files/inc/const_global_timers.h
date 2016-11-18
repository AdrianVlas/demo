#ifndef __CONST_GLOBAL_TIMERS__
#define __CONST_GLOBAL_TIMERS__

#define MAX_VALUE_TIMER_FOR_COUNT_SIGNAL_ACHR_CHAPV            4 //мс - досягнення цього значення є умовою спроби запустити сигнал АПВ (якщо він не блокується)
#define TIMEOUT_ACHR_CHAPV_100MS                               100 //мс
#define TIMEOUT_CHAPV_1MS                                      (1 + 1) /*попревка на 1 зроблена, щоб правильно працював макрос з реалізацією логіки АЧР/ЧАПВ*/
#define TIMEOUT_BLOCK_CHAPV_5MS                                5

enum _timers
{
  INDEX_TIMER_INPUT_START = 0,
  INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START = (INDEX_TIMER_INPUT_START + NUMBER_INPUTS),
  INDEX_TIMER_DF_PAUSE_START                            = (INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + NUMBER_DEFINED_FUNCTIONS),
  INDEX_TIMER_DF_WORK_START                             = (INDEX_TIMER_DF_PAUSE_START + NUMBER_DEFINED_FUNCTIONS),
  INDEX_TIMER_CTRL_PHASE_U                              = (INDEX_TIMER_DF_WORK_START + NUMBER_DEFINED_FUNCTIONS),
  INDEX_TIMER_CTRL_PHASE_U_D,
  INDEX_TIMER_CTRL_PHASE_PHI,
  INDEX_TIMER_CTRL_PHASE_PHI_D,
  INDEX_TIMER_CTRL_PHASE_F,
  INDEX_TIMER_CTRL_PHASE_F_D,
  INDEX_TIMER_CTRL_PHASE_TMP1_100MS,
  INDEX_TIMER_CTRL_PHASE_TMP2_100MS,

  MAX_NUMBER_GLOBAL_TIMERS
};

#endif
