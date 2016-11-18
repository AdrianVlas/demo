#ifndef __CONST_GLOBAL__
#define __CONST_GLOBAL__

#define N_BIG   7

//Величина, яка необхідна для періодичної (період 1  мс   ) генерації переривань каналом 1 таймеру 2 з прескаллером 2000 = (1999 + 1)
#define TIM2_CCR1_VAL                   30
//Величина, яка необхідна для періодичної (період 10 мс   ) генерації переривань каналом 1 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR1_VAL                   1000
//Величина, яка необхідна для періодичної (період 1 мс   ) генерації переривань каналом 2 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR2_VAL                   100
//Величина, яка необхідна для періодичної (як мінімум, період 10 мкс   ) генерації переривань каналом 3 таймеру 4 з прескаллером 600 = (599 + 1)
#define TIM4_CCR3_VAL                   1
//Величина, яка необхідна для періодичної (період 0,625 мс) генерації переривань каналом 1 таймеру 3 з прескаллером 1 = (0 + 1)
#define TIM5_CCR1_2_3_VAL               37500

#define MAX_NUMBER_LINES_VMP            8

#define DELTA_TIME_FOR_TIMERS           1
#define PERIOD_SIGNAL_OUTPUT_MODE_2     1000 /*мс*/

#define NUMBER_ANALOG_CANALES_VAL_1     6
#define NUMBER_ANALOG_CANALES_VAL_2     3

#define NUMBER_ANALOG_CANALES           (                               \
                                         NUMBER_ANALOG_CANALES_VAL_1  + \
                                         NUMBER_ANALOG_CANALES_VAL_2    \
                                        )

#define NUMBER_INPUTS_1                 5
#define NUMBER_INPUTS_2                 5
#define NUMBER_INPUTS                   (NUMBER_INPUTS_1 + NUMBER_INPUTS_2)
#define NUMBER_OUTPUTS_1                6
#define NUMBER_OUTPUTS_2                4
#define NUMBER_OUTPUTS                  (NUMBER_OUTPUTS_1 + NUMBER_OUTPUTS_2)

enum _ID_RANG_SOURCE 
{
  ID_OUTPUT = 0,
  ID_LED,
  ID_DTIMER,
  ID_DTRIGER,
  ID_DAND,
  ID_DOR,
  ID_DXOR,
  ID_DNOT,
  
  MAX_NUMBER_ID_RANG_SOURCE
};

#define NUMBER_LEDS                     8

//#define NUMBER_DEFINED_ELEMENTS         6
#define NUMBER_DEFINED_FUNCTIONS        8
#define NUMBER_DEFINED_TRIGGERS         4
#define NUMBER_DEFINED_AND              8
#define NUMBER_DEFINED_OR               8
#define NUMBER_DEFINED_XOR              8
#define NUMBER_DEFINED_NOT              16

#define NUMBER_DEFINED_BUTTONS          6

#define NUMBER_IN_AND                   8
#define NUMBER_IN_OR                    8

enum _configuration {
CTRL_PHASE_BIT_CONFIGURATION,
EL_BIT_CONFIGURATION,

TOTAL_NUMBER_PROTECTION
};

/*****************************************/
//Константи для ранжування функціональних кнопок
/*****************************************/
enum __rang_button {
RANG_BUTTON_RESET_LEDS  = 0,
RANG_BUTTON_RESET_RELES,
RANG_BUTTON_MISCEVE_DYSTANCIJNE,
RANG_BUTTON_VKL_VV,
RANG_BUTTON_OTKL_VV,
RANG_BUTTON_RESET_BLOCK_READY_TU_VID_ZAHYSTIV
};

#define NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON     6
#define NUMBER_EL_SIGNAL_FOR_RANG_BUTTON          0
#define NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG_BUTTON  0

#define NUMBER_TOTAL_SIGNAL_FOR_RANG_BUTTON       (                                            \
                                                   NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON     + \
                                                   NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG_BUTTON  + \
                                                   NUMBER_EL_SIGNAL_FOR_RANG_BUTTON            \
                                                  ) 
/*****************************************/

