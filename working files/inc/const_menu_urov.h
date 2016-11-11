#ifndef __CONST_MENU_UROV__
#define __CONST_MENU_UROV__

#define EKRAN_SETPOINT_UROV_GROUP1 (EKRAN_CHOSE_SETTINGS             + 1)
#define EKRAN_SETPOINT_UROV_GROUP2 (EKRAN_SETPOINT_UROV_GROUP1       + 1)
#define EKRAN_SETPOINT_UROV_GROUP3 (EKRAN_SETPOINT_UROV_GROUP2       + 1)
#define EKRAN_SETPOINT_UROV_GROUP4 (EKRAN_SETPOINT_UROV_GROUP3       + 1)
#define EKRAN_TIMEOUT_UROV_GROUP1  (EKRAN_TIMEOUT_APV_GROUP4         + 1)
#define EKRAN_TIMEOUT_UROV_GROUP2  (EKRAN_TIMEOUT_UROV_GROUP1        + 1)
#define EKRAN_TIMEOUT_UROV_GROUP3  (EKRAN_TIMEOUT_UROV_GROUP2        + 1)
#define EKRAN_TIMEOUT_UROV_GROUP4  (EKRAN_TIMEOUT_UROV_GROUP3        + 1)
#define EKRAN_CONTROL_UROV         (EKRAN_CONTROL_APV                + 1)


#define MAX_ROW_FOR_SETPOINT_UROV  1
#define MAX_ROW_FOR_TIMEOUT_UROV   2
#define MAX_ROW_FOR_CONTROL_UROV   11

#define INDEX_ML_TMOUROV1          0
#define INDEX_ML_TMOUROV2          1

#define COL_SETPOINT_UROV_BEGIN    5
#define COL_SETPOINT_UROV_COMMA    6
#define COL_SETPOINT_UROV_END      8

#define COL_TMO_UROV_1_BEGIN       4
#define COL_TMO_UROV_1_COMMA       6
#define COL_TMO_UROV_1_END         8

#define COL_TMO_UROV_2_BEGIN       4
#define COL_TMO_UROV_2_COMMA       6
#define COL_TMO_UROV_2_END         8

#define INDEX_ML_CTRUROV_STATE                  0
#define CTR_UROV_STATE                          (1 << INDEX_ML_CTRUROV_STATE)          
#define INDEX_ML_CTRUROV_STARTED_FROM_MTZ1      1
#define CTR_UROV_STARTED_FROM_MTZ1              (1 << INDEX_ML_CTRUROV_STARTED_FROM_MTZ1)          
#define INDEX_ML_CTRUROV_STARTED_FROM_MTZ2      2
#define CTR_UROV_STARTED_FROM_MTZ2              (1 << INDEX_ML_CTRUROV_STARTED_FROM_MTZ2)          
#define INDEX_ML_CTRUROV_STARTED_FROM_MTZ3      3
#define CTR_UROV_STARTED_FROM_MTZ3              (1 << INDEX_ML_CTRUROV_STARTED_FROM_MTZ3)          
#define INDEX_ML_CTRUROV_STARTED_FROM_MTZ4      4
#define CTR_UROV_STARTED_FROM_MTZ4              (1 << INDEX_ML_CTRUROV_STARTED_FROM_MTZ4)          
#define INDEX_ML_CTRUROV_STARTED_FROM_ZDZ       5
#define CTR_UROV_STARTED_FROM_ZDZ               (1 << INDEX_ML_CTRUROV_STARTED_FROM_ZDZ)          
#define INDEX_ML_CTRUROV_STARTED_FROM_ZOP1      6
#define CTR_UROV_STARTED_FROM_ZOP1              (1 << INDEX_ML_CTRUROV_STARTED_FROM_ZOP1)
#define INDEX_ML_CTRUROV_STARTED_FROM_UMIN1     7
#define CTR_UROV_STARTED_FROM_UMIN1             (1 << INDEX_ML_CTRUROV_STARTED_FROM_UMIN1)
#define INDEX_ML_CTRUROV_STARTED_FROM_UMIN2     8
#define CTR_UROV_STARTED_FROM_UMIN2             (1 << INDEX_ML_CTRUROV_STARTED_FROM_UMIN2)
#define INDEX_ML_CTRUROV_STARTED_FROM_UMAX1     9
#define CTR_UROV_STARTED_FROM_UMAX1             (1 << INDEX_ML_CTRUROV_STARTED_FROM_UMAX1)
#define INDEX_ML_CTRUROV_STARTED_FROM_UMAX2     10
#define CTR_UROV_STARTED_FROM_UMAX2             (1 << INDEX_ML_CTRUROV_STARTED_FROM_UMAX2)

#define CTR_UROV_MASKA                    (                                \
                                            CTR_UROV_STATE                |\
                                            CTR_UROV_STARTED_FROM_MTZ1    |\
                                            CTR_UROV_STARTED_FROM_MTZ2    |\
                                            CTR_UROV_STARTED_FROM_MTZ3    |\
                                            CTR_UROV_STARTED_FROM_MTZ4    |\
                                            CTR_UROV_STARTED_FROM_ZDZ     |\
                                            CTR_UROV_STARTED_FROM_ZOP1    |\
                                            CTR_UROV_STARTED_FROM_UMIN1   |\
                                            CTR_UROV_STARTED_FROM_UMIN2   |\
                                            CTR_UROV_STARTED_FROM_UMAX1   |\
                                            CTR_UROV_STARTED_FROM_UMAX2    \
                                          )

#endif
