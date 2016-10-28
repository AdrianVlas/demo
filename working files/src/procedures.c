#include "header.h"

/*****************************************************/
//Функція обновлення змінних при зміні конфігурації
/*****************************************************/
unsigned int action_after_changing_of_configuration(unsigned int new_configuration, __SETTINGS *target_label)
{
  unsigned int error_window = 0;
  
  /************************/
  //Спершу перевіряємо чи не знаходимося зараз ми у такому вікні, яке забороняє змінювати текучу конфігурацію
  /************************/
  //Перевірка МТЗ
  if ((new_configuration & ( 1<< MTZ_BIT_CONFIGURATION )) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_MTZ)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_MTZ) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_MTZ) 
       )  
       ||  
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_MTZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_MTZ_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_MTZ_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_MTZ_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_MTZ)
      )
      error_window |= (1 << MTZ_BIT_CONFIGURATION );
  }
  //Перевірка ЗДЗ
  if ((new_configuration & (1<<ZDZ_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZDZ) ||
       (current_ekran.current_level == EKRAN_CONTROL_ZDZ        )
      )
      error_window |= (1 << ZDZ_BIT_CONFIGURATION );
  }
  //Перевірка АПВ
  if ((new_configuration & (1<<APV_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_APV)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_TIMEOUT_GROUP1_APV) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_TIMEOUT_GROUP4_APV) 
       )  
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_APV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_APV_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_APV        )
      )
      error_window |= (1 << APV_BIT_CONFIGURATION );
  }
  //Перевірка УРОВ
  if ((new_configuration & (1<<UROV_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UROV)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UROV) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UROV) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UROV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UROV_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UROV_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UROV_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UROV        )
      )
      error_window |= (1 << UROV_BIT_CONFIGURATION );
  }
  //Перевірка ЗОП(КОФ)
  if ((new_configuration & (1<<ZOP_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_ZOP)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_ZOP) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_ZOP) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_ZOP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_ZOP_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_ZOP_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_ZOP_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_ZOP        )
      )
      error_window |= (1 << ZOP_BIT_CONFIGURATION );
  }
  //Перевірка Umin
  if ((new_configuration & (1<<UMIN_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMIN)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMIN) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMIN) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UMIN_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UMIN_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UMIN_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UMIN_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UMIN        )
      )
      error_window |= (1 << UMIN_BIT_CONFIGURATION );
  }
  //Перевірка Umax
  if ((new_configuration & (1<<UMAX_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_UMAX)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_UMAX) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_UMAX) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_UMAX_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_UMAX_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_UMAX_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_UMAX_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_UMAX        )
      )
      error_window |= (1 << UMAX_BIT_CONFIGURATION );
  }
  //Перевірка АВР
  if ((new_configuration & (1<<AVR_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_AVR)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_AVR) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_AVR) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_AVR_UMIN_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_AVR_UMAX_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_AVR_GENERAL_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_AVR_UMAX_GROUP4   )
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_AVR        )
      )
      error_window |= (1 << AVR_BIT_CONFIGURATION );
  }
  //Перевірка "Перевірка фазування"
  if ((new_configuration & (1<<CTRL_PHASE_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level == EKRAN_CHOOSE_SETTINGS_CTRL_PHASE)
       || 
       (
        (current_ekran.current_level >= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP1_CTRL_PHASE) &&
        (current_ekran.current_level <= EKRAN_CHOOSE_SETPOINT_TIMEOUT_GROUP4_CTRL_PHASE) 
       )  
       ||
       (
        (current_ekran.current_level >= EKRAN_SETPOINT_CTRL_PHASE_GROUP1) &&
        (current_ekran.current_level <= EKRAN_SETPOINT_CTRL_PHASE_GROUP4)
       )
       ||  
       (
        (current_ekran.current_level >= EKRAN_TIMEOUT_CTRL_PHASE_GROUP1) &&
        (current_ekran.current_level <= EKRAN_TIMEOUT_CTRL_PHASE_GROUP4)
       )
       ||  
       (current_ekran.current_level == EKRAN_CONTROL_CTRL_PHASE)
      )
      error_window |= (1 << CTRL_PHASE_BIT_CONFIGURATION );
  }
  //Перевірка "Розширеної логіки"
  if ((new_configuration & (1<<EL_BIT_CONFIGURATION)) == 0)
  {
    if(
       (current_ekran.current_level >= EKRAN_EXTENDED_LIGIC) &&
       (current_ekran.current_level <= EKRAN_RANGUVANNJA_D_NOT16)  
      )
      error_window |= (1 << EL_BIT_CONFIGURATION );
  }
  /************************/
  
  if (error_window == 0)
  {
    //Вводимо нову конфігурацю у цільову структуру
    target_label->configuration = new_configuration;
    
    unsigned int maska[N_SMALL] = {0, 0}, maska_1[N_BIG] = {0, 0, 0, 0, 0, 0, 0}, maska_2 = 0;
  
    //Перевіряємо, чи МТЗ зараз знято з конфігурації
    if ((target_label->configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені МТЗ
      target_label->control_mtz &= (unsigned int)(~(CTR_MTZ_1 | CTR_MTZ_2 | CTR_MTZ_3 | CTR_MTZ_4));
   
      //Виводим ступені МТЗ з АПВ
      target_label->control_apv &= (unsigned int)(~(CTR_APV_STARTED_FROM_MTZ1 | CTR_APV_STARTED_FROM_MTZ2 | CTR_APV_STARTED_FROM_MTZ3  | CTR_APV_STARTED_FROM_MTZ4));

      //Виводим ступені МТЗ з ЗДЗ
      target_label->control_zdz &= (unsigned int)(~(CTR_ZDZ_STARTED_FROM_MTZ1 | CTR_ZDZ_STARTED_FROM_MTZ2 | CTR_ZDZ_STARTED_FROM_MTZ3  | CTR_ZDZ_STARTED_FROM_MTZ4));
      
      //Виводим ступені МТЗ з УРОВ
      target_label->control_urov &= (unsigned int)(~(CTR_UROV_STARTED_FROM_MTZ1 | CTR_UROV_STARTED_FROM_MTZ2 | CTR_UROV_STARTED_FROM_MTZ3 | CTR_UROV_STARTED_FROM_MTZ4));
      
      //Формуємо маски функцій МТЗ
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + 
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_MTZ_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  i
                 )
                );
      
      maska_2 = 0;
      for (int i = 0; i < NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за МТЗ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за МТЗ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи ЗДЗ зараз знято з конфігурації
    if ((target_label->configuration & (1<<ZDZ_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ЗДЗ
      target_label->control_zdz &= (unsigned int)(~CTR_ZDZ_STATE);

      //Виводим захист ЗДЗ з УРОВ
      target_label->control_urov &= (unsigned int)(~CTR_UROV_STARTED_FROM_ZDZ);
      
      //Формуємо маки функцій ЗДЗ
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ZDZ_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  i
                 )
                );

      maska_2 = 0;
      for (int i = 0; i < NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗДЗ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за ЗДЗ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів, які відповідають за ЗДЗ
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи АПВ зараз знято з конфігурації
    if ((target_label->configuration & (1<<APV_BIT_CONFIGURATION)) == 0)
    {
      //Виводим АПВ
      target_label->control_apv &= (unsigned int)(~(CTR_APV_STAGE_1 | CTR_APV_STAGE_2 | CTR_APV_STAGE_3 | CTR_APV_STAGE_4));
   
      //Формуємо маки функцій АПВ
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_APV_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT     +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT     +
                  i
                 )
                );

      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_APV_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG     +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG     +
                  i
                 )
                );
      
      maska_2 = 0;
      for (int i = 0; i < NUMBER_APV_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON     + 
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за АПВ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за АПВ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи УРОВ зараз знято з конфігурації
    if ((target_label->configuration & (1<<UROV_BIT_CONFIGURATION)) == 0)
    {
      //Виводим УРОВ
      target_label->control_urov &= (unsigned int)(~CTR_UROV_STATE);
   
        //Формуємо маки функцій УРОВ
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_UROV_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UROV_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  i
                 )
                );

      maska_2 = 0;
      for (int i = 0; i < NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за УРОВ
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за УРОВ
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи ЗОП(КОФ) зараз знято з конфігурації
    if ((target_label->configuration & (1<<ZOP_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ЗОП(КОФ)
      target_label->control_zop &= (unsigned int)(~CTR_ZOP_STATE);
   
      //Виводим захисти ЗОП(КОФ) з УРОВ
      target_label->control_urov &= (unsigned int)(~CTR_UROV_STARTED_FROM_ZOP1);

      //Формуємо маки функцій ЗОП(КОФ)
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_ZOP_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  i
                 )
                );

      maska_2 = 0;
      for (int i = 0; i < NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за ЗОП(КОФ)
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за ЗОП(КОФ)
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи Umin зараз знято з конфігурації
    if ((target_label->configuration & (1<<UMIN_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені Umin
      target_label->control_Umin &= (unsigned int)(~(CTR_UMIN1 | CTR_UMIN2));
   
      //Виводим ступені Umin з УРОВ
      target_label->control_urov &= (unsigned int)(~(CTR_UROV_STARTED_FROM_UMIN1 | CTR_UROV_STARTED_FROM_UMIN2));
      
      //Формуємо маски функцій Umin
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UMIN_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG       +
                  i
                 )
                );

      maska_2 = 0;
      for (int i = 0; i < NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за Umin
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за Umin
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи Umax зараз знято з конфігурації
    if ((target_label->configuration & (1<<UMAX_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені Umax
      target_label->control_Umax &= (unsigned int)(~(CTR_UMAX1 | CTR_UMAX2));

      //Виводим ступені Umax з УРОВ
      target_label->control_urov &= (unsigned int)(~(CTR_UROV_STARTED_FROM_UMAX1 | CTR_UROV_STARTED_FROM_UMAX2));
   
      //Формуємо маски функцій Umax
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT       +
                  i
                 )
                );
     
      for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
      for (int i = 0; i < NUMBER_UMAX_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  i
                 )
                );


      maska_2 = 0;
      for (int i = 0; i < NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів, які відповідають за Umax
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів, які відповідають за Umax
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи АВР зараз знято з конфігурації
    if ((target_label->configuration & (1<<AVR_BIT_CONFIGURATION)) == 0)
    {
      //Виводим ступені АВР
      target_label->control_avr &= (unsigned int)(~CTR_AVR);
   
      //Формуємо маски функцій АВР
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_AVR_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT       +
                  i
                 )
                );
     
      maska_1[0] = 0;
      maska_1[1] = 0;
      maska_1[2] = 0;
      maska_1[3] = 0;
      maska_1[4] = 0;
      maska_1[5] = 0;
      for (int i = 0; i < NUMBER_AVR_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG       +
                  i
                 )
                );


      maska_2 = 0;
      for (int i = 0; i < NUMBER_AVR_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //Перевіряємо, чи "Перевірка фазування" зараз знята з конфігурації
    if ((target_label->configuration & (1<<CTRL_PHASE_BIT_CONFIGURATION)) == 0)
    {
      //Виводим всі "Перевірки фазування"
      target_label->control_ctrl_phase &= (unsigned int)(~CTR_CTRL_PHASE_U | CTR_CTRL_PHASE_PHI | CTR_CTRL_PHASE_F | CTR_CTRL_PHASE_SEQ_TN1 | CTR_CTRL_PHASE_SEQ_TN2);
   
      //Формуємо маски функцій "Перевірка фазування"
      maska[0] = 0;
      maska[1] = 0;
      for (int i = 0; i < NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_AVR_SIGNAL_FOR_RANG_INPUT       +
                  i
                 )
                );
     
      maska_1[0] = 0;
      maska_1[1] = 0;
      maska_1[2] = 0;
      maska_1[3] = 0;
      maska_1[4] = 0;
      maska_1[5] = 0;
      for (int i = 0; i < NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG       +
                  NUMBER_AVR_SIGNAL_FOR_RANG       +
                  i
                 )
                );


      maska_2 = 0;
      for (int i = 0; i < NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_AVR_SIGNAL_FOR_RANG_BUTTON
                           )
               );

      //Знімаємо всі функції для ранжування входів
      for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
        target_label->ranguvannja_buttons[i] &= ~maska_2;
      //Знімаємо всі функції для ранжування входів
      for (int i = 0; i < NUMBER_INPUTS; i++)
      {
        target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
        target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
      }
      //Знімаємо всі функції для ранжування виходів
      for (int i = 0; i < NUMBER_OUTPUTS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування світоіндикаторів
      for (int i = 0; i < NUMBER_LEDS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
        target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
        target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
        target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
      }
      //Знімаємо всі функції для ранжування оприділювальних функцій
      for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
          target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
        }
      }
      //Знімаємо всі функції для ранжування оприділювальних триґерів
      for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) 
        {
          target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
          target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
          target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
        }
      }
  
      //Знімаємо всі функції для ранжування визначуваних "І"
      for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "Викл.АБО"
      for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
      }
  
      //Знімаємо всі функції для ранжування визначуваних "НЕ"
      for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
      {
        for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
      }
    }

    //"Розширена логіка"
    maska[0] = 0;
    maska[1] = 0;
    unsigned int array_full[N_BIG] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
    unsigned int *point_to_mask_array;
    for (unsigned int i = 0; i < N_BIG; i++ ) maska_1[i] = 0;
    maska_2 = 0;
    if ((target_label->configuration & (1<<EL_BIT_CONFIGURATION)) == 0)
    {
      //Видаляємо всі сигнали розгиреної логіки
      
      //Формуємо маски функцій Розширеної логіки
      for (int i = 0; i < NUMBER_EL_SIGNAL_FOR_RANG_INPUT; i++)
        _SET_BIT(
                 maska, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT       +
                  NUMBER_AVR_SIGNAL_FOR_RANG_INPUT        +
                  NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG_INPUT +
                  i
                 )
                );
     
      for (int i = 0; i < NUMBER_EL_SIGNAL_FOR_RANG; i++)
        _SET_BIT(
                 maska_1, 
                 (
                  NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
                  NUMBER_APV_SIGNAL_FOR_RANG        +
                  NUMBER_UROV_SIGNAL_FOR_RANG       +
                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
                  NUMBER_UMAX_SIGNAL_FOR_RANG       +
                  NUMBER_AVR_SIGNAL_FOR_RANG        +
                  NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG +
                  i
                 )
                );


      for (int i = 0; i < NUMBER_EL_SIGNAL_FOR_RANG_BUTTON; i++) maska_2 = (maska_2 <<1) + 0x1;
      maska_2 =(
                maska_2 << (
                            NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
                            NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON       +
                            NUMBER_AVR_SIGNAL_FOR_RANG_BUTTON        +
                            NUMBER_CTRL_PHASE_SIGNAL_FOR_RANG_BUTTON
                           )
               );
      
      point_to_mask_array = array_full;
    }