/*****************************************/
//Константи для ранжування дискретних виходів-свтодіодів-О-функцій-реєстраторів
/*****************************************/
enum __rang_output_led_df_reg {
RANG_BLOCK_VKL_VV = 0,
RANG_RESET_LEDS,
RANG_RESET_RELES,
RANG_MISCEVE_DYSTANCIJNE,
RANG_STATE_VV,
RANG_PUSK_K1_AVR,
RANG_PUSK_K2_AVR,
RANG_TN1,
RANG_TN2,
RANG_OTKL_VID_ZOVN_ZAHYSTIV,
RANG_VKL_VV,
RANG_CTRL_VKL,
RANG_OTKL_VV,
RANG_CTRL_OTKL,
RANG_PRYVID_VV,
RANG_PEREVYSHCHENNJA_Inom_VYMK,
RANG_KRYTYCHNYJ_RESURS_VYMYKACHA,
RANG_VYCHERPANYJ_RESURS_VYMYKACHA,
RANG_DEFECT,
RANG_AVAR_DEFECT,
RANG_WORK_A_REJESTRATOR,
RANG_WORK_D_REJESTRATOR,
RANG_VIDKL_VID_ZAKHYSTIV,
RANG_WORK_BO,
RANG_WORK_BV,
RANG_1_GRUPA_USTAVOK,
RANG_2_GRUPA_USTAVOK,
RANG_3_GRUPA_USTAVOK,
RANG_4_GRUPA_USTAVOK,
RANG_INVERS_DV_GRUPA_USTAVOK,
RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV,
RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV,
RANG_READY_TU,
RANG_SETTINGS_CHANGED,

RANG_DF1_IN,
RANG_DF1_OUT,
RANG_DF2_IN,
RANG_DF2_OUT,
RANG_DF3_IN,
RANG_DF3_OUT,
RANG_DF4_IN,
RANG_DF4_OUT,
RANG_DF5_IN,
RANG_DF5_OUT,
RANG_DF6_IN,
RANG_DF6_OUT,
RANG_DF7_IN,
RANG_DF7_OUT,
RANG_DF8_IN,
RANG_DF8_OUT,
RANG_DT1_SET,
RANG_DT1_RESET,
RANG_DT1_OUT,
RANG_DT2_SET,
RANG_DT2_RESET,
RANG_DT2_OUT,
RANG_DT3_SET,
RANG_DT3_RESET,
RANG_DT3_OUT,
RANG_DT4_SET,
RANG_DT4_RESET,
RANG_DT4_OUT,
RANG_D_AND1,
RANG_D_AND2,
RANG_D_AND3,
RANG_D_AND4,
RANG_D_AND5,
RANG_D_AND6,
RANG_D_AND7,
RANG_D_AND8,
RANG_D_OR1,
RANG_D_OR2,
RANG_D_OR3,
RANG_D_OR4,
RANG_D_OR5,
RANG_D_OR6,
RANG_D_OR7,
RANG_D_OR8,
RANG_D_XOR1,
RANG_D_XOR2,
RANG_D_XOR3,
RANG_D_XOR4,
RANG_D_XOR5,
RANG_D_XOR6,
RANG_D_XOR7,
RANG_D_XOR8,
RANG_D_NOT1,
RANG_D_NOT2,
RANG_D_NOT3,
RANG_D_NOT4,
RANG_D_NOT5,
RANG_D_NOT6,
RANG_D_NOT7,
RANG_D_NOT8,
RANG_D_NOT9,
RANG_D_NOT10,
RANG_D_NOT11,
RANG_D_NOT12,
RANG_D_NOT13,
RANG_D_NOT14,
RANG_D_NOT15,
RANG_D_NOT16,
RANG_ERROR_CONF_EL,

RANG_ERROR_DELTA_U_CTRL_PHASE,
RANG_ERROR_DELTA_PHI_CTRL_PHASE,
RANG_ERROR_DELTA_F_CTRL_PHASE,
RANG_ERROR_SEC_TN1_CTRL_PHASE,
RANG_ERROR_SEC_TN2_CTRL_PHASE
};

#define NUMBER_GENERAL_SIGNAL_FOR_RANG     34
#define NUMBER_EL_SIGNAL_FOR_RANG          69
#define NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG  5

