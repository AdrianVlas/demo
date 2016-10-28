#include "header.h"

/*****************************************************/
//Пошук нового каналу для оцифровки
/*****************************************************/
inline void find_new_ADC_canal_to_read(unsigned int command_word_adc_diff, unsigned int *point_active_index_command_word_adc)
{
  unsigned int command_word_adc_diff_tmp = command_word_adc_diff;
  
  unsigned int command_word_adc_diff_val_1 = command_word_adc_diff_tmp & maska_canaliv_fapch_1;
  unsigned int command_word_adc_diff_val_2 = command_word_adc_diff_tmp & maska_canaliv_fapch_2;
    
  if (
      ((status_adc_read_work & DATA_VAL_2_READ) != 0) &&
      (command_word_adc_diff_val_2 != 0) &&
      (
       ((status_adc_read_work & DATA_VAL_1_READ) == 0) ||
       (command_word_adc_diff_val_1 == 0) ||
       (command_word_adc_diff_val_1 == maska_canaliv_fapch_1)
      )   
     )  
  {
    command_word_adc_diff_tmp = command_word_adc_diff_val_2;
  }
  else
  {
    if (
        ((status_adc_read_work & DATA_VAL_1_READ) != 0) &&
        (command_word_adc_diff_val_1 != 0) 
       )  
    {
      command_word_adc_diff_tmp = command_word_adc_diff_val_1;
    }
    else
    {
      command_word_adc_diff_tmp &= (unsigned int)(~(maska_canaliv_fapch_1 | maska_canaliv_fapch_2));
    }
  }
  
  while ((command_word_adc_diff_tmp & (1 << (*point_active_index_command_word_adc))) == 0)
  {
    /*
    Ще не знайдено каналу, який треба оцифровувати
    */
    *point_active_index_command_word_adc = (*point_active_index_command_word_adc + 1) % NUMBER_INPUTs_ADCs;
  }
}
/*****************************************************/

/*****************************************************/
//Управління читанням даних з АЦП
/*****************************************************/
void control_reading_ADCs(void)
{
  //Обновляємо робоче командне слово і вибираємо які канали треба оцифровувати
  if (adc_DATA_VAL_1_read != 0)
  {
    adc_DATA_VAL_1_read = false;
    status_adc_read_work |= DATA_VAL_1_READ;
      
    /*
    Канали по яких буде розраховуватися частота мають оцифровуватися як 
    омога ближче до спрацювання таймеру подачі команди оцифровки
    */
    command_word_adc      &= (unsigned int)(~maska_canaliv_fapch_1);
    command_word_adc_work &= (unsigned int)(~maska_canaliv_fapch_1);
        
    command_word_adc |= READ_DATA_VAL_1;
  }

  if (adc_DATA_VAL_2_read != 0)
  {
    adc_DATA_VAL_2_read = false;
    status_adc_read_work |= DATA_VAL_2_READ;
      
    /*
    Канали по яких буде розраховуватися частотамають оцифровуватися як 
    омога ближче до спрацювання таймеру подачі команди оцифровки
    */
    command_word_adc      &= (unsigned int)(~maska_canaliv_fapch_2);
    command_word_adc_work &= (unsigned int)(~maska_canaliv_fapch_2);
        
    command_word_adc |= READ_DATA_VAL_2;
  }

  
  if (adc_TEST_VAL_read != 0) 
  {
    adc_TEST_VAL_read = false;
    status_adc_read_work |= TEST_VAL_READ;
      
    command_word_adc |= READ_TEST_VAL;
  }
  
  unsigned int command_word_adc_diff = command_word_adc ^ command_word_adc_work;
  if (command_word_adc_diff != 0)
  {
    /*
    Є канали, які чекають на оцифровку
    */
    find_new_ADC_canal_to_read(command_word_adc_diff, &active_index_command_word_adc);
      
    /*
    Зараз active_index_command_word_adc вказує у масиві input_adc на канал, 
    який треба оцифрувати
    */
      
    /*
    Визначаємо, який зараз активний АЦП
    */
    unsigned int active_adc_old = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) == 0) ? 1 : 2;

    /*
    Визначаємо, який зараз треба активовувати АЦП і які дані треба передати
    */
    unsigned int active_adc_new = input_adc[active_index_command_word_adc][0];
    unsigned int command_word = input_adc[active_index_command_word_adc][1];
    
    //Визначаємо наступний стан
    if (
        (state_reading_ADCs == STATE_READING_ADCs_NONE) ||
        (state_reading_ADCs == STATE_READING_READ     )
       )
    {
      state_reading_ADCs = STATE_READING_WRITE;
      /*
      Вибирваємо відповідний АЦП
      */
      switch (active_adc_new)
      {
      case 1:
        {
          //АЦП1
          GPIO_SELECT_ADC->BSRRH = GPIO_SELECTPin_ADC;
          break;
        }
        case 2:
        {
          //АЦП2
          GPIO_SELECT_ADC->BSRRL = GPIO_SELECTPin_ADC;
          break;
        }
      default:
        {
          //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
          total_error_sw_fixed(50);
        } 
      }
    }
    else
    {
      if (active_adc_old == active_adc_new)
      {
        state_reading_ADCs = STATE_READING_WRITE_READ;
      }
      else
      {
        state_reading_ADCs = STATE_READING_READ;
        command_word = 0;
      }
    }
    
    if (command_word != 0)
    {
      /*
      Помічаємо, що зараз будемо нове командне слово передавати
      */
      command_word_adc_work |= (1 << active_index_command_word_adc);
    }

    //Передаємо командне слово
    while ((SPI_ADC->SR & SPI_I2S_FLAG_TXE) == RESET);      //Очікуємо, поки SPI стане вільним
    GPIO_SPI_ADC->BSRRH = GPIO_NSSPin_ADC;                  //Виставляємо chip_select
    SPI_ADC->DR = (uint16_t)command_word;                   //Відправляємо командне число
  }
  else 
  {
    if (
        (state_reading_ADCs == STATE_READING_READ     ) ||
        (state_reading_ADCs == STATE_READING_ADCs_NONE)
       )
    {
      //Усі канали вже оцифровані
      state_reading_ADCs = STATE_READING_ADCs_NONE;

      command_word_adc = 0;
      command_word_adc_work = 0;
      active_index_command_word_adc = 0;
    }
    else
    {
      /*
      Треба ще отримати значення з останнього каналуна, який зараз тільки
      щоно оцифровувався
      */
      state_reading_ADCs = STATE_READING_READ;
      while ((SPI_ADC->SR & SPI_I2S_FLAG_TXE) == RESET);      //Очікуємо, поки SPI стане вільним
      GPIO_SPI_ADC->BSRRH = GPIO_NSSPin_ADC;                  //Виставляємо chip_select
      SPI_ADC->DR = 0;                                        //Відправляємо число (але так, щоб нове контрольне слово не записувалося)
    }
  }
}
/*****************************************************/

/*************************************************************************
Опрацьовуємо інтеграільні величини
 *************************************************************************/
void operate_test_ADCs(void)
{
  /*******************************************************
  Вираховування середнього значення контрольних точок
  *******************************************************/
  unsigned int temp;

  //GND для АЦП1
  unsigned int gnd_tmp = 0;
  for (unsigned int i = 0; i < NUMBER_GND_ADC1; i++)
  {
    temp = output_adc[index_GND_ADC1[i]].value;
    gnd_adc1_averange_sum[i] += temp;
    gnd_adc1_averange_sum[i] -= gnd_adc1_moment_value[i][index_array_of_one_value];
    gnd_adc1_moment_value[i][index_array_of_one_value] = temp;
    gnd_tmp += gnd_adc1_averange[i] = gnd_adc1_averange_sum[i] >> VAGA_NUMBER_POINT;
    if (temp > 0xA1) _SET_BIT(set_diagnostyka, ERROR_GND_ADC1_TEST_COARSE_BIT);
    else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC1_TEST_COARSE_BIT);
  }
  gnd_adc1 = gnd_tmp / NUMBER_GND_ADC1;
  
  //GND для АЦП2
  gnd_tmp = 0;
  for (unsigned int i = 0; i < NUMBER_GND_ADC2; i++)
  {
    temp = output_adc[index_GND_ADC2[i]].value;
    gnd_adc2_averange_sum[i] += temp;
    gnd_adc2_averange_sum[i] -= gnd_adc2_moment_value[i][index_array_of_one_value];
    gnd_adc2_moment_value[i][index_array_of_one_value] = temp;
    gnd_tmp += gnd_adc2_averange[i] = gnd_adc2_averange_sum[i] >> VAGA_NUMBER_POINT;
    if (temp > 0xA1) _SET_BIT(set_diagnostyka, ERROR_GND_ADC2_TEST_COARSE_BIT);
    else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC2_TEST_COARSE_BIT);
  }
  gnd_adc2 = gnd_tmp / NUMBER_GND_ADC2;
  
  //VREF для АЦП1
  temp = output_adc[C_VREF_ADC1].value;
  vref_adc1_averange_sum += temp;
  vref_adc1_averange_sum -= vref_adc1_moment_value[index_array_of_one_value];
  vref_adc1_moment_value[index_array_of_one_value] = temp;
  vref_adc1 = vref_adc1_averange_sum >> VAGA_NUMBER_POINT;
  if ((temp < 0x614) || (temp > 0x9EB)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC1_TEST_COARSE_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VREF_ADC1_TEST_COARSE_BIT);
  
  //VREF для АЦП2
  temp = output_adc[C_VREF_ADC2].value;
  vref_adc2_averange_sum += temp;
  vref_adc2_averange_sum -= vref_adc2_moment_value[index_array_of_one_value];
  vref_adc2_moment_value[index_array_of_one_value] = temp;
  vref_adc2 = vref_adc2_averange_sum >> VAGA_NUMBER_POINT;
  if ((temp < 0x614) || (temp > 0x9EB)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC2_TEST_COARSE_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VREF_ADC2_TEST_COARSE_BIT);

  //VDD для АЦП1
  temp = output_adc[C_VDD_ADC1].value; 
  vdd_adc1_averange_sum += temp;
  vdd_adc1_averange_sum -= vdd_adc1_moment_value[index_array_of_one_value];
  vdd_adc1_moment_value[index_array_of_one_value] = temp;
  vdd_adc1 = vdd_adc1_averange_sum >> VAGA_NUMBER_POINT;
  if ((temp <0x6F2) || (temp > 0xD48)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC1_TEST_COARSE_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC1_TEST_COARSE_BIT);

  //VDD для АЦП2
  temp = output_adc[C_VDD_ADC2].value; 
  vdd_adc2_averange_sum += temp;
  vdd_adc2_averange_sum -= vdd_adc2_moment_value[index_array_of_one_value];
  vdd_adc2_moment_value[index_array_of_one_value] = temp;
  vdd_adc2 = vdd_adc2_averange_sum >> VAGA_NUMBER_POINT;
  if ((temp <0x6F2) || (temp > 0xD48)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC2_TEST_COARSE_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC2_TEST_COARSE_BIT);

  //Всі масиви одної величини ми вже опрацювали  
  if((++index_array_of_one_value) == NUMBER_POINT)
    index_array_of_one_value = 0;
  else if (index_array_of_one_value > NUMBER_POINT)
  {
    //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
    total_error_sw_fixed(21);
  }
  /*******************************************************/
}
/*************************************************************************/

