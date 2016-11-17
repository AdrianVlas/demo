#ifndef __TYPE_DEFINITION__
#define __TYPE_DEFINITION__

#define SRAM1 _Pragma("location=\"variables_RAM1\"")

typedef struct
{
  int current_level;                  //Текучий рівень відображуваного екрану
 
  int index_position;                 //Вказує на якій ми зараз є позиції

  int position_cursor_x;              //X - координата курсору
  int position_cursor_y;              //Y - координата курсору
  
  unsigned int edition;               //0 - вікно зараз не в режимі редагується;
                                      //1 - вікно зараз в режимі редагування;
                                      //2 - вікно чекає підтвердження редагування
                                      //3 - у вікні виведено повідомлення про те, що під час введення даних була зафіксована помилка величиниданих
  
  unsigned int cursor_on;             //1 - курсор відображається/ 0 - курсор не відображається
  unsigned int cursor_blinking_on;    //1 - курсор мигає/ 0 - курсор не мигає
  
  unsigned int current_action;        //Дія яку треба виконати з текучим екраном
                                      //0 - Нічого не виконувати
                                      //1 - Перемістити курсор, ш/або змінити стан його відображення
                                      //2 - Повністю обновити інформацію

} __CURRENT_EKRAN;

typedef struct
{
  int position_cursor_x;              //X - координата курсору

  unsigned int cursor_on;             //1 - курсор відображається/ 0 - курсор не відображається
  unsigned int cursor_blinking_on;    //1 - курсор мигає/ 0 - курсор не мигає
} __PREVIOUS_STATE_CURSOR;

