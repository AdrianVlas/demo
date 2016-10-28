extern volatile unsigned int semaphore_adc_irq;
extern volatile unsigned int adc_DATA_VAL_1_read;
extern volatile unsigned int adc_DATA_VAL_2_read;
extern volatile unsigned int adc_TEST_VAL_read;
extern volatile unsigned int status_adc_read_work;
extern const unsigned int input_adc[NUMBER_INPUTs_ADCs][2];
extern EXTENDED_OUTPUT_DATA output_adc[NUMBER_INPUTs_ADCs];
extern ROZSHYRENA_VYBORKA rozshyrena_vyborka;

extern unsigned int command_word_adc, command_word_adc_work, active_index_command_word_adc;
extern unsigned int state_reading_ADCs;

extern uint32_t step_val_1;
extern uint32_t step_val_2;
extern uint32_t penultimate_tick_VAL_1, previous_tick_VAL_1;
extern uint32_t penultimate_tick_VAL_2, previous_tick_VAL_2;

extern DATA_FOR_OSCYLOGRAPH data_for_oscylograph[MAX_INDEX_DATA_FOR_OSCYLOGRAPH];
extern unsigned int head_data_for_oscylograph;
extern unsigned int tail_data_for_oscylograph, VAL_1_tail_data_for_oscylograph, VAL_2_tail_data_for_oscylograph;

extern VYBORKA_XY perechid_cherez_nul[MAX_INDEX_PhK][2];
extern unsigned int fix_perechid_cherez_nul[MAX_INDEX_PhK];
extern unsigned int fix_perechid_cherez_nul_TN1_TN2, fix_perechid_cherez_nul_TN1_TN2_work;
extern POPEREDNJY_PERECHID poperednij_perechid;

extern volatile unsigned int semaphore_delta_phi;

extern int delta_phi_index_1, delta_phi_index_2;
extern int delta_phi_index_1_work_middle, delta_phi_index_2_work_middle;
extern int delta_phi_index_1_work_low, delta_phi_index_2_work_low;
extern int delta_phi[2], delta_phi_synchro, delta_phi_min, delta_phi_max; 
extern unsigned int bank_delta_phi;
extern unsigned int reset_delta_phi;
extern int speed_delta_phi[2];
extern unsigned int tick_0[2];

extern int frequency_locking_phi;
extern unsigned int frequency_locking_bank;
extern float frequency_locking_cos[2], frequency_locking_sin[2];

extern unsigned int maska_canaliv_fapch_1;
extern float frequency_val_1, frequency_val_1_work;
extern unsigned int tick_period_1, tick_period_1_work;
extern unsigned int tick_c1, tick_c1_work;

extern unsigned int maska_canaliv_fapch_2;
extern float frequency_val_2, frequency_val_2_work;
extern unsigned int tick_period_2, tick_period_2_work;
extern unsigned int tick_c2, tick_c2_work;

extern float frequency_val_1_min, frequency_val_1_max;
extern float frequency_val_2_min, frequency_val_2_max;
extern unsigned int command_restart_monitoring_frequency;

extern const unsigned int index_GND_ADC1[NUMBER_GND_ADC1];
extern unsigned int gnd_adc1_moment_value[NUMBER_GND_ADC1][NUMBER_POINT];
extern unsigned int gnd_adc1_averange_sum[NUMBER_GND_ADC1];
extern unsigned int gnd_adc1_averange[NUMBER_GND_ADC1];
extern unsigned int gnd_adc1;

extern const unsigned int index_GND_ADC2[NUMBER_GND_ADC2];
extern unsigned int gnd_adc2_moment_value[NUMBER_GND_ADC2][NUMBER_POINT];
extern unsigned int gnd_adc2_averange_sum[NUMBER_GND_ADC2];
extern unsigned int gnd_adc2_averange[NUMBER_GND_ADC2];
extern unsigned int gnd_adc2;

extern unsigned int vref_adc1_moment_value[NUMBER_POINT];
extern unsigned int vref_adc1_averange_sum;
extern unsigned int vref_adc1;

