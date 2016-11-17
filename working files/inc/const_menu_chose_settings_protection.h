#ifndef __CONST_MENU_CHOSE_SETTING_PROTECTION__
#define __CONST_MENU_CHOSE_SETTING_PROTECTION__

#define EKRAN_CHOOSE_SETTINGS_CTRL_PHASE                (EKRAN_MEASURMENT_POWER + 1)
#define EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_CTRL_PHASE (EKRAN_CHOOSE_SETTINGS_CTRL_PHASE + 1)
#define EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_CTRL_PHASE (EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_CTRL_PHASE + 1)
#define EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_CTRL_PHASE (EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP2_CTRL_PHASE + 1)
#define EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_CTRL_PHASE (EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP3_CTRL_PHASE + 1)

#define MAX_ROW_FOR_CHOSE_SETTINGS_ANY_PROTECTION                    3
#define MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TWO_GROUP         (NUMBER_GROUP_USTAVOK + 1)
#define MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS                         2
#define MAX_ROW_FOR_CHOSE_SETPOINTS_TIMEOUTS_AVR                     5
#define MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_CONTROL           1
#define MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT           1
#define MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_TIMEOUT_CONTROL   2
#define MAX_ROW_FOR_CHOSE_SETTINGS_PROTECTION_WITH_SETPOINTS_CONTROL 2

#define INDEX_ML_GROUP1                                   0
#define INDEX_ML_GROUP2                                   1
#define INDEX_ML_GROUP3                                   2
#define INDEX_ML_GROUP4                                   3
#define INDEX_ML_CONTROL_WITH_GROUPS                      4

//Це є основні позиції
#define INDEX_ML_SETPOINT                                 0
#define INDEX_ML_TIMEOUT                                  1
#define INDEX_ML_CONTROL                                  2

#define INDEX_ML_SETPOINT_AVR_UMIN                        0
#define INDEX_ML_SETPOINT_AVR_UMAX                        1
#define INDEX_ML_TIMEOUT_AVR_GENERAL                      2
#define INDEX_ML_TIMEOUT_AVR_UMIN                         3
#define INDEX_ML_TIMEOUT_AVR_UMAX                         4

//Це є додаткові позиції, якщо вони не співпадають з основними
#define INDEX_ML_WITH_CONTROL_CONTROL                     0
#define INDEX_ML_WITH_TIMEOUT_TIMEOUT                     0
#define INDEX_ML_WITH_TIMEOUT_CONTROL_TIMEOUT             0
#define INDEX_ML_WITH_TIMEOUT_CONTROL_CONTROL             1
#define INDEX_ML_WITH_SETPOINT_CONTROL_SETPOINT           0
#define INDEX_ML_WITH_SETPOINT_CONTROL_CONTROL            1


#endif