/*************************************************************************
Опрацьовуємо дані для перетворення Фур'є
 *************************************************************************/
void Fourier(unsigned int number_val_group)
{
  unsigned int index_first_canal, number_canals;
  int *data_sin, *data_cos;
  
  switch (number_val_group)
  {
  case N_VAL_1:
    {
      number_canals = NUMBER_ANALOG_CANALES_VAL_1;

      index_first_canal = I_Ia;
      number_canals = NUMBER_ANALOG_CANALES_VAL_1;
      
      data_sin = data_sin_val_1;
      data_cos = data_cos_val_1;
      
      break;
    }
  case N_VAL_2:
    {
      number_canals = NUMBER_ANALOG_CANALES_VAL_2;

      index_first_canal = I_Ua2;
      number_canals = NUMBER_ANALOG_CANALES_VAL_2;
      
      data_sin = data_sin_val_2;
      data_cos = data_cos_val_2;
      
      break;
    }
  default:
    {
      //Якщо сюди дійшла програма, значить відбулася недопустива помилка, тому треба зациклити програму, щоб вона пішла на перезагрузку
      total_error_sw_fixed(22);
    }
  }
  
  unsigned int index_data_sin_cos_array_tmp = index_data_sin_cos_array[number_val_group];
  unsigned int max_size_data_sin_cos = NUMBER_POINT*number_canals;
  unsigned int index_sin_cos_array_tmp = index_sin_cos_array[number_val_group];
  
  for (unsigned int i = 0; i < number_canals; i++)
  {
    //Зчитуємо миттєве значення яке треба опрацювати
    int temp_value_1 = ADCs_data[index_first_canal + i];
    int temp_value_2;
    unsigned int i_ort_tmp = 2*(index_first_canal + i);

    //Ортогональні SIN
    ortogonal_irq[i_ort_tmp] -= data_sin[index_data_sin_cos_array_tmp];
    temp_value_2 = (int)((float)temp_value_1*sin_data_f[index_sin_cos_array_tmp]);
    data_sin[index_data_sin_cos_array_tmp] = temp_value_2;
    ortogonal_irq[i_ort_tmp] += temp_value_2;
    
    //Ортогональні COS
    ortogonal_irq[i_ort_tmp + 1] -= data_cos[index_data_sin_cos_array_tmp];
    temp_value_2 = (int)((float)temp_value_1*cos_data_f[index_sin_cos_array_tmp]);
    data_cos[index_data_sin_cos_array_tmp] = temp_value_2;
    ortogonal_irq[i_ort_tmp + 1] += temp_value_2;
    
    if((++index_data_sin_cos_array_tmp) >= max_size_data_sin_cos) index_data_sin_cos_array_tmp = 0;
  }
  index_data_sin_cos_array[number_val_group] = index_data_sin_cos_array_tmp;
  
  if((++index_sin_cos_array_tmp) >= NUMBER_POINT) index_sin_cos_array_tmp = 0;
  index_sin_cos_array[number_val_group] = index_sin_cos_array_tmp;

  //Копіювання для інших систем
  unsigned int first_index = 2*index_first_canal;

  unsigned int bank_ortogonal_tmp = bank_ortogonal;
  for(unsigned int i = 0; i < (2*number_canals); i++ ) ortogonal[first_index + i][bank_ortogonal_tmp] = ortogonal_irq[first_index + i];
}
/*************************************************************************/

/*************************************************************************
Детектор частоти для каналів групи 1
*************************************************************************/
void fapch_val_1()
{
  unsigned int bank_measurement_high_tmp = bank_measurement_high;
  int index_1 = -1;
  unsigned int maska_canaliv_fapch_tmp = 0;

  /*****
  Шучаємо сигнал по якому будемо розраховувати частоту
  *****/  
  if (measurement_high[bank_measurement_high_tmp][IM_UA1] >= PORIG_FOR_FAPCH)
  {
    index_1 = INDEX_PhK_UA1;
    maska_canaliv_fapch_tmp = READ_Ua1;
  }
  else if (measurement_high[bank_measurement_high_tmp][IM_UB1] >= PORIG_FOR_FAPCH)
  {
    index_1 = INDEX_PhK_UB1;
    maska_canaliv_fapch_tmp = READ_Ub1;
  }
  else if (measurement_high[bank_measurement_high_tmp][IM_UC1] >= PORIG_FOR_FAPCH)
  {
    index_1 = INDEX_PhK_UC1;
    maska_canaliv_fapch_tmp = READ_Uc1;
  }
  delta_phi_index_1 = index_1;
  maska_canaliv_fapch_1 = maska_canaliv_fapch_tmp;
  /*****/

  uint32_t step_val_tmp;
  /*****/
  //Частота ТН1
  /*****/
  step_val_tmp = step_val_1;
  if (
      (index_1 >= 0) &&
      (fix_perechid_cherez_nul[index_1] != 0)
     )   
  {
    fix_perechid_cherez_nul[index_1] = 0;
    
    fix_perechid_cherez_nul_TN1_TN2 |= (1 << N_VAL_1);
    
    unsigned int delta_tick;
    long long tick_tmp;
    int delta_value;
    unsigned int tick_p1, x1_tmp, x2_tmp;
      
    /*Знаходимо час переходу через 0 попереднього разу з врахуванням лінійної апроксимації для ТН1*/
    delta_value = perechid_cherez_nul[index_1][0].y2 - perechid_cherez_nul[index_1][0].y1;
    x1_tmp = perechid_cherez_nul[index_1][0].x1;
    x2_tmp = perechid_cherez_nul[index_1][0].x2;
    if (x2_tmp > x1_tmp) delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long)perechid_cherez_nul[index_1][0].x1) - ((long long)perechid_cherez_nul[index_1][0].y1)*((long long)delta_tick)/((long long)delta_value);
    if (tick_tmp < 0) 
    {
      tick_tmp += 0x100000000;
      tick_p1 = (unsigned int)tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000) tick_p1 = (unsigned int)tick_tmp;
      else 
      {
        tick_tmp -= 0x100000000;
        tick_p1 = (unsigned int)tick_tmp;
      }
    }

    /*Знаходимо час переходу через 0 поточного разу з врахуванням лінійної апроксимації для ТН1*/
    delta_value = perechid_cherez_nul[index_1][1].y2 - perechid_cherez_nul[index_1][1].y1;
    x1_tmp = perechid_cherez_nul[index_1][1].x1;
    x2_tmp = perechid_cherez_nul[index_1][1].x2;
    if (x2_tmp > x1_tmp) delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long)perechid_cherez_nul[index_1][1].x1) - ((long long)perechid_cherez_nul[index_1][1].y1)*((long long)delta_tick)/((long long)delta_value);
    if (tick_tmp < 0) 
    {
      tick_tmp += 0x100000000;
      tick_c1 = (unsigned int)tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000) tick_c1 = (unsigned int)tick_tmp;
      else 
      {
        tick_tmp -= 0x100000000;
        tick_c1 = (unsigned int)tick_tmp;
      }
    }
    /***/
      
    if (tick_c1 > tick_p1) delta_tick = tick_c1 - tick_p1;
    else
    {
      long long delta_tick_64 = tick_c1 + 0x100000000 - tick_p1;
      delta_tick = delta_tick_64;
    }
    tick_period_1 = delta_tick;
    
    /*****
    Розрахунок частоти
    *****/
    if (
        (tick_period_1 <= MAX_TICK_PERIOD) &&
        (tick_period_1 >= MIN_TICK_PERIOD)
       ) 
    {
      frequency_val_1 = (float)MEASUREMENT_TIM_FREQUENCY/(float)tick_period_1;

      step_val_tmp = tick_period_1 >> VAGA_NUMBER_POINT;
      if ((tick_period_1 - (step_val_tmp << VAGA_NUMBER_POINT)) >= (1 << (VAGA_NUMBER_POINT - 1))) step_val_tmp++;
    }
    else
    {
      step_val_tmp = TIM5_CCR1_2_3_VAL;
      if (tick_period_1 > MAX_TICK_PERIOD) frequency_val_1 = -2; /*Частота нижче порогу визначеного константою MIN_FREQUENCY*/
      else frequency_val_1 = -3; /*Частота вище порогу визначеного константою MAX_FREQUENCY*/
    }
    /****/
  }
  else
  {
    if (index_1 < 0)
    {
      step_val_tmp = TIM5_CCR1_2_3_VAL;
      frequency_val_1 = -1; /*Частота не визначена*/
      
      reset_delta_phi = true;
      periodical_tasks_CALC_DELTA_PHI = true;
    }
  }
  /*****/

  /*****/
  //ФАПЧ ТН1
  /*****/
  if (step_val_1 != step_val_tmp)
  {
    //Треба змінити частоту дискретизації
    step_val_1 = step_val_tmp;

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
//    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) == RESET)
//    {
//      uint32_t capture_new;
//      unsigned int delta;
//      TIM5->CCR1 = (capture_new = (previous_tick_VAL_1 + (delta = step_val_1)));
//    
//      unsigned int repeat;
//      unsigned int previous_tick;
//      do
//      {
//        repeat = 0;
//        uint32_t current_tick = TIM5->CNT;
//
//        uint32_t delta_time = 0;
//        if (capture_new < current_tick)
//        {
//          uint64_t delta_time_64 = capture_new + 0x100000000 - current_tick;
//          delta_time = delta_time_64;
//        }
//        else delta_time = capture_new - current_tick;
//
//        if ((delta_time > delta) || (delta_time == 0))
//        {
//          if (TIM_GetITStatus(TIM5, TIM_IT_CC1) == RESET)
//          {
//            if (delta < step_val_tmp)
//            {
//              uint32_t delta_tick;
//              if (current_tick < previous_tick)
//              {
//                uint64_t delta_tick_64 = current_tick + 0x100000000 - previous_tick;
//                delta_tick = delta_tick_64;
//              }
//              else delta_tick = current_tick - previous_tick;
//              
//              delta = delta_tick + 1;
//            }
//            else if (delta == step_val_tmp)
//              delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
//            else
//            {
//              //Теоретично цього ніколи не мало б бути
//              total_error_sw_fixed(81);
//            }
//            TIM5->CCR1 = (capture_new = (TIM5->CNT +  delta));
//            previous_tick = current_tick;
//            repeat = 0xff;
//          }
//        }
//      }
//      while (repeat != 0);
//    }
    /***********************************************************/
  }
  
  if ((command_restart_monitoring_frequency & (1 << 0)) != 0)
  {
    frequency_val_1_min = 50;
    frequency_val_1_max = 50;
    
    command_restart_monitoring_frequency &= (unsigned int)(~(1 << 0));
  }
  else
  {
    if (frequency_val_1 >= 0)
    {
      if (frequency_val_1 > frequency_val_1_max) frequency_val_1_max = frequency_val_1;
      if (frequency_val_1 < frequency_val_1_min) frequency_val_1_min = frequency_val_1;
    }
  }
  /*****/
}
/*****************************************************/