extern unsigned int vref_adc2_moment_value[NUMBER_POINT];
extern unsigned int vref_adc2_averange_sum;
extern unsigned int vref_adc2;

extern unsigned int vdd_adc1_moment_value[NUMBER_POINT];
extern unsigned int vdd_adc1_averange_sum;
extern unsigned int vdd_adc1;

extern unsigned int vdd_adc2_moment_value[NUMBER_POINT];
extern unsigned int vdd_adc2_averange_sum;
extern unsigned int vdd_adc2;

extern unsigned int index_array_of_one_value;

extern EXTENDED_SAMPLE ADCs_data_raw[NUMBER_ANALOG_CANALES];
extern int ADCs_data[NUMBER_ANALOG_CANALES];
extern int current_data[NUMBER_ANALOG_CANALES*NUMBER_POINT*NUMBER_PERIOD_TRANSMIT];
extern unsigned int index_array_of_current_data_value;

extern volatile unsigned int changed_ustuvannja; 
extern unsigned char crc_ustuvannja;
extern unsigned int ustuvannja_meas[NUMBER_ANALOG_CANALES], ustuvannja[NUMBER_ANALOG_CANALES], edit_ustuvannja[NUMBER_ANALOG_CANALES];
extern int phi_ustuvannja_meas[NUMBER_ANALOG_CANALES], phi_ustuvannja[NUMBER_ANALOG_CANALES], phi_edit_ustuvannja[NUMBER_ANALOG_CANALES];
extern float phi_ustuvannja_sin_cos_meas[2*NUMBER_ANALOG_CANALES], phi_ustuvannja_sin_cos[2*NUMBER_ANALOG_CANALES], phi_edit_ustuvannja_sin_cos[2*NUMBER_ANALOG_CANALES];

extern const float sin_data_f[NUMBER_POINT];
extern const float cos_data_f[NUMBER_POINT];
extern unsigned int index_sin_cos_array[NUMBER_ADCs];
extern unsigned int index_data_sin_cos_array[NUMBER_ADCs];
extern int data_sin_val_1[NUMBER_POINT*NUMBER_ANALOG_CANALES_VAL_1];
extern int data_cos_val_1[NUMBER_POINT*NUMBER_ANALOG_CANALES_VAL_1];
extern int data_sin_val_2[NUMBER_POINT*NUMBER_ANALOG_CANALES_VAL_2];
extern int data_cos_val_2[NUMBER_POINT*NUMBER_ANALOG_CANALES_VAL_2];
extern unsigned int index_data_sin_and_cos_array;
extern int ortogonal_irq[2*NUMBER_ANALOG_CANALES];
extern int ortogonal[2*NUMBER_ANALOG_CANALES][2];
extern volatile unsigned int semaphore_measure_values_low;
extern unsigned int bank_ortogonal;

extern volatile unsigned int semaphore_measure_values_low1;

extern unsigned int number_inputs_for_fix_one_second;
extern unsigned int number_inputs_for_fix_one_period;
extern volatile unsigned int measurement[NUMBER_ANALOG_CANALES + 8];
extern unsigned int measurement_high[2][NUMBER_ANALOG_CANALES + 8], bank_measurement_high;
extern unsigned int measurement_middle[NUMBER_ANALOG_CANALES + 8]; 
extern unsigned int measurement_low[NUMBER_ANALOG_CANALES + 8]; 

extern const unsigned int index_converter_Ib_p[NUMBER_ANALOG_CANALES];
extern const unsigned int index_converter_I04_p[NUMBER_ANALOG_CANALES];
extern const unsigned int index_converter_Ib_l[NUMBER_ANALOG_CANALES];
extern const unsigned int index_converter_I04_l[NUMBER_ANALOG_CANALES];
extern int ortogonal_calc[2*FULL_ORT_MAX];
extern int ortogonal_calc_low[2*FULL_ORT_MAX];
extern int phi_angle[FULL_ORT_MAX];
extern int base_index_for_angle;
extern int phi_angle_high[2][FULL_ORT_MAX];
extern unsigned int bank_phi_angle_high;


