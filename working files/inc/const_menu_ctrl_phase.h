#ifndef __CONST_MENU_CTRL_PHASE__
#define __CONST_MENU_CTRL_PHASE__

#define EKRAN_SETPOINT_CTRL_PHASE_GROUP1   (EKRAN_CHOSE_SETTINGS + 1)
#define EKRAN_SETPOINT_CTRL_PHASE_GROUP2   (EKRAN_SETPOINT_CTRL_PHASE_GROUP1 + 1)
#define EKRAN_SETPOINT_CTRL_PHASE_GROUP3   (EKRAN_SETPOINT_CTRL_PHASE_GROUP2 + 1)
#define EKRAN_SETPOINT_CTRL_PHASE_GROUP4   (EKRAN_SETPOINT_CTRL_PHASE_GROUP3 + 1)
#define EKRAN_TIMEOUT_CTRL_PHASE_GROUP1    (EKRAN_SETPOINT_CTRL_PHASE_GROUP4 + 1)
#define EKRAN_TIMEOUT_CTRL_PHASE_GROUP2    (EKRAN_TIMEOUT_CTRL_PHASE_GROUP1 + 1)
#define EKRAN_TIMEOUT_CTRL_PHASE_GROUP3    (EKRAN_TIMEOUT_CTRL_PHASE_GROUP2 + 1)
#define EKRAN_TIMEOUT_CTRL_PHASE_GROUP4    (EKRAN_TIMEOUT_CTRL_PHASE_GROUP3 + 1)
#define EKRAN_CONTROL_CTRL_PHASE           (EKRAN_TIMEOUT_CTRL_PHASE_GROUP4 + 1)

#define MAX_ROW_FOR_SETPOINT_CTRL_PHASE        3
#define MAX_ROW_FOR_TIMEOUT_CTRL_PHASE         6
#define MAX_ROW_FOR_CONTROL_CTRL_PHASE         5

#define INDEX_ML_STPCTRL_PHASE_U               0
#define INDEX_ML_STPCTRL_PHASE_PHI             1
#define INDEX_ML_STPCTRL_PHASE_F               2

#define INDEX_ML_TMOCTRL_PHASE_U               0
#define INDEX_ML_TMOCTRL_PHASE_U_D             1
#define INDEX_ML_TMOCTRL_PHASE_PHI             2
#define INDEX_ML_TMOCTRL_PHASE_PHI_D           3
#define INDEX_ML_TMOCTRL_PHASE_F               4
#define INDEX_ML_TMOCTRL_PHASE_F_D             5

#define COL_SETPOINT_CTRL_PHASE_U_BEGIN        5
#define COL_SETPOINT_CTRL_PHASE_U_COMMA        7
#define COL_SETPOINT_CTRL_PHASE_U_END          8

#define COL_SETPOINT_CTRL_PHASE_PHI_BEGIN      5
#define COL_SETPOINT_CTRL_PHASE_PHI_COMMA      7
#define COL_SETPOINT_CTRL_PHASE_PHI_END        8

#define COL_SETPOINT_CTRL_PHASE_F_BEGIN        4
#define COL_SETPOINT_CTRL_PHASE_F_COMMA        5
#define COL_SETPOINT_CTRL_PHASE_F_END          7

#define COL_TMO_CTRL_PHASE_U_BEGIN             4
#define COL_TMO_CTRL_PHASE_U_COMMA             6
#define COL_TMO_CTRL_PHASE_U_END               8

#define COL_TMO_CTRL_PHASE_U_D_BEGIN           4
#define COL_TMO_CTRL_PHASE_U_D_COMMA           6
#define COL_TMO_CTRL_PHASE_U_D_END             8

#define COL_TMO_CTRL_PHASE_PHI_BEGIN           4
#define COL_TMO_CTRL_PHASE_PHI_COMMA           6
#define COL_TMO_CTRL_PHASE_PHI_END             8

#define COL_TMO_CTRL_PHASE_PHI_D_BEGIN         4
#define COL_TMO_CTRL_PHASE_PHI_D_COMMA         6
#define COL_TMO_CTRL_PHASE_PHI_D_END           8

#define COL_TMO_CTRL_PHASE_F_BEGIN             4
#define COL_TMO_CTRL_PHASE_F_COMMA             6
#define COL_TMO_CTRL_PHASE_F_END               8

#define COL_TMO_CTRL_PHASE_F_D_BEGIN           4
#define COL_TMO_CTRL_PHASE_F_D_COMMA           6
#define COL_TMO_CTRL_PHASE_F_D_END             8

#define INDEX_CTR_CTRL_PHASE_U                 0
#define CTR_CTRL_PHASE_U                       (1 << INDEX_CTR_CTRL_PHASE_U)
#define INDEX_CTR_CTRL_PHASE_PHI               1
#define CTR_CTRL_PHASE_PHI                     (1 << INDEX_CTR_CTRL_PHASE_PHI)
#define INDEX_CTR_CTRL_PHASE_F                 2
#define CTR_CTRL_PHASE_F                       (1 << INDEX_CTR_CTRL_PHASE_F)
#define INDEX_CTR_CTRL_PHASE_SEQ_TN1           3
#define CTR_CTRL_PHASE_SEQ_TN1                 (1 << INDEX_CTR_CTRL_PHASE_SEQ_TN1)
#define INDEX_CTR_CTRL_PHASE_SEQ_TN2           4
#define CTR_CTRL_PHASE_SEQ_TN2                 (1 << INDEX_CTR_CTRL_PHASE_SEQ_TN2)

#define CTR_CTRL_PHASE_MASKA                   (                         \
                                                CTR_CTRL_PHASE_U        |\
                                                CTR_CTRL_PHASE_PHI      |\
                                                CTR_CTRL_PHASE_F        |\
                                                CTR_CTRL_PHASE_SEQ_TN1  |\
                                                CTR_CTRL_PHASE_SEQ_TN2   \
                                               )

#endif
