#ifndef __CONST_MENU_ALARMS__
#define __CONST_MENU_ALARMS__

#define EKRAN_TIMEOUT_ALARMS1           (EKRAN_CHOOSE_SETTINGS_ALARMS2  + 1)
#define EKRAN_TIMEOUT_ALARMS2           (EKRAN_TIMEOUT_ALARMS1  + 1)
#define EKRAN_CONTROL_ALARMS1           (EKRAN_TIMEOUT_ALARMS2  + 1)
#define EKRAN_CONTROL_ALARMS2           (EKRAN_CONTROL_ALARMS1  + 1)
#define EKRAN_RANGUVANNJA_ALARMS1       (EKRAN_CONTROL_ALARMS2 + 1) 
#define EKRAN_RANGUVANNJA_ALARMS2       (EKRAN_RANGUVANNJA_ALARMS1 + 1) 

#define MAX_ROW_FOR_TIMEOUT_ALARMS        1
#define MAX_ROW_FOR_CONTROL_ALARMS        1
#define MAX_ROW_RANGUVANNJA_ALARMS        NUMBER_TOTAL_SIGNAL_FOR_RANG

#define INDEX_ML_ALARM_SOUND_PERIOD       0

#define COL_TMO_ALARM_SOUND_PERIOD_BEGIN  6
#define COL_TMO_ALARM_SOUND_PERIOD_COMMA  8
#define COL_TMO_ALARM_SOUND_PERIOD_END    9

#define ALARMS_SEPARATOR                  16

#define INDEX_ML_CTR_ALARMS_MODE          0
#define CTR_ALARMS_MODE                   (1 << INDEX_ML_CTR_ALARMS_MODE)          

#define CTR_ALARMS_MASKA                  (                     \
                                            CTR_ALARMS_MODE     \
                                          )


#endif