extern int P_plus[2];
extern int P_minus[2];
extern int Q_1q[2];
extern int Q_2q[2];
extern int Q_3q[2];
extern int Q_4q[2];
extern unsigned int lichylnyk_1s_po_20ms;
extern volatile unsigned int bank_for_enegry;
extern int P, Q, cos_phi_x1000;
extern unsigned int S;
extern double energy[MAX_NUMBER_INDEXES_ENERGY];
extern unsigned int clean_energy;
extern unsigned int information_about_clean_energy;

extern const int ea[2];
extern const int eb[2];
extern const int ec[2];
extern unsigned int sequence_TN1;
extern unsigned int sequence_TN2;

extern int sector_1_mtz_tznp[8];
extern int sector_2_mtz_tznp[8];

//Направлена МТЗ
extern unsigned int sector_directional_mtz[4][3];
extern unsigned int Uxy_bilshe_porogu[3];
extern unsigned int Ix_bilshe_porogu[3];
extern unsigned int temp_states_for_mtz;

extern unsigned int i1_bilshe_porogu, i2_bilshe_porogu;

extern unsigned int TN1_TN2;
extern volatile unsigned int state_inputs;
extern volatile unsigned int active_inputs;
extern unsigned int state_outputs;
extern unsigned int state_outputs_raw;
extern volatile unsigned int state_signal_outputs;
extern volatile unsigned int state_leds;
extern volatile unsigned int state_trigger_leds;
extern volatile unsigned int active_functions[N_BIG];
extern unsigned int misceve_dystancijne, misceve_dystancijne_ctrl;
extern unsigned int trigger_active_functions[N_BIG], trigger_active_functions_ctrl[N_BIG];
extern unsigned char crc_trg_func, crc_trg_func_ctrl;
extern volatile unsigned int trigger_functions_USB[N_BIG];
extern volatile unsigned int trigger_functions_RS485[N_BIG];
extern unsigned int copying_active_functions;
extern volatile unsigned int active_functions_copy[N_BIG];
extern volatile unsigned int active_functions_trg[N_BIG];
extern unsigned int pressed_buttons;
extern volatile unsigned int activation_function_from_interface;
extern volatile unsigned int reset_trigger_function_from_interface;
extern unsigned int diagnostyka_before[3];
extern volatile unsigned int diagnostyka[3];
extern volatile unsigned int set_diagnostyka[3];
extern volatile unsigned int clear_diagnostyka[3];

extern int global_timers[MAX_NUMBER_GLOBAL_TIMERS];
extern unsigned int timer_prt_signal_output_mode_2;
extern unsigned int output_timer_prt_signal_output_mode_2;
extern unsigned int etap_execution_df[NUMBER_DEFINED_FUNCTIONS];

extern unsigned int previous_states_APV_0;
extern unsigned int trigger_APV_0;

extern unsigned int previous_states_AVR_0;
extern unsigned int trigger_AVR_0;

extern _Bool previous_states_ready_tu;
extern _Bool trigger_ready_tu;

extern unsigned int start_restart;

extern unsigned char working_ekran[MAX_ROW_LCD][MAX_COL_LCD];
extern unsigned int rewrite_ekran_once_more;

extern volatile unsigned int new_state_keyboard;
extern unsigned int new_state_keyboard_for_db;
extern unsigned char time_set_keyboard[NUMBER_KEY_KEYBOARD];

extern unsigned int time_rewrite;

extern __CURRENT_EKRAN current_ekran;
extern int position_in_current_level_menu[MAX_LEVEL_MENU];
extern int previous_level_in_current_level_menu[MAX_LEVEL_MENU];

extern volatile unsigned int periodical_tasks_TEST_SETTINGS;
extern volatile unsigned int periodical_tasks_TEST_USTUVANNJA;
extern volatile unsigned int periodical_tasks_TEST_TRG_FUNC;
extern volatile unsigned int periodical_tasks_TEST_TRG_FUNC_LOCK;
extern volatile unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_AR;
extern volatile unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK;
extern volatile unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_DR;
extern volatile unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK;
extern volatile unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR;
extern volatile unsigned int periodical_tasks_TEST_INFO_REJESTRATOR_PR_ERR_LOCK;
extern volatile unsigned int periodical_tasks_TEST_RESURS;
extern volatile unsigned int periodical_tasks_TEST_RESURS_LOCK;
extern volatile unsigned int periodical_tasks_TEST_FLASH_MEMORY;
extern volatile unsigned int periodical_tasks_CALCULATION_ANGLE;
extern volatile unsigned int periodical_tasks_CALC_ENERGY_DATA;
extern volatile unsigned int periodical_tasks_CALC_DELTA_PHI;