#define NUMBER_TOTAL_SIGNAL_FOR_RANG       (                                                      \
                                                              NUMBER_GENERAL_SIGNAL_FOR_RANG    + \
                                                              NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG + \
                                                              NUMBER_EL_SIGNAL_FOR_RANG           \
                                                             ) 

/*****************************************/

/*****************************************/
//Макски сигналів, які активовуються або з д.входів, або з інтирфейсів чи функціональних кнопок
/*****************************************/

#define MASKA_FOR_INPUT_SIGNALS_0        (unsigned int)(              \
    (1 << (RANG_BLOCK_VKL_VV - 0))                            /* 0*/  \
  | (1 << (RANG_RESET_LEDS - 0))                              /* 1*/  \
  | (1 << (RANG_RESET_RELES - 0))                             /* 2*/  \
  | (1 << (RANG_MISCEVE_DYSTANCIJNE - 0))                     /* 3*/  \
  | (1 << (RANG_STATE_VV - 0))                                /* 4*/  \
  | (1 << (RANG_PUSK_K1_AVR - 0))                             /* 5*/  \
  | (1 << (RANG_PUSK_K2_AVR - 0))                             /* 6*/  \
  | (1 << (RANG_OTKL_VID_ZOVN_ZAHYSTIV - 0))                  /* 7*/  \
  | (1 << (RANG_VKL_VV - 0))                                  /*10*/  \
  | (1 << (RANG_CTRL_VKL - 0))                                /*11*/  \
  | (1 << (RANG_OTKL_VV - 0))                                 /*12*/  \
  | (1 << (RANG_CTRL_OTKL - 0))                               /*13*/  \
  | (1 << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV - 0))       /*31*/  \
)

#define MASKA_FOR_INPUT_SIGNALS_1                  0
#define MASKA_FOR_INPUT_SIGNALS_2                  0
#define MASKA_FOR_INPUT_SIGNALS_3                  0
#define MASKA_FOR_INPUT_SIGNALS_4                  0
#define MASKA_FOR_INPUT_SIGNALS_5                  0
#define MASKA_FOR_INPUT_SIGNALS_6                  0
/*****************************************/


/*****************************************/
//Макски всіх сигналів Перевірки фазування
/*****************************************/
#define MASKA_CTRL_PHASE_SIGNALS_0                 0
#define MASKA_CTRL_PHASE_SIGNALS_1                 0
#define MASKA_CTRL_PHASE_SIGNALS_2                 0
#define MASKA_CTRL_PHASE_SIGNALS_3                 0

#define MASKA_CTRL_PHASE_SIGNALS_4 (unsigned int)(                  \
     (1 << (RANG_ERROR_DELTA_U_CTRL_PHASE - 128))            /*130*/\
   | (1 << (RANG_ERROR_DELTA_PHI_CTRL_PHASE - 128))          /*131*/\
   | (1 << (RANG_ERROR_DELTA_F_CTRL_PHASE - 128))            /*132*/\
   | (1 << (RANG_ERROR_SEC_TN1_CTRL_PHASE - 128))            /*133*/\
   | (1 << (RANG_ERROR_SEC_TN2_CTRL_PHASE - 128))            /*134*/\
)

#define MASKA_CTRL_PHASE_SIGNALS_5                 0
#define MASKA_CTRL_PHASE_SIGNALS_6                 0
/*****************************************/

/*****************************************/
//Макски всіх сигналів розширеної логіки
/*****************************************/
#define MASKA_EL_SIGNALS_0                  0
#define MASKA_EL_SIGNALS_1                  0
#define MASKA_EL_SIGNALS_2                  0
#define MASKA_EL_SIGNALS_3                  0