typedef struct
{
  //Тип пристрою
  unsigned int device_id;
  
  //Дискретні входи
  unsigned int type_of_input;                          //Тип дискретного входу 0 - прямий, 1 - інверсний
  unsigned int type_of_input_signal;                   //Тип сигналу дискретного входу 0 - постійний , 1 - змінний
  unsigned int dopusk_dv[NUMBER_INPUTS];              //Допуски ДВ
    
  unsigned int ranguvannja_inputs[N_SMALL*NUMBER_INPUTS]; //Ранжування дискретних входів
  unsigned int ranguvannja_outputs[N_BIG*NUMBER_OUTPUTS]; //Ранжування дискретних вхиодів
  unsigned int ranguvannja_leds[N_BIG*NUMBER_LEDS];       //Ранжування свіотіндикаторів

  unsigned int ranguvannja_analog_registrator[N_BIG];     //Ранжування аналогового реєстратора
  unsigned int ranguvannja_digital_registrator[N_BIG];    //Ранжування дискретного реєстратора
  
  //Блок ввімкнення-вимкнення вимикача
  unsigned int ranguvannja_off_cb[N_BIG];                 //Ранжування блоку вимкнення силового вимикача
  unsigned int ranguvannja_on_cb[N_BIG];                  //Ранжування блоку ввімкнення силового вимикача
  
  //Тип дискретних виходів
  unsigned int type_of_output;                        //Тип дискретних виходів
                                                      //0 - Командний
                                                      //1 - Сигналький (постійний або імпульсний)
  
  unsigned int type_of_output_modif;                  //Тип дискретних сигнальних виходів
                                                      //0 - Сигналький (постійний)
                                                      //1 - Сигналький (імпульсний)
  
  //Тип світлоіндикаторів
  unsigned int type_of_led;                           //Тип світлоіндикаторі
                                                      //0 - Нормальний
                                                      //1 - Тригерний
    
  unsigned int number_iteration_el;                                             //Максимадбна кількість ітерацій для розширеної логіки
//  unsigned int number_defined_df;                                               //Кількість визначуваних функцій у конфігурації приладу
//  unsigned int number_defined_dt;                                               //Кількість визначуваних тригерів у конфігурації приладу
//  unsigned int number_defined_and;                                              //Кількість визначуваних "І" у конфігурації приладу
//  unsigned int number_defined_or;                                               //Кількість визначуваних "АБО" у конфігурації приладу
//  unsigned int number_defined_xor;                                              //Кількість визначуваних "Викл.ВБО" у конфігурації приладу
//  unsigned int number_defined_not;                                              //Кількість визначуваних "НЕ" у конфігурації приладу

  //Опреділювальні функції
  unsigned int type_df;                                                 //Тип опреділювальної функції
                                                                        //0 - Пряма
                                                                        //1 - Зворотня
  unsigned int timeout_pause_df[NUMBER_DEFINED_FUNCTIONS];              //Час затримки спрацюваня опреділювальної функції
  unsigned int timeout_work_df[NUMBER_DEFINED_FUNCTIONS];               //Час роботи опреділювальної функції
  unsigned int ranguvannja_df_source_plus[N_BIG*NUMBER_DEFINED_FUNCTIONS];  //Ранжування прямих команд опреділювальниї функцій
  unsigned int ranguvannja_df_source_minus[N_BIG*NUMBER_DEFINED_FUNCTIONS]; //Ранжування інверсних команд опреділювальниї функцій
  unsigned int ranguvannja_df_source_blk[N_BIG*NUMBER_DEFINED_FUNCTIONS];   //Ранжування команд блокування опреділювальниї функцій

  unsigned int ranguvannja_set_dt_source_plus[N_BIG*NUMBER_DEFINED_TRIGGERS];   //Ранжування прямих команд встановлення опреділювальних триґерів
  unsigned int ranguvannja_set_dt_source_minus[N_BIG*NUMBER_DEFINED_TRIGGERS];  //Ранжування інверсних команд встановлення опреділювальних триґерів
  unsigned int ranguvannja_reset_dt_source_plus[N_BIG*NUMBER_DEFINED_TRIGGERS]; //Ранжування прямих команд скидання опреділювальних триґерів
  unsigned int ranguvannja_reset_dt_source_minus[N_BIG*NUMBER_DEFINED_TRIGGERS];//Ранжування інверсних команд скидання опреділювальних триґерів

  unsigned int ranguvannja_d_and[N_BIG*NUMBER_DEFINED_AND];                     //Ранжування команд джерел визначуваних "І"
  unsigned int ranguvannja_d_or[N_BIG*NUMBER_DEFINED_OR];                       //Ранжування команд джерел визначуваних "АБО"
  unsigned int ranguvannja_d_xor[N_BIG*NUMBER_DEFINED_XOR];                     //Ранжування команд джерел визначуваних "Викл.АБО"
  unsigned int ranguvannja_d_not[N_BIG*NUMBER_DEFINED_NOT];                     //Ранжування команд джерел визначуваних "НЕ"
  
  unsigned int ranguvannja_buttons[NUMBER_DEFINED_BUTTONS];                     //Ранжування опреділюваних кнопок
  
  unsigned int configuration;         //Конфігурація приладу
  
  //ЗОП(КОФ)
  unsigned int setpoint_zop[NUMBER_GROUP_USTAVOK];          //уставка ЗОП(КОФ)
  int timeout_zop[NUMBER_GROUP_USTAVOK];                    //Витримка  ЗОП(КОФ)
  unsigned int control_zop;                                 //Поле для управління ЗОП(КОФ)

  //Umin
  unsigned int setpoint_Umin1[NUMBER_GROUP_USTAVOK];        //уставка Umin1
  unsigned int setpoint_Umin1_Iblk[NUMBER_GROUP_USTAVOK];   //уставка блокування по струму Umin1
  unsigned int setpoint_Umin2[NUMBER_GROUP_USTAVOK];        //уставка Umin2
  unsigned int setpoint_Umin2_Iblk[NUMBER_GROUP_USTAVOK];   //уставка блокування по струму Umin2
  int timeout_Umin1[NUMBER_GROUP_USTAVOK];                    //Витримка  Umin1
  int timeout_Umin2[NUMBER_GROUP_USTAVOK];                    //Витримка  Umin2
  unsigned int control_Umin;                                 //Поле для управління Umin
  
  //Umax
  unsigned int setpoint_Umax1[NUMBER_GROUP_USTAVOK];        //уставка Umax1
  unsigned int setpoint_Umax2[NUMBER_GROUP_USTAVOK];        //уставка Umax2
  int timeout_Umax1[NUMBER_GROUP_USTAVOK];                  //Витримка  Umax1
  int timeout_Umax2[NUMBER_GROUP_USTAVOK];                  //Витримка  Umax2
  unsigned int control_Umax;                                //Поле для управління Umax

  //АВР
  unsigned int setpoint_avr_tn1_U1_Umin[NUMBER_GROUP_USTAVOK];          //уставка АВР "U1min < U1x"
  unsigned int setpoint_avr_tn1_U2_low_work[NUMBER_GROUP_USTAVOK];      //уставка АВР "U2min > U2x"
  unsigned int setpoint_avr_tn2_U2_Umin[NUMBER_GROUP_USTAVOK];          //уставка АВР "U2min < U2x"
  unsigned int setpoint_avr_tn2_U1_low_work[NUMBER_GROUP_USTAVOK];      //уставка АВР "U1min > U1x"
  unsigned int setpoint_avr_tn1_U1_Umax[NUMBER_GROUP_USTAVOK];          //уставка АВР "U1max > U1x"
  unsigned int setpoint_avr_tn1_U2_high_work[NUMBER_GROUP_USTAVOK];     //уставка АВР "U2max < U2x"
  unsigned int setpoint_avr_tn2_U2_Umax[NUMBER_GROUP_USTAVOK];          //уставка АВР "U2max > U2x"
  unsigned int setpoint_avr_tn2_U1_high_work[NUMBER_GROUP_USTAVOK];     //уставка АВР "U1max < U1x"
  int timeout_avr_blk_k1[NUMBER_GROUP_USTAVOK];               //Витримка "T блок. АВР кан.1"
  int timeout_avr_pusk_k1[NUMBER_GROUP_USTAVOK];              //Витримка "T пуска кан.1"
  int timeout_avr_d_diji_k1[NUMBER_GROUP_USTAVOK];            //Витримка "T разр.деств.кан.1"
  int timeout_avr_vvimk_rez_k1[NUMBER_GROUP_USTAVOK];         //Витримка "T вкл.рез.кан.1"
  int timeout_avr_vvimk_k1[NUMBER_GROUP_USTAVOK];             //Витримка "T вкл.кан.1"
  int timeout_avr_vymk_rob_k1_Umin[NUMBER_GROUP_USTAVOK];     //Витримка "T откл.раб.кан.1 Umin"
  int timeout_avr_vymk_rob_k1_Umax[NUMBER_GROUP_USTAVOK];     //Витримка "T откл.раб.кан.1 Umax"
  int timeout_avr_vymk_k1[NUMBER_GROUP_USTAVOK];              //Витримка "T откл.кан.1"
  int timeout_avr_blk_k2[NUMBER_GROUP_USTAVOK];               //Витримка "T блок. АВР кан.2"
  int timeout_avr_pusk_k2[NUMBER_GROUP_USTAVOK];              //Витримка "T пуска кан.2"
  int timeout_avr_d_diji_k2[NUMBER_GROUP_USTAVOK];            //Витримка "T разр.деств.кан.2"
  int timeout_avr_vvimk_rez_k2[NUMBER_GROUP_USTAVOK];         //Витримка "T вкл.рез.кан.2"
  int timeout_avr_vvimk_k2[NUMBER_GROUP_USTAVOK];             //Витримка "T вкл.кан.2"
  int timeout_avr_vymk_rob_k2_Umin[NUMBER_GROUP_USTAVOK];     //Витримка "T откл.раб.кан.2 Umin"
  int timeout_avr_vymk_rob_k2_Umax[NUMBER_GROUP_USTAVOK];     //Витримка "T откл.раб.кан.2 Umax"
  int timeout_avr_vymk_k2[NUMBER_GROUP_USTAVOK];              //Витримка "T откл.кан.2"

  unsigned int control_avr;                                   //Поле для управління АВР
  
  //Перевірка фазування
  unsigned int setpoint_ctrl_phase_U[NUMBER_GROUP_USTAVOK];   //Уставка UФАЗ
  unsigned int setpoint_ctrl_phase_phi[NUMBER_GROUP_USTAVOK]; //Уставка PHIФАЗ
  unsigned int setpoint_ctrl_phase_f[NUMBER_GROUP_USTAVOK];   //Уставка FФАЗ
  int timeout_ctrl_phase_U[NUMBER_GROUP_USTAVOK];             //Витримка UФАЗ
  int timeout_ctrl_phase_U_d[NUMBER_GROUP_USTAVOK];           //Витримка UФАЗ подовження
  int timeout_ctrl_phase_phi[NUMBER_GROUP_USTAVOK];           //Витримка PHIФАЗ
  int timeout_ctrl_phase_phi_d[NUMBER_GROUP_USTAVOK];         //Витримка PHIФАЗ подовження
  int timeout_ctrl_phase_f[NUMBER_GROUP_USTAVOK];             //Витримка FФАЗ
  int timeout_ctrl_phase_f_d[NUMBER_GROUP_USTAVOK];           //Витримка FФАЗ подовження
  unsigned int control_ctrl_phase;
  

  unsigned int TCurrent;                //Коефіцієнт трансформації для трансформатора струмів
  unsigned int TVoltage;                //Коефіцієнт трансформації для трансформатора напруг
  unsigned int control_transformator;   //налаштування для меню "Трансформатор"

  unsigned int password1;                                       //Пароль для редагування з меню
  unsigned int password2;                                       //Пароль для очистки лічилчників енегії і ресурсу вимикача
  unsigned int timeout_deactivation_password_interface_USB;     //Час деактивації паролю для редагування з інтерфейсу USB
  unsigned int password_interface_USB;                          //Пароль для редагування з інтерфейсу USB
  unsigned int timeout_deactivation_password_interface_RS485;   //Час деактивації паролю для редагування з інтерфейсу RS485
  unsigned int password_interface_RS485;                        //Пароль для редагування з інтерфейсу RS485
  
  unsigned int timeout_idle_new_settings;
  
  //Вимикач
  unsigned int setpoint_Inom;                   //Номінальний струм вимикача
  unsigned int setpoint_r_kom_st_Inom;          //Ресурс комунікаційної стійкості при номінальному струмі вимикача
  unsigned int setpoint_Inom_vymk;              //Номінальний струм вимкнення вимикача
  unsigned int setpoint_r_kom_st_Inom_vymk;     //Ресурс комунікаційної стійкості при номінальному струмі вимкнення вимикача
  unsigned int setpoint_pochatkovyj_resurs;     //Початковий ресурс вимикача
  unsigned int setpoint_krytychnyj_resurs;      //Критичний ресурс вимикача
  unsigned int setpoint_pochatkova_k_vymk;      //Початкова кількість вимкнень
  int timeout_swch_on;                          //Витримка T вкл.
  int timeout_swch_off;                         //Витримка T відкл.
  int timeout_swch_udl_blk_on;                  //Витримка T "удлинение сигнала блокировки включения"
  int timeout_pryvoda_VV;                       //Витримка T "Привіода ВВ"
  unsigned int control_switch;                  //Поле для управління вимикачем
  
  //Аналоговий реєстратор
  unsigned int prefault_number_periods; //Час доаварійного масиву (кількість періодів промислової частоти)
  unsigned int postfault_number_periods;//Час післяарійного масиву (кількість періодів промислової частоти)

  //Комунікація
  unsigned int name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];//І'мя ячейки
  unsigned short int user_register[(M_ADDRESS_LAST_USER_REGISTER_DATA - M_ADDRESS_FIRST_USER_REGISTER_DATA) + 1]; //Регістри користувача
  unsigned int volatile address;                      //Адреса
  int speed_RS485;                                    //швидкість обміну
                                                        // 0 - 9600
                                                        // 1 - 14400
                                                        // 2 - 19200
                                                        // 3 - 28800
                                                        // 4 - 38400
                                                        // 5 - 57600
                                                        // 6 - 115200
  int pare_bit_RS485;                                 //паритет
                                                        // 0 - NONE
                                                        // 1 - ODD
                                                        // 2 - EVEN
  int number_stop_bit_RS485;                          //кількість стоп-біт
                                                        // 0 - 1 stop-bit
                                                        // 1 - 2 stop-bits
  unsigned int time_out_1_RS485;                        //time-out наступного символу = X/10 символу
  
  int language;                                         //мова меню  0= змінна мов не підтримується; 1=RU; 2=UA; 3=EN; 4=KZ; 5=др.
  
  unsigned int control_extra_settings_1;                //Поле для додаткових налаштувань

  
  unsigned char time_setpoints[7+1];                     //Час останніх змін уставок-витримок-управління
                                                         //Останній байт масиву сигналізує мітку звідки зміни були проведені
                                                            //0 - мінімальні параметри
                                                            //1 - клавіатура
                                                            //2 - USB
                                                            //3 - RS-485
  
  unsigned char time_ranguvannja[7+1];                    //Час останніх змін ранжування
                                                            //0 - мінімальні параметри
                                                            //1 - клавіатура
                                                            //2 - USB
                                                            //3 - RS-485
} __SETTINGS;

