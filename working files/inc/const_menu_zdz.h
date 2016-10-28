#ifndef __CONST_MENU_ZDZ__
#define __CONST_MENU_ZDZ__

#define EKRAN_CONTROL_ZDZ               (EKRAN_CONTROL_MTZ + 1)

#define MAX_ROW_FOR_CONTROL_ZDZ  5

#define INDEX_ML_CTRZDZ_STATE                  0
#define CTR_ZDZ_STATE                          (1 << INDEX_ML_CTRZDZ_STATE)          
#define INDEX_ML_CTRZDZ_STARTED_FROM_MTZ1      1
#define CTR_ZDZ_STARTED_FROM_MTZ1              (1 << INDEX_ML_CTRZDZ_STARTED_FROM_MTZ1)          
#define INDEX_ML_CTRZDZ_STARTED_FROM_MTZ2      2
#define CTR_ZDZ_STARTED_FROM_MTZ2              (1 << INDEX_ML_CTRZDZ_STARTED_FROM_MTZ2)          
#define INDEX_ML_CTRZDZ_STARTED_FROM_MTZ3      3
#define CTR_ZDZ_STARTED_FROM_MTZ3              (1 << INDEX_ML_CTRZDZ_STARTED_FROM_MTZ3)          
#define INDEX_ML_CTRZDZ_STARTED_FROM_MTZ4      4
#define CTR_ZDZ_STARTED_FROM_MTZ4              (1 << INDEX_ML_CTRZDZ_STARTED_FROM_MTZ4)          

#define CTR_ZDZ_MASKA                    (                              \
                                            CTR_ZDZ_STATE              |\
                                            CTR_ZDZ_STARTED_FROM_MTZ1  |\
                                            CTR_ZDZ_STARTED_FROM_MTZ2  |\
                                            CTR_ZDZ_STARTED_FROM_MTZ3  |\
                                            CTR_ZDZ_STARTED_FROM_MTZ4   \
                                          )

#endif