//    else
//    {
//      //Формуємо маски тільки тих сигналів розширеної логіки, які виведені з конфігурації у кількісному значенні
//      unsigned int array_defined_logic[NUMBER_DEFINED_ELEMENTS][2] =
//      {
//        {target_label->number_defined_df , NUMBER_DEFINED_FUNCTIONS},
//        {target_label->number_defined_dt , NUMBER_DEFINED_TRIGGERS },
//        {target_label->number_defined_and, NUMBER_DEFINED_AND      },
//        {target_label->number_defined_or , NUMBER_DEFINED_OR       },
//        {target_label->number_defined_xor, NUMBER_DEFINED_XOR      },
//        {target_label->number_defined_not, NUMBER_DEFINED_NOT      }
//      };
//      const unsigned int input_signals[NUMBER_DEFINED_ELEMENTS]  = {1, 2, 0, 0, 0, 0};
//      const unsigned int others_signals[NUMBER_DEFINED_ELEMENTS] = {2, 3, 1, 1, 1, 1};
//      const unsigned int button_signals[NUMBER_DEFINED_ELEMENTS] = {1, 2, 0, 0, 0, 0};
//      
//      unsigned int input_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
//                                 NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
//                                 NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT      +
//                                 NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
//                                 NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT         +
//                                 NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT       +
//                                 NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
//                                 NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT +
//                                 NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
//                                 NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
//                                 NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT       +
//                                 NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT       +
//                                 NUMBER_VMP_SIGNAL_FOR_RANG_INPUT;
//      
//      unsigned int others_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
//                                  NUMBER_MTZ_SIGNAL_FOR_RANG        +
//                                  NUMBER_MTZ04_SIGNAL_FOR_RANG      +
//                                  NUMBER_ZDZ_SIGNAL_FOR_RANG        +
//                                  NUMBER_ZZ_SIGNAL_FOR_RANG         +
//                                  NUMBER_TZNP_SIGNAL_FOR_RANG       +
//                                  NUMBER_APV_SIGNAL_FOR_RANG        +
//                                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
//                                  NUMBER_UROV_SIGNAL_FOR_RANG       +
//                                  NUMBER_ZOP_SIGNAL_FOR_RANG        +
//                                  NUMBER_UMIN_SIGNAL_FOR_RANG       +
//                                  NUMBER_UMAX_SIGNAL_FOR_RANG       +
//                                  NUMBER_VMP_SIGNAL_FOR_RANG;
//      
//      unsigned int button_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
//                                  NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
//                                  NUMBER_MTZ04_SIGNAL_FOR_RANG_BUTTON      +
//                                  NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
//                                  NUMBER_ZZ_SIGNAL_FOR_RANG_BUTTON         +
//                                  NUMBER_TZNP_SIGNAL_FOR_RANG_BUTTON       +
//                                  NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
//                                  NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_BUTTON +
//                                  NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
//                                  NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON        +
//                                  NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON       +
//                                  NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON       +
//                                  NUMBER_VMP_SIGNAL_FOR_RANG_BUTTON;
//      
//      for (unsigned int index = 0; index < NUMBER_DEFINED_ELEMENTS; index++)
//      {
//        //Формуємо маски функцій Розширеної логіки
//        for (unsigned int i = 0; i < input_signals[index]*(array_defined_logic[index][1] - array_defined_logic[index][0]); i++)
//          _SET_BIT(maska, (input_shift + input_signals[index]*array_defined_logic[index][0] + i));
//        
//        input_shift += input_signals[index]*array_defined_logic[index][1];
//     
//        for (unsigned int i = 0; i < others_signals[index]*(array_defined_logic[index][1] - array_defined_logic[index][0]); i++)
//          _SET_BIT(maska_1, (others_shift + others_signals[index]*array_defined_logic[index][0] + i));
//        
//        others_shift += others_signals[index]*array_defined_logic[index][1];
//
//        for (unsigned int i = 0; i < button_signals[index]*(array_defined_logic[index][1] - array_defined_logic[index][0]); i++)
//          maska_2 |= 1 << (button_shift + button_signals[index]*array_defined_logic[index][0] + i);
//        
//        button_shift += button_signals[index]*array_defined_logic[index][1];
//      }
//      
//      point_to_mask_array = maska_1;
//    }

    //Знімаємо всі функції для ранжування входів, які відповідають за Розширеної логіки
    for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      target_label->ranguvannja_buttons[i] &= ~maska_2;
    //Знімаємо всі функції для ранжування входів, які відповідають за Розширеної логіки
    for (int i = 0; i < NUMBER_INPUTS; i++)
    {
      target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
      target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
    }
    //Знімаємо всі функції для ранжування виходів
    for (int i = 0; i < NUMBER_OUTPUTS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
    }
    //Знімаємо всі функції для ранжування світоіндикаторів
    for (int i = 0; i < NUMBER_LEDS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
    }
    //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
    for (unsigned int j = 0; j < N_BIG; j++ ) 
    {
      target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
      target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
      target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
      target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
    }
    //Очищємо всі функції для ранжування оприділювальних функцій
    for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~point_to_mask_array[j];
        target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~point_to_mask_array[j];
        target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~point_to_mask_array[j];
      }
    }
    //Очищємо всі функції для ранжування оприділювальних триґерів
    for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) 
      {
        target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~point_to_mask_array[j];
        target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~point_to_mask_array[j];
        target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~point_to_mask_array[j];
        target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~point_to_mask_array[j];
      }
    }
  
    //Очищємо всі функції для ранжування визначуваних "І"
    for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~point_to_mask_array[j];
    }
  
    //Очищємо всі функції для ранжування визначуваних "АБО"
    for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~point_to_mask_array[j];
    }
  
    //Очищємо всі функції для ранжування визначуваних "Викл.АБО"
    for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~point_to_mask_array[j];
    }
  
    //Очищємо всі функції для ранжування визначуваних "НЕ"
    for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
    {
      for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~point_to_mask_array[j];
    }
  }
  
  return error_window;
}
/*****************************************************/

