#ifndef __CONSTANTS_DEFINED_FUNCTIONS__
#define __CONSTANTS_DEFINED_FUNCTIONS__

#define EKRAN_LIST_SETTINGS_FOR_DF              (EKRAN_GENERAL_PICKUPS_EL + 1)
#define EKRAN_LIST_TYPE_DF                      (EKRAN_LIST_SETTINGS_FOR_DF + 1)
#define EKRAN_LIST_DF_FOR_RANGUVANNJA           (EKRAN_LIST_TYPE_DF + 1)
#define EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS      (EKRAN_LIST_DF_FOR_RANGUVANNJA + 1)
#define EKRAN_TIMEOUT_DF1                       (EKRAN_LIST_DF_FOR_TIMEOUT_SETTINGS + 1)
#define EKRAN_TIMEOUT_DF2                       (EKRAN_TIMEOUT_DF1 + 1)
#define EKRAN_TIMEOUT_DF3                       (EKRAN_TIMEOUT_DF2 + 1)
#define EKRAN_TIMEOUT_DF4                       (EKRAN_TIMEOUT_DF3 + 1)
#define EKRAN_TIMEOUT_DF5                       (EKRAN_TIMEOUT_DF4 + 1)
#define EKRAN_TIMEOUT_DF6                       (EKRAN_TIMEOUT_DF5 + 1)
#define EKRAN_TIMEOUT_DF7                       (EKRAN_TIMEOUT_DF6 + 1)
#define EKRAN_TIMEOUT_DF8                       (EKRAN_TIMEOUT_DF7 + 1)
#define EKRAN_RANGUVANNJA_DF1                   (EKRAN_TIMEOUT_DF8 + 1)
#define EKRAN_RANGUVANNJA_DF2                   (EKRAN_RANGUVANNJA_DF1 + 1)
#define EKRAN_RANGUVANNJA_DF3                   (EKRAN_RANGUVANNJA_DF2 + 1)
#define EKRAN_RANGUVANNJA_DF4                   (EKRAN_RANGUVANNJA_DF3 + 1)
#define EKRAN_RANGUVANNJA_DF5                   (EKRAN_RANGUVANNJA_DF4 + 1)
#define EKRAN_RANGUVANNJA_DF6                   (EKRAN_RANGUVANNJA_DF5 + 1)
#define EKRAN_RANGUVANNJA_DF7                   (EKRAN_RANGUVANNJA_DF6 + 1)
#define EKRAN_RANGUVANNJA_DF8                   (EKRAN_RANGUVANNJA_DF7 + 1)

#define MAX_ROW_SETTINGS_DF                      3
#define MAX_ROW_FOR_TYPE_DF                      /*NUMBER_DEFINED_FUNCTIONS*/((int)current_settings.number_defined_df)
#define MAX_ROW_FOR_LIST_DF                      /*NUMBER_DEFINED_FUNCTIONS*/((int)current_settings.number_defined_df)
#define MAX_ROW_TIMEOUT_DF                       2
#define MAX_ROW_RANGUVANNJA_DF                   NUMBER_TOTAL_SIGNAL_FOR_RANG


#define INDEX_ML_SETTING_DF_SOURCE            0
#define INDEX_ML_SETTING_DF_TIMEOUT           1
#define INDEX_ML_SETTING_DF_TYPE              2

#define INDEX_ML_TMO_DF_PAUSE                 0
#define INDEX_ML_TMO_DF_WORK                  1

#define COL_TMO_DF_PAUSE_BEGIN                3
#define COL_TMO_DF_PAUSE_COMMA                6
#define COL_TMO_DF_PAUSE_END                  8

#define COL_TMO_DF_WORK_BEGIN                 3
#define COL_TMO_DF_WORK_COMMA                 6
#define COL_TMO_DF_WORK_END                   8

#endif 