extern const unsigned char odynyci_vymirjuvannja[MAX_NAMBER_LANGUAGE][NUMBER_ODYNYCI_VYMIRJUVANNJA];

extern unsigned int fixed_power_down_into_RTC; 
extern unsigned char time[7]; 
extern unsigned char time_copy[7]; 
extern unsigned char calibration;
extern unsigned char calibration_copy;
extern unsigned int copying_time;
extern unsigned char time_edit[7]; 
extern unsigned char calibration_edit;
extern unsigned int copy_register8_RTC;
extern int etap_reset_of_bit;
extern int etap_settings_test_frequency;
extern unsigned char temp_register_rtc[2];

extern volatile unsigned int changed_settings; 
extern unsigned char crc_settings;
extern __SETTINGS current_settings_prt, current_settings,  edition_settings,current_settings_interfaces;
extern int * const type_mtz_arr[NUMBER_LEVEL_MTZ];
extern const unsigned int mtz_settings_prt[NUMBER_LEVEL_MTZ][MTZ_SETTINGS_LENGTH];
extern const unsigned int mtz_tmr_const[NUMBER_LEVEL_MTZ][NUMBER_LEVEL_TMR_CONST];
extern const unsigned int mtz_prysk_tmr_const[2][NUMBER_LEVEL_TMR_MTZ_PRYSK_CONST];
extern const unsigned int mtz_const_menu_settings_prt[NUMBER_LEVEL_MTZ][MTZ_CONST_MENU_SETTINGS_LENGTH];
extern const unsigned int mtz_prysk_const_menu_settings_prt[2][MTZ_PR_CONST_MENU_SETTINGS_LENGTH];
extern const unsigned int i_nom_const;
extern const unsigned int u_linear_nom_const;
extern unsigned int * const setpoint_mtz[NUMBER_LEVEL_MTZ];
extern unsigned int * const setpoint_mtz_n_vpered[NUMBER_LEVEL_MTZ];
extern unsigned int * const setpoint_mtz_n_nazad[NUMBER_LEVEL_MTZ];
extern unsigned int * const setpoint_mtz_U[NUMBER_LEVEL_MTZ];
extern unsigned int * const setpoint_mtz_po_napruzi[NUMBER_LEVEL_MTZ];
extern int * const timeout_mtz[NUMBER_LEVEL_MTZ];
extern int * const timeout_mtz_prysk[2];
extern int * const timeout_mtz_n_vpered[NUMBER_LEVEL_MTZ];
extern int * const timeout_mtz_n_vpered_prysk[2];
extern int * const timeout_mtz_n_nazad[NUMBER_LEVEL_MTZ];
extern int * const timeout_mtz_n_nazad_prysk[2];
extern int * const timeout_mtz_po_napruzi[NUMBER_LEVEL_MTZ];
extern int * const timeout_mtz_po_napruzi_prysk[2];
extern int * const timeout_mtz_vvid_prysk[2];
extern _Bool previous_state_mtz_po_incn;
extern _Bool previous_state_mtz_po_uncn;
extern unsigned int p_global_trigger_state_mtz2;

//Визначення періодів у хвилину і більше
extern unsigned int number_seconds;
extern volatile unsigned int number_minutes;

//Змінні для визначеня ресурсу процесора-програми
extern volatile unsigned int restart_resurs_count;
extern unsigned int resurs_temp;
extern unsigned int resurs_global;
extern unsigned int resurs_global_min;
extern unsigned int resurs_global_max;

extern unsigned int restart_timing_watchdog;
extern unsigned int time_1_watchdog_input;
extern unsigned int time_2_watchdog_input;
extern unsigned int time_delta_watchdog_input;
extern unsigned int time_delta_watchdog_input_min;
extern unsigned int time_delta_watchdog_input_max;