///*****************************************************/
////Функція обновлення змінних при зміні кількості елементів розширеної логіки
///*****************************************************/
//void action_after_changing_number_el(__SETTINGS *target_label, unsigned int element)
//{
//  unsigned int maska[N_SMALL] = {0, 0}, maska_1[N_BIG] = {0, 0, 0, 0, 0, 0, 0, 0}, maska_2 = 0;
//
//  //Формуємо маски тільки тих сигналів розширеної логіки, які виведені з конфігурації у кількісному значенні
//  unsigned int array_defined_logic[NUMBER_DEFINED_ELEMENTS][2] =
//  {
//    {target_label->number_defined_df , NUMBER_DEFINED_FUNCTIONS},
//    {target_label->number_defined_dt , NUMBER_DEFINED_TRIGGERS },
//    {target_label->number_defined_and, NUMBER_DEFINED_AND      },
//    {target_label->number_defined_or , NUMBER_DEFINED_OR       },
//    {target_label->number_defined_xor, NUMBER_DEFINED_XOR      },
//    {target_label->number_defined_not, NUMBER_DEFINED_NOT      }
//  };
//  const unsigned int input_signals[NUMBER_DEFINED_ELEMENTS]  = {1, 2, 0, 0, 0, 0};
//  const unsigned int others_signals[NUMBER_DEFINED_ELEMENTS] = {2, 3, 1, 1, 1, 1};
//  const unsigned int button_signals[NUMBER_DEFINED_ELEMENTS] = {1, 2, 0, 0, 0, 0};
//  
//  unsigned int input_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG_INPUT    + 
//                             NUMBER_MTZ_SIGNAL_FOR_RANG_INPUT        +
//                             NUMBER_MTZ04_SIGNAL_FOR_RANG_INPUT      +
//                             NUMBER_ZDZ_SIGNAL_FOR_RANG_INPUT        +
//                             NUMBER_ZZ_SIGNAL_FOR_RANG_INPUT         +
//                             NUMBER_TZNP_SIGNAL_FOR_RANG_INPUT       +
//                             NUMBER_APV_SIGNAL_FOR_RANG_INPUT        +
//                             NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_INPUT +
//                             NUMBER_UROV_SIGNAL_FOR_RANG_INPUT       +
//                             NUMBER_ZOP_SIGNAL_FOR_RANG_INPUT        +
//                             NUMBER_UMIN_SIGNAL_FOR_RANG_INPUT       +
//                             NUMBER_UMAX_SIGNAL_FOR_RANG_INPUT       +
//                             NUMBER_VMP_SIGNAL_FOR_RANG_INPUT;
//      
//  unsigned int others_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG    + 
//                              NUMBER_MTZ_SIGNAL_FOR_RANG        +
//                              NUMBER_MTZ04_SIGNAL_FOR_RANG      +
//                              NUMBER_ZDZ_SIGNAL_FOR_RANG        +
//                              NUMBER_ZZ_SIGNAL_FOR_RANG         +
//                              NUMBER_TZNP_SIGNAL_FOR_RANG       +
//                              NUMBER_APV_SIGNAL_FOR_RANG        +
//                              NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG +
//                              NUMBER_UROV_SIGNAL_FOR_RANG       +
//                              NUMBER_ZOP_SIGNAL_FOR_RANG        +
//                              NUMBER_UMIN_SIGNAL_FOR_RANG       +
//                              NUMBER_UMAX_SIGNAL_FOR_RANG       +
//                              NUMBER_VMP_SIGNAL_FOR_RANG;
//      
//  unsigned int button_shift = NUMBER_GENERAL_SIGNAL_FOR_RANG_BUTTON    + 
//                              NUMBER_MTZ_SIGNAL_FOR_RANG_BUTTON        +
//                              NUMBER_MTZ04_SIGNAL_FOR_RANG_BUTTON      +
//                              NUMBER_ZDZ_SIGNAL_FOR_RANG_BUTTON        +
//                              NUMBER_ZZ_SIGNAL_FOR_RANG_BUTTON         +
//                              NUMBER_TZNP_SIGNAL_FOR_RANG_BUTTON       +
//                              NUMBER_APV_SIGNAL_FOR_RANG_BUTTON        +
//                              NUMBER_ACHR_CHAPV_SIGNAL_FOR_RANG_BUTTON +
//                              NUMBER_UROV_SIGNAL_FOR_RANG_BUTTON       +
//                              NUMBER_ZOP_SIGNAL_FOR_RANG_BUTTON        +
//                              NUMBER_UMIN_SIGNAL_FOR_RANG_BUTTON       +
//                              NUMBER_UMAX_SIGNAL_FOR_RANG_BUTTON       +
//                              NUMBER_VMP_SIGNAL_FOR_RANG_BUTTON;
//      
//  for (unsigned int index = 0; index < element; index++)
//  {
//    input_shift  += input_signals[index]*array_defined_logic[index][1];
//    others_shift += others_signals[index]*array_defined_logic[index][1];
//    button_shift += button_signals[index]*array_defined_logic[index][1];
//  }
//  
//  //Формуємо маски функцій елемента Розширеної логіки
//  for (unsigned int i = 0; i < input_signals[element]*(array_defined_logic[element][1] - array_defined_logic[element][0]); i++)
//    _SET_BIT(maska, (input_shift + input_signals[element]*array_defined_logic[element][0] + i));
//    
//  for (unsigned int i = 0; i < others_signals[element]*(array_defined_logic[element][1] - array_defined_logic[element][0]); i++)
//    _SET_BIT(maska_1, (others_shift + others_signals[element]*array_defined_logic[element][0] + i));
//    
//  for (unsigned int i = 0; i < button_signals[element]*(array_defined_logic[element][1] - array_defined_logic[element][0]); i++)
//    maska_2 |= 1 << (button_shift + button_signals[element]*array_defined_logic[element][0] + i);
//    
//  //Знімаємо всі функції для ранжування входів, які відповідають за Розширену логіку
//  for (int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
//    target_label->ranguvannja_buttons[i] &= ~maska_2;
//  //Знімаємо всі функції для ранжування входів, які відповідають за Розширену логіку
//  for (int i = 0; i < NUMBER_INPUTS; i++)
//  {
//    target_label->ranguvannja_inputs[N_SMALL*i  ] &= ~maska[0];
//    target_label->ranguvannja_inputs[N_SMALL*i+1] &= ~maska[1];
//  }
//  //Знімаємо всі функції для ранжування виходів
//  for (int i = 0; i < NUMBER_OUTPUTS; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_outputs[N_BIG*i+j] &= ~maska_1[j];
//  }
//  //Знімаємо всі функції для ранжування світоіндикаторів
//  for (int i = 0; i < NUMBER_LEDS; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_leds[N_BIG*i+j] &= ~maska_1[j];
//  }
//  //Знімаємо всі функції для ранжування аналогового, дискретного реєстраторів, блоків ввімкнення і вимкнення
//  for (unsigned int j = 0; j < N_BIG; j++ ) 
//  {
//    target_label->ranguvannja_analog_registrator[j]  &= ~maska_1[j];
//    target_label->ranguvannja_digital_registrator[j] &= ~maska_1[j];
//    target_label->ranguvannja_off_cb[j] &= ~maska_1[j];
//    target_label->ranguvannja_on_cb[j]  &= ~maska_1[j];
//  }
//  //Очищємо всі функції для ранжування оприділювальних функцій
//  for (int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) 
//    {
//      target_label->ranguvannja_df_source_plus[N_BIG*i+j]  &= ~maska_1[j];
//      target_label->ranguvannja_df_source_minus[N_BIG*i+j] &= ~maska_1[j];
//      target_label->ranguvannja_df_source_blk[N_BIG*i+j]   &= ~maska_1[j];
//    }
//  }
//  //Очищємо всі функції для ранжування оприділювальних триґерів
//  for (int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) 
//    {
//      target_label->ranguvannja_set_dt_source_plus[N_BIG*i+j]    &= ~maska_1[j];
//      target_label->ranguvannja_set_dt_source_minus[N_BIG*i+j]   &= ~maska_1[j];
//      target_label->ranguvannja_reset_dt_source_plus[N_BIG*i+j]  &= ~maska_1[j];
//      target_label->ranguvannja_reset_dt_source_minus[N_BIG*i+j] &= ~maska_1[j];
//    }
//  }
//  
//  //Очищємо всі функції для ранжування визначуваних "І"
//  for(unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_and[N_BIG*i+j] &= ~maska_1[j];
//  }
//  
//  //Очищємо всі функції для ранжування визначуваних "АБО"
//  for(unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_or[N_BIG*i+j] &= ~maska_1[j];
//  }
//  
//  //Очищємо всі функції для ранжування визначуваних "Викл.АБО"
//  for(unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_xor[N_BIG*i+j] &= ~maska_1[j];
//  }
//  
//  //Очищємо всі функції для ранжування визначуваних "НЕ"
//  for(unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
//  {
//    for (unsigned int j = 0; j < N_BIG; j++ ) target_label->ranguvannja_d_not[N_BIG*i+j] &= ~maska_1[j];
//  }
//}
///*****************************************************/