/*************************************************************************
Детектор частоти для каналів групи 2
*************************************************************************/
void fapch_val_2()
{
  unsigned int bank_measurement_high_tmp = bank_measurement_high;
  int index_2 = -1;
  unsigned int maska_canaliv_fapch_tmp = 0;

  /*****
  Шучаємо сигнал по якому будемо розраховувати частоту
  *****/  
  if (measurement_high[bank_measurement_high_tmp][IM_UA2] >= PORIG_FOR_FAPCH)
  {
    index_2 = INDEX_PhK_UA2;
    maska_canaliv_fapch_tmp = READ_Ua2;
  }
  else if (measurement_high[bank_measurement_high_tmp][IM_UB2] >= PORIG_FOR_FAPCH)
  {
    index_2 = INDEX_PhK_UB2;
    maska_canaliv_fapch_tmp = READ_Ub2;
  }
  else if (measurement_high[bank_measurement_high_tmp][IM_UC2] >= PORIG_FOR_FAPCH)
  {
    index_2 = INDEX_PhK_UC2;
    maska_canaliv_fapch_tmp = READ_Uc2;
  }
  delta_phi_index_2 = index_2;
  maska_canaliv_fapch_2 = maska_canaliv_fapch_tmp;
  /*****/

  uint32_t step_val_tmp;
  /*****/
  //Частота ТН2
  /*****/
  step_val_tmp = step_val_2;
  if (
      (index_2 >= 0) &&
      (fix_perechid_cherez_nul[index_2] != 0)
     )   
  {
    fix_perechid_cherez_nul[index_2] = 0;

    fix_perechid_cherez_nul_TN1_TN2 |= (1 << N_VAL_2);

    unsigned int delta_tick;
    long long tick_tmp;
    int delta_value;
    unsigned int tick_p2, x1_tmp, x2_tmp;
      
    /*Знаходимо час переходу через 0 попереднього разу з врахуванням лінійної апроксимації для ТН2*/
    delta_value = perechid_cherez_nul[index_2][0].y2 - perechid_cherez_nul[index_2][0].y1;
    x1_tmp = perechid_cherez_nul[index_2][0].x1;
    x2_tmp = perechid_cherez_nul[index_2][0].x2;
    if (x2_tmp > x1_tmp) delta_tick = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long)perechid_cherez_nul[index_2][0].x1) - ((long long)perechid_cherez_nul[index_2][0].y1)*((long long)delta_tick)/((long long)delta_value);
    if (tick_tmp < 0) 
    {
      tick_tmp += 0x100000000;
      tick_p2 = (unsigned int)tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000) tick_p2 = (unsigned int)tick_tmp;
      else 
      {
        tick_tmp -= 0x100000000;
        tick_p2 = (unsigned int)tick_tmp;
      }
    }

    /*Знаходимо час переходу через 0 поточного разу з врахуванням лінійної апроксимації для ТН2*/
    delta_value = perechid_cherez_nul[index_2][1].y2 - perechid_cherez_nul[index_2][1].y1;
    x1_tmp = perechid_cherez_nul[index_2][1].x1;
    x2_tmp = perechid_cherez_nul[index_2][1].x2;
    if (x2_tmp > x1_tmp) x1_tmp = x2_tmp - x1_tmp;
    else
    {
      long long delta_tick_64 = x2_tmp + 0x100000000 - x1_tmp;
      delta_tick = delta_tick_64;
    }
    tick_tmp = ((long long)perechid_cherez_nul[index_2][1].x1) - ((long long)perechid_cherez_nul[index_2][1].y1)*((long long)delta_tick)/((long long)delta_value);
    if (tick_tmp < 0) 
    {
      tick_tmp += 0x100000000;
      tick_c2 = (unsigned int)tick_tmp;
    }
    else
    {
      if (tick_tmp < 0x100000000) tick_c2 = (unsigned int)tick_tmp;
      else 
      {
        tick_tmp -= 0x100000000;
        tick_c2 = (unsigned int)tick_tmp;
      }
    }
    /***/
      
    if (tick_c2 > tick_p2) delta_tick = tick_c2 - tick_p2;
    else
    {
      long long delta_tick_64 = tick_c2 + 0x100000000 - tick_p2;
      delta_tick = delta_tick_64;
    }
    tick_period_2 = delta_tick;
    
    /*****
    Розрахунок частоти
    *****/
    if (
        (tick_period_2 <= MAX_TICK_PERIOD) &&
        (tick_period_2 >= MIN_TICK_PERIOD)
       ) 
    {
      frequency_val_2 = (float)MEASUREMENT_TIM_FREQUENCY/(float)tick_period_2;

      step_val_tmp = tick_period_2 >> VAGA_NUMBER_POINT;
      if ((tick_period_2 - (step_val_tmp << VAGA_NUMBER_POINT)) >= (1 << (VAGA_NUMBER_POINT - 1))) step_val_tmp++;
    }
    else
    {
      step_val_tmp = TIM5_CCR1_2_3_VAL;
      if (tick_period_2 > MAX_TICK_PERIOD) frequency_val_2 = -2; /*Частота нижче порогу визначеного константою MIN_FREQUENCY*/
      else frequency_val_2 = -3; /*Частота вище порогу визначеного константою MAX_FREQUENCY*/
    }
    /****/
  }
  else
  {
    if (index_2 < 0)
    {
      step_val_tmp = TIM5_CCR1_2_3_VAL;
      frequency_val_2 = -1; /*Частота не визначена*/
      
      reset_delta_phi = true;
      periodical_tasks_CALC_DELTA_PHI = true;
    }
  }
  /*****/

  /*****/
  //ФАПЧ ТН2
  /*****/
  if (step_val_2 != step_val_tmp)
  {
    //Треба змінити частоту дискретизації
    step_val_2 = step_val_tmp;

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
//    if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == RESET)
//    {
//      uint32_t capture_new;
//      unsigned int delta;
//      TIM5->CCR2 = (capture_new = (previous_tick_VAL_2 + (delta = step_val_2)));
//    
//      unsigned int repeat;
//      unsigned int previous_tick;
//      do
//      {
//        repeat = 0;
//        uint32_t current_tick = TIM5->CNT;
//
//        uint32_t delta_time = 0;
//        if (capture_new < current_tick)
//        {
//          uint64_t delta_time_64 = capture_new + 0x100000000 - current_tick;
//          delta_time = delta_time_64;
//        }
//        else delta_time = capture_new - current_tick;
//
//        if ((delta_time > delta) || (delta_time == 0))
//        {
//          if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == RESET)
//          {
//            if (delta < step_val_tmp)
//            {
//              uint32_t delta_tick;
//              if (current_tick < previous_tick)
//              {
//                uint64_t delta_tick_64 = current_tick + 0x100000000 - previous_tick;
//                delta_tick = delta_tick_64;
//              }
//              else delta_tick = current_tick - previous_tick;
//              
//              delta = delta_tick + 1;
//            }
//            else if (delta == step_val_tmp)
//              delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
//            else
//            {
//              //Теоретично цього ніколи не мало б бути
//              total_error_sw_fixed(82);
//            }
//            TIM5->CCR2 = (capture_new = (TIM5->CNT +  delta));
//            previous_tick = current_tick;
//            repeat = 0xff;
//          }
//        }
//      }
//      while (repeat != 0);
//    }
    /***********************************************************/
  }
  
  if ((command_restart_monitoring_frequency & (1 << 1)) != 0)
  {
    frequency_val_2_min = 50;
    frequency_val_2_max = 50;
    
    command_restart_monitoring_frequency &= (unsigned int)(~(1 << 1));
  }
  else
  {
    if (frequency_val_2 >= 0)
    {
      if (frequency_val_2 > frequency_val_2_max) frequency_val_2_max = frequency_val_2;
      if (frequency_val_2 < frequency_val_2_min) frequency_val_2_min = frequency_val_2;
    }
  }
  /*****/
}
/*****************************************************/