typedef struct
{
  //Стан обміну
  int state_execution;        //-1 - драйвер готовий до початку нових транзакцій
                              // 0 - відбувається обмін
                              // 1 - обмін завершений без помилок
                              // 2 - обмін завершений з помилками
  
  
  //Ідентифікатор пристрою, з яким ведетьсяобмін
  int device_id;              //-1 - пристрій не визначений
                              // EEPROM_ADDRESS - EEPROM
                              // RTC_ADDRESS - RTC
  //Код операції
  int action;                 //-1 - не визначений
                              // 0 - запис адреси для подальшого зчитування
                              // 1 - зчитування
                              // 2 - запис адреси і запис даних
  

  //Внутрішня адреса по якій буде іти звертання
  unsigned int internal_address;

  //Кількість байт для читання/запису (корисних)
  unsigned int number_bytes;
  
  //Вказівник на буфер (корисний)
  uint8_t volatile* point_buffer;

} __DRIVER_I2C;

typedef struct
{
  unsigned int next_address;
  unsigned int saving_execution;
  unsigned int number_records;
} __INFO_REJESTRATOR;

typedef struct
{
  unsigned int state_execution;     //стан виконуваної заразоперації
  
  unsigned int code_operation;      //Код виконуваної операції

} __DRIVER_SPI_DF;