/*****************************************************/
//Функція, яка визначає кількість біт затримки, який допускається між байтами у RS-485 згідно з визначеними настройками
/*****************************************************/
void calculate_namber_bit_waiting_for_rs_485(void)
{
  unsigned int number_bits_rs_485_waiting_tmp = 10;
  
  //Підраховуємо кількість біт у байті
  if (current_settings.pare_bit_RS485 > 0) number_bits_rs_485_waiting_tmp++;
  if (current_settings.number_stop_bit_RS485 > 0) number_bits_rs_485_waiting_tmp++;
  
  //Врахування швидкості
  unsigned int multiplicator;
  switch (current_settings.speed_RS485)
  {
  case 0:
    {
      multiplicator = 12; //9600
      break;
    }
  case 1:
    {
      multiplicator = 8;  //14400
      break;
    }
  case 2:
    {
      multiplicator = 6;  //19200
      break;
    }
  case 3:
    {
      multiplicator = 4;  //28800
      break;
    }
  case 4:
    {
      multiplicator = 3;  //38400
      break;
    }
  case 5:
    {
      multiplicator = 2;  //57600
      break;
    }
  default:
    {
      multiplicator = 1;  //115200
      break;
    }
  }
  number_bits_rs_485_waiting_tmp *= (multiplicator*current_settings.time_out_1_RS485);
  number_bits_rs_485_waiting_tmp /= 10;
  
  number_bits_rs_485_waiting = number_bits_rs_485_waiting_tmp;
}
/*****************************************************/