/*****************************************************/
//Переривання від прийнятого байту по канаду SPI, який обслуговує вимірювальну систему
/*****************************************************/
void SPI_ADC_IRQHandler(void)
{
//#ifdef SYSTEM_VIEWER_ENABLE
//  SEGGER_SYSVIEW_RecordEnterISR();
//#endif

  //Фіксуємо вхід у переривання обробки даних від АЦП
  semaphore_adc_irq  = true;
  
  //Фіксуємо час оцифровки
  uint32_t tick_output_adc_tmp = TIM5->CNT;
  
  //На самому початку знімаємо chip_select
  GPIO_SPI_ADC->BSRRL = GPIO_NSSPin_ADC;
  uint16_t read_value = SPI_ADC->DR;

  static uint32_t tick_output_adc_p;
  /*
  Аналізуємо прийняті дані
  */
  if (
      (state_reading_ADCs == STATE_READING_WRITE_READ) ||
      (state_reading_ADCs == STATE_READING_READ)
     )
  {
    unsigned int shift = ((GPIO_SELECT_ADC->ODR & GPIO_SELECTPin_ADC) == 0) ? 0 : NUMBER_CANALs_ADC;
    unsigned int number_canal = shift + ((read_value >> 12) & 0xf);

    output_adc[number_canal].tick = tick_output_adc_p;
    output_adc[number_canal].value = read_value & 0xfff;
  }
  tick_output_adc_p = tick_output_adc_tmp;
  /***/
  
  //Виконуємо операції з читання АЦП
  control_reading_ADCs();
  
  /*
  Подальші діх виконуємо тільки тоді, коли всі канали вже оцифровані
  */
  if (state_reading_ADCs == STATE_READING_ADCs_NONE)
  {
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif
  
    /*******************************************************/
    //Перевіряємо, чи відбувалися зміни юстування
    /*******************************************************/
    if (changed_ustuvannja == CHANGED_ETAP_ENDED) /*Це є умова, що нові дані підготовлені для передачі їх у роботу вимірювальною системою (і при цьому зараз дані не змінюються)*/
    {
      //Копіюємо масив юстування у копію цього масиву але з яким працює (читає і змінює) тільки вимірювальна захистема
      for(unsigned int k = 0; k < NUMBER_ANALOG_CANALES; k++) 
      {
        ustuvannja_meas[k] = ustuvannja[k];
      }
      
      //Помічаємо, що зміни прийняті вимірювальною системою, але ще треба прийняти системою захистів
      changed_ustuvannja = CHANGED_ETAP_ENDED_EXTRA_ETAP;
    }
    /*****************************************************/

    /*
    Формуємо значення оцифровуваних каналів
    */
    unsigned int command_word = 0;
    if ((status_adc_read_work & DATA_VAL_1_READ) != 0)
    {
      command_word |= (1 << I_Ia ) | (1 << I_Ib ) | (1 << I_Ic ) |
                      (1 << I_Ua1) | (1 << I_Ub1) | (1 << I_Uc1);
    }
      
    if ((status_adc_read_work & DATA_VAL_2_READ) != 0)
    {
      command_word |= (1 << I_Ua2) | (1 << I_Ub2) | (1 << I_Uc2);
    }
      
    uint32_t _x1, _x2, _DX, _dx;
    int _y1, _y2;
    long long _y;
      
    unsigned int gnd_adc  = gnd_adc1; 
    unsigned int vref_adc = vref_adc1; 

    uint32_t _x = previous_tick_VAL_1;
    /*****/
    //Формуємо значення Ia
    /*****/
    if ((command_word & (1 << I_Ia)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ia].tick;
      _y1 = ADCs_data_raw[I_Ia].value;
        
      _y2 = output_adc[C_Ia_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ia_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ia])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ia_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ia_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ia])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_Ia] = _y;
      
      ADCs_data_raw[I_Ia].tick = _x2;
      ADCs_data_raw[I_Ia].value = _y2;
    }
    /*****/

    /*****/
    //Формуємо значення Ib
    /*****/
    if ((command_word & (1 << I_Ib)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ib].tick;
      _y1 = ADCs_data_raw[I_Ib].value;
        
      _y2 = output_adc[C_Ib_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ib_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ib])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ib_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ib_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ib])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_Ib] = _y;
      
      ADCs_data_raw[I_Ib].tick = _x2;
      ADCs_data_raw[I_Ib].value = _y2;
    }
    /*****/
    
    /*****/
    //Формуємо значення Ic
    /*****/
    if ((command_word & (1 << I_Ic)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ic].tick;
      _y1 = ADCs_data_raw[I_Ic].value;
        
      _y2 = output_adc[C_Ic_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ic_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ic])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ic_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ic_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ic])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      ADCs_data[I_Ic] = _y;
      
      ADCs_data_raw[I_Ic].tick = _x2;
      ADCs_data_raw[I_Ic].value = _y2;
    }
    /*****/
    
    gnd_adc  = gnd_adc2; 
    vref_adc = vref_adc2; 

    /*****/
    //Формуємо значення Ua1
    /*****/
    if ((command_word & (1 << I_Ua1)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ua1].tick;
      _y1 = ADCs_data_raw[I_Ua1].value;
        
      _y2 = output_adc[C_Ua1_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ua1_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ua1])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ua1_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ua1_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ua1])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);
      
      if ((_y >= 0) && (ADCs_data[I_Ua1] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UA1][0].x1 = poperednij_perechid.Ua1_x1;
        perechid_cherez_nul[INDEX_PhK_UA1][0].y1 = poperednij_perechid.Ua1_y1;
        perechid_cherez_nul[INDEX_PhK_UA1][0].x2 = poperednij_perechid.Ua1_x2;
        perechid_cherez_nul[INDEX_PhK_UA1][0].y2 = poperednij_perechid.Ua1_y2;
          
        //Поточний перехід
        poperednij_perechid.Ua1_x1 = perechid_cherez_nul[INDEX_PhK_UA1][1].x1 = penultimate_tick_VAL_1;
        poperednij_perechid.Ua1_y1 = perechid_cherez_nul[INDEX_PhK_UA1][1].y1 = ADCs_data[I_Ua1];
        poperednij_perechid.Ua1_x2 = perechid_cherez_nul[INDEX_PhK_UA1][1].x2 = _x;
        poperednij_perechid.Ua1_y2 = perechid_cherez_nul[INDEX_PhK_UA1][1].y2 = _y;
          
        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UA1] = 0xff;
      }
      ADCs_data[I_Ua1] = _y;
      
      ADCs_data_raw[I_Ua1].tick = _x2;
      ADCs_data_raw[I_Ua1].value = _y2;
    }
    /*****/
    
    /*****/
    //Формуємо значення Ub1
    /*****/
    if ((command_word & (1 << I_Ub1)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ub1].tick;
      _y1 = ADCs_data_raw[I_Ub1].value;
        
      _y2 = output_adc[C_Ub1_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ub1_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ub1])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ub1_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ub1_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ub1])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);
 
      if ((_y >= 0) && (ADCs_data[I_Ub1] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UB1][0].x1 = poperednij_perechid.Ub1_x1;
        perechid_cherez_nul[INDEX_PhK_UB1][0].y1 = poperednij_perechid.Ub1_y1;
        perechid_cherez_nul[INDEX_PhK_UB1][0].x2 = poperednij_perechid.Ub1_x2;
        perechid_cherez_nul[INDEX_PhK_UB1][0].y2 = poperednij_perechid.Ub1_y2;
          
        //Поточний перехід
        poperednij_perechid.Ub1_x1 = perechid_cherez_nul[INDEX_PhK_UB1][1].x1 = penultimate_tick_VAL_1;
        poperednij_perechid.Ub1_y1 = perechid_cherez_nul[INDEX_PhK_UB1][1].y1 = ADCs_data[I_Ub1];
        poperednij_perechid.Ub1_x2 = perechid_cherez_nul[INDEX_PhK_UB1][1].x2 = _x;
        poperednij_perechid.Ub1_y2 = perechid_cherez_nul[INDEX_PhK_UB1][1].y2 = _y;
          
        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UB1] = 0xff;
      }
      ADCs_data[I_Ub1] = _y;
      
      ADCs_data_raw[I_Ub1].tick = _x2;
      ADCs_data_raw[I_Ub1].value = _y2;
    }
    /*****/
    
    /*****/
    //Формуємо значення Uc1
    /*****/
    if ((command_word & (1 << I_Uc1)) != 0)
    {
      _x1 = ADCs_data_raw[I_Uc1].tick;
      _y1 = ADCs_data_raw[I_Uc1].value;
        
      _y2 = output_adc[C_Uc1_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Uc1_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Uc1])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Uc1_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Uc1_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Uc1])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      if ((_y >= 0) && (ADCs_data[I_Uc1] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UC1][0].x1 = poperednij_perechid.Uc1_x1;
        perechid_cherez_nul[INDEX_PhK_UC1][0].y1 = poperednij_perechid.Uc1_y1;
        perechid_cherez_nul[INDEX_PhK_UC1][0].x2 = poperednij_perechid.Uc1_x2;
        perechid_cherez_nul[INDEX_PhK_UC1][0].y2 = poperednij_perechid.Uc1_y2;
          
        //Поточний перехід
        poperednij_perechid.Uc1_x1 = perechid_cherez_nul[INDEX_PhK_UC1][1].x1 = penultimate_tick_VAL_1;
        poperednij_perechid.Uc1_y1 = perechid_cherez_nul[INDEX_PhK_UC1][1].y1 = ADCs_data[I_Uc1];
        poperednij_perechid.Uc1_x2 = perechid_cherez_nul[INDEX_PhK_UC1][1].x2 = _x;
        poperednij_perechid.Uc1_y2 = perechid_cherez_nul[INDEX_PhK_UC1][1].y2 = _y;
          
        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UC1] = 0xff;
      }
      ADCs_data[I_Uc1] = _y;
      
      ADCs_data_raw[I_Uc1].tick = _x2;
      ADCs_data_raw[I_Uc1].value = _y2;
    }
    /*****/
    
    _x = previous_tick_VAL_2;
     /*****/
    //Формуємо значення Ua2
    /*****/
    if ((command_word & (1 << I_Ua2)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ua2].tick;
      _y1 = ADCs_data_raw[I_Ua2].value;
        
      _y2 = output_adc[C_Ua2_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ua2_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ua2])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ua2_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ua2_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ua2])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      if ((_y >= 0) && (ADCs_data[I_Ua2] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UA2][0].x1 = poperednij_perechid.Ua2_x1;
        perechid_cherez_nul[INDEX_PhK_UA2][0].y1 = poperednij_perechid.Ua2_y1;
        perechid_cherez_nul[INDEX_PhK_UA2][0].x2 = poperednij_perechid.Ua2_x2;
        perechid_cherez_nul[INDEX_PhK_UA2][0].y2 = poperednij_perechid.Ua2_y2;
          
        //Поточний перехід
        poperednij_perechid.Ua2_x1 = perechid_cherez_nul[INDEX_PhK_UA2][1].x1 = penultimate_tick_VAL_2;
        poperednij_perechid.Ua2_y1 = perechid_cherez_nul[INDEX_PhK_UA2][1].y1 = ADCs_data[I_Ua2];
        poperednij_perechid.Ua2_x2 = perechid_cherez_nul[INDEX_PhK_UA2][1].x2 = _x;
        poperednij_perechid.Ua2_y2 = perechid_cherez_nul[INDEX_PhK_UA2][1].y2 = _y;
          
        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UA2] = 0xff;
      }
      ADCs_data[I_Ua2] = _y;
      
      ADCs_data_raw[I_Ua2].tick = _x2;
      ADCs_data_raw[I_Ua2].value = _y2;
    }
    /*****/

     /*****/
    //Формуємо значення Ub2
    /*****/
    if ((command_word & (1 << I_Ub2)) != 0)
    {
      _x1 = ADCs_data_raw[I_Ub2].tick;
      _y1 = ADCs_data_raw[I_Ub2].value;
        
      _y2 = output_adc[C_Ub2_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Ub2_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Ub2])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Ub2_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Ub2_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Ub2])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      if ((_y >= 0) && (ADCs_data[I_Ub2] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UB2][0].x1 = poperednij_perechid.Ub2_x1;
        perechid_cherez_nul[INDEX_PhK_UB2][0].y1 = poperednij_perechid.Ub2_y1;
        perechid_cherez_nul[INDEX_PhK_UB2][0].x2 = poperednij_perechid.Ub2_x2;
        perechid_cherez_nul[INDEX_PhK_UB2][0].y2 = poperednij_perechid.Ub2_y2;
          
        //Поточний перехід
        poperednij_perechid.Ub2_x1 = perechid_cherez_nul[INDEX_PhK_UB2][1].x1 = penultimate_tick_VAL_2;
        poperednij_perechid.Ub2_y1 = perechid_cherez_nul[INDEX_PhK_UB2][1].y1 = ADCs_data[I_Ub2];
        poperednij_perechid.Ub2_x2 = perechid_cherez_nul[INDEX_PhK_UB2][1].x2 = _x;
        poperednij_perechid.Ub2_y2 = perechid_cherez_nul[INDEX_PhK_UB2][1].y2 = _y;
          
        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UB2] = 0xff;
      }
      ADCs_data[I_Ub2] = _y;
      
      ADCs_data_raw[I_Ub2].tick = _x2;
      ADCs_data_raw[I_Ub2].value = _y2;
    }
    /*****/
    
     /*****/
    //Формуємо значення Uc2
    /*****/
    if ((command_word & (1 << I_Uc2)) != 0)
    {
      _x1 = ADCs_data_raw[I_Uc2].tick;
      _y1 = ADCs_data_raw[I_Uc2].value;
        
      _y2 = output_adc[C_Uc2_1].value - gnd_adc - vref_adc;
      if (abs(_y2) > 87)
      {
        _x2 = output_adc[C_Uc2_1].tick;
        _y2 = (int)(_y2*ustuvannja_meas[I_Uc2])>>(USTUVANNJA_VAGA - 4);
      }
      else
      {
        _y2 = output_adc[C_Uc2_16].value - gnd_adc - vref_adc;

        _x2 = output_adc[C_Uc2_16].tick;
        _y2 = (int)((-_y2)*ustuvannja_meas[I_Uc2])>>(USTUVANNJA_VAGA);
      }
      
      if (_x2 > _x1) _DX = _x2 - _x1;
      else
      {
        uint64_t _DX_64 = _x2 + 0x100000000 - _x1;
        _DX = _DX_64;
      }
      if (_x >= _x1) _dx = _x - _x1;
      else
      {
        uint64_t _dx_64 = _x + 0x100000000 - _x1;
        _dx = _dx_64;
      }
      _y = ((long long)_y1) + ((long long)(_y2 - _y1))*((long long)_dx)/((long long)_DX);

      if ((_y >= 0) && (ADCs_data[I_Uc2] < 0))
      {
        /*
        Зафіксований перехід через нуль
        */
        //Попередній перехід
        perechid_cherez_nul[INDEX_PhK_UC2][0].x1 = poperednij_perechid.Uc2_x1;
        perechid_cherez_nul[INDEX_PhK_UC2][0].y1 = poperednij_perechid.Uc2_y1;
        perechid_cherez_nul[INDEX_PhK_UC2][0].x2 = poperednij_perechid.Uc2_x2;
        perechid_cherez_nul[INDEX_PhK_UC2][0].y2 = poperednij_perechid.Uc2_y2;
          
        //Поточний перехід
        poperednij_perechid.Uc2_x1 = perechid_cherez_nul[INDEX_PhK_UC2][1].x1 = penultimate_tick_VAL_2;
        poperednij_perechid.Uc2_y1 = perechid_cherez_nul[INDEX_PhK_UC2][1].y1 = ADCs_data[I_Uc2];
        poperednij_perechid.Uc2_x2 = perechid_cherez_nul[INDEX_PhK_UC2][1].x2 = _x;
        poperednij_perechid.Uc2_y2 = perechid_cherez_nul[INDEX_PhK_UC2][1].y2 = _y;
          
        //Помічаємо, що перехід зафіксований
        fix_perechid_cherez_nul[INDEX_PhK_UC2] = 0xff;
      }
      ADCs_data[I_Uc2] = _y;
      
      ADCs_data_raw[I_Uc2].tick = _x2;
      ADCs_data_raw[I_Uc2].value = _y2;
    }
    /*****/

    unsigned int head_data_for_oscylograph_tmp = head_data_for_oscylograph;
    unsigned int x2, x1, delta_x; 

    if ((status_adc_read_work & DATA_VAL_1_READ) != 0)
    {
      //Перевірка на правильну послідовність фаз
      static int current_vector[2][2];
      static unsigned int index_of_currnet_vector;
      static unsigned int ready_vectors; 
      sequence_TN1 = sequence_phases(current_vector, &index_of_currnet_vector, &ready_vectors, /*ADCs_data_raw*/ADCs_data, I_Ua1, IM_UA1);
      
      /*
      Необхідно опрацювати оцифровані дані для перетворення Фур'є
      */
      Fourier(N_VAL_1);
      
      //Формуємо дані для розширеної виборки
      x1 = rozshyrena_vyborka.VAL_1_time_p = penultimate_tick_VAL_1;
      x2 = rozshyrena_vyborka.VAL_1_time_c = previous_tick_VAL_1;
      for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES_VAL_1; i++) 
      {
        rozshyrena_vyborka.VAL_1_data_p[i] = rozshyrena_vyborka.VAL_1_data_c[i];
        rozshyrena_vyborka.VAL_1_data_c[i] = ADCs_data[I_Ia + i];
      }

      /*******************************************************
      Формування апроксимованих значень для каналів групи 1
      *******************************************************/
      if (head_data_for_oscylograph_tmp != VAL_1_tail_data_for_oscylograph)
      {
        if (x2 > x1) delta_x = x2 - x1;
        else
        {
          long long delta_x_64 = x2 + 0x100000000 - x1;
          delta_x = delta_x_64;
        }

        while (head_data_for_oscylograph_tmp != VAL_1_tail_data_for_oscylograph)
        {
          unsigned int VAL_1_tail_data_for_oscylograph_tmp = VAL_1_tail_data_for_oscylograph;
    
          unsigned int x = data_for_oscylograph[VAL_1_tail_data_for_oscylograph_tmp].time_stemp;
        
          unsigned int dx;
          if (x >= x1) dx = x - x1;
          else
          {
            long long dx_64 = x + 0x100000000 - x1;
            dx = dx_64;
          }
          
          if (dx > delta_x) 
          {
            break;
          }

          for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES_VAL_1; i++)
          {
            int y1 = rozshyrena_vyborka.VAL_1_data_p[i], y2 = rozshyrena_vyborka.VAL_1_data_c[i];
            long long y;
            if (dx <= delta_x)
            {
              y = ((long long)(y2 - y1))*((long long)dx)/((long long)delta_x) + ((long long)y1);
            }
            else
            {
              y = 0;
            }
            data_for_oscylograph[VAL_1_tail_data_for_oscylograph_tmp].data[I_Ia + i] = y;
          }
          data_for_oscylograph[VAL_1_tail_data_for_oscylograph_tmp].VAL_1_fix = 0xff;

          if (++VAL_1_tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) VAL_1_tail_data_for_oscylograph = 0;
        }
      }
      /******************************************************/
      
      /*
      Виконуємо операції по визначенню частоти і підстройці частоти
      */
      fapch_val_1();
    
      status_adc_read_work &= (unsigned int)(~DATA_VAL_1_READ);

      /**************************************************/
      //Виставляємо повідомлення про завершення оброки першої групи вимірювальних величин
      /**************************************************/
      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_VAL_1;
      /**************************************************/
    }
    if ((status_adc_read_work & DATA_VAL_2_READ) != 0)
    {
      //Перевірка на правильну послідовність фаз
      static int current_vector[2][2];
      static unsigned int index_of_currnet_vector;
      static unsigned int ready_vectors; 
      sequence_TN2 = sequence_phases(current_vector, &index_of_currnet_vector, &ready_vectors, /*ADCs_data_raw*/ADCs_data, I_Ua2, IM_UA2);
      /*
      Необхідно опрацювати оцифровані дані для перетворення Фур'є для
      аналогових величин групи 2
      */
      Fourier(N_VAL_2);
      
      //Формуємо дані для розширеної виборки
      x1 = rozshyrena_vyborka.VAL_2_time_p = penultimate_tick_VAL_2;
      x2 = rozshyrena_vyborka.VAL_2_time_c = previous_tick_VAL_2;
      for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES_VAL_2; i++) 
      {
        rozshyrena_vyborka.VAL_2_data_p[i] = rozshyrena_vyborka.VAL_1_data_c[i];
        rozshyrena_vyborka.VAL_2_data_c[i] = ADCs_data[I_Ua2 + i];
      }
  
      /*******************************************************
      Формування апроксимованих значень для каналів групи 2
      *******************************************************/
      if (head_data_for_oscylograph_tmp != VAL_2_tail_data_for_oscylograph)
      {
        if (x2 > x1) delta_x = x2 - x1;
        {
          long long delta_x_64 = x2 + 0x100000000 - x1;
          delta_x = delta_x_64;
        }

        while (head_data_for_oscylograph_tmp != VAL_2_tail_data_for_oscylograph)
        {
          unsigned int VAL_2_tail_data_for_oscylograph_tmp = VAL_2_tail_data_for_oscylograph;
    
          unsigned int x = data_for_oscylograph[VAL_2_tail_data_for_oscylograph_tmp].time_stemp;
        
          unsigned int dx;
          if (x >= x1) dx = x - x1;
          {
            long long dx_64 = x + 0x100000000 - x1;
            dx = dx_64;
          }
          
          if (dx > delta_x) break;

          for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES_VAL_2; i++)
          {
            int y1 = rozshyrena_vyborka.VAL_2_data_p[i], y2 = rozshyrena_vyborka.VAL_2_data_c[i];
            long long y;
            if (dx <= delta_x)
            {
              y = ((long long)(y2 - y1))*((long long)dx)/((long long)delta_x) + ((long long)y1);
            }
            else
            {
              y = 0;
            }
            data_for_oscylograph[VAL_2_tail_data_for_oscylograph_tmp].data[I_Ua2 + i] = y;
          }
          data_for_oscylograph[VAL_2_tail_data_for_oscylograph_tmp].VAL_2_fix = 0xff;

          if (++VAL_2_tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) VAL_2_tail_data_for_oscylograph = 0;
        }
      }
      /******************************************************/
      
        /*
        Виконуємо операції по визначенню частоти і підстройці частоти для 
        групи вимірювальних величин 2
        */
        fapch_val_2();
    
      status_adc_read_work &= (unsigned int)(~DATA_VAL_2_READ);

      /**************************************************/
      //Виставляємо повідомлення про завершення оброки другої групи вимірювальних величин
      /**************************************************/
      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_VAL_2;
      /**************************************************/
    }
    if ((fix_perechid_cherez_nul_TN1_TN2 & ((1 << N_VAL_1) | (1 << N_VAL_2))) == ((1 << N_VAL_1) | (1 << N_VAL_2))) 
    {
      if (semaphore_delta_phi == 0)
      {
        fix_perechid_cherez_nul_TN1_TN2_work = fix_perechid_cherez_nul_TN1_TN2;
        fix_perechid_cherez_nul_TN1_TN2 = 0;

        frequency_val_1_work = frequency_val_1;
        tick_period_1_work = tick_period_1;
        tick_c1_work = tick_c1;
        delta_phi_index_1_work_middle = delta_phi_index_1;

        frequency_val_2_work = frequency_val_2;
        tick_period_2_work = tick_period_2;
        tick_c2_work = tick_c2;
        delta_phi_index_2_work_middle = delta_phi_index_2;
        
        periodical_tasks_CALC_DELTA_PHI = true;
      }
    }
    
    /********************************************************
    Формуємо масив миттєвих значень і виконуємо операції для аналогового реєстратора
    ********************************************************/
    unsigned int number_postfault_slices = 0;
    if (head_data_for_oscylograph_tmp != tail_data_for_oscylograph)
    {
      while (
             (head_data_for_oscylograph_tmp != tail_data_for_oscylograph) &&
             (data_for_oscylograph[tail_data_for_oscylograph].VAL_1_fix != 0) &&
             (data_for_oscylograph[tail_data_for_oscylograph].VAL_2_fix != 0)
            )
      {
        unsigned int tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;
        for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
        {
          int data_tmp = data_for_oscylograph[tail_data_for_oscylograph_tmp].data[i];
          
          //Цифровий осцилограф
          current_data[index_array_of_current_data_value++] = data_tmp;
          
          if((prescaler_ar & MASKA_BIT_FOR_PRESCALER) == 0)
          {
            //Масив миттєвих аналогових виборок для аналогового реєстратора
            array_ar[index_array_ar_current++] = data_tmp;
          }
        }
        //Індекс цифрового осцилографа
        if (index_array_of_current_data_value >= (NUMBER_ANALOG_CANALES*NUMBER_POINT*NUMBER_PERIOD_TRANSMIT)) index_array_of_current_data_value = 0;/*Умова мал аб бути ==, але щоб перестахуватися на невизначену помилку я поставив >=*/

        //Масив дискретних сигналів для аналогового реєстратора
        unsigned int *label_to_active_functions_source = data_for_oscylograph[tail_data_for_oscylograph_tmp].active_functions;
        if((prescaler_ar & MASKA_BIT_FOR_PRESCALER) != 0)
        {
          for (unsigned int i = 0; i < N_BIG; i++)  active_functions_trg[i] = *(label_to_active_functions_source + i);
        }
        else
        {
          for (unsigned int i = 0; i < N_BIG; i++)  active_functions_trg[i] |= *(label_to_active_functions_source + i);

          unsigned short int *label_to_active_functions_trg = (unsigned short int*)active_functions_trg;
          for(unsigned int i = 0; i < number_word_digital_part_ar; i++) array_ar[index_array_ar_current++] = *(label_to_active_functions_trg + i);
          //Індекс масиву об'єднаних виборок для аналогового реєстратора
          if (index_array_ar_current >= SIZE_BUFFER_FOR_AR) index_array_ar_current = 0;/*Умова мала б бути ==, але щоб перестахуватися на невизначену помилку я поставив >=*/
          
          //Інкрементуємо кількість зрізів доданих у даному перериванні до післяаварійного масиву об'єднаних виборок для аналогового реєстратора
          if (
              (data_for_oscylograph[tail_data_for_oscylograph_tmp].state_ar_record == STATE_AR_START) ||
              (data_for_oscylograph[tail_data_for_oscylograph_tmp].state_ar_record == STATE_AR_SAVE_SRAM_AND_SAVE_FLASH)
             )
          {
            number_postfault_slices++;
          }
        }
        prescaler_ar++;
    
        data_for_oscylograph[tail_data_for_oscylograph_tmp].VAL_1_fix = 0;
        data_for_oscylograph[tail_data_for_oscylograph_tmp].VAL_2_fix = 0;

        if (++tail_data_for_oscylograph >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) tail_data_for_oscylograph = 0;
      }
    }
    /**************************************************/
    //При необхідності повідомляємо про вихід з формування миттєвих значень
    /**************************************************/
    if (wait_of_receiving_current_data  == true) wait_of_receiving_current_data  = false;
    /**************************************************/

    //Управління аналоговим реємстратором
    if ((state_ar_record == STATE_AR_START) || (state_ar_record == STATE_AR_SAVE_SRAM_AND_SAVE_FLASH))
    {
      static unsigned int uncopied_postfault_time_sapmles;

      /*
      Вже новий зріз післяаварійного масиву у функції доданий у масив
      */
        
      if (state_ar_record == STATE_AR_START)
      {
        int difference;
        /*
        оскільки number_postfault_slices післяаварійних зрізів доданио у масив,
        то для визначення першої мітки післяаварійного масиву від index_array_ar_current
        відняти кількість миттєвих значень у одному зрізі
        */
        /*
        Встановлюємо мітку першого миттєвого значееня післяаваріного масиву і 
        тимчасово помісчаємо її у змінну "вигрузки" для того, щоб дальша програма 
        мала "універсальний", тобто прстіший, вигляд
        */
        difference = index_array_ar_current - number_postfault_slices*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar);
        if (difference >= 0) index_array_ar_heat = difference;
        else index_array_ar_heat = difference + SIZE_BUFFER_FOR_AR;

        //Встановлюємо мітку "вигрузки"
        difference = index_array_ar_heat - (current_settings_prt.prefault_number_periods << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar);
        if (difference >= 0) index_array_ar_tail = difference;
        else index_array_ar_tail = difference + SIZE_BUFFER_FOR_AR;
          
        //Визначаємо скільки треба зрізів записати для післяаварійного масиву
        /*
        !!!Я вибрав саме кількість зрізів а не кількість миттєвих значень, щоб числа
        були меншими і можна було для контролю не рухатися з кроком у кількість каналів,
        а змінюючи цю велицину на одиницю!!!
        */
        uncopied_postfault_time_sapmles = (current_settings_prt.postfault_number_periods << VAGA_NUMBER_POINT_AR);

        /*
        Щоб не було ситуації, що переривання по таймеру ще не відмітило деякі часові
        зрізи, що їх треба додати до післяаварійного масиву - то помічаємо ці часові зрізи, що вони вже 
        мають бути включені у аналоговий реєстратор.
        */
        unsigned int tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;
        while (head_data_for_oscylograph_tmp != tail_data_for_oscylograph_tmp)
        {
           data_for_oscylograph[tail_data_for_oscylograph_tmp++].state_ar_record = state_ar_record;
           if (tail_data_for_oscylograph_tmp >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) tail_data_for_oscylograph_tmp = 0;
        }

        //Переходимо у стан роботи аналогового реєстратора "загрузка і вигрузка"
        state_ar_record = STATE_AR_SAVE_SRAM_AND_SAVE_FLASH;

      }
        
      /*
      Встановлюємо мітку "загрузки" і діагностику переповнення буферу з таких 
      міркувань, що наша програма побудована на принципі, що до цього часу у 
      змінній index_array_ar_heat знаходиться значення мітки "загрузки" до додавання 
      number_postfault_slices зрізів. 
      При цьосму вже додано number_postfault_slices зрізів і змінна
      index_array_ar_current відповідає з текучий стан "заповнення" буферу миттєвих 
      значень для аналогового реєстратора.
      */

      /*
      Спочатку здійснюємо контроль переповнення буферу (теопретично ця помилка ніколи б
      не мала виникати, якщо я правильно вибрав розмір масиву з уразуваннями 
      максимальними розмірами доаварійного і післяаварійного масивів і швидкостей
      "загрузки" і "вигрузки")
      */
      int difference_before, difference_after;
      unsigned int index_array_ar_tail_tmp = index_array_ar_tail;
      difference_before = (index_array_ar_heat - index_array_ar_tail_tmp);
      if (difference_before < 0) difference_before += SIZE_BUFFER_FOR_AR;
      difference_after = (index_array_ar_current - index_array_ar_tail_tmp);
      if (difference_after < 0) difference_after += SIZE_BUFFER_FOR_AR;
      if ((difference_after - difference_before) != number_postfault_slices*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar))
      {
        //Помилкова ситуація, яка викликана переповненням 
        _SET_BIT(set_diagnostyka, ERROR_AR_OVERLOAD_BUFFER_BIT);
      }
      else
      {
        //Нема помилкової ситуації, яка викликана переповненням
        _SET_BIT(clear_diagnostyka, ERROR_AR_OVERLOAD_BUFFER_BIT);
      }

      //Встановлюємо мітку "загрузки" і кількість зрізів, які ще треба добавити до післяаварійного масиву
      if (uncopied_postfault_time_sapmles >= number_postfault_slices)
      {
        index_array_ar_heat = index_array_ar_current;
        uncopied_postfault_time_sapmles -= number_postfault_slices;
      }
      else
      {
        int difference = index_array_ar_current - (number_postfault_slices - uncopied_postfault_time_sapmles);
        if (difference >= 0) index_array_ar_heat = difference;
        else index_array_ar_heat = difference + SIZE_BUFFER_FOR_AR;

        uncopied_postfault_time_sapmles = 0;
      }

      if (uncopied_postfault_time_sapmles == 0)
      {
        /*
        Всі післяаварійні зрізи вже запаисані і тому переводимо режим роботи 
        аналогового реєстратора у режим тільки запису тих значень які ще не записані
        */
        state_ar_record = STATE_AR_ONLY_SAVE_FLASH;

        /*
        Щоб не було ситуації, що переривання по таймеру вже відмітило нові часові
        зрізи, які треба додати до післяаварійного масиву, але ще вони не оброблені,
        бо не було оцифровки АЦП - то помічаємо ці часові зрізи, що вони вже 
        виходять за післяаварійний масив.
        */
        unsigned int tail_data_for_oscylograph_tmp = tail_data_for_oscylograph;
        while (head_data_for_oscylograph_tmp != tail_data_for_oscylograph_tmp)
        {
           data_for_oscylograph[tail_data_for_oscylograph_tmp++].state_ar_record = state_ar_record;
           if (tail_data_for_oscylograph_tmp >= MAX_INDEX_DATA_FOR_OSCYLOGRAPH) tail_data_for_oscylograph_tmp = 0;
        }
      }
        
    }
    /*******************************************************/
    
    if ((status_adc_read_work & TEST_VAL_READ) != 0)
    {
      //Треба опрацювати інтегральні величини
      operate_test_ADCs();
    
      status_adc_read_work &= (unsigned int)(~TEST_VAL_READ);

      /**************************************************/
      //Виставляємо повідомлення про завершення тестових величин
      /**************************************************/
      control_word_of_watchdog |= WATCHDOG_MEASURE_STOP_TEST_VAL;
      /**************************************************/
      
      /**************************************************/
      //Якщо зараз стоїть блокування то його знімаємо
      /**************************************************/
      if (_CHECK_SET_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT) != 0)
      {
        //Повне роозблоковування обміну з мікросхемами для драйверу I2C
        _CLEAR_BIT(control_i2c_taskes, TASK_BLK_OPERATION_BIT);
      }
      /**************************************************/
    }
  
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
  }

  if ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0)
  {
    //Новий обмін не почався функцією control_reading_ADCs
    
    /*
    Можливо з часу останнього виклику функції control_reading_ADCs до даного
    моментк переривання від таймера поставило нові задачі на оцифровку
    */
    
    /*
    Забороняємо генерацію переривань, щоб поки ми аналізуємо чяи немає нових даних
    на оцифровку ще новіші не появилися
    */
    __disable_interrupt();
    
    /*
    Аналізуємо, чи немає нових даних на оцифровку і якщо до даного моменту нових
    даних не появилося, то розблоковуємо можливість початку оцифровки з переривання
    таймеру ( chip select виставлений у 1)
    */
    if (
        (adc_DATA_VAL_1_read == false) &&
        (adc_DATA_VAL_2_read == false) &&
        (adc_TEST_VAL_read   == false)
       )
    {
      semaphore_adc_irq  = false;
    }
    
    /*
    Дозволяємо генерацію переривань
    */
     __enable_interrupt();
  }
  else semaphore_adc_irq  = false;

  
  if (semaphore_adc_irq  != false)
  {
    /*
    Ця умова може бути тільки у одному випадку: якщо при аналізі на нові дані на оцифровку
    з моменту отаннього запуску функції control_reading_ADCs до заборони переривань
    функцією __disable_interrupt дані всетаки появилися.
    
    Модемо і маємо запусти функцію control_reading_ADCs. Конфлікту не буде, бо
    chip_select покищо виставлений у високий стан (це дає остання перевірка 
    if ((GPIO_SPI_ADC->ODR & GPIO_NSSPin_ADC) != 0)) і змінна semaphore_adc_irq ще
    не рівна false, а тим чином блокує запуск оцифровки х переривання таймеру
    */
    
    control_reading_ADCs();
    
    /*
    Скидаємо прапорець. який сигналізує що ми у перериванні обробки оцифрованих даних, які прийшли по SPI
    інтерфейсу. Але цей прапорець вже нічого не блокує, бо запуск функції control_reading_ADCs
    при гарантованій умові, що дані на оцифровку "стоять у черзі"  мусить почати
    оцифровку - а це можливе тільки тоді, коли chip_select цією функцією буде вустановлено у низький рівень
    
    А встановиться chip_select назад у високий рівень тільки коли знову згенерується переривання від
    каналі SPI
    */
    semaphore_adc_irq  = false;
  }
  
