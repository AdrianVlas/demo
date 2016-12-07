#ifndef __CONSTANTS_DEFINED_TRIGGERS__
#define __CONSTANTS_DEFINED_TRIGGERS__

#define EKRAN_LIST_DT                           (EKRAN_RANGUVANNJA_DF8 + 1)
#define EKRAN_SET_RESET_DT1                     (EKRAN_LIST_DT + 1)
#define EKRAN_SET_RESET_DT2                     (EKRAN_SET_RESET_DT1 + 1)
#define EKRAN_SET_RESET_DT3                     (EKRAN_SET_RESET_DT2 + 1)
#define EKRAN_SET_RESET_DT4                     (EKRAN_SET_RESET_DT3 + 1)
#define EKRAN_RANGUVANNJA_SET_DT1               (EKRAN_SET_RESET_DT4 + 1)
#define EKRAN_RANGUVANNJA_RESET_DT1             (EKRAN_RANGUVANNJA_SET_DT1 + 1)
#define EKRAN_RANGUVANNJA_SET_DT2               (EKRAN_RANGUVANNJA_RESET_DT1 + 1)
#define EKRAN_RANGUVANNJA_RESET_DT2             (EKRAN_RANGUVANNJA_SET_DT2 + 1)
#define EKRAN_RANGUVANNJA_SET_DT3               (EKRAN_RANGUVANNJA_RESET_DT2 + 1)
#define EKRAN_RANGUVANNJA_RESET_DT3             (EKRAN_RANGUVANNJA_SET_DT3 + 1)
#define EKRAN_RANGUVANNJA_SET_DT4               (EKRAN_RANGUVANNJA_RESET_DT3 + 1)
#define EKRAN_RANGUVANNJA_RESET_DT4             (EKRAN_RANGUVANNJA_SET_DT4 + 1)

#define MAX_ROW_FOR_LIST_DT                      NUMBER_DEFINED_TRIGGERS/*((int)current_settings.number_defined_dt)*/
#define MAX_ROW_FOR_LIST_SET_RESET_DT            2
#define MAX_ROW_RANGUVANNJA_DT                   NUMBER_TOTAL_SIGNAL_FOR_RANG

#define INDEX_ML_SET_DT                          0
#define INDEX_ML_RESET_DT                        1

#define INDEX_ML_LIST_TYPE_SOURCE_PLUS_DT        0
#define INDEX_ML_LIST_TYPE_SOURCE_MINUS_DT       1

#endif 