/******************************************************
//Функція, яка фіксує зміни у настройках і запускає запис настройок у EEPROM
-----------------------------------------------------
setting_rang: 0 - запис уставок
              1 - запис ранжування
              2 - ігнорувати цей параметр

source:       0 - мінімальні параметри
              1 - клавіатура
              2 - USB
              3 - RS-485
---------------------------------------------------
******************************************************/
void fix_change_settings(unsigned int setting_rang, unsigned int source)
{
  if (setting_rang < 2)
  {
    unsigned char *point_to_target;
  
    if (setting_rang == 0) point_to_target = (&current_settings)->time_setpoints;
    else point_to_target = (&current_settings)->time_ranguvannja;
    
    //Записуємо час останніх змін
    unsigned char *label_to_time_array;
    if (copying_time == 0) label_to_time_array = time;
    else label_to_time_array = time_copy;
    for (unsigned int i = 0; i < 7; i++) *(point_to_target + i) = *(label_to_time_array + i);
    *(point_to_target + 7) = (unsigned char)(source & 0xff);
  }
  
  //Помічаємо, що таблиця змінилася і її треба буде з системи захистів зкопіювати у таблицю з якою працює система захистів
  changed_settings = CHANGED_ETAP_ENDED;
  if (_CHECK_SET_BIT(active_functions, RANG_SETTINGS_CHANGED) == 0) current_settings_interfaces = current_settings;
  
  //Запускаємо запис у EEPROM
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_SETTINGS_EEPROM_BIT);
}
/*****************************************************/

/*****************************************************/
/*
Активація внесених змін у налаштування через інтерфейси
*/
/*****************************************************/
unsigned int set_new_settings_from_interface(unsigned int source)
{
  unsigned int error = 0;
  
  //Вказівник на системний час
  unsigned char *label_to_time_array;
  if (copying_time == 0) label_to_time_array = time;
  else label_to_time_array = time_copy;
  
  if ((type_of_settings_changed & (1 << DEFAULT_SETTINGS_SET_BIT)) != 0)
  {
    for (unsigned int i = 0; i < 7; i++) current_settings_interfaces.time_setpoints[i] = current_settings_interfaces.time_ranguvannja[i] = *(label_to_time_array + i);
    current_settings_interfaces.time_setpoints[7] = current_settings_interfaces.time_ranguvannja[7] = 0;
  }
  
  if ((type_of_settings_changed & (1 << SETTINGS_DATA_CHANGED_BIT)) != 0)
  {
    for (unsigned int i = 0; i < 7; i++) current_settings_interfaces.time_setpoints[i] = *(label_to_time_array + i);
    current_settings_interfaces.time_setpoints[7] = source;
  }
  
  if ((type_of_settings_changed & (1 << RANGUVANNJA_DATA_CHANGED_BIT)) != 0)
  {
    for (unsigned int i = 0; i < 7; i++) current_settings_interfaces.time_ranguvannja[i] = *(label_to_time_array + i);
    current_settings_interfaces.time_ranguvannja[7] = source;
  }
  
  unsigned int reconfiguration_RS_485 = 0, reconfiguration_RS_485_with_reset_usart = 0;
  if (
      (current_settings.speed_RS485 != current_settings_interfaces.speed_RS485) ||
      (current_settings.pare_bit_RS485 != current_settings_interfaces.pare_bit_RS485) ||
      (current_settings.number_stop_bit_RS485 != current_settings_interfaces.number_stop_bit_RS485) ||
      (current_settings.time_out_1_RS485 != current_settings_interfaces.time_out_1_RS485)
     )
  {
    //Помічаємо, що треба переконфігурувати інтерфейс RS-485
    reconfiguration_RS_485 = 1;
    
    if (
        (current_settings.speed_RS485 != current_settings_interfaces.speed_RS485) ||
        (current_settings.pare_bit_RS485 != current_settings_interfaces.pare_bit_RS485) ||
        (current_settings.number_stop_bit_RS485 != current_settings_interfaces.number_stop_bit_RS485)
       )
    {
      //Помічаємо, що треба переконфігурувати USART для інтерфейсу RS-485
      reconfiguration_RS_485_with_reset_usart = 1;
    }
  }
  
  unsigned int change_timeout_ar = 0;
  if (
      (current_settings.prefault_number_periods != current_settings_interfaces.prefault_number_periods) ||
      (current_settings.postfault_number_periods != current_settings_interfaces.postfault_number_periods)
     ) 
  {
    //Помічаємо, що додатково ще треба буде виконати дії по зміні часових витримок аналогового реєстратора
    change_timeout_ar = 1;
    
    unsigned int semaphore_read_state_ar_record_copy = semaphore_read_state_ar_record;

    /*Встановлюємо симафор - суть якого полягає у тому, що якщо процес запису нової 
    аварії не йде - то на час його установлення новий запис починати не можна, якщо ж вже іде ноий запис,
    то він має продовжуватися і, навпаки, блокувати роботу аналогового реєстратора не можна*/
    semaphore_read_state_ar_record = 1;

    if (
        (state_ar_record == STATE_AR_NO_RECORD      ) ||
        (state_ar_record == STATE_AR_TEMPORARY_BLOCK)
       )   
    {
      /*На даний момент не йде запис текучого аналогового аварійного процесу,
      тому для зміни часових настройок тимчасово встановлюємо стан роботи
      аналогового реєстратора у заблокований режим*/
      state_ar_record = STATE_AR_TEMPORARY_BLOCK; 
      
    }
    else
    {
      //Операція тимчасово недоступна, бо іде робота аналогового реєстратора
      error = ERROR_SLAVE_DEVICE_BUSY;
      semaphore_read_state_ar_record = semaphore_read_state_ar_record_copy;
    }
  }
  
  unsigned int set_password_USB = false;
  if (
      (current_settings.password_interface_USB != current_settings_interfaces.password_interface_USB) &&
      (current_settings_interfaces.password_interface_USB != 0)  
     )   
  {
    set_password_USB = true;
  }
  
  unsigned int set_password_RS485 = false;
  if (
      (current_settings.password_interface_RS485 != current_settings_interfaces.password_interface_RS485) &&
      (current_settings_interfaces.password_interface_RS485 != 0)  
     )   
  {
    set_password_RS485 = true;
  }
  
  if (error == 0)
  {
    if ((type_of_settings_changed & (1 << DEFAULT_SETTINGS_SET_BIT)) != 0)
    {
      //Переводимо меню у висхідний стан
      for(unsigned int i=0; i<MAX_LEVEL_MENU; i++)
      {
        if ((i == EKRAN_LEVEL_PASSWORD) || (i == EKRAN_LEVEL_SET_NEW_PASSWORD1) || (i == EKRAN_LEVEL_SET_NEW_PASSWORD2)) position_in_current_level_menu[i] = INDEX_LINE_NUMBER_1_FOR_LEVEL_PASSWORD;
        else  position_in_current_level_menu[i] = 0;
        previous_level_in_current_level_menu[i] = -1;
      }
  
      //Визначення початкового стану екрану
      current_ekran.current_level = EKRAN_MAIN;
      current_ekran.index_position = position_in_current_level_menu[current_ekran.current_level];
      current_ekran.position_cursor_y = current_ekran.index_position;
      current_ekran.edition = 0;
      current_ekran.cursor_on = 0;
      current_ekran.cursor_blinking_on = 0;  
    }
    
    if (set_password_USB   != false) password_set_USB   = 1;
    if (set_password_RS485 != false) password_set_RS485 = 1;
    
    //Помічаємо, що поля структури зараз будуть змінені
    changed_settings = CHANGED_ETAP_EXECUTION;
              
    //Копіюємо введені зміни у робочу структуру
    current_settings = current_settings_interfaces;
    if (reconfiguration_RS_485 != 0)
    {
      //Підраховуємо нову величину затримки у бітах, яка допускається між байтами у RS-485 згідно з визначеними настройками
      calculate_namber_bit_waiting_for_rs_485();
      //Виставляємо команду про переконфігурування RS-485
      if (reconfiguration_RS_485_with_reset_usart != 0) make_reconfiguration_RS_485 = 0xff;
    }
    if (
        (state_ar_record == STATE_AR_TEMPORARY_BLOCK) ||
        (semaphore_read_state_ar_record != 0)  
       )
    {
      /*
      Ця ситуація може бути, коли встановлюються мінімальні настройки,
      або коли змінюється ширина доаварійного або післяаварійного процесу
      аналогового реєстратора.
      При цьому завжди має бути, що змінна state_ar_record рівна величині
      STATE_AR_TEMPORARY_BLOCK і змінна semaphore_read_state_ar_record
      не рівна нулю. Ящо ці 
      умови не виконуються - то треба перезапустити прилад,
      бо програмне забезпечення себе веде непередбачуваним шляхом.
      */
      if(
         ((change_timeout_ar != 0)) &&
         (state_ar_record == STATE_AR_TEMPORARY_BLOCK) &&
         (semaphore_read_state_ar_record != 0)  
        )
      {
        //Виконуємо дії по зміні часових витримок аналогового реєстратора
        actions_after_changing_tiomouts_ar();

        //Розблоковуємо роботу аналогового реєстратора
        state_ar_record = STATE_AR_NO_RECORD;

        //Знімаємо семафор
        semaphore_read_state_ar_record = 0;
      }
      else
      {
        //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
        total_error_sw_fixed(41);
      }
    }

    fix_change_settings(2, source);

    //Виставляємо признак, що на екрані треба обновити інформацію
    new_state_keyboard |= (1<<BIT_REWRITE);
  }
  
  return error;
}
/*****************************************************/