#define MASKA_EL_SIGNALS_4        (unsigned int)(                    \
    (1 << (RANG_DF1_IN - 128))                                /*135*/\
  | (1 << (RANG_DF1_OUT - 128))                               /*136*/\
  | (1 << (RANG_DF2_IN - 128))                                /*137*/\
  | (1 << (RANG_DF2_OUT - 128))                               /*138*/\
  | (1 << (RANG_DF3_IN - 128))                                /*139*/\
  | (1 << (RANG_DF3_OUT - 128))                               /*140*/\
  | (1 << (RANG_DF4_IN - 128))                                /*141*/\
  | (1 << (RANG_DF4_OUT - 128))                               /*142*/\
  | (1 << (RANG_DF5_IN - 128))                                /*143*/\
  | (1 << (RANG_DF5_OUT - 128))                               /*144*/\
  | (1 << (RANG_DF6_IN - 128))                                /*145*/\
  | (1 << (RANG_DF6_OUT - 128))                               /*146*/\
  | (1 << (RANG_DF7_IN - 128))                                /*147*/\
  | (1 << (RANG_DF7_OUT - 128))                               /*148*/\
  | (1 << (RANG_DF8_IN - 128))                                /*149*/\
  | (1 << (RANG_DF8_OUT - 128))                               /*150*/\
  | (1 << (RANG_DT1_SET - 128))                               /*151*/\
  | (1 << (RANG_DT1_RESET - 128))                             /*152*/\
  | (1 << (RANG_DT1_OUT - 128))                               /*153*/\
  | (1 << (RANG_DT2_SET - 128))                               /*154*/\
  | (1 << (RANG_DT2_RESET - 128))                             /*155*/\
  | (1 << (RANG_DT2_OUT - 128))                               /*156*/\
  | (1 << (RANG_DT3_SET - 128))                               /*157*/\
  | (1 << (RANG_DT3_RESET - 128))                             /*158*/\
  | (1 << (RANG_DT3_OUT - 128))                               /*159*/\
)
    
#define MASKA_EL_SIGNALS_5        (unsigned int)(                    \
    (1 << (RANG_DT4_SET - 160))                               /*160*/\
  | (1 << (RANG_DT4_RESET - 160))                             /*161*/\
  | (1 << (RANG_DT4_OUT - 160))                               /*162*/\
  | (1 << (RANG_D_AND1 - 160))                                /*163*/\
  | (1 << (RANG_D_AND2 - 160))                                /*164*/\
  | (1 << (RANG_D_AND3 - 160))                                /*165*/\
  | (1 << (RANG_D_AND4 - 160))                                /*166*/\
  | (1 << (RANG_D_AND5 - 160))                                /*167*/\
  | (1 << (RANG_D_AND6 - 160))                                /*168*/\
  | (1 << (RANG_D_AND7 - 160))                                /*169*/\
  | (1 << (RANG_D_AND8 - 160))                                /*170*/\
  | (1 << (RANG_D_OR1 - 160))                                 /*171*/\
  | (1 << (RANG_D_OR2 - 160))                                 /*172*/\
  | (1 << (RANG_D_OR3 - 160))                                 /*173*/\
  | (1 << (RANG_D_OR4 - 160))                                 /*174*/\
  | (1 << (RANG_D_OR5 - 160))                                 /*175*/\
  | (1 << (RANG_D_OR6 - 160))                                 /*176*/\
  | (1 << (RANG_D_OR7 - 160))                                 /*177*/\
  | (1 << (RANG_D_OR8 - 160))                                 /*178*/\
  | (1 << (RANG_D_XOR1 - 160))                                /*179*/\
  | (1 << (RANG_D_XOR2 - 160))                                /*180*/\
  | (1 << (RANG_D_XOR3 - 160))                                /*181*/\
  | (1 << (RANG_D_XOR4 - 160))                                /*182*/\
  | (1 << (RANG_D_XOR5 - 160))                                /*183*/\
  | (1 << (RANG_D_XOR6 - 160))                                /*184*/\
  | (1 << (RANG_D_XOR7 - 160))                                /*185*/\
  | (1 << (RANG_D_XOR8 - 160))                                /*186*/\
  | (1 << (RANG_D_NOT1 - 160))                                /*187*/\
  | (1 << (RANG_D_NOT2 - 160))                                /*188*/\
  | (1 << (RANG_D_NOT3 - 160))                                /*189*/\
  | (1 << (RANG_D_NOT4 - 160))                                /*190*/\
  | (1 << (RANG_D_NOT5 - 160))                                /*191*/\
)