typedef struct
{
  unsigned char label_start_record;
  unsigned char time[7]; 
  unsigned int TCurrent;
  unsigned int TVoltage;
  unsigned char name_of_cell[MAX_CHAR_IN_NAME_OF_CELL];
} __HEADER_AR;

typedef enum __STATE_READING_ADCs {
STATE_READING_ADCs_NONE = 0,
STATE_READING_WRITE,
STATE_READING_WRITE_READ,
STATE_READING_READ
} STATE_READING_ADCs;

typedef enum _full_ort_index __full_ort_index;

typedef struct
{
  uint32_t tick;
  unsigned int value;

} EXTENDED_OUTPUT_DATA;

typedef struct
{
  uint32_t tick;
  int value;
} EXTENDED_SAMPLE;

typedef struct
{
  unsigned int VAL_1_time_p;
  int VAL_1_data_p[NUMBER_ANALOG_CANALES_VAL_1];

  unsigned int VAL_1_time_c;
  int VAL_1_data_c[NUMBER_ANALOG_CANALES_VAL_1];

  unsigned int VAL_2_time_p;
  int VAL_2_data_p[NUMBER_ANALOG_CANALES_VAL_2];

  unsigned int VAL_2_time_c;
  int VAL_2_data_c[NUMBER_ANALOG_CANALES_VAL_2];
} ROZSHYRENA_VYBORKA;

