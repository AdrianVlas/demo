#ifndef __CONST_MENU_TRANSFORMATOR__
#define __CONST_MENU_TRANSFORMATOR__

#define EKRAN_TRANSFORMATOR_INFO                        (EKRAN_LIST_LEDS_FOR_RANGUVANNJA   + 1)
#define EKRAN_TRANSFORMATOR_INFO_SETPOINT               (EKRAN_TRANSFORMATOR_INFO          + 1)
#define EKRAN_TRANSFORMATOR_INFO_CONTROL                (EKRAN_TRANSFORMATOR_INFO_SETPOINT + 1)

#define MAX_ROW_FOR_TRANSFORMATOR_INFO_SETPOINT         2  
#define MAX_ROW_FOR_TRANSFORMATOR_INFO_CONTROL          1  

#define INDEX_ML_TT                         0
#define INDEX_ML_TN                         1

#define COL_TT_BEGIN                        6
#define COL_TT_END                          9

#define COL_TN_BEGIN                        6
#define COL_TN_END                          8

#define INDEX_ML_CTR_TRANSFORMATOR_LINE_PHASE           0          
#define CTR_TRANSFORMATOR_LINE_PHASE                    (1 << INDEX_ML_CTR_TRANSFORMATOR_LINE_PHASE)   

#define CTR_TRANSFORMATOR_MASKA                         (                                 \
                                                         CTR_TRANSFORMATOR_LINE_PHASE     \
                                                        )


#endif