//#ifdef SYSTEM_VIEWER_ENABLE
//  SEGGER_SYSVIEW_RecordExitISR();
//#endif
}
/*****************************************************/

/*****************************************************/
//Визначенням кутів
/*****************************************************/
void calc_angle(void) 
{
  //Копіюємо вимірювання
  semaphore_measure_values_low1 = 1;
  for (unsigned int i = 0; i < (NUMBER_ANALOG_CANALES + 8); i++ ) 
  {
    measurement_low[i] = measurement_middle[i];
  }
  semaphore_measure_values_low1 = 0;
              
  int ortogonal_low_tmp[2*FULL_ORT_MAX];
  int delta_phi_synchro_tmp;
  
  //Виставляємо семафор заборони обновлення значень з системи захистів
  semaphore_measure_values_low = 1;
  for(unsigned int i = 0; i < (2*FULL_ORT_MAX); i++ )
  {
      ortogonal_low_tmp[i] = ortogonal_calc_low[i];
  } 
  delta_phi_synchro_tmp = delta_phi_synchro;
  //Знімаємо семафор заборони обновлення значень з системи захистів
  semaphore_measure_values_low = 0;
  
  //Визначаємо, який вектор беремо за осному
  __full_ort_index index_base = FULL_ORT_Ua1;

  /***
  Визначаємо, який останній вектор можна брати за основу
  ***/
  __full_ort_index index_last = FULL_ORT_Uca2;
  /***/
  
  int base_index_for_angle_tmp = -1;
  __full_ort_index index = index_base;
  while( index <= index_last)
  {
    unsigned int index_m;
    switch (index)
    {
    case FULL_ORT_Ua1:
      {
        index_m = IM_UA1;
        break;
      }
    case FULL_ORT_Ub1:
      {
        index_m = IM_UB1;
        break;
      }
    case FULL_ORT_Uc1:
      {
        index_m = IM_UC1;
        break;
      }
    case FULL_ORT_Ua2:
      {
        index_m = IM_UA2;
        break;
      }
    case FULL_ORT_Ub2:
      {
        index_m = IM_UB2;
        break;
      }
    case FULL_ORT_Uc2:
      {
        index_m = IM_UC2;
        break;
      }
    case FULL_ORT_Uab1:
      {
        index_m = IM_UAB1;
        break;
      }
    case FULL_ORT_Ubc1:
      {
        index_m = IM_UBC1;
        break;
      }
    case FULL_ORT_Uca1:
      {
        index_m = IM_UCA1;
        break;
      }
    case FULL_ORT_Uab2:
      {
        index_m = IM_UAB2;
        break;
      }
    case FULL_ORT_Ubc2:
      {
        index_m = IM_UBC2;
        break;
      }
    case FULL_ORT_Uca2:
      {
        index_m = IM_UCA2;
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(61);
      }
    }
  
    if (measurement_low[index_m] >= PORIG_CHUTLYVOSTI_VOLTAGE)
    {
      base_index_for_angle_tmp = index;
      break; //Вихід із циклу while
    }
    index++;
  }
  base_index_for_angle = base_index_for_angle_tmp;
  
  if (base_index_for_angle_tmp >= 0)
  {
    //Координати базового вектору, відносно якого будемо шукати кути
    int ortogonal_base[2] = {ortogonal_low_tmp[2*base_index_for_angle_tmp], ortogonal_low_tmp[2*base_index_for_angle_tmp + 1]};
    
#define SIN_BASE   ortogonal_base[0]
#define COS_BASE   ortogonal_base[1]

    unsigned int amplituda_base = sqrt_64((unsigned long long)((long long)SIN_BASE*(long long)SIN_BASE) + (unsigned long long)((long long)COS_BASE*(long long)COS_BASE));
    if (amplituda_base != 0)
    {
      for (__full_ort_index index_tmp = index_base; index_tmp < FULL_ORT_MAX; index_tmp++)
      {
        if (index_tmp == index)
        {
          phi_angle[index_tmp] = 0;
          continue;
        }
        else
        {
          unsigned int porig_chutlyvosti;
          unsigned int index_m;
          switch (index_tmp)
          {
          case FULL_ORT_Ua1:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UA1;
              break;
            }
          case FULL_ORT_Ub1:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UB1;
              break;
            }
          case FULL_ORT_Uc1:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UC1;
              break;
            }
          case FULL_ORT_Ua2:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UA2;
              break;
            }
          case FULL_ORT_Ub2:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UB2;
              break;
            }
          case FULL_ORT_Uc2:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UC2;
              break;
            }
          case FULL_ORT_Uab1:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UAB1;
              break;
            }
          case FULL_ORT_Ubc1:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UBC1;
              break;
            }
          case FULL_ORT_Uca1:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UCA1;
              break;
            }
          case FULL_ORT_Uab2:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UAB2;
              break;
            }
          case FULL_ORT_Ubc2:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UBC2;
              break;
            }
          case FULL_ORT_Uca2:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_VOLTAGE;
              index_m = IM_UCA2;
              break;
            }
          case FULL_ORT_Ia:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_IA;
              break;
            }
          case FULL_ORT_Ib:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_IB;
              break;
            }
          case FULL_ORT_Ic:
            {
              porig_chutlyvosti = PORIG_CHUTLYVOSTI_CURRENT;
              index_m = IM_IC;
              break;
            }
          default:
            {
              //Теоретично цього ніколи не мало б бути
              total_error_sw_fixed(62);
            }
          }
      
          if (measurement_low[index_m] >= porig_chutlyvosti)
          {
            //Розраховуємо кут
#define SIN_TARGET ortogonal_low_tmp[2*index_tmp]
#define COS_TARGET ortogonal_low_tmp[2*index_tmp + 1]

            unsigned int amplituda_target = sqrt_64((unsigned long long)((long long)SIN_TARGET*(long long)SIN_TARGET) + (unsigned long long)((long long)COS_TARGET*(long long)COS_TARGET));
      
            if (amplituda_target != 0)
            {
              //Вираховуємо COS і SIN кута різниці між векторами
              long long cos_fi = COS_TARGET*COS_BASE + SIN_TARGET*SIN_BASE;
              long long sin_fi = -(SIN_TARGET*COS_BASE - COS_TARGET*SIN_BASE);//З розділу: "Неймовірно, але факт", тобто що я не можу пояснити

#undef SIN_TARGET
#undef COS_TARGET
      
              float sin_fi_f = ( ((float)sin_fi) / ((float)amplituda_target) ) / ((float)amplituda_base);
              if (sin_fi_f > 1) sin_fi_f = 1;
              else if (sin_fi_f < -1) sin_fi_f = -1;
      
              float angle_f = asin(sin_fi_f)*180 / PI;
              int angle_int = (int)(angle_f*10); //беремо точність до десятих
              if (angle_int < 0) angle_int = -angle_int;
      
              if ((sin_fi >= 0) && (cos_fi >= 0))
              {
                //1-ий квадрант
                //angle_int залишається без зміни
              }
              else if ((sin_fi >= 0) && (cos_fi <  0))
              {
                //2-ий квадрант
                angle_int = 1800 - angle_int;
              }
              else if ((sin_fi <  0) && (cos_fi <  0))
              {
                //3-ий квадрант
                angle_int = 1800 + angle_int;
              }
              else
              {
                //4-ий квадрант
                angle_int = 3600 - angle_int;
              }
    
              if (angle_int >= 3600) angle_int -= 3600;
              else if (angle_int < 0) angle_int += 3600;
      
              phi_angle[index_tmp] = angle_int;
      
            }
            else
            {
              phi_angle[index_tmp] = -1;
            }

          }
          else
          {
            //Модуль досліджуваного вектора менше порогу - кут невизначений
            phi_angle[index_tmp] = -1;
          }
        }
      }
    }
    else
    {
      //Амплітуда базового вектору вимірювання по незрозумілій для мене причини рівна 0 (я думаю, що сюди програма не мала б ніколи заходити). Це перестарховка.
      for (__full_ort_index index_tmp = FULL_ORT_Ua1; index_tmp < FULL_ORT_MAX; index_tmp++) phi_angle[index_tmp] = -1;
    }