/******************************************************
Відновлення триґерних функцій
******************************************************/
void restore_trigger_functions(unsigned int *active_functions_point)
{
  active_functions[RANG_DT1_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT1_OUT);
  active_functions[RANG_DT2_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT2_OUT);
  active_functions[RANG_DT3_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT3_OUT);
  active_functions[RANG_DT4_OUT >> 5] |= _CHECK_SET_BIT(active_functions_point, RANG_DT4_OUT);

  unsigned int tmp_state;

  tmp_state = _CHECK_SET_BIT(active_functions_point, RANG_BLOCK_AVR);
  trigger_AVR_0 |= ((tmp_state != 0) << 0);
  active_functions[RANG_BLOCK_AVR >> 5] |= tmp_state;
}
/*****************************************************/

/*****************************************************/
//Функція зміни інформації по діагностиці
/*****************************************************/
void changing_diagnostyka_state(void)
{
  /*****
  Визначаємо новий стан діагностики
  
  Алгоритм:
  - Копіюємо у тимчасові масиви біти, які треба скинути і які треба виставити у 
    діагностиці
  - Після цього копіювання більш пріоритетні ситстеми можуть добавляти (не 
    скидають ніколи!!!) нові біти у глобальні масиви
  - Біти , які ми будуть скидалися/встановлюватися очищаємо з глобальних масивів
    (якщо в процесі цього алгоритму більшпріоритетні системи добавили чи 
    добавлятимуть нові біти, то вони не скинуться, а опрацюються при наступному 
    виклику цієї функції)

  - Спочатку очищаємо біти а потім встановлюємо, бо фіксація події має більший 
    пріоритет за очищення
  *****/
  unsigned int clear_diagnostyka_tmp[3], set_diagnostyka_tmp[3];
  
  clear_diagnostyka_tmp[0] = clear_diagnostyka[0];
  clear_diagnostyka_tmp[1] = clear_diagnostyka[1];
  clear_diagnostyka_tmp[2] = clear_diagnostyka[2];

  set_diagnostyka_tmp[0] = set_diagnostyka[0];
  set_diagnostyka_tmp[1] = set_diagnostyka[1];
  set_diagnostyka_tmp[2] = set_diagnostyka[2];
    
  diagnostyka[0] &= (unsigned int)(~clear_diagnostyka_tmp[0]); 
  diagnostyka[0] |= set_diagnostyka_tmp[0]; 

  diagnostyka[1] &= (unsigned int)(~clear_diagnostyka_tmp[1]); 
  diagnostyka[1] |= set_diagnostyka_tmp[1]; 

  diagnostyka[2] &= (unsigned int)(~clear_diagnostyka_tmp[2]); 
  diagnostyka[2] |= set_diagnostyka_tmp[2]; 
  
  diagnostyka[2] &= USED_BITS_IN_LAST_INDEX; 

  clear_diagnostyka[0] &= (unsigned int)(~clear_diagnostyka_tmp[0]);
  clear_diagnostyka[1] &= (unsigned int)(~clear_diagnostyka_tmp[1]);
  clear_diagnostyka[2] &= (unsigned int)(~clear_diagnostyka_tmp[2]);
  
  set_diagnostyka[0] &= (unsigned int)(~set_diagnostyka_tmp[0]);
  set_diagnostyka[1] &= (unsigned int)(~set_diagnostyka_tmp[1]);
  set_diagnostyka[2] &= (unsigned int)(~set_diagnostyka_tmp[2]);
  /*****/
  
  //Визначаємо, чи відбулися зміни
  unsigned int value_changes[3], diagnostyka_now[3];
  /*
  Робимо копію тепершньої діагностики, бо ця функція працює на найнижчому пріоритеті,
  тому під час роботи може появитися нові значення, які ми не врахували у цій функції
  */
  diagnostyka_now[0] = diagnostyka[0];
  diagnostyka_now[1] = diagnostyka[1];
  diagnostyka_now[2] = diagnostyka[2];
  value_changes[0] = diagnostyka_before[0] ^ diagnostyka_now[0];
  value_changes[1] = diagnostyka_before[1] ^ diagnostyka_now[1];
  value_changes[2] = diagnostyka_before[2] ^ diagnostyka_now[2];
  
  /*
  У реєстраторі програмних подій має реєструватися тільки перехід з пасивного стану у активний
  таких подій як " Старт устр.    " і " Рестарт устр.  "
  тому перехід з активного у пачсивний ми ігноруємо і крім того затираємо біти, які його "засигналізували"
  */
  
  /*****/
  //Подія " Старт устр.    "
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_START_SYSTEM_BIT) != 0)
  {
    //Зафіксовано що подія " Старт устр.    " змінила свій стан
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_START_SYSTEM_BIT) == 0)
    {
      /*
      Новий стан події " Старт устр.    " є неактивний стан
      Тому робимо так, щоб ця подія не попала у реєстратор програмних подій таким операціями
      - знімаємо встановлений біт про зміну стану діагностики
      - знімаємо повідомлення, що у попередньому стані діагностики ця подія була активною
      - у текучому стані діагностики нічого міняти не треба, бо цей сигнал є неактивним
      */
      _CLEAR_BIT(value_changes, EVENT_START_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_START_SYSTEM_BIT);
    }
  }
  /*****/
  
  /*****/
  //Подія " Рестарт устр.  "
  /*****/
  if (_CHECK_SET_BIT(value_changes, EVENT_RESTART_SYSTEM_BIT) != 0)
  {
    //Зафіксовано що подія " Рестарт устр.  " змінила свій стан
    if (_CHECK_SET_BIT(diagnostyka_now, EVENT_RESTART_SYSTEM_BIT) == 0)
    {
      /*
      Новий стан події " Рестарт устр.  " є неактивний стан
      Тому робимо так, щоб ця подія не попала у реєстратор програмних подій таким операціями
      - знімаємо встановлений біт про зміну стану діагностики
      - знімаємо повідомлення, що у попередньому стані діагностики ця подія була активною
      - у текучому стані діагностики нічого міняти не треба, бо цей сигнал є неактивним
      */
      _CLEAR_BIT(value_changes, EVENT_RESTART_SYSTEM_BIT);
      _CLEAR_BIT(diagnostyka_before, EVENT_RESTART_SYSTEM_BIT);
    }
  }
  /*****/

  //Перевіряємо, чи треба виконувати дії поо зміні діагностики
  if (
      (value_changes[0] != 0) ||
      (value_changes[1] != 0) ||
      (value_changes[2] != 0)
     )
  {
    //Є біти, які треба встановити, або зняти
    
    /*****/
    //При можливості формуємо запис у реєстратор програмних помилок
    /*****/
    if (_CHECK_SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT) == 0)
    {
      /*
      Новий запис робимо тільки тоді, коли попередньо не було зафіксовано 
      переповнення буферу, інакше чикаємо, поки запис у реєстратор програмних подій
      знімить подію про переповнення буферу
      */
      
      //Визначаємо кількість доступних комірок у буфері для реєстратора програмних подій
      int number_empty_cells;
      unsigned int head = head_fifo_buffer_pr_err_records, tail = tail_fifo_buffer_pr_err_records;
      number_empty_cells = (int)(((unsigned int)tail) - ((unsigned int)head));
      while (number_empty_cells <= 0) number_empty_cells += MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
      if (number_empty_cells == 1)
      {
        //Це є остання вільна комірка, то помічаємо, що з цим записом відбувається повне заповнення буферу
        _SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT);
        /*
        відбулася зміна стану діагностики, яку треба врахувати у даному записфі ,тому
        робимо повторноу копію тепершньої діагностики, яка може врахувати і ті зміни які відбулися
        між операціями копіювання стану діагностики на початку цієї функції і
        операцією, як зараз ми будемо виконувати
        */
        diagnostyka_now[0] = diagnostyka[0];
        diagnostyka_now[1] = diagnostyka[1];
        diagnostyka_now[2] = diagnostyka[2];
        
        //Підраховуємо нову кількість змін в діагностиці
        value_changes[0] = diagnostyka_before[0] ^ diagnostyka_now[0];
        value_changes[1] = diagnostyka_before[1] ^ diagnostyka_now[1];
        value_changes[2] = diagnostyka_before[2] ^ diagnostyka_now[2];
      }

      //Вираховуємо кількість змін сигналів
      unsigned int number_changes = 0;
      for(unsigned int i = 0; i < (8*sizeof(value_changes)); i++)
      {
        if (_CHECK_SET_BIT(value_changes, i) != 0) number_changes++;
      }

      if(number_changes != 0)
      {
        /*
        теоретично може бути ситуація, що ми знімали повідомлення про переповнення буферу
        FIFO для записів реєстратора програмних подій  і це була єдина зміна, али при цьому
        є тільки одна вільна комірка, тому ми знову виставили повідомлення про переповнення.
        Тому ми не можемо зняти це повідомлення - тому і не можемо робити нового запису, 
        бо фактично і зміни ніякої нема
        */
        
        //Визначаємо індекс у масиві буферу програмних помилок з якого трбе почати заповнювати дані
        unsigned int index_into_buffer_pr_err = head*SIZE_ONE_RECORD_PR_ERR;
      
        //Помічаємо мітку початку запису
        buffer_pr_err_records[index_into_buffer_pr_err + 0] = LABEL_START_RECORD_PR_ERR;

        //Час фіксації зміни у діагностиці
        if(
           (_CHECK_SET_BIT(diagnostyka, EVENT_START_SYSTEM_BIT   ) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_RESTART_SYSTEM_BIT ) == 0) &&
           (_CHECK_SET_BIT(diagnostyka, EVENT_STOP_SYSTEM_BIT    ) == 0)
          )
        {
          //Вже відбулося перше зчитуванння часу - тобто системний час у нас є
          unsigned char *label_to_time_array;
          if (copying_time == 0) label_to_time_array = time;
          else label_to_time_array = time_copy;
          for(unsigned int i = 0; i < 7; i++) buffer_pr_err_records[index_into_buffer_pr_err + 1 + i] = *(label_to_time_array + i);
        }
        else
        {
          //Ще не відбулося перше зчитуванння часу - тому покищо ці поля записуємо числом 0xff, а потім, коли системний час зчитається, то ми це поле обновимо
          for(unsigned int i = 0; i < 7; i++)  buffer_pr_err_records[index_into_buffer_pr_err + 1 + i] = 0xff;
        }

        buffer_pr_err_records[index_into_buffer_pr_err + 8] = number_changes & 0xff;
      
        //Записуємо попередній стан діагностики
        buffer_pr_err_records[index_into_buffer_pr_err + 9 ] =  diagnostyka_before[0]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 10] = (diagnostyka_before[0] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 11] = (diagnostyka_before[0] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 12] = (diagnostyka_before[0] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 13] =  diagnostyka_before[1]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 14] = (diagnostyka_before[1] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 15] = (diagnostyka_before[1] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 16] = (diagnostyka_before[1] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 17] =  diagnostyka_before[2]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 18] = (diagnostyka_before[2] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 19] = (diagnostyka_before[2] >> 16) & 0xff;

        //Записуємо теперішній стан діагностики
        buffer_pr_err_records[index_into_buffer_pr_err + 20] =  diagnostyka_now[0]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 21] = (diagnostyka_now[0] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 22] = (diagnostyka_now[0] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 23] = (diagnostyka_now[0] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 24] =  diagnostyka_now[1]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 25] = (diagnostyka_now[1] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 26] = (diagnostyka_now[1] >> 16) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 27] = (diagnostyka_now[1] >> 24) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 28] =  diagnostyka_now[2]        & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 29] = (diagnostyka_now[2] >> 8 ) & 0xff;
        buffer_pr_err_records[index_into_buffer_pr_err + 30] = (diagnostyka_now[2] >> 16) & 0xff;
        
        /*
        У реєстраторі програмних подій має реєструватися тільки перехід з пасивного стану у активний
        таких події як " Останов.устр.  " і ця подія не має бути активною у текучій діагностиці,
        бо це, фактично, подія, яка мала місце до останнього включення приладу в роботу.
        Тобто це подія "попереднього сеансу роботи"
        тому перехід з пасивного у фіксуємо ми фіксуємо і зразу затираємо цей біт у теперішній діагностіці,
        яка після цього буде зкомійомана у масив. який відповідає за попередній стан діагностики, так і у масив
        діагностики, який відповідає за текучий стан подій діагностики
        */
        if (_CHECK_SET_BIT(value_changes, EVENT_STOP_SYSTEM_BIT) != 0)
        {
          /*
          Cтан події " Останов.устр.  " змінився (допускається нашим програмним забезпеченням
          зміна з пасивного стану у активний стан)
          Тому робимо такі операції
          - знімаємо встановлений біт у масиві, який зараз буде копіюватися у масив попереднього стану
          - знімаємо встановлений біт у масиві, який відповідає за текучий стан подій діагностики
          */
          _CLEAR_BIT(diagnostyka_now, EVENT_STOP_SYSTEM_BIT);
          _CLEAR_BIT(diagnostyka, EVENT_STOP_SYSTEM_BIT);
        }

        //Фіксуємо попередній стан, який ми вже записали і відносно якого будемо визначати нові зміни
        diagnostyka_before[0] = diagnostyka_now[0];
        diagnostyka_before[1] = diagnostyka_now[1];
        diagnostyka_before[2] = diagnostyka_now[2];

        //Підготовлюємося до запуску запису у реєстратор програмних подій
          unsigned int next_index_into_fifo_buffer = head + 1;
          while(next_index_into_fifo_buffer >= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER) next_index_into_fifo_buffer -= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
          
          //Встановлюємо нове значення голови буфера FIFO 
          head_fifo_buffer_pr_err_records = next_index_into_fifo_buffer;
      }
    }
    /*****/
  }
}
/*****************************************************/