typedef struct
{
  unsigned int time_stemp;
  unsigned int VAL_1_fix;
  unsigned int VAL_2_fix;
  int data [NUMBER_ANALOG_CANALES];
  unsigned int active_functions[N_BIG];
  int state_ar_record;
  
} DATA_FOR_OSCYLOGRAPH;


typedef struct 
{
  unsigned int x1;
  int y1;

  unsigned int x2;
  int y2;

} VYBORKA_XY;

typedef struct
{
  unsigned int Ua1_x1;
  int Ua1_y1;
  unsigned int Ua1_x2;
  int Ua1_y2;

  unsigned int Ub1_x1;
  int Ub1_y1;
  unsigned int Ub1_x2;
  int Ub1_y2;

  unsigned int Uc1_x1;
  int Uc1_y1;  
  unsigned int Uc1_x2;
  int Uc1_y2;  

  unsigned int Ua2_x1;
  int Ua2_y1;
  unsigned int Ua2_x2;
  int Ua2_y2;

  unsigned int Ub2_x1;
  int Ub2_y1;
  unsigned int Ub2_x2;
  int Ub2_y2;

  unsigned int Uc2_x1;
  int Uc2_y1;  
  unsigned int Uc2_x2;
  int Uc2_y2;  
} POPEREDNJY_PERECHID;

//typedef struct
//{
//  unsigned int present;
//  int start_index;
//  int stop_index;
//  int number_per_index;
//  int real_number;
//} EL_FILTER_STRUCT;

#endif