#undef SIN_BASE
#undef COS_BASE
    
  }
  else
  {
    //Не зафіксовано вектора вимірювання, відносно якого можна розраховувати кути
    for (__full_ort_index index_tmp = FULL_ORT_Ua1; index_tmp < FULL_ORT_MAX; index_tmp++) phi_angle[index_tmp] = -1;
  }
  
  /***
  Копіювання кутів для більш пріоритетних систем
  ***/
  unsigned int bank_phi_angle_high_tmp = (bank_phi_angle_high ^ 0x1) & 0x1;
  for (__full_ort_index index_tmp = FULL_ORT_Ua1; index_tmp < FULL_ORT_MAX; index_tmp++) phi_angle_high[bank_phi_angle_high_tmp][index_tmp] = phi_angle[index_tmp];
  bank_phi_angle_high = bank_phi_angle_high_tmp;
  /***/
    
  
  /***
  Синхронізація між ФАПЧ1 і ФАПЧ2
  ***/
  unsigned int frequency_locking_bank_tmp = (frequency_locking_bank ^ 0x1) & 0x1;
  if (
      (delta_phi_synchro_tmp != UNDEF_PHI) &&
      (delta_phi_index_1_work_low >= INDEX_PhK_UA1)  && (delta_phi_index_1_work_low <= INDEX_PhK_UC1) &&
      (delta_phi_index_2_work_low >= INDEX_PhK_UA2)  && (delta_phi_index_2_work_low <= INDEX_PhK_UC2)
     )   
  {
    //Можна виконувати розрахунки для синхронізації
    int phi_angle_fapch1;
    switch (delta_phi_index_1_work_low)
    {
    case INDEX_PhK_UA1:
      {
        phi_angle_fapch1 = phi_angle[FULL_ORT_Ua1];
        break;
      }
    case INDEX_PhK_UB1:
      {
        phi_angle_fapch1 = phi_angle[FULL_ORT_Ub1];
        break;
      }
    case INDEX_PhK_UC1:
      {
        phi_angle_fapch1 = phi_angle[FULL_ORT_Uc1];
        break;
      }
   default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(84);
      }
    }
        
    int phi_angle_fapch2;
    switch (delta_phi_index_2_work_low)
    {
    case INDEX_PhK_UA2:
      {
        phi_angle_fapch2 = phi_angle[FULL_ORT_Ua2];
        break;
      }
    case INDEX_PhK_UB2:
      {
        phi_angle_fapch2 = phi_angle[FULL_ORT_Ub2];
        break;
      }
    case INDEX_PhK_UC2:
      {
        phi_angle_fapch2 = phi_angle[FULL_ORT_Uc2];
        break;
      }
   default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(85);
      }
    }
        
    if (
        (phi_angle_fapch1 >= 0) &&
        (phi_angle_fapch2 >= 0)
       )   
    {
      frequency_locking_phi += (phi_angle_fapch2 - phi_angle_fapch1 - delta_phi_synchro_tmp);
      if ((frequency_locking_phi <= -3600) || (frequency_locking_phi >= 3600)) frequency_locking_phi %= 3600;

      float radian_frequency_locking_phi = PI*((float)frequency_locking_phi)/1800.0f; /*кут у нас розраховується з тчністю до десятих, тому і ми ділимо на 1800, а не на 180*/
      frequency_locking_cos[frequency_locking_bank_tmp] = arm_cos_f32(radian_frequency_locking_phi);
      frequency_locking_sin[frequency_locking_bank_tmp] = arm_sin_f32(radian_frequency_locking_phi);
    }
    else
    {
      frequency_locking_phi = 0;
      frequency_locking_cos[frequency_locking_bank_tmp] = 1.0f;
      frequency_locking_sin[frequency_locking_bank_tmp] = 0.0f;
    }
  }
  else
  {
    frequency_locking_phi = 0;
    frequency_locking_cos[frequency_locking_bank_tmp] = 1.0f;
    frequency_locking_sin[frequency_locking_bank_tmp] = 0.0f;
  }
  frequency_locking_bank = frequency_locking_bank_tmp;
  /***/
}