/*****************************************************/
//Підрахунок кількості втановлених одиничок у області пам'яті початок якої визначається вказівником source
/*****************************************************/
unsigned int count_number_set_bit(unsigned int* source, unsigned int total_number)
{
  unsigned int count = 0;
  unsigned int offset, shift;
  
  for (unsigned int i = 0; i < total_number; i++)  
  {
    offset = i >> 5; //Це є, фактично, ділення на 32
    shift  = (i & (32 - 1)); //Це є, фактично, визначення остачі від ділення на 32
    if ( ( (*(source + offset))  & ((unsigned int)(1 << shift)) ) != 0 ) count++;
  }

  return count;
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності настройок
/*****************************************************/
void control_settings(void)
{
  unsigned char crc_settings_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1 = (unsigned char*)(&current_settings); 
  unsigned char  *point_2 = (unsigned char*)(&current_settings_prt); 
  unsigned int i = 0, difference = 0;
  while ((difference == 0) && (i < sizeof(__SETTINGS)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_settings_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }
  
  if ((difference == 0) && (crc_settings == crc_settings_tmp))
  {
    //Контроль достовірності таблиці настройок пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності таблиці настройок не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності юстування
/*****************************************************/
void control_ustuvannja(void)
{
  unsigned char crc_ustuvannja_tmp = 0, temp_value_1, temp_value_2;
  unsigned char  *point_1; 
  unsigned char  *point_2; 
  unsigned int i = 0, difference = 0;

  /*
  У контрольу суму входить індетифікатор типу набору юстуючих коефіцієнтів,
  який займає розмір типу unsigned int
  */
  unsigned int adjustment_id_tmp = ADJUSTMENT_ID;
  point_1 = (unsigned char*)(&adjustment_id_tmp); 
  while (i < sizeof(unsigned int))
  {
    temp_value_1 = *(point_1);
    crc_ustuvannja_tmp += temp_value_1;
    point_1++;
    i++;
  }
  
  point_1 = (unsigned char*)(&ustuvannja); 
  point_2 = (unsigned char*)(&ustuvannja_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(ustuvannja)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  point_1 = (unsigned char*)(&phi_ustuvannja); 
  point_2 = (unsigned char*)(&phi_ustuvannja_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(phi_ustuvannja)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  point_1 = (unsigned char*)(&phi_ustuvannja_sin_cos); 
  point_2 = (unsigned char*)(&phi_ustuvannja_sin_cos_meas);
  i = 0;
  while ((difference == 0) && (i < sizeof(phi_ustuvannja_sin_cos)))
  {
    temp_value_1 = *(point_1);
    temp_value_2 = *(point_2);
    crc_ustuvannja_tmp += temp_value_1;
    if (temp_value_1 != temp_value_2) difference = 0xff;
    point_1++;
    point_2++;
    i++;
  }

  /*
  Не треба контролювати ідентичність серійног номеру, який працює з вимірювально системою,
  чи системою захистів, чи ще якоюсь високопріоритетною і високонадійною від завад системою,
  бо його просто не існує
  */
  point_1 = (unsigned char*)(&serial_number_dev); 
  i = 0;
  while ((difference == 0) && (i < sizeof(serial_number_dev)))
  {
    temp_value_1 = *(point_1);
    crc_ustuvannja_tmp += temp_value_1;
    point_1++;
    i++;
  }
  
  if ((difference == 0) && (crc_ustuvannja == crc_ustuvannja_tmp))
  {
    //Контроль достовірності юстування пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності юстування не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності триґерної інформації
/*****************************************************/
void control_trg_func(void)
{
  unsigned char crc_trg_func_tmp = 0, temp_value_1;
  unsigned char  *point; 
  unsigned int i;
  
  temp_value_1 = misceve_dystancijne_ctrl & 0xff;
  crc_trg_func_tmp += temp_value_1;
  
  point = (unsigned char*)(trigger_active_functions_ctrl);
  i = 0;  
  while (i < sizeof(trigger_active_functions_ctrl))
  {
    temp_value_1 = *(point);
    crc_trg_func_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_trg_func == crc_trg_func_tmp)
  {
    //Контроль достовірності юстування пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності юстування не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT);
  }
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності інформації по реєстраторах
/*****************************************************/
unsigned int control_info_rejestrator(__INFO_REJESTRATOR* info_rejestrator_point, unsigned char crc_info_rejestrator)
{
  unsigned int result;
  unsigned char crc_info_rejestrator_tmp = 0, temp_value_1;
  unsigned char  *point = (unsigned char*)(info_rejestrator_point); 
  unsigned int i = 0;
  while (i < sizeof(__INFO_REJESTRATOR))
  {
    temp_value_1 = *(point);
    crc_info_rejestrator_tmp += temp_value_1;
    point++;
    i++;
  }
  
  if (crc_info_rejestrator == crc_info_rejestrator_tmp)
  {
    //Контроль достовірності реєстратора пройшов успішно
    result = 1;    
  }
  else
  {
    //Контроль достовірності реєстратора не пройшов
    result = 0;    
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
//Контроль достовірності лічильника ресурсу
/*****************************************************/
void control_resurs(void)
{
  unsigned char crc_resurs_tmp = 0, temp_value;
  unsigned char *point;
  
  point = (unsigned char*)(&resurs_vymykacha_ctrl); 
  for (unsigned int i = 0; i < sizeof(resurs_vymykacha_ctrl); i++)
  {
    temp_value = *(point);
    crc_resurs_tmp += temp_value;
    point++;
  }

  point = (unsigned char*)(&resurs_vidkljuchennja_ctrl); 
  for (unsigned int i = 0; i < sizeof(resurs_vidkljuchennja_ctrl); i++)
  {
    temp_value = *(point);
    crc_resurs_tmp += temp_value;
    point++;
  }

  if (crc_resurs_ctrl == crc_resurs_tmp)
  {
    //Контроль достовірності ресурсу лічильника пройшов успішно
    
    //Скидаємо повідомлення у слові діагностики
    _SET_BIT(clear_diagnostyka, ERROR_RESURS_EEPROM_CONTROL_BIT);
  }
  else
  {
    //Контроль достовірності юстування не пройшов

    //Виствляємо повідомлення у слові діагностики
    _SET_BIT(set_diagnostyka, ERROR_RESURS_EEPROM_CONTROL_BIT);
  }
  
}
/*****************************************************/

/*****************************************************/
//Визначення номеру місяця з константи __DATE__
/*****************************************************/
int str_to_int_DATE_Mmm(void)
{
  static const char month_names[12*3] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  
  int i = 0;
  while(i < 12)
  {
    if (
        (month_names[3*i + 0] == __DATE__[0]) &&
        (month_names[3*i + 1] == __DATE__[1]) &&
        (month_names[3*i + 2] == __DATE__[2])
       )
    {
      break;
    }
    
    i++;
  } 
  
  if (i < 12)
  { 
    i += 1; /*щоб нумерація місяців ішла з "1", а не з "0"*/
    return ((i / 10) << 4) + (i % 10);
  }
  else return 0;
}
/*****************************************************/

/*****************************************************/
/*****************************************************/
//
/*****************************************************/