extern unsigned int time_1_watchdog_output;
extern unsigned int time_2_watchdog_output;
extern unsigned int time_delta_watchdog_output;
extern unsigned int time_delta_watchdog_output_min;
extern unsigned int time_delta_watchdog_output_max;

extern unsigned char Temporaty_I2C_Buffer[SIZE_PAGE_EEPROM + 2];
extern unsigned int number_busy_state;
extern unsigned int type_error_of_exchanging_via_i2c;
extern unsigned int low_speed_i2c;
extern __DRIVER_I2C driver_i2c;
extern unsigned int control_i2c_taskes[2];
extern unsigned int comparison_writing;
extern unsigned int state_i2c_task;
extern unsigned char read_write_i2c_buffer[SIZE_BUFFER_FOR_EEPROM_EXCHNGE];

//DataFlash
extern unsigned char RxBuffer_SPI_DF[SIZE_PAGE_DATAFLASH_MAX + 10];
extern unsigned char TxBuffer_SPI_DF[SIZE_PAGE_DATAFLASH_MAX + 10];
extern unsigned int number_bytes_transfer_spi_df;
extern unsigned int number_bytes_transfer_spi_df_copy;
extern unsigned char TxBuffer_SPI_DF_copy[SIZE_PAGE_DATAFLASH_MAX + 10];
extern unsigned int code_operation_copy;
extern int number_chip_dataflsh_exchange;
extern __DRIVER_SPI_DF driver_spi_df[NUMBER_DATAFLASH_CHIP];
extern volatile unsigned int error_into_spi_df;
extern unsigned int dataflash_not_busy;
extern volatile unsigned int control_tasks_dataflash;
extern unsigned char buffer_for_manu_read_record[SIZE_BUFFER_FOR_DR_RECORD];
extern unsigned char buffer_for_USB_read_record_ar[SIZE_PAGE_DATAFLASH_2];
extern unsigned char buffer_for_RS485_read_record_ar[SIZE_PAGE_DATAFLASH_2];
extern unsigned char buffer_for_USB_read_record_dr[SIZE_BUFFER_FOR_DR_RECORD];
extern unsigned char buffer_for_RS485_read_record_dr[SIZE_BUFFER_FOR_DR_RECORD];
extern unsigned char buffer_for_USB_read_record_pr_err[SIZE_ONE_RECORD_PR_ERR];
extern unsigned char buffer_for_RS485_read_record_pr_err[SIZE_ONE_RECORD_PR_ERR];

extern unsigned int what_we_are_reading_from_dataflash_1;
extern unsigned int what_we_are_reading_from_dataflash_2;

//Аналоговий реєстратор
extern unsigned char crc_info_rejestrator_ar;
extern volatile __INFO_REJESTRATOR info_rejestrator_ar;
extern unsigned char crc_info_rejestrator_ar_ctrl;
extern __INFO_REJESTRATOR info_rejestrator_ar_ctrl;
extern volatile unsigned int size_one_ar_record;
extern unsigned int number_word_digital_part_ar;
extern volatile unsigned int max_number_records_ar;
extern volatile unsigned int semaphore_read_state_ar_record;
extern unsigned int continue_previous_record_ar;
extern volatile int state_ar_record;
extern short int array_ar[SIZE_BUFFER_FOR_AR];
extern short int word_SRAM1;
extern unsigned int index_array_ar_current;
extern volatile unsigned int index_array_ar_heat;
extern volatile unsigned int index_array_ar_tail;
extern unsigned int prescaler_ar;
extern __HEADER_AR header_ar;
extern unsigned char buffer_for_save_ar_record[SIZE_PAGE_DATAFLASH_2];
extern unsigned int temporary_address_ar;
extern volatile unsigned int count_to_save;
extern volatile unsigned int permit_copy_new_data;
extern unsigned int copied_number_samples, total_number_samples;
extern unsigned int etap_writing_part_page_ar_into_dataflash;
extern unsigned int number_record_of_ar_for_menu;
extern unsigned int number_record_of_ar_for_USB;
extern unsigned int number_record_of_ar_for_RS485;
extern int first_number_time_sample_for_USB;
extern int last_number_time_sample_for_USB;
extern int first_number_time_sample_for_RS485;
extern int last_number_time_sample_for_RS485;