/*****************************************************/

/*****************************************************
Розрахунко зсуцву фаз між шиною і напругою
******************************************************/
void delta_phi_routine(void)
{
  if (reset_delta_phi != false)
  {
    reset_delta_phi = false;
    delta_phi[bank_delta_phi] = UNDEF_PHI;
    speed_delta_phi[bank_delta_phi] = UNDEF_SPEED_PHI;
    
    bank_delta_phi = (bank_delta_phi ^ 0x1) & 0x1;
  }
  else
  {
    /*****/
    //Різниця фаз між ТН1 і ТН2
    /*****/

    semaphore_delta_phi = 1;
  
    unsigned int fix_perechid_cherez_nul_TN1_TN2_work_tmp = fix_perechid_cherez_nul_TN1_TN2_work;
    fix_perechid_cherez_nul_TN1_TN2_work = 0;

    float frequency_val_1_work_tmp = frequency_val_1_work;
    unsigned int tick_period_1_work_tmp = tick_period_1_work;
    unsigned int tick_c1_work_tmp = tick_c1_work;
    delta_phi_index_1_work_low = delta_phi_index_1_work_middle;

    float frequency_val_2_work_tmp = frequency_val_2_work;
    unsigned int tick_period_2_work_tmp = tick_period_2_work;
    unsigned int tick_c2_work_tmp = tick_c2_work;
    delta_phi_index_2_work_low = delta_phi_index_2_work_middle;
  
    semaphore_delta_phi = 0;
  
    if ((fix_perechid_cherez_nul_TN1_TN2_work_tmp & ((1 << N_VAL_1) | (1 << N_VAL_2))) == ((1 << N_VAL_1) | (1 << N_VAL_2))) 
    {
      if (
          (frequency_val_1_work_tmp > 0) &&
          (frequency_val_2_work_tmp > 0)
         ) 
      {
        //Можна розраховувати швидкість зміни зсуву фаз у градусах/c (з точністю до десятих градума - тому беремо число 3600, а не 360)
        speed_delta_phi[bank_delta_phi] = (int)(3600.0f*(frequency_val_1_work_tmp - frequency_val_2_work_tmp));
      
        unsigned int min_tick_period = tick_period_1_work_tmp, max_tick_period = tick_period_1_work_tmp;
        if (tick_period_2_work_tmp < min_tick_period) min_tick_period = tick_period_2_work_tmp;
        if (tick_period_2_work_tmp > max_tick_period) max_tick_period = tick_period_2_work_tmp;
        if (
            (max_tick_period <= MAX_TICK_PERIOD) &&
            (min_tick_period >= MIN_TICK_PERIOD)
           )   
        {
          //Можна обчислювати ріжницю фаз між ТН1 і ТН2
          long long delta_phi_tick = ((long long)tick_c1_work_tmp) - ((long long)tick_c2_work_tmp);
          unsigned long long modul_delta_phi_tick = llabs(delta_phi_tick);
    
          if (
              (delta_phi_tick < 0) &&
              (modul_delta_phi_tick >= max_tick_period)
             )       
          {
            /*
            Випадок, коли таймер перейшов своє максимальне значення при фіксації переходу
            через нуль ТН1, а коли був перехід через нуль ТН2 - то ще таймер не перейшов 
            своє максимальне значення
            */
            delta_phi_tick = delta_phi_tick + 0x100000000;
            modul_delta_phi_tick = llabs(delta_phi_tick);
          }
        
          while (modul_delta_phi_tick > tick_period_2_work_tmp) 
          {
            if (delta_phi_tick < 0 ) delta_phi_tick += tick_period_2_work_tmp;
            else delta_phi_tick -= tick_period_2_work_tmp;
            modul_delta_phi_tick = llabs(delta_phi_tick);
          }
          if (delta_phi_tick < 0 ) delta_phi_tick += tick_period_2_work_tmp;
    
          //Можна розраховувати зсув фаз у градусах (з точністю до десятих градума - тому беремо число 3600, а не 360)
          int delta_phi_tmp = (int)((3600.0f*((float)delta_phi_tick)*frequency_val_2_work_tmp)/((float)MEASUREMENT_TIM_FREQUENCY));
          if ((delta_phi_tmp <= -3600) || ( delta_phi_tmp >= 3600))  delta_phi_tmp %= 3600;
          delta_phi[bank_delta_phi] = delta_phi_tmp;
          
          if (delta_phi[bank_delta_phi] < delta_phi_min) delta_phi_min = delta_phi[bank_delta_phi];
          if (delta_phi[bank_delta_phi] > delta_phi_max) delta_phi_max = delta_phi[bank_delta_phi];
        }
        else  delta_phi[bank_delta_phi] = UNDEF_PHI;
      }
      else 
      {
        delta_phi[bank_delta_phi] = UNDEF_PHI;
        speed_delta_phi[bank_delta_phi] = UNDEF_SPEED_PHI;
      }
    
      tick_0[bank_delta_phi] = tick_c1_work_tmp;
      //Змінюємо банки
      bank_delta_phi = (bank_delta_phi ^ 0x1) & 0x1;
    }
  }
}
/*****************************************************/