#define MASKA_EL_SIGNALS_6        (unsigned int)(                    \
    (1 << (RANG_D_NOT6 - 192))                                /*192*/\
  | (1 << (RANG_D_NOT7 - 192))                                /*193*/\
  | (1 << (RANG_D_NOT8 - 192))                                /*194*/\
  | (1 << (RANG_D_NOT9 - 192))                                /*195*/\
  | (1 << (RANG_D_NOT10 - 192))                               /*196*/\
  | (1 << (RANG_D_NOT11 - 192))                               /*197*/\
  | (1 << (RANG_D_NOT12 - 192))                               /*198*/\
  | (1 << (RANG_D_NOT13 - 192))                               /*199*/\
  | (1 << (RANG_D_NOT14 - 192))                               /*200*/\
  | (1 << (RANG_D_NOT15 - 192))                               /*201*/\
  | (1 << (RANG_D_NOT16 - 192))                               /*202*/\
  | (1 << (RANG_ERROR_CONF_EL - 192))                         /*203*/\
)
/*****************************************/


/*****************************************/
//Макска сигналів, які мають записуватися у енергонезалежну пам'ять
/*****************************************/
#define MASKA_TRIGGER_SIGNALES_0                  0
#define MASKA_TRIGGER_SIGNALES_1                  0
#define MASKA_TRIGGER_SIGNALES_2                  0
#define MASKA_TRIGGER_SIGNALES_3                  0

#define MASKA_TRIGGER_SIGNALES_4               (unsigned int)(       \
     (1 << (RANG_DT1_OUT - 128))                              /*153*/\
   | (1 << (RANG_DT2_OUT - 128))                              /*156*/\
   | (1 << (RANG_DT3_OUT - 128))                              /*159*/\
)

#define MASKA_TRIGGER_SIGNALES_5               (unsigned int)(       \
     (1 << (RANG_DT4_OUT - 160))                              /*162*/\
)

#define MASKA_TRIGGER_SIGNALES_6                  0
/*****************************************/


/*****************************************/
//Макска інформативних сигналів
/*****************************************/

#define MASKA_INFO_SIGNALES_0  (unsigned int)(                      \
     (1 << (RANG_STATE_VV - 0))                               /* 4*/\
   | (1 << (RANG_PUSK_K1_AVR - 0))                            /* 5*/\
   | (1 << (RANG_PUSK_K2_AVR - 0))                            /* 6*/\
   | (1 << (RANG_DEFECT - 0))                                 /*18*/\
   | (1 << (RANG_AVAR_DEFECT - 0))                            /*19*/\
   | (1 << (RANG_WORK_A_REJESTRATOR - 0))                     /*20*/\
   | (1 << (RANG_WORK_D_REJESTRATOR - 0))                     /*21*/\
)     

#define MASKA_INFO_SIGNALES_1  (unsigned int)(                      \
     (1 << (RANG_SETTINGS_CHANGED - 32))                      /*33*/\
)     

#define MASKA_INFO_SIGNALES_2                  0
#define MASKA_INFO_SIGNALES_3                  0
#define MASKA_INFO_SIGNALES_4                  0
#define MASKA_INFO_SIGNALES_5                  0
#define MASKA_INFO_SIGNALES_6                  0
/*****************************************/

/*****************************************/
//Максимальна кількість сигналів (функцій), які можуть бути зранжовані на входи/виходи/світлоіндикатори/д.реєстратор/оф
/*****************************************/
#define VAGA_MAX_FUNCTIONS_IN_INPUT             3
#define MAX_FUNCTIONS_IN_INPUT                  (1 << VAGA_MAX_FUNCTIONS_IN_INPUT)
#define VAGA_MAX_FUNCTIONS_IN_OUTPUT            4
#define MAX_FUNCTIONS_IN_OUTPUT                 (1 << VAGA_MAX_FUNCTIONS_IN_OUTPUT)
#define VAGA_MAX_FUNCTIONS_IN_LED               3
#define MAX_FUNCTIONS_IN_LED                    (1 << VAGA_MAX_FUNCTIONS_IN_LED)
#define VAGA_MAX_FUNCTIONS_IN_DF                4
#define MAX_FUNCTIONS_IN_DF                     (1 << VAGA_MAX_FUNCTIONS_IN_DF)
#define MAX_FUNCTIONS_IN_DT                     6
#define MAX_FUNCTIONS_IN_D_AND                  NUMBER_IN_AND
#define MAX_FUNCTIONS_IN_D_OR                   NUMBER_IN_OR
#define MAX_FUNCTIONS_IN_D_XOR                  2
#define MAX_FUNCTIONS_IN_D_NOT                  1
#define VAGA_MAX_FUNCTIONS_IN_DB                3
#define MAX_FUNCTIONS_IN_DB                     (1 << VAGA_MAX_FUNCTIONS_IN_DB)
#define VAGA_MAX_FUNCTIONS_IN_AREG              5
#define MAX_FUNCTIONS_IN_AREG                   (1 << VAGA_MAX_FUNCTIONS_IN_AREG)
#define VAGA_MAX_FUNCTIONS_IN_DREG              5
#define MAX_FUNCTIONS_IN_DREG                   (1 << VAGA_MAX_FUNCTIONS_IN_DREG)
#define VAGA_MAX_FUNCTIONS_IN_OFF_CB            5
#define MAX_FUNCTIONS_IN_OFF_CB                 (1 << VAGA_MAX_FUNCTIONS_IN_OFF_CB)
#define VAGA_MAX_FUNCTIONS_IN_ON_CB             5
#define MAX_FUNCTIONS_IN_ON_CB                  (1 << VAGA_MAX_FUNCTIONS_IN_ON_CB)
/*****************************************/


