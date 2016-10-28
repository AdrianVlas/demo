#ifndef __CONST_MENU_MEASURMENT__
#define __CONST_MENU_MEASURMENT__

#define EKRAN_MEASURMENT                (EKRAN_COFIGURATION + 1)
#define EKRAN_MEASURMENT_CURRENT        (EKRAN_MEASURMENT + 1)
#define EKRAN_MEASURMENT_VOLTAGE_TYPE   (EKRAN_MEASURMENT_CURRENT + 1)
#define EKRAN_MEASURMENT_VOLTAGE_PHASE  (EKRAN_MEASURMENT_VOLTAGE_TYPE + 1)
#define EKRAN_MEASURMENT_VOLTAGE_LINE   (EKRAN_MEASURMENT_VOLTAGE_PHASE + 1)
#define EKRAN_MEASURMENT_FREQUENCY      (EKRAN_MEASURMENT_VOLTAGE_LINE + 1)
#define EKRAN_MEASURMENT_ANGLE          (EKRAN_MEASURMENT_FREQUENCY + 1)
#define EKRAN_MEASURMENT_POWER          (EKRAN_MEASURMENT_ANGLE + 1)
#define EKRAN_ENERGY                    (EKRAN_MEASURMENT_POWER + 1)


#define MAX_ROW_FOR_MEASURMENT                  5
#define MAX_ROW_FOR_MEASURMENT_VOLTAGE_TYPE     2
#define MAX_ROW_FOR_EKRAN_ENERGY                MAX_NUMBER_INDEXES_ENERGY

#define INDEX_ML_MEASURMENT_CURRENT     0
#define INDEX_ML_MEASURMENT_VOLTAGE     1
#define INDEX_ML_MEASURMENT_FREQUENCY   2
#define INDEX_ML_MEASURMENT_ANGLE       3
#define INDEX_ML_MEASURMENT_POWER       4


#define INDEX_ML_MEASURMENT_VOLTAGE_PHASE       0
#define INDEX_ML_MEASURMENT_VOLTAGE_LINE        1

#define MAX_ROW_FOR_MEASURMENT_CURRENT          (3 + 2)
#define MAX_ROW_FOR_MEASURMENT_VOLTAGE_PHASE    (3 + 3)
#define MAX_ROW_FOR_MEASURMENT_VOLTAGE_LINE     (3 + 3)
#define MAX_ROW_FOR_MEASURMENT_FREQUENCY        (1 + 1)
#define MAX_ROW_FOR_MEASURMENT_ANGLE            FULL_ORT_MAX
#define MAX_ROW_FOR_MEASURMENT_POWER            4

#define INDEX_ML_Ia                     0
#define INDEX_ML_Ib                     1
#define INDEX_ML_Ic                     2
#define INDEX_ML_I2                     3
#define INDEX_ML_I1                     4

#define INDEX_ML_Ua1                    0
#define INDEX_ML_Ub1                    1
#define INDEX_ML_Uc1                    2
#define INDEX_ML_Ua2                    3
#define INDEX_ML_Ub2                    4
#define INDEX_ML_Uc2                    5

#define INDEX_ML_Uab1                   0
#define INDEX_ML_Ubc1                   1
#define INDEX_ML_Uca1                   2
#define INDEX_ML_Uab2                   3
#define INDEX_ML_Ubc2                   4
#define INDEX_ML_Uca2                   5

#define INDEX_ML_F_val_1                0
#define INDEX_ML_F_val_2                1

#define INDEX_ML_P                      0
#define INDEX_ML_Q                      1
#define INDEX_ML_S                      2
#define INDEX_ML_COS_PHI                3

#endif