/*****************************************************
Розрахунок різниці фаз між ТН1 і ТН2 у будь-який момент часу
*****************************************************/
void current_delta_phi(void)
{
  //Вибираємо банк, у якому є останні підготовлені дані
  unsigned int bank_delta_phi_tmp = (bank_delta_phi ^ 0x1) & 0x1;
  
  if (
      (delta_phi[bank_delta_phi_tmp] != UNDEF_PHI) &&
      (speed_delta_phi[bank_delta_phi_tmp] != UNDEF_SPEED_PHI)  
     )   
  {
    //Можна розраховувати ммиттєвий кут розузгодження
    uint32_t current_tick = TIM5->CNT;
    int deta_tick = current_tick - tick_0[bank_delta_phi_tmp];
    if (deta_tick < 0)
    {
      long long deta_tick_tmp = deta_tick;
      deta_tick_tmp += 0x100000000;
      deta_tick = deta_tick_tmp;
    }
    int delta_phi_synchro_tmp = delta_phi[bank_delta_phi_tmp] + (int)((float)speed_delta_phi[bank_delta_phi_tmp]*((float)deta_tick)/((float)MEASUREMENT_TIM_FREQUENCY));
    if ((delta_phi_synchro_tmp <= -3600) || ( delta_phi_synchro_tmp >= 3600))  delta_phi_synchro_tmp %= 3600;
    delta_phi_synchro = delta_phi_synchro_tmp;
  }
  else delta_phi_synchro = UNDEF_PHI;
}
/*****************************************************/

/*****************************************************/
//Контроль правильної послідовності фаз
/*****************************************************/
unsigned int sequence_phases(int p_current_vector[][2], unsigned int *p_index_of_currnet_vector, unsigned int *p_ready_vectors, /*EXTENDED_SAMPLE*/int p_data[], unsigned int index_first_c_data, unsigned int index_first_i_data)
{
  unsigned int result = CONST_SEQ_UNDEF; //Результат у випадку, якщо хоча б одна напруга нижче порогу
  
  unsigned int bank_measurement_high_tmp = bank_measurement_high;
  if (
      (measurement_high[bank_measurement_high_tmp][index_first_i_data  + 0] >= PORIG_FOR_FAPCH) &&
      (measurement_high[bank_measurement_high_tmp][index_first_i_data  + 1] >= PORIG_FOR_FAPCH) &&
      (measurement_high[bank_measurement_high_tmp][index_first_i_data  + 2] >= PORIG_FOR_FAPCH)
     )  
  {
   
    int a = p_data[index_first_c_data + 0]/*.value*/*ea[0] + p_data[index_first_c_data + 1]/*.value*/*eb[0] + p_data[index_first_c_data + 2]/*.value*/*ec[0];
    int b = p_data[index_first_c_data + 0]/*.value*/*ea[1] + p_data[index_first_c_data + 1]/*.value*/*eb[1] + p_data[index_first_c_data + 2]/*.value*/*ec[1];
    
    p_current_vector[*p_index_of_currnet_vector][0] = a;
    p_current_vector[*p_index_of_currnet_vector][1] = b;
    *p_index_of_currnet_vector = (*p_index_of_currnet_vector + 1) & 0x1; //після цієї операції p_index_of_currnet_vector вказує на сумарний вектор (t - dt) і у це місце буде записуватися наступний сумарний вектор
    if (*p_ready_vectors < 2) (*p_ready_vectors)++;
 
    if (*p_ready_vectors >= 2)
    {
      long long value = ((long long)p_current_vector[*p_index_of_currnet_vector][0])*((long long)b) - 
                        ((long long)p_current_vector[*p_index_of_currnet_vector][1])*((long long)a);
      
      if (value > 0) result = CONST_SEQ_OK; //є симетрія
      else if (value < 0) result = CONST_SEQ_FAIL; //немає симетрії
      else result = CONST_SEQ_UNREAL; //невизначений стан, який ніколи не мав би виникати (вектори не крутяться, або прокрутилися аж на 180 градусів)
    }
    else
    {
      //ще не можна перевіряти послідловність, бо немає двох векторів
      result = CONST_SEQ_UNDEF;
    }
    
  }
  else
  {
    //Хоча б одна напруга нижче порогу
    p_current_vector[0][0] = 0;
    p_current_vector[0][1] = 0;
    p_current_vector[1][0] = 0;
    p_current_vector[1][1] = 0;
    *p_index_of_currnet_vector = 0;
    *p_ready_vectors = 0;
    
    result = CONST_SEQ_UNDEF;
  }
  
  return result;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