/*****************************************/
//Константи для комунікації
/*****************************************/
#define MAX_CHAR_IN_NAME_OF_CELL                              32
/*****************************************/

/*****************************************/
//Константи для фіксації зміни настройок
/*****************************************/
#define CHANGED_ETAP_NONE                                     0
#define CHANGED_ETAP_EXECUTION                                1
#define CHANGED_ETAP_ENDED                                    2
#define CHANGED_ETAP_ENDED_EXTRA_ETAP                         3
/*****************************************/

/*****************************************/
//Константи для ранжування функціональних кнопок
/*****************************************/
enum __odynyci_vymirjuvannja {
INDEX_A = 0,
INDEX_V,
INDEX_DEGREE,
INDEX_SECOND,

NUMBER_ODYNYCI_VYMIRJUVANNJA
};
/*****************************************/

/*****************************************/
//Тип входів логічних елементів
/*****************************************/
#define TYPE_INPUT_0_1        0
#define TYPE_INPUT_1_0        1
/*****************************************/

/*****************************************/
//Індетифікатор типу набору юстуючих коефіцієнтів
/*****************************************/
//#define ADJUSTMENT_3I0_IA_IB_IC                             1
//#define ADJUSTMENT_3I0_IA_IC_3U0                            2
//#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_UABTN2             3
//#define ADJUSTMENT_3I0_IA_IB_IC_UA_UB_UC_3U0                4
#define ADJUSTMENT_IA_IB_IC_UA1_UB1_UC1_UA2_UB2_UC2             5
#define ADJUSTMENT_ID                                         ADJUSTMENT_IA_IB_IC_UA1_UB1_UC1_UA2_UB2_UC2
/*****************************************/

/*****************************************/
//Версія програмного забезпечення
/*****************************************/
#define VERSIA_PZ                                             12
#define MODYFIKACIA_VERSII_PZ                                 0
#define ZBIRKA_VERSII_PZ                                      0
/*****************************************/

/*****************************************/
//Час компіляції
/*****************************************/

#define YEAR_VER                                              ((__DATE__[9] != ' ') ? (((__DATE__[9] - 0x30) << 4) | (__DATE__[10] - 0x30)) : (__DATE__[10] - 0x30))
#define MONTH_VER                                             str_to_int_DATE_Mmm()
#define DAY_VER                                               ((__DATE__[4] != ' ') ? (((__DATE__[4] - 0x30) << 4) | (__DATE__[5] - 0x30)) : (__DATE__[5] - 0x30))
#define HOUR_VER                                              (((__TIME__[0] - 0x30) << 4) | (__TIME__[1] - 0x30))
#define MINUTE_VER                                            (((__TIME__[3] - 0x30) << 4) | (__TIME__[4] - 0x30))
#define SECOND_VER                                            (((__TIME__[6] - 0x30) << 4) | (__TIME__[7] - 0x30))
/*****************************************/

/*****************************************/
//Версія універсальної карти памяті
/*****************************************/
#define VERSIA_GMM                                             7
#define MODYFIKACIA_VERSII_GMM                                 0
/*****************************************/
#endif