//Дискретний реєстратор
extern unsigned char crc_info_rejestrator_dr;
extern volatile __INFO_REJESTRATOR info_rejestrator_dr;
extern unsigned char crc_info_rejestrator_dr_ctrl;
extern __INFO_REJESTRATOR info_rejestrator_dr_ctrl;
extern unsigned int state_dr_record;
extern unsigned int number_records_dr_waiting_for_saving_operation;
extern unsigned char buffer_for_save_dr_record[SIZE_BUFFER_FOR_DR_RECORD];
extern unsigned char buffer_for_save_dr_record_level_1[SIZE_BUFFER_FOR_DR_RECORD];
extern unsigned char buffer_for_save_dr_record_level_2[SIZE_BUFFER_FOR_DR_RECORD];
extern unsigned int part_writing_dr_into_dataflash;
extern unsigned int number_record_of_dr_for_menu;
extern unsigned int number_record_of_dr_for_USB;
extern unsigned int number_record_of_dr_for_RS485;
extern unsigned int part_reading_dr_from_dataflash_for_menu;
extern unsigned int part_reading_dr_from_dataflash_for_USB;
extern unsigned int part_reading_dr_from_dataflash_for_RS485;
extern unsigned int state_current_monitoring;
extern unsigned int measurements_phase_max_dr[SIZE_ARRAY_FIX_MAX_MEASUREMENTS];
extern unsigned int measurements_U_min_dr[SIZE_ARRAY_FIX_MAX_MEASUREMENTS];    
extern unsigned int measurements_U_max_dr[SIZE_ARRAY_FIX_MAX_MEASUREMENTS];    
extern unsigned int measurements_ZOP_max_dr[SIZE_ARRAY_FIX_MAX_MEASUREMENTS];  
extern unsigned int max_phase_current_dr;
extern unsigned int min_voltage_dr;
extern unsigned int max_voltage_dr;
extern unsigned int number_max_phase_dr;
extern unsigned int number_min_U_dr;
extern unsigned int number_max_U_dr;
extern unsigned int number_max_ZOP_dr;
extern unsigned int type_view_max_values_dr;
extern int index_cell_into_array_for_min_max_measurement_dr;

//Реєстратор програмних помилок
extern unsigned char crc_info_rejestrator_pr_err;
extern volatile __INFO_REJESTRATOR info_rejestrator_pr_err;
extern unsigned char crc_info_rejestrator_pr_err_ctrl;
extern __INFO_REJESTRATOR info_rejestrator_pr_err_ctrl;
extern unsigned char buffer_pr_err_records[SIZE_BUFFER_FOR_PR_ERR];
extern volatile unsigned int head_fifo_buffer_pr_err_records;
extern volatile unsigned int tail_fifo_buffer_pr_err_records;
extern volatile unsigned int temporary_block_writing_records_pr_err_into_DataFlash;
extern unsigned int etap_writing_pr_err_into_dataflash;
extern unsigned int number_recods_writing_into_dataflash_now;
extern unsigned int number_record_of_pr_err_into_menu;
extern unsigned int number_record_of_pr_err_into_USB;
extern unsigned int number_record_of_pr_err_into_RS485;

//Очистка інформації по реєстраторах
extern volatile unsigned int clean_rejestrators;

extern const unsigned char letters[69][2];
extern const unsigned char extra_letters[12][1 + MAX_NAMBER_LANGUAGE];

extern int current_language;

//Лічильник ресурсу
extern unsigned int koef_resurs_changed;
extern float K_resurs_prt, K_resurs;
extern unsigned int resurs_vymykacha, resurs_vymykacha_ctrl;
extern unsigned int resurs_vidkljuchennja, resurs_vidkljuchennja_ctrl;
extern unsigned char crc_resurs, crc_resurs_ctrl;
extern unsigned int restart_counter;

//RS-485
extern unsigned char TxBuffer_RS485[BUFFER_RS485];
extern unsigned char RxBuffer_RS485[BUFFER_RS485];
extern int TxBuffer_RS485_count;
extern int volatile RxBuffer_RS485_count;
extern int RxBuffer_RS485_count_previous;
extern unsigned int time_last_receive_byte;
extern unsigned int max_reaction_time_rs_485;
extern unsigned int make_reconfiguration_RS_485;
extern volatile unsigned int number_bits_rs_485_waiting;
extern unsigned int mark_current_tick_RS_485;
extern unsigned int timeout_idle_RS485;

//USB
extern uint8_t  USART_Rx_Buffer[USART_RX_DATA_SIZE]; 
extern uint32_t USART_Rx_ptr_in;
extern uint32_t USART_Rx_ptr_out;
extern uint32_t USART_Rx_length;

extern uint8_t  USB_Tx_State;

//Для UDP
extern volatile u32 count_out;
extern u32 count_out_previous;
extern uint16_t previous_count_tim4_USB;
extern u8 buffer_out[BUFFER_USB];
extern unsigned char usb_received[BUFFER_USB];
extern unsigned char usb_transmiting[BUFFER_USB];
extern int usb_received_count;
extern int usb_transmiting_count;
extern unsigned char data_usb_transmiting;
extern unsigned int timeout_idle_USB;

//MODBUS-RTU
extern unsigned int registers_address_read;
extern unsigned int registers_address_write;
extern unsigned int data_write_to_memory;
extern unsigned int number_registers_read;
extern unsigned short int registers_values[64] /*@ "variables_RAM1"*/;
extern unsigned int action_is_continued;
extern unsigned int part_transmit_carrent_data;
extern unsigned int command_to_receive_current_data;
extern int current_data_transmit[NUMBER_ANALOG_CANALES*NUMBER_POINT*NUMBER_PERIOD_TRANSMIT] /*@ "variables_RAM1"*/;
extern volatile unsigned int wait_of_receiving_current_data; 
extern unsigned int password_set_USB, password_set_RS485;
extern unsigned int password_changed;
extern unsigned int password_ustuvannja;
extern unsigned int information_about_restart_counter;
extern unsigned int *point_to_edited_rang;
extern unsigned int number_32bit_in_target;
extern unsigned int clear_array_rang[N_BIG];
extern unsigned int set_array_rang[N_BIG];
extern unsigned int restart_timeout_interface;
extern unsigned int timeout_idle_new_settings;
extern unsigned int restart_timeout_idle_new_settings;
extern unsigned int type_of_settings_changed;

extern unsigned int serial_number_dev;
extern unsigned int edit_serial_number_dev;

//Для відображення інформації про причину відключення
extern unsigned int info_vidkluchennja_vymykacha;
extern unsigned char info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MAX_NUMBER][7]; 

extern volatile unsigned int control_word_of_watchdog;
extern unsigned int test_watchdogs;

/**************************************************************
 * Змінна використовується в функції вибору групи уставок:
 * protections.c --> setpoints_selecting()
 **************************************************************/
extern unsigned int gr_ustavok_tmp;

//Змінна глобальної помилки
extern unsigned int total_error;

extern unsigned short const __checksum;
extern unsigned int __checksum_begin;
extern unsigned int __checksum_end;

extern unsigned int __ICFEDIT_region_RAM1_start__;
extern unsigned int __ICFEDIT_region_RAM1_size__;


#ifdef DEBUG_TEST
//extern unsigned int time_1, time_2, max_delay;

//Тимчасові змінні
//extern unsigned int temp_value_for_debbuging_1;
//extern unsigned int temp_value_for_debbuging_2;
//extern unsigned int temp_value_for_debbuging_3;

extern unsigned int temp_value_3I0_1;
extern unsigned int temp_value_3I0_other;
extern unsigned int temp_value_IA;
extern unsigned int temp_value_IC;
extern unsigned int temp_value_UA;
extern unsigned int temp_value_UB;
extern unsigned int temp_value_UC;
extern unsigned int temp_value_3U0;
extern unsigned int temp_value_I2;
extern unsigned int temp_value_I1;

#endif
