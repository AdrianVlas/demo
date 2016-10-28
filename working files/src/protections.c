#include "header.h"

/*****************************************************/
//Діагностика АЦП
/*****************************************************/
inline void diagnostyca_adc_execution(void)
{
  if (gnd_adc1 >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC1_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC1_TEST_BIT);

  if (gnd_adc2 >0x51) _SET_BIT(set_diagnostyka, ERROR_GND_ADC2_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_GND_ADC2_TEST_BIT);

  if ((vref_adc1 <0x709) || (vref_adc1 > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC1_TEST_BIT);
  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC1_TEST_BIT);

  if ((vref_adc2 <0x709) || (vref_adc2 > 0x8f5)) _SET_BIT(set_diagnostyka, ERROR_VREF_ADC2_TEST_BIT);
  else _SET_BIT(clear_diagnostyka,ERROR_VREF_ADC2_TEST_BIT);

  if ((vdd_adc1 <0x8F9) || (vdd_adc1 > 0xC24)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC1_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC1_TEST_BIT);

  if ((vdd_adc2 <0x8F9) || (vdd_adc2 > 0xC24)) _SET_BIT(set_diagnostyka, ERROR_VDD_ADC2_TEST_BIT);
  else _SET_BIT(clear_diagnostyka, ERROR_VDD_ADC2_TEST_BIT);
}
/*****************************************************/

/*****************************************************/
//Пошук розрядності числа
/*****************************************************/
inline unsigned int norma_value(unsigned long long y)
{
  unsigned long long temp=y;
  unsigned int rezultat =0;
  if (temp == 0) return 0;

  while(temp !=0)
  {
    temp = temp>>1;
    rezultat++;
  }
  return rezultat-1;
}
/*****************************************************/

/*****************************************************/
//Пошук розрядності числа типу int
/*****************************************************/
inline unsigned int get_order(int value)
{
  unsigned int i = 0;

  if (value == 0) return 1;  
  if (value < 0) value =-value;

  while ((value >> (++i)) != 0); 

  return i;    
}
/*****************************************************/

/*****************************************************/
//Розрахунок кореня квадратного методом половинного ділення  з прогнозуванням розрядності числа
/*****************************************************/
unsigned int sqrt_64(unsigned long long y)
{
   unsigned int b;
   unsigned int a;
   unsigned int c;
   unsigned int norma_rez;
   unsigned long long temp;
   
   norma_rez = norma_value(y)>>1;
   
   a = (1<<norma_rez) - 1;
   b = (1<<(norma_rez+1));
 
   do 
    {
     c = (a + b)>>1;
     temp = (unsigned long long)c*(unsigned long long)c; 
     if (temp != y)
       {
        if ( temp > y) b= c; else a= c;
       } 
     else return c;
    }
   while ((b-a)>1);

   c = (a + b)>>1;

   return c ;
}
/*****************************************************/

/*****************************************************/
//Розрахунок кореня квадратного методом половинного ділення  з прогнозуванням розрядності числа
/*****************************************************/
inline unsigned int sqrt_32(unsigned int y)
{
   unsigned int b;
   unsigned int a;
   unsigned int c;
   unsigned int norma_rez;
   unsigned int temp;
   
   norma_rez = norma_value(y)>>1;
   
   a = (1<<norma_rez) - 1;
   b = (1<<(norma_rez+1));
 
   do 
    {
     c = (a + b)>>1;
     temp = c*c; 
     if (temp != y)
       {
        if ( temp > y) b= c; else a= c;
       } 
     else return c;
    }
   while ((b-a)>1);

   c = (a + b)>>1;

   return c ;
}
/*****************************************************/

/*****************************************************/
//Розрахунки віимірювань
/*****************************************************/
inline void calc_measurement(unsigned int number_group_stp)
{
  int ortogonal_local[2*NUMBER_ANALOG_CANALES];
  //Виставляємо семафор заборони обновлення значень з вимірювальної системи

  //Копіюємо вхідні велечини у локальні змінні
  /*
  оскільки для дискретного перетворення Фурє коефіцієнти діляться на число виборок і множиться на 2 (еквівалент 2/Т),
  то це ми можемо зробити зміщенням
  */
  /*
  Проведені мною розрахунки показують, що якщо просумувати добуток миттєвих значень на синус/косинус за період,
  а потім результат поділити на 2/Т (зробити це відповідним зсуваом, про який я писав вище),
  то максимана розрядність резутьтату буде рівна макисальній розрядності вхідного сигналу
  Тобто для 3I0            - це 19 біт + знак = ((11 біт + знак)*16*16)
        для фазних струмів - це 15 біт + знак = ((11 біт + знак)*16   )
  оскільки нам ще треба це число піднімати до квадрату а аж потім добувати корінь квадратний з суми квадратів, то
  фазний струм можна підносити до кваррату - переповнення не буде, бо (15 біт *2) = 30 біт Бо 32 біт unsigned int
  А аж потім забрати множенння на 16, щоб збільшити точність вимірювання
  
  Для 3I0 можливе переповнення - тому треба або:
  1.  
  Перше 16-кратне підсилення забрати прямо з ортогональних для 3I0,
  тоді ортогоанльні стануть не більше 15-розрядного числа + знак.
  Друге 16-кратне підсилення забрати вже в остаточному результаті
    
  2.Використати 64-бітну арифметику.
    
  До 17 листопада 2014 року використовуввся перший метод.
  Але виникала похибка при розрахунку стуму вищих гармонік. Припускаю, що могло бути
  зв'язане з тим, що коли відкидадися молодші розряди - то струм першої гармоніки ставав
  трохи меншим за ремальне значення - а тоді виростав струм вищих гармонік
    
  Тому пробую використати другий метод з 17 листопада 2014 року  
  
  Для покращення точності з 18 листопада 2014 року я забираю амплітуду sin/cos вже
  перед самими розрахунками
  */
  
  unsigned int bank_ortogonal_tmp = (bank_ortogonal + 1) & 0x1;
  for(unsigned int i=0; i<(2*NUMBER_ANALOG_CANALES); i++ )
  {
    ortogonal_local[i] = ortogonal[i][bank_ortogonal_tmp];
  }
  bank_ortogonal = bank_ortogonal_tmp;

  unsigned int copy_to_low_tasks = (semaphore_measure_values_low == 0) ? true : false;
  if (copy_to_low_tasks == true) current_delta_phi();
  
  /*******************************************************/
  //Перевіряємо, чи відбувалися зміни юстування
  /*******************************************************/
  if (changed_ustuvannja == CHANGED_ETAP_ENDED_EXTRA_ETAP) /*Це є умова, що нові дані підготовлені для передачі їх у роботу системою захистів(і при цьому зараз дані не змінюються)*/
  {
    //Копіюємо масив юстування у копію цього масиву але з яким працює (читає і змінює) тільки вимірювальна захистема
    for(unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++) 
    {
      phi_ustuvannja_meas[i] = phi_ustuvannja[i];
      phi_ustuvannja_sin_cos_meas[2*i    ] = phi_ustuvannja_sin_cos[2*i    ];
      phi_ustuvannja_sin_cos_meas[2*i + 1] = phi_ustuvannja_sin_cos[2*i + 1];
    }
      
    //Помічаємо, що зміни прийняті системою захистів
    changed_ustuvannja = CHANGED_ETAP_NONE;
  }
  /*****************************************************/

  /***
  Довертаємо кути і копіюємо ортогональні для низькопріоритетних задач
  ***/
  for (unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
    float sin_alpha = ((float)ortogonal_local[2*i    ])/((float)((1 << (VAGA_NUMBER_POINT - 1))));
    float cos_alpha = ((float)ortogonal_local[2*i + 1])/((float)((1 << (VAGA_NUMBER_POINT - 1))));
    
    float sin_beta;
    float cos_beta;
    if (i < I_Ua2)
    {
      sin_beta =  phi_ustuvannja_sin_cos_meas[2*i    ];
      cos_beta =  phi_ustuvannja_sin_cos_meas[2*i + 1];
    }
    else
    {
       unsigned int frequency_locking_bank_tmp = frequency_locking_bank & 0x1;
       sin_beta = phi_ustuvannja_sin_cos_meas[2*i    ]*frequency_locking_cos[frequency_locking_bank_tmp] + phi_ustuvannja_sin_cos_meas[2*i + 1]*frequency_locking_sin[frequency_locking_bank_tmp];
       cos_beta = phi_ustuvannja_sin_cos_meas[2*i + 1]*frequency_locking_cos[frequency_locking_bank_tmp] - phi_ustuvannja_sin_cos_meas[2*i    ]*frequency_locking_sin[frequency_locking_bank_tmp];
    }
    
    unsigned int new_index = index_converter_Ib_p[i];
    int ortogonal_sin = ortogonal_calc[2*new_index    ] = (int)(sin_alpha*cos_beta + cos_alpha*sin_beta);
    int ortogonal_cos = ortogonal_calc[2*new_index + 1] = (int)(cos_alpha*cos_beta - sin_alpha*sin_beta);

    
    //Копіюємо ортогональні для розрахунку кутів
    if (copy_to_low_tasks == true)
    {
      ortogonal_calc_low[2*new_index    ] = ortogonal_sin;
      ortogonal_calc_low[2*new_index + 1] = ortogonal_cos;
    }
  }
  /***/
  
  /*
  ---------------------------------------------------------------------------------------------------------
  150А (150 000мА) - максимальний фазний струм
  Коефіцієнст переведення в мА  - Koef_1 = 84,978173543997808495193432804655 для фазних струмів (5439/64 = 84,984375)
  
  Тоді для 150А максимально можливе значення ортогональних може бути
  150000/Koef_1 = 1765,0303364589078874793160507446
  Якщо врахувати, що сигнал є підсиленим у 16 раз, то максимальне значення ортогональних може бути
  16*150000/Koef_1 = 28240,485383342526199669056811914 < 28241(0x6E51) це є 15 бітне число (+ можливий знак)
  ---------------------------------------------------------------------------------------------------------

  ---------------------------------------------------------------------------------------------------------
  150В (150 000мВ) - максимальна фазна напруга
  Коефіцієнст переведення в мВ  - Koef_1 = 64,883134509545420915167731259667 для фазних напруг (4152/64 = 64,875)
  
  Тоді для 150В максимально можливе значення ортогональних може бути
  150000/Koef_1 = 2311,848851536795430557291797995
  Якщо врахувати, що сигнал є підсиленим у 16 раз, то максимальне значення ортогональних може бути
  16*150000/Koef_1 = 36989,581624588726888916668767919 < 36990(0x907E) це є 16 бітне число (+ можливий знак) - тобто число виходить 17-бітне
  
  Якщо з фазної напруги розраховується лінійна напруга, то, якзо припустити що вектори розврнуті у різні сторони, то  максимальне
  значення ортогональних може бути
  2*16*150000/Koef_1 = 73979,163249177453777833337535838 < 73980(0x120FC) це є 17 бітне число (+ можливий знак) - тобто число виходить 18-бітне
  ---------------------------------------------------------------------------------------------------------
  
  ---------------------------------------------------------------------------------------------------------
  2А (2 000мА * 10 = 20 000(десятих мА)) - максимальний струм 3I0
  Коефіцієнст переведення в десяті мА  - Koef_1* = 169,95634708799561699038686560931 для 3I0  для 3I0 при вираженні у десятих міліамперів (170/1 = 170)

  
  Тоді для 2А максимально можливе значення ортогональних може бути
  20000/Koef_1* = 117,67727621049018824880803941698
  Якщо врахувати, що сигнал є підсиленим у 16 раз (підсилення в 256 раз ми вже зменшили до 16), то максимальне значення ортогональних може бути
  16*20000/Koef_1* = 1882,8364193678430119809286306717 < 1883(0x075B) це є 11 бітне число (+ можливий знак)
  ---------------------------------------------------------------------------------------------------------
  */
  
  /***/
  //Розраховуємо діюче значення через перетворення Фур'є
  /***/
  for(unsigned int i = 0; i < NUMBER_ANALOG_CANALES; i++)
  {
#if (I_Ia == 0)
    if (i <= I_Ic)
#else
    if ((i >= I_Ia) && (i <= I_Ic))
#endif
    {
      unsigned int delta_index = (i - I_Ia);
      unsigned int index_m = IM_IA + delta_index;
      unsigned int index_ort = FULL_ORT_Ia + delta_index;
      
      measurement[index_m] = ( MNOGNYK_I_DIJUCHE*(sqrt_32((unsigned int)(ortogonal_calc[2*index_ort]*ortogonal_calc[2*index_ort]) + (unsigned int)(ortogonal_calc[2*index_ort+1]*ortogonal_calc[2*index_ort+1]))) ) >> (VAGA_DILENNJA_I_DIJUCHE + 4);
    }
    else
    {
      unsigned int delta_index = (i - I_Ua1);
      unsigned int index_m = IM_UA1 + delta_index;
      unsigned int index_ort = FULL_ORT_Ua1 + delta_index;
      
      measurement[index_m] = ( MNOGNYK_U_DIJUCHE*(sqrt_32((unsigned int)(ortogonal_calc[2*index_ort]*ortogonal_calc[2*index_ort]) + (unsigned int)(ortogonal_calc[2*index_ort+1]*ortogonal_calc[2*index_ort+1]))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
    }
    
  }
  /***/

  int _x, _y;
  //Ubc1
  _x = ortogonal_calc[2*FULL_ORT_Ubc1 + 0] = ortogonal_calc[2*FULL_ORT_Ub1    ] - ortogonal_calc[2*FULL_ORT_Uc1    ];
  _y = ortogonal_calc[2*FULL_ORT_Ubc1 + 1] = ortogonal_calc[2*FULL_ORT_Ub1 + 1] - ortogonal_calc[2*FULL_ORT_Uc1 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Ubc1 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Ubc1 + 1] = _y;
  }
  measurement[IM_UBC1] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  
  //Uca1
  _x = ortogonal_calc[2*FULL_ORT_Uca1 + 0] = ortogonal_calc[2*FULL_ORT_Uc1    ] - ortogonal_calc[2*FULL_ORT_Ua1    ];
  _y = ortogonal_calc[2*FULL_ORT_Uca1 + 1] = ortogonal_calc[2*FULL_ORT_Uc1 + 1] - ortogonal_calc[2*FULL_ORT_Ua1 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uca1 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uca1 + 1] = _y;
  }
  measurement[IM_UCA1] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

  //Uab1
  _x = ortogonal_calc[2*FULL_ORT_Uab1 + 0] = ortogonal_calc[2*FULL_ORT_Ua1    ] - ortogonal_calc[2*FULL_ORT_Ub1    ];
  _y = ortogonal_calc[2*FULL_ORT_Uab1 + 1] = ortogonal_calc[2*FULL_ORT_Ua1 + 1] - ortogonal_calc[2*FULL_ORT_Ub1 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uab1 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uab1 + 1] = _y;
  }
  measurement[IM_UAB1] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

  //Ubc2
  _x = ortogonal_calc[2*FULL_ORT_Ubc2 + 0] = ortogonal_calc[2*FULL_ORT_Ub2    ] - ortogonal_calc[2*FULL_ORT_Uc2    ];
  _y = ortogonal_calc[2*FULL_ORT_Ubc2 + 1] = ortogonal_calc[2*FULL_ORT_Ub2 + 1] - ortogonal_calc[2*FULL_ORT_Uc2 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Ubc2 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Ubc2 + 1] = _y;
  }
  measurement[IM_UBC2] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  
  //Uca2
  _x = ortogonal_calc[2*FULL_ORT_Uca2 + 0] = ortogonal_calc[2*FULL_ORT_Uc2    ] - ortogonal_calc[2*FULL_ORT_Ua2    ];
  _y = ortogonal_calc[2*FULL_ORT_Uca2 + 1] = ortogonal_calc[2*FULL_ORT_Uc2 + 1] - ortogonal_calc[2*FULL_ORT_Ua2 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uca2 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uca2 + 1] = _y;
  }
  measurement[IM_UCA2] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);

  //Uab2
  _x = ortogonal_calc[2*FULL_ORT_Uab2 + 0] = ortogonal_calc[2*FULL_ORT_Ua2    ] - ortogonal_calc[2*FULL_ORT_Ub2    ];
  _y = ortogonal_calc[2*FULL_ORT_Uab2 + 1] = ortogonal_calc[2*FULL_ORT_Ua2 + 1] - ortogonal_calc[2*FULL_ORT_Ub2 + 1];
  if (copy_to_low_tasks == true)
  {
    ortogonal_calc_low[2*FULL_ORT_Uab2 + 0] = _x;
    ortogonal_calc_low[2*FULL_ORT_Uab2 + 1] = _y;
  }
  measurement[IM_UAB2] = ( MNOGNYK_U_DIJUCHE*(sqrt_64((unsigned long long)((long long)_x*(long long)_x) + (unsigned long long)((long long)_y*(long long)_y))) ) >> (VAGA_DILENNJA_U_DIJUCHE + 4);
  /***/
}
/*****************************************************/

/*****************************************************/
//Модуль обробки дискретних входів
/*****************************************************/
inline void input_scan(void)
{
  unsigned int state_inputs_into_pin, temp_state_inputs_into_pin; //Змінні у якій формуємо значення входів отримані із входів процесора (пінів)
  static unsigned int state_inputs_into_pin_trigger; //У цій змінній зберігається попередній стан піна, у випадку коли ми перевіряємо .чи утримається цей стан до кінця тактування таймера допуску

  /***************************/
  //У цій частині знімаємо стани входів процесора (пінів), які відповідають за дискретні входи
  /*
  -----------------------------
  значення поміщається у відпорвідні біти змінної state_inputs_into_pin
    "є     сигнал " - відповідає встановленому біту (1)
    "немає сигналу" - відповідає скинутому     біту (0)
  -----------------------------
  */
  unsigned int temp_state_inputs_into_pin_1 = _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_INPUTS_1) & ((1 << NUMBER_INPUTS_1) - 1);
  unsigned int temp_state_inputs_into_pin_2 = _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_INPUTS_2) & ((1 << NUMBER_INPUTS_2) - 1);
  temp_state_inputs_into_pin = temp_state_inputs_into_pin_1 | (temp_state_inputs_into_pin_2 << NUMBER_INPUTS_1);
  
  //Змінюємо порядок, щоб наймений номер відповідав нумерації на приладі
  state_inputs_into_pin = 0;
  for (unsigned int index = 0; index < NUMBER_INPUTS; index++)
  {
    if ((temp_state_inputs_into_pin & (1 << index)) != 0) 
    {
      if (index < NUMBER_INPUTS_1)
        state_inputs_into_pin |= 1 << (NUMBER_INPUTS_1 - index - 1);
      else
        state_inputs_into_pin |= 1 << index;
    }
  }
  /***************************/
  
  /***************************/
  //Обробка таймерів допуску дискретних входів
  /***************************/
  for(unsigned int i = 0; i < NUMBER_INPUTS; i++)
  {
    unsigned int maska = 1<<i;
    int max_value_timer = current_settings_prt.dopusk_dv[i];

    if (global_timers[INDEX_TIMER_INPUT_START + i] < 0)
    {
      //Провірка на зміну стану дискретного входу здійснюється тільки тоді, коли величина таймере є від'ємною
      //Що означає що таймер не запущений у попередній момент
      
      //Перевіряємо чи таеперішній стан входу відповідає попердньомук стану
      if ((state_inputs_into_pin & maska) != (state_inputs & maska))
      {
        //Якщо стан входу змінився, то виконуємо дії по зміні стану

        //1-дія: Запям'ятовуємо текучий стан входу
        state_inputs_into_pin_trigger &= ~maska;
        state_inputs_into_pin_trigger |= (state_inputs_into_pin & maska);

        //2-дія: Запускаємо таймер допуску дискретного входу
        global_timers[INDEX_TIMER_INPUT_START + i] = 0;
      }
    }
    else
    {
      //Якщо таймер запущений виконуємо дії з таймером
      if (global_timers[INDEX_TIMER_INPUT_START + i] < max_value_timer)
      {
        //Якщо таймер ще не дійшов до свого макисмуму, то просто збільшуємо його величину
        global_timers[INDEX_TIMER_INPUT_START + i] += DELTA_TIME_FOR_TIMERS;
        
        //У випадку, якщо тип сигналу на вхід подається змінний
        //і ми перевіряємо чи не відбувся перехід "є сигнал"->"немає сигналу"
        //то поява сигналу під час тактування таймера допуску означає, що сигнал на вході є - 
        //а це означає, що треба зупинити nаймер, бо переходу "є сигнал"->"немає сигналу" на протязі тактування таймеру не зафіксовано 
        if ((current_settings_prt.type_of_input_signal & maska) != 0)
        {
           if ((state_inputs_into_pin_trigger & maska) == 0)
           {
             if ((state_inputs_into_pin & maska) != 0)
               global_timers[INDEX_TIMER_INPUT_START + i] = -1;
           }
        }
      }
    }
    
    //Якщо величина таймера допуска знаходиться у свому максимальному значенні, то перевіряємо, чи фактично змінився стано входу
    if (global_timers[INDEX_TIMER_INPUT_START + i] >= max_value_timer)
    {
      //Перевіряємо, чи стан піна змінився у порівнянні із станом, який був на момент запуску таймера допуску дискретного входу
      unsigned int state_1, state_2;
        
      state_1 = state_inputs_into_pin_trigger & maska;
      state_2 = state_inputs_into_pin  & maska;
        
      if (state_1 == state_2)
      {
        //Якщо два стани співпадають, то ми вважаємо, що відбулася зіна стану дискретного входу і формуємо новий стан входу
        //При цьому враховуємо що для прявого    входу 1 - це активний вхід, а 0 - це пасивний вхід
        //                        для інверсного входу 0 - це активний вхід, а 1 - це пасивний вхід
          state_inputs &=  ~maska;
          state_inputs |=   state_2;
      }
        
      //У будь-якому випадк, чи змінився стан входу, чи ні, а оскілька таймер допуску дотактував до кінця, то скидаємотаймер у висхідне від'ємне занчення
      global_timers[INDEX_TIMER_INPUT_START + i] = -1;
    }
  }
  /***************************/
  
  /***************************/
  //Визначаємо, який сигнал є зараз активним
  /***************************/
  active_inputs = (state_inputs ^ current_settings_prt.type_of_input);
  /***************************/
  
}
/*****************************************************/

/*****************************************************/
//Функція обробки таймерів
/*****************************************************/
inline void clocking_global_timers(void)
{
  //Опрацьовуємо дискретні входи
  input_scan();
  
  //опрацьовуємо всі решта таймери логіки
  for (unsigned int i = (INDEX_TIMER_INPUT_START + NUMBER_INPUTS); i < MAX_NUMBER_GLOBAL_TIMERS; i++)
  {
    if (global_timers[i] >= 0)
    {
      //Першою умовою того, що таймер треба тактувати є той факт, що величина таймеру не від'ємна

      //Перевіряємо чи треба збільшувати величину таймеру, якщо він ще не досягнув свого максимуму
      if (global_timers[i] <= (0x7fffffff - DELTA_TIME_FOR_TIMERS)) global_timers[i] += DELTA_TIME_FOR_TIMERS;
    }
  }
  
  if (++timer_prt_signal_output_mode_2 >= PERIOD_SIGNAL_OUTPUT_MODE_2)
  {
    timer_prt_signal_output_mode_2 = 0;
    output_timer_prt_signal_output_mode_2 ^= true;
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання Ориділювальних функцій - має запускатися після відкрпацювання блоків всіх захистів
/*****************************************************/
inline void df_handler(volatile unsigned int *p_active_functions, unsigned int *p_changed_state_with_start_new_timeout)
{
  /*
  Джерела активації формуємо в source_activation_df
  Формуємо маску вже активних функцій у maska_active_df
  */
  unsigned int source_activation_df = 0;
  unsigned int state_df = 0;
  for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
  {
    unsigned int number_byte_in, number_bit_in, number_byte_out, number_bit_out;
    switch (i)
    {
    case 0:
      {
        number_byte_in = RANG_DF1_IN >> 5;
        number_bit_in = RANG_DF1_IN & 0x1f;

        number_byte_out = RANG_DF1_OUT >> 5;
        number_bit_out = RANG_DF1_OUT & 0x1f;
        
        break;
      }
    case 1:
      {
        number_byte_in = RANG_DF2_IN >> 5;
        number_bit_in = RANG_DF2_IN & 0x1f;

        number_byte_out = RANG_DF2_OUT >> 5;
        number_bit_out = RANG_DF2_OUT & 0x1f;
        
        break;
      }
    case 2:
      {
        number_byte_in = RANG_DF3_IN >> 5;
        number_bit_in = RANG_DF3_IN & 0x1f;

        number_byte_out = RANG_DF3_OUT >> 5;
        number_bit_out = RANG_DF3_OUT & 0x1f;
        
        break;
      }
    case 3:
      {
        number_byte_in = RANG_DF4_IN >> 5;
        number_bit_in = RANG_DF4_IN & 0x1f;

        number_byte_out = RANG_DF4_OUT >> 5;
        number_bit_out = RANG_DF4_OUT & 0x1f;
        
        break;
      }
    case 4:
      {
        number_byte_in = RANG_DF5_IN >> 5;
        number_bit_in = RANG_DF5_IN & 0x1f;

        number_byte_out = RANG_DF5_OUT >> 5;
        number_bit_out = RANG_DF5_OUT & 0x1f;
        
        break;
      }
    case 5:
      {
        number_byte_in = RANG_DF6_IN >> 5;
        number_bit_in = RANG_DF6_IN & 0x1f;

        number_byte_out = RANG_DF6_OUT >> 5;
        number_bit_out = RANG_DF6_OUT & 0x1f;
        
        break;
      }
    case 6:
      {
        number_byte_in = RANG_DF7_IN >> 5;
        number_bit_in = RANG_DF7_IN & 0x1f;

        number_byte_out = RANG_DF7_OUT >> 5;
        number_bit_out = RANG_DF7_OUT & 0x1f;
        
        break;
      }
    case 7:
      {
        number_byte_in = RANG_DF8_IN >> 5;
        number_bit_in = RANG_DF8_IN & 0x1f;

        number_byte_out = RANG_DF8_OUT >> 5;
        number_bit_out = RANG_DF8_OUT & 0x1f;
        
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(43);
        break;
      }
    }

    if (i < NUMBER_DEFINED_FUNCTIONS/*current_settings_prt.number_defined_df*/)
    {
      /***
      Джерело активації ОФ-ії
      ***/
      source_activation_df |= ((p_active_functions[number_byte_in] & (1 << number_bit_in) ) >> number_bit_in ) << i;
      //Перевіряємо ще, чи не іде утимування активним джерела ОФ через таймер-утримування (для активації через кнопки або інтерфейс)
      if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= 0)
      {
        //Таймер запущений, або вже зупинився
        //Факт запуску цього таймеру означає, що активація відбувалася через кнопку, або інтерфейс
        //Тому для забеспечення роботи логічної схеми до кінця роботи цього таймеру виставляємо, що джерело активації активне
        source_activation_df |= (1 << i);
      
        //Відмічаємо, джерело активації утримуємться у активному стані у масиві активуючих функцій
        p_active_functions[number_byte_in] |= (1 << number_bit_in);
      
        //У випадку, якщо таймер дійшов до свого макисального значення, то скидаємо роботу цього таймеру
        if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] >= ((int)current_settings_prt.timeout_pause_df[i]))
          global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = -1;
      }
      /***/

      /***
      Формування маски до цього часу активних ОФ-ій
      ***/
      state_df |= ((p_active_functions[number_byte_out] & ((unsigned int)(1 << number_bit_out))) != 0) << i;
      /***/
    }
    else
    {
      //Таймери, які не задіяні скидаємо з роботи
      global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = -1;
      global_timers[INDEX_TIMER_DF_PAUSE_START + i] = -1;
      global_timers[INDEX_TIMER_DF_WORK_START + i] = -1;
      
      etap_execution_df[i] = NONE_DF;
    }
  }
  
  //Визначаємо, чи активовуються опреділювані функції через свої ранжовані функції-джерела
  unsigned int source_blk_df = 0;
  for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS/*current_settings_prt.number_defined_df*/; i++)
  {
    if (
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      if(
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i    ] & p_active_functions[0] ) != 0) ||
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 1] & p_active_functions[1] ) != 0) ||
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 2] & p_active_functions[2] ) != 0) ||
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 3] & p_active_functions[3] ) != 0) ||
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 4] & p_active_functions[4] ) != 0) ||
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 5] & p_active_functions[5] ) != 0) ||
         ( ( current_settings_prt.ranguvannja_df_source_plus[N_BIG*i + 6] & p_active_functions[6] ) != 0)
        )
      {
        source_activation_df |= (1 << i);
      }
    }

    if (
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      if(
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i    ] & ((unsigned int)(~p_active_functions[0])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 1] & ((unsigned int)(~p_active_functions[1])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 2] & ((unsigned int)(~p_active_functions[2])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 3] & ((unsigned int)(~p_active_functions[3])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 4] & ((unsigned int)(~p_active_functions[4])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 5] & ((unsigned int)(~p_active_functions[5])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_minus[N_BIG*i + 6] & ((unsigned int)(~p_active_functions[6])) ) != 0 )
        )
      {
        source_activation_df |= (1<< i);
      }
    }

    if (
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані насправді на джерело блокування
      if(
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i    ] & p_active_functions[0] ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 1] & p_active_functions[1] ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 2] & p_active_functions[2] ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 3] & p_active_functions[3] ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 4] & p_active_functions[4] ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 5] & p_active_functions[5] ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_df_source_blk[N_BIG*i + 6] & p_active_functions[6] ) != 0 )
        )
      {
        source_blk_df |= (1<< i);
      }
    }
    
    //Запускаємо у роботу лоргічну хему роботи опреділюваної функції
    switch (etap_execution_df[i])
    {
    case NONE_DF:
      {
        if ((source_activation_df & (1<<i)) != 0)
        {
           if (current_settings_prt.timeout_pause_df[i] > 0)
           {
             //Випадок, коли є перед активацією ОФ, треба витримати таймер павзи
             //Запускаємо відраховування таймера паузи
             global_timers[INDEX_TIMER_DF_PAUSE_START + i] = 0;
             //Змінюємо етап виконання логічної схеми на очікування завершення часу павзи
             etap_execution_df[i] = START_TIMER_PAUSE_DF;
           }
           else
           {
             //Випадок, коли таймер павзи рівний нулю, тобто треба зразу активовувати ОФ і запускати таймер роботи

             //Встановлюємо стан даної ОФ в "АКТИВНИЙ"
             state_df |= (1 << i);
             //Запускаємо відраховування таймера роботи
             global_timers[INDEX_TIMER_DF_WORK_START + i] = 0;
             //Переходимо на відраховування таймеру роботи - протягом цього часу ОФ гарантовано активується (якщо немає умови блокування)
             etap_execution_df[i] = EXECUTION_DF;
           }
           
           //Фіксація про зміну стану з початком відслідковуванням нової витримки
           *p_changed_state_with_start_new_timeout |= (1 << i);
        }
        break;
      }
    case START_TIMER_PAUSE_DF:
      {
        //Зараз іде відлік часу таймеру павзи
        //Перевіряємо, чи ОФ ще утимується в стані активації через своє джерело
        if ((source_activation_df & (1<< i)) != 0)
        {
          //Перевіряємо, чи завершилася робота таймеру павзи
          if (global_timers[INDEX_TIMER_DF_PAUSE_START + i] >= ((int)current_settings_prt.timeout_pause_df[i]))
          {
            if ((*p_changed_state_with_start_new_timeout & (1 << i)) == 0)
            {
              //Завершився час роботи таймеру павзи
              global_timers[INDEX_TIMER_DF_PAUSE_START + i] = -1;
              //Встановлюємо стан даної ОФ в "АКТИВНИЙ"
              state_df |= (1 << i);
              //Запускаємо відраховування таймера роботи
              global_timers[INDEX_TIMER_DF_WORK_START + i] = 0;
              //Переходимо на відраховування таймеру роботи - протягом цього часу ОФ гарантовано активується (якщо немає умови блокування)
              etap_execution_df[i] = EXECUTION_DF;
           
              //Фіксація про зміну стану з початком відслідковуванням нової витримки
              *p_changed_state_with_start_new_timeout |= (1 << i);
            }
          }
        }
        else
        {
          //Активація знята до завершення роботи таймеру павзи, тому скидаємо всю роботу по даній оприділюваеній функції
          global_timers[INDEX_TIMER_DF_PAUSE_START + i] = -1;
          etap_execution_df[i] = NONE_DF;
           
          //Відміна фіксації про зміну стану з початком відслідковуванням нової витримки
          *p_changed_state_with_start_new_timeout &= (unsigned int)(1 << i);
        }
        break;
      }
    case EXECUTION_DF:
      {
        //Подальша робота даної ОФ залежить від типу ОФ
        if ((current_settings_prt.type_df & (1<<i)) != 0)
        {
          //Дана ОФ ЗВОРОТНЯ
          //Згідно логічної схеми утримуємо ОФ таймер роботи має запуститися після деактивації джерела
          if ((source_activation_df & (1<< i)) != 0)
          {
            //Джерело ще є активне, а тому таймер роботи утримуємо у нульовому значенні
            global_timers[INDEX_TIMER_DF_WORK_START + i] = 0;
          }
        }

        //Перевіряємо, чи завершилася робота таймеру роботи
        /*
        Умовою продовження активації ОФ є випадок коли ОФ зворотня і джерело є активним
        Впринципі на рівні таймеру все б мало працювати правильно, але є один нюанс, коли
        функція є звороньою і джерело активне, то код вище скине таймер роботи в 0, 
        тобто він буде гарантовано в 0, але коли уставка "Таймер роботи" для даної функції буде 0,
        то по аналізу самого таймеру робота функції може зупинитися, що не правильно.
        Тому тут ще я добавляю перевірку, що ОФ не може деактивуватися навіть коли таймер роботи
        перевищує свою уставку у випадку коли і ОФ є зворотньою і джерело є активним
        */
        if (
            (global_timers[INDEX_TIMER_DF_WORK_START + i] >= ((int)current_settings_prt.timeout_work_df[i])) &&
            (
             !( 
               ((current_settings_prt.type_df & (1 << i)) != 0) && 
               ((source_activation_df         & (1 << i)) != 0) 
              )
            )  
           )
        {
          if ((*p_changed_state_with_start_new_timeout & (1 << i)) == 0)
          {
            //Завершився час роботи таймеру роботи
            global_timers[INDEX_TIMER_DF_WORK_START + i] = -1;
            //Переводимо ОФ у ПАСИВНИЙ стан
            state_df &= ~(1 << i);
          
            //Перевіряємо, чи нам треба перейти в очікування деактивації джерела ОФ, чи перейти у висхідний стан
            if ((source_activation_df & (1<< i)) == 0)
            {
              //Переходимо у режим висхідний стан
              etap_execution_df[i] = NONE_DF;
            }
            else 
            {
              //Якщо джерело запуску цієї ОФ ще активне, то переходимо на оцікування його завершення
              etap_execution_df[i] = WAITING_DEACTIVATION_SOURCE_DF;
            }
          }
        }
        break;
      }
    case WAITING_DEACTIVATION_SOURCE_DF:
      {
        //У цьому режимі ми перебуваємо доти, поки джерело активації ОФ не буде зняте
        if ((source_activation_df & (1 << i)) == 0)
        {
          //Переходимо у режим висхідний стан
          etap_execution_df[i] = NONE_DF;
        }
        break;
      }
    default: break;
    }
  }

  //Установлюємо, або скидаємо ОФ у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені виходи ОФ-ій, тому що інші ОФ-ії
  можуть бути джерелом активації, але джерелом активації може буте попереднє значення ОФ, а не те,
  що зараз встановлюється. А оскілдьки у другому масиві ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом активації/деактивації)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
  {
    //Установлюємо, або скидаємо ОФ
    unsigned int index_df;
    switch (i)
    {
    case 0:
      {
       index_df = RANG_DF1_OUT;
        break;
      }
    case 1:
      {
        index_df = RANG_DF2_OUT;
        break;
      }
    case 2:
      {
        index_df = RANG_DF3_OUT;
        break;
      }
    case 3:
      {
        index_df = RANG_DF4_OUT;
        break;
      }
    case 4:
      {
        index_df = RANG_DF5_OUT;
        break;
      }
    case 5:
      {
        index_df = RANG_DF6_OUT;
        break;
      }
    case 6:
      {
        index_df = RANG_DF7_OUT;
        break;
      }
    case 7:
      {
        index_df = RANG_DF8_OUT;
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(44);
        break;
      }
    }
      
    if ((state_df & (1 << i)) != 0 )
    {
      /*
      ОФ функція зараз є активною
      але перед тим, як виставити реальний стан цієї функції на даний момент часу - 
      перевіряєио, чи не йде блокування її 
      */     
      if ((source_blk_df & (1<< i)) == 0 ) _SET_BIT(p_active_functions, index_df);
      else _CLEAR_BIT(p_active_functions, index_df);
    }
    else
    {
      _CLEAR_BIT(p_active_functions, index_df);
    }
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання Ориділювальних триґерів - має запускатися після відкрпацювання опреділювальних функцій
/*****************************************************/
inline void dt_handler(volatile unsigned int *p_active_functions)
{
  /*
  Попередній стан визначуваних триґерів формуємо у state_defined_triggers
  Джерела встановлення формуємо в source_set_dt
  Джерела скидання формуємо в source_reset_dt
  */
  unsigned int state_defined_triggers = 0, source_set_dt = 0, source_reset_dt = 0;
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS/*current_settings_prt.number_defined_dt*/; i++)
  {
    unsigned int number_byte_set, number_bit_set, number_byte_reset, number_bit_reset;
    unsigned int index_dt;
    switch (i)
    {
    case 0:
      {
        number_byte_set = RANG_DT1_SET >> 5;
        number_bit_set = RANG_DT1_SET & 0x1f;

        number_byte_reset = RANG_DT1_RESET >> 5;
        number_bit_reset = RANG_DT1_RESET & 0x1f;
        
        index_dt = RANG_DT1_OUT;
        
        break;
      }
    case 1:
      {
        number_byte_set = RANG_DT2_SET >> 5;
        number_bit_set = RANG_DT2_SET & 0x1f;

        number_byte_reset = RANG_DT2_RESET >> 5;
        number_bit_reset = RANG_DT2_RESET & 0x1f;
        
        index_dt = RANG_DT2_OUT;
        
        break;
      }
    case 2:
      {
        number_byte_set = RANG_DT3_SET >> 5;
        number_bit_set = RANG_DT3_SET & 0x1f;

        number_byte_reset = RANG_DT3_RESET >> 5;
        number_bit_reset = RANG_DT3_RESET & 0x1f;
        
        index_dt = RANG_DT3_OUT;
        
        break;
      }
    case 3:
      {
        number_byte_set = RANG_DT4_SET >> 5;
        number_bit_set = RANG_DT4_SET & 0x1f;

        number_byte_reset = RANG_DT4_RESET >> 5;
        number_bit_reset = RANG_DT4_RESET & 0x1f;
        
        index_dt = RANG_DT4_OUT;
        
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(55);
        break;
      }
    }

    state_defined_triggers |= ( (p_active_functions[index_dt >> 5] & ( 1 << (index_dt & 0x1f) ) ) >> (index_dt & 0x1f) ) << i;
    /***
    Джерела встановлення і скидання ОТ
    ***/
    source_set_dt   |= ((p_active_functions[number_byte_set  ] & (1 << number_bit_set  ) ) >> number_bit_set   ) << i;
    source_reset_dt |= ((p_active_functions[number_byte_reset] & (1 << number_bit_reset) ) >> number_bit_reset ) << i;
    /***/
  }
  
  //Визначаємо, чи встановлюються/скидаються опреділювані триґери через свої ранжовані функції-джерела
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS/*current_settings_prt.number_defined_dt*/; i++)
  {
    if (
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      if(
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i    ] & p_active_functions[0]) != 0) ||
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 1] & p_active_functions[1]) != 0) ||
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 2] & p_active_functions[2]) != 0) ||
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 3] & p_active_functions[3]) != 0) ||
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 4] & p_active_functions[4]) != 0) ||
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 5] & p_active_functions[5]) != 0) ||
         (( current_settings_prt.ranguvannja_set_dt_source_plus[N_BIG*i + 6] & p_active_functions[6]) != 0)
        )
      {
        source_set_dt |= (1 << i);
      }
    }

    if (
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      if(
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i    ] & ((unsigned int)(~p_active_functions[0])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 1] & ((unsigned int)(~p_active_functions[1])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 2] & ((unsigned int)(~p_active_functions[2])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 3] & ((unsigned int)(~p_active_functions[3])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 4] & ((unsigned int)(~p_active_functions[4])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 5] & ((unsigned int)(~p_active_functions[5])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_set_dt_source_minus[N_BIG*i + 6] & ((unsigned int)(~p_active_functions[6])) ) != 0 )
        )
      {
        source_set_dt |= (1<< i);
      }
    }

    if (
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані на джерело прямих функцій
      if(
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i    ] & p_active_functions[0]) != 0) ||
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 1] & p_active_functions[1]) != 0) ||
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 2] & p_active_functions[2]) != 0) ||
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 3] & p_active_functions[3]) != 0) ||
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 4] & p_active_functions[4]) != 0) ||
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 5] & p_active_functions[5]) != 0) ||
         (( current_settings_prt.ranguvannja_reset_dt_source_plus[N_BIG*i + 6] & p_active_functions[6]) != 0)
        )
      {
        source_reset_dt |= (1 << i);
      }
    }

    if (
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i    ] !=0) || 
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 1] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 2] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 3] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 4] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 5] !=0) ||
        (current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 6] !=0)
       )
    {
      //Випадок, якщо функції зранжовані на джерело інверсних функцій
      if(
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i    ] & ((unsigned int)(~p_active_functions[0])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 1] & ((unsigned int)(~p_active_functions[1])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 2] & ((unsigned int)(~p_active_functions[2])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 3] & ((unsigned int)(~p_active_functions[3])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 4] & ((unsigned int)(~p_active_functions[4])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 5] & ((unsigned int)(~p_active_functions[5])) ) != 0 ) ||
         ( ( current_settings_prt.ranguvannja_reset_dt_source_minus[N_BIG*i + 6] & ((unsigned int)(~p_active_functions[6])) ) != 0 )
        )
      {
        source_reset_dt |= (1<< i);
      }
    }

    //Запускаємо у роботу лоргічну схему роботи опреділюваного триґера (встановлення має пріоритет)
    if ((source_set_dt   & (1 << i)) != 0) state_defined_triggers |= (1 << i);
    if ((source_reset_dt & (1 << i)) != 0) state_defined_triggers &= (unsigned int)(~(1 << i));
  }

  //Установлюємо, або скидаємо ОТ у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходыв ОТ-ів, тому що інші ОТ-и
  можуть бути джерелом встановлення/скидання, але джерелом встановлення/скидання може буте попереднє значення ОТ, а не те,
  що зараз встановлюється. А оскілдьки у ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
  {
    //Установлюємо, або скидаємо ОТ
    unsigned int index_dt;
    switch (i)
    {
    case 0:
      {
       index_dt = RANG_DT1_OUT;
        break;
      }
    case 1:
      {
       index_dt = RANG_DT2_OUT;
        break;
      }
    case 2:
      {
       index_dt = RANG_DT3_OUT;
        break;
      }
    case 3:
      {
       index_dt = RANG_DT4_OUT;
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(56);
        break;
      }
    }
      
    if ((state_defined_triggers & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_dt);
    else _CLEAR_BIT(p_active_functions, index_dt);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "І"
/*****************************************************/
inline void d_and_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_and = 0;

  //Визначаємо стан всіх визначуваних "І" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND/*current_settings_prt.number_defined_and*/; i++)
  {
    if (
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 0] & p_active_functions[0]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 0]) && 
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 1] & p_active_functions[1]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 1]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 2] & p_active_functions[2]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 2]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 3] & p_active_functions[3]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 3]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 4] & p_active_functions[4]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 4]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 5] & p_active_functions[5]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 5]) &&
        ((current_settings_prt.ranguvannja_d_and[N_BIG*i + 6] & p_active_functions[6]) == current_settings_prt.ranguvannja_d_and[N_BIG*i + 6])
       )
    {
      state_defined_and |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "І" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"І", тому що інші В-"І"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"І", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_AND; i++)
  {
    //Установлюємо, або скидаємо В-"І"
    unsigned int index_d_and = RANG_D_AND1 + i;
      
    if ((state_defined_and & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_and);
    else _CLEAR_BIT(p_active_functions, index_d_and);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "АБО"
/*****************************************************/
inline void d_or_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_or = 0;

  //Визначаємо стан всіх визначуваних "АБО" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR/*current_settings_prt.number_defined_or*/; i++)
  {
    if (
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 0] & p_active_functions[0]) != 0) || 
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 1] & p_active_functions[1]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 2] & p_active_functions[2]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 3] & p_active_functions[3]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 4] & p_active_functions[4]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 5] & p_active_functions[5]) != 0) ||
        ((current_settings_prt.ranguvannja_d_or[N_BIG*i + 6] & p_active_functions[6]) != 0)
       )
    {
      state_defined_or |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "АБО" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"АБО", тому що інші В-"АБО"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"АБО", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_OR; i++)
  {
    //Установлюємо, або скидаємо В-"АБО"
    unsigned int index_d_or = RANG_D_OR1 + i;
      
    if ((state_defined_or & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_or);
    else _CLEAR_BIT(p_active_functions, index_d_or);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "Викл.АБО"
/*****************************************************/
inline void d_xor_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_xor = 0;

  //Визначаємо стан всіх визначуваних "Викл.АБО" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR/*current_settings_prt.number_defined_xor*/; i++)
  {
    unsigned int temp_array[N_BIG];
    temp_array[0] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 0] & p_active_functions[0];
    temp_array[1] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 1] & p_active_functions[1];
    temp_array[2] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 2] & p_active_functions[2];
    temp_array[3] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 3] & p_active_functions[3];
    temp_array[4] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 4] & p_active_functions[4];
    temp_array[5] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 5] & p_active_functions[5];
    temp_array[6] = current_settings_prt.ranguvannja_d_xor[N_BIG*i + 6] & p_active_functions[6];
    
    if (
        (temp_array[0] != 0) || 
        (temp_array[1] != 0) ||
        (temp_array[2] != 0) ||
        (temp_array[3] != 0) ||
        (temp_array[4] != 0) ||
        (temp_array[5] != 0) ||
        (temp_array[6] != 0)
       )
    {
      unsigned int signals = 0;
      for (unsigned int j = 0; j < N_BIG; j++)
      {
        for (unsigned int k = 0; k < 32; k++)
        {
          if ((temp_array[j] & (1 << k)) != 0) signals++;
        }
      }
      if (signals == 1) state_defined_xor |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "Викл.АБО" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"Викл.АБО", тому що інші В-"Викл.АБО"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"Викл.АБО", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_XOR; i++)
  {
    //Установлюємо, або скидаємо В-"Викл.АБО"
    unsigned int index_d_xor = RANG_D_XOR1 + i;
      
    if ((state_defined_xor & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_xor);
    else _CLEAR_BIT(p_active_functions, index_d_xor);
  }
}
/*****************************************************/

/*****************************************************/
//Опрацювання визначуваних "НЕ"
/*****************************************************/
inline void d_not_handler(volatile unsigned int *p_active_functions)
{
  unsigned int state_defined_not = 0;

  //Визначаємо стан всіх визначуваних "НЕ" (не виставляючи поки що їх у робочому масиві)
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT/*current_settings_prt.number_defined_not*/; i++)
  {
    if (
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 0] & p_active_functions[0]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 1] & p_active_functions[1]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 2] & p_active_functions[2]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 3] & p_active_functions[3]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 4] & p_active_functions[4]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 5] & p_active_functions[5]) == 0) &&
        ((current_settings_prt.ranguvannja_d_not[N_BIG*i + 6] & p_active_functions[6]) == 0)
       )
    {
      state_defined_not |= (1 << i);
    }
  }

  //Установлюємо, або скидаємо визначувані "НЕ" у масиві функцій, які зараз будуть активовуватися
  /*
  Цей цикл і попередній не об'єднаі в один, а навпаки розєднані, бо у першому ми використовуємо
  масив p_active_functions у якому ще не встановлені нові значення виходів В-"НЕ", тому що інші В-"НЕ"
  можуть бути джерелом , але джерелом може буте попереднє значення В-"НЕ", а не те,
  що зараз встановлюється. А оскілдьки ми встановлюємо значення у масиві
  p_active_functions, які набувають зараз тільки ваги, то щоб не вийшло об'єднання попереднього значення
  і теперішнього то цикли роз'єднані (цикл аналізу джерел і логіки з циклом встановлення/скидання)
  */
  for (unsigned int i = 0; i < NUMBER_DEFINED_NOT; i++)
  {
    //Установлюємо, або скидаємо В-"НЕ"
    unsigned int index_d_not = RANG_D_NOT1 + i;
      
    if ((state_defined_not & (1 << i)) != 0 ) _SET_BIT(p_active_functions, index_d_not);
    else _CLEAR_BIT(p_active_functions, index_d_not);
  }
}
/*****************************************************/

/*****************************************************/
// "Перевірка фазування"
/*****************************************************/
void ctrl_phase_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  static unsigned int TN1_bilshe_porogu, TN2_bilshe_porogu;
  unsigned int setpoint_TN1, setpoint_TN2;
    
  if (TN1_bilshe_porogu == 0) setpoint_TN1 = PORIG_FOR_FAPCH*U_DOWN/100;
  else setpoint_TN1 = PORIG_FOR_FAPCH;
  if (TN2_bilshe_porogu == 0) setpoint_TN2 = PORIG_FOR_FAPCH*U_DOWN/100;
  else setpoint_TN2 = PORIG_FOR_FAPCH;
  
  TN1_bilshe_porogu = (measurement[IM_UA1] >= setpoint_TN1) && (measurement[IM_UB1] >= setpoint_TN1) && (measurement[IM_UC1] >= setpoint_TN1);
  TN2_bilshe_porogu = (measurement[IM_UA2] >= setpoint_TN2) && (measurement[IM_UB2] >= setpoint_TN2) && (measurement[IM_UC2] >= setpoint_TN2);

  unsigned int logic_CTRL_PHASE_0 = 0;
  
  static unsigned int state_delta_U, state_delta_phi, state_delta_f;
  if ((TN1_bilshe_porogu != 0) && (TN2_bilshe_porogu != 0))
  {
    //Різниця напруг
    unsigned int setpoint_U;
    setpoint_U = (state_delta_U == 0) ? current_settings_prt.setpoint_ctrl_phase_U[number_group_stp] : current_settings_prt.setpoint_ctrl_phase_U[number_group_stp]*U_DOWN/100;
    unsigned int Ua1 = measurement[IM_UA1];
    unsigned int Ua2 = measurement[IM_UA2];
    unsigned int Ub1 = measurement[IM_UB1];
    unsigned int Ub2 = measurement[IM_UB2];
    unsigned int Uc1 = measurement[IM_UC1];
    unsigned int Uc2 = measurement[IM_UC2];
    state_delta_U = ((unsigned int)(abs(Ua1 - Ua2)) >= setpoint_U) && 
                    ((unsigned int)(abs(Ub1 - Ub2)) >= setpoint_U) && 
                    ((unsigned int)(abs(Uc1 - Uc2)) >= setpoint_U);
    
    //Різниця фаз
    unsigned int setpoint_phi;
    setpoint_phi = (state_delta_phi == 0) ? current_settings_prt.setpoint_ctrl_phase_phi[number_group_stp] : current_settings_prt.setpoint_ctrl_phase_phi[number_group_stp]*KOEF_POVERNENNJA_GENERAL/100;
    unsigned int bank_phi_angle_high_tmp = bank_phi_angle_high;
    int phi_Ua1 = phi_angle_high[bank_phi_angle_high_tmp][FULL_ORT_Ua1];
    int phi_Ua2 = phi_angle_high[bank_phi_angle_high_tmp][FULL_ORT_Ua2];
    int phi_Ub1 = phi_angle_high[bank_phi_angle_high_tmp][FULL_ORT_Ub1];
    int phi_Ub2 = phi_angle_high[bank_phi_angle_high_tmp][FULL_ORT_Ub2];
    int phi_Uc1 = phi_angle_high[bank_phi_angle_high_tmp][FULL_ORT_Uc1];
    int phi_Uc2 = phi_angle_high[bank_phi_angle_high_tmp][FULL_ORT_Uc2];
    state_delta_phi = ((phi_Ua1 >= 0) && (phi_Ua2 >= 0) && ((unsigned int)(abs(phi_Ua1 - phi_Ua2)) >= setpoint_phi)) && 
                      ((phi_Ub1 >= 0) && (phi_Ub2 >= 0) && ((unsigned int)(abs(phi_Ub1 - phi_Ub2)) >= setpoint_phi)) && 
                      ((phi_Uc1 >= 0) && (phi_Uc2 >= 0) && ((unsigned int)(abs(phi_Uc1 - phi_Uc2)) >= setpoint_phi));

    //Різниця частот
    unsigned int setpoint_f;
    setpoint_f = (state_delta_f == 0) ? current_settings_prt.setpoint_ctrl_phase_f[number_group_stp] : current_settings_prt.setpoint_ctrl_phase_f[number_group_stp]*KOEF_POVERNENNJA_GENERAL/100;
    int frequency_val_1_int = (int)frequency_val_1;
    int frequency_val_2_int = (int)frequency_val_2;
    state_delta_f = (frequency_val_1_int >= 0) && (frequency_val_2_int >= 0) && (((unsigned int)(abs(1000*frequency_val_1_int - 1000*frequency_val_2_int))) >= setpoint_f);

    logic_CTRL_PHASE_0 |= ((state_delta_U   != 0) && ((current_settings_prt.control_ctrl_phase & CTR_CTRL_PHASE_U  ) != 0)) << 0;
    logic_CTRL_PHASE_0 |= ((state_delta_phi != 0) && ((current_settings_prt.control_ctrl_phase & CTR_CTRL_PHASE_PHI) != 0)) << 1;
    logic_CTRL_PHASE_0 |= ((state_delta_f   != 0) && ((current_settings_prt.control_ctrl_phase & CTR_CTRL_PHASE_F  ) != 0)) << 2;
  }
  else
  {
    state_delta_U = 0;
    state_delta_phi = 0;
    state_delta_f = 0;
  }
  
  _TIMER_T_0(INDEX_TIMER_CTRL_PHASE_U  , current_settings_prt.timeout_ctrl_phase_U  [number_group_stp], logic_CTRL_PHASE_0, 0, logic_CTRL_PHASE_0, 3);
  _TIMER_0_T(INDEX_TIMER_CTRL_PHASE_U_D, current_settings_prt.timeout_ctrl_phase_U_d[number_group_stp], logic_CTRL_PHASE_0, 3, logic_CTRL_PHASE_0, 4);

  _TIMER_T_0(INDEX_TIMER_CTRL_PHASE_PHI  , current_settings_prt.timeout_ctrl_phase_phi  [number_group_stp], logic_CTRL_PHASE_0, 1, logic_CTRL_PHASE_0, 5);
  _TIMER_0_T(INDEX_TIMER_CTRL_PHASE_PHI_D, current_settings_prt.timeout_ctrl_phase_phi_d[number_group_stp], logic_CTRL_PHASE_0, 5, logic_CTRL_PHASE_0, 6);

  _TIMER_T_0(INDEX_TIMER_CTRL_PHASE_F  , current_settings_prt.timeout_ctrl_phase_f  [number_group_stp], logic_CTRL_PHASE_0, 2, logic_CTRL_PHASE_0, 7);
  _TIMER_0_T(INDEX_TIMER_CTRL_PHASE_F_D, current_settings_prt.timeout_ctrl_phase_f_d[number_group_stp], logic_CTRL_PHASE_0, 7, logic_CTRL_PHASE_0, 8);

  logic_CTRL_PHASE_0 |= ((sequence_TN1 == CONST_SEQ_FAIL) && ((current_settings_prt.control_ctrl_phase & CTR_CTRL_PHASE_SEQ_TN1) != 0) && (TN1_bilshe_porogu != 0)) << 9;
  logic_CTRL_PHASE_0 |= ((sequence_TN2 == CONST_SEQ_FAIL) && ((current_settings_prt.control_ctrl_phase & CTR_CTRL_PHASE_SEQ_TN2) != 0) && (TN2_bilshe_porogu != 0)) << 10;
  
  _TIMER_T_0(INDEX_TIMER_CTRL_PHASE_TMP1_100MS, 100, logic_CTRL_PHASE_0, 9 , logic_CTRL_PHASE_0, 11);
  _TIMER_T_0(INDEX_TIMER_CTRL_PHASE_TMP2_100MS, 100, logic_CTRL_PHASE_0, 10, logic_CTRL_PHASE_0, 12);
  
  if (_GET_OUTPUT_STATE(logic_CTRL_PHASE_0, 4)) _SET_BIT(p_active_functions, RANG_ERROR_DELTA_U_CTRL_PHASE);
  else  _CLEAR_BIT(p_active_functions, RANG_ERROR_DELTA_U_CTRL_PHASE);
  
  if (_GET_OUTPUT_STATE(logic_CTRL_PHASE_0, 6)) _SET_BIT(p_active_functions, RANG_ERROR_DELTA_PHI_CTRL_PHASE);
  else  _CLEAR_BIT(p_active_functions, RANG_ERROR_DELTA_PHI_CTRL_PHASE);
  
  if (_GET_OUTPUT_STATE(logic_CTRL_PHASE_0, 8)) _SET_BIT(p_active_functions, RANG_ERROR_DELTA_F_CTRL_PHASE);
  else  _CLEAR_BIT(p_active_functions, RANG_ERROR_DELTA_F_CTRL_PHASE);
  
  if (_GET_OUTPUT_STATE(logic_CTRL_PHASE_0, 11)) _SET_BIT(p_active_functions, RANG_ERROR_SEC_TN1_CTRL_PHASE);
  else  _CLEAR_BIT(p_active_functions, RANG_ERROR_SEC_TN1_CTRL_PHASE);
  
  if (_GET_OUTPUT_STATE(logic_CTRL_PHASE_0, 12)) _SET_BIT(p_active_functions, RANG_ERROR_SEC_TN2_CTRL_PHASE);
  else  _CLEAR_BIT(p_active_functions, RANG_ERROR_SEC_TN2_CTRL_PHASE);
}
/*****************************************************/

/*****************************************************/
//Перевірка на необхідність завершення роботи аналогового/дискретного реєстраторів
/*****************************************************/
inline unsigned int stop_regisrator(volatile unsigned int* carrent_active_functions, unsigned int* ranguvannja_registrator)
{
  unsigned int stop = 0;

  {
    if (
        ((carrent_active_functions[0] & ranguvannja_registrator[0]) == 0) &&
        ((carrent_active_functions[1] & ranguvannja_registrator[1]) == 0) &&
        ((carrent_active_functions[2] & ranguvannja_registrator[2]) == 0) &&
        ((carrent_active_functions[3] & ranguvannja_registrator[3]) == 0) &&
        ((carrent_active_functions[4] & ranguvannja_registrator[4]) == 0) &&
        ((carrent_active_functions[5] & ranguvannja_registrator[5]) == 0) &&
        ((carrent_active_functions[6] & ranguvannja_registrator[6]) == 0)
      )
    {
      //Зафіксовано, що ні одне джерело активації реєстратора зараз не активне
      
      if (
          ((carrent_active_functions[0] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_0) == 0) &&
          ((carrent_active_functions[1] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_1) == 0) &&
          ((carrent_active_functions[2] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_2) == 0) &&
          ((carrent_active_functions[3] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_3) == 0) &&
          ((carrent_active_functions[4] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_4) == 0) &&
          ((carrent_active_functions[5] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_5) == 0) &&
          ((carrent_active_functions[6] & MASKA_FOR_CONTINUE_GET_DR_ACTINE_WORD_6) == 0)
        )
      {
        //Зафіксовано, що всі функції, які можуть утримувати реєстратор активним зараз скинуті
          
        //Перевіряємо, чи всі таймери, які працють у логіці схеми виключені
        unsigned int global_timers_work = 0, i = INDEX_TIMER_DF_PAUSE_START;
        while ((i < NEXT_TIMER) && (global_timers_work == 0))
        {
          if (global_timers[i] >= 0) 
          {
            if (
                (
                 (i != INDEX_TIMER_PRYVOD_VV) ||
                 (  
                  ((current_settings_prt.control_switch & CTR_PRYVOD_VV) != 0) &&
                  (global_timers[i] < current_settings_prt.timeout_pryvoda_VV) 
                 )   
                ) 
               )
            global_timers_work = 1;
          }
          i++;
        }
          
        if (global_timers_work == 0)
        {
          //Зафіксовано, що всі таймери, які працюють у лозіці неактивні
        
          //Помічаємо, що реєстратор може бути зупиненим
          stop = 0xff;
        }
      }
    }
  }
  
  return stop;
}
/*****************************************************/

/*****************************************************/
//Зафіксована невизначена помилка роботи дискретного реєстратора
/*****************************************************/
void fix_undefined_error_dr(volatile unsigned int* carrent_active_functions)
{
  //Виставляємо помилку з записом в дисретний реєстратор
  _SET_BIT(set_diagnostyka, ERROR_DR_UNDEFINED_BIT);
  _SET_BIT(carrent_active_functions, RANG_DEFECT);
  //Переводимо режим роботи з реєстратором у сатн "На даний момент ніких дій з дискретним реєстратором не виконується" 
  state_dr_record = STATE_DR_NO_RECORD;
  //Скидаєсо сигнал роботи дискретного реєстратора
  _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
}
/*****************************************************/

/*****************************************************/
//Подача команди почати запис у DataFalsh
/*****************************************************/
inline void command_start_saving_record_dr_into_dataflash(void)
{
  //У структурі по інформації стану реєстраторів виставляємо повідомлення, що почався запис і ще не закінчився
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);
  info_rejestrator_dr.saving_execution = 1;
        
  //Виставляємо першу частину запису
  part_writing_dr_into_dataflash = 0;
  //Виставляємо команду запису у мікросхему DataFlash
  control_tasks_dataflash |= TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR;
}
/*****************************************************/

/*****************************************************/
//Функція обробки черги зформованих записів дискретного реєстратора
/*****************************************************/
inline void routine_for_queue_dr(void)
{
  if(
     (state_dr_record == STATE_DR_MAKE_RECORD)            || /*Стоїть умова сформовані дані передати на запис у DataFlsh без повторного початку  нового запису, бо умова завершення запиу    досягнкта, а не стояла умова примусового завершення даного сформованого запису*/
     (state_dr_record == STATE_DR_CUT_RECORD)             || /*Стоїть умова сформовані дані передати на запис у DataFlsh   з повторним  початком нового запису, бо умова завершення запиу не досягнкта, а    стояла умова примусового завершення даного сформованого запису*/
     (number_records_dr_waiting_for_saving_operation != 0)  
    )
  {
    if ((control_tasks_dataflash & TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR) == 0)
    {
      //На даний момент запису у Datafalsh не проводиться
      unsigned char *buffer_source, *buffer_target;
        
      if (number_records_dr_waiting_for_saving_operation == 0)
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++)
        {
          *(buffer_target + i) = *(buffer_source + i);
          *(buffer_source + i) = 0xff;
        }
        number_records_dr_waiting_for_saving_operation = 0;
      }
      else if (number_records_dr_waiting_for_saving_operation == 1)
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record_level_1;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) *(buffer_target +i ) = *(buffer_source + i);
        
        if((state_dr_record == STATE_DR_MAKE_RECORD) || (state_dr_record == STATE_DR_CUT_RECORD))
        {
          buffer_target = buffer_for_save_dr_record_level_1;
          buffer_source = buffer_for_save_dr_record;
          for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
          {
            *(buffer_target + i) = *(buffer_source + i);
            *(buffer_source + i) = 0xff;
          }
          number_records_dr_waiting_for_saving_operation = 1;
        }
        else number_records_dr_waiting_for_saving_operation = 0;
      }
      else
      {
        buffer_target = buffer_for_save_dr_record_level_2;
        buffer_source = buffer_for_save_dr_record_level_1;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) *(buffer_target + i) = *(buffer_source + i);
        buffer_target = buffer_for_save_dr_record_level_1;
        buffer_source = buffer_for_save_dr_record;
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
        {
          *(buffer_target + i) = *(buffer_source + i);
          *(buffer_source + i) = 0xff;
        }
        number_records_dr_waiting_for_saving_operation = 1;
      }
        
      //Подаємо команду почати запис у DataFalsh
      command_start_saving_record_dr_into_dataflash();
    }
    else
    {
      //На даний момент запису у Datafalsh проводиться
      //Треба наш теперішній запис поставити у чергу
      if (number_records_dr_waiting_for_saving_operation == 0)
      {
        //На даний момент крім того, буфера - з якого іде безпосередній запис у DataFalsh, більше ніяких записів у черзі немає
        //Переміщаємо текучий запис у буфер buffer_for_save_dr_record_level_1
        for(unsigned int i = 0; i < SIZE_BUFFER_FOR_DR_RECORD; i++) 
        {
          buffer_for_save_dr_record_level_1[i] = buffer_for_save_dr_record[i];
          buffer_for_save_dr_record[i] = 0xff;
        }
    
        //Помічаємо, що один запис очікує передау його на безпосередній запис у dataFalsh
        number_records_dr_waiting_for_saving_operation = 1;
      }
      else if (number_records_dr_waiting_for_saving_operation >= 1)
      {
         if((state_dr_record == STATE_DR_MAKE_RECORD) || (state_dr_record == STATE_DR_CUT_RECORD))
         {
          //На даний момент крім того, буфера - з якого іде безпосередній запис у DataFalsh, ще один сформований запис стоїть у буфері buffer_for_save_dr_record_level_1
          /*
          Тому даний запис залишаємо у цьому самому буфері buffer_for_save_dr_record, але збільшуємо кількість записів. які стоять у черзі
          і якщо буде спроба почати новий запис, то будемо фіксувати помилку втрати даних
          */
          number_records_dr_waiting_for_saving_operation = 2;
         }
      }
    }
    
    if (state_dr_record == STATE_DR_MAKE_RECORD)
    {
      //Помічаємо, що ми готові приймати наступний запис
      state_dr_record = STATE_DR_NO_RECORD;
    }
    else if (state_dr_record == STATE_DR_CUT_RECORD)
    {
      //Помічаємо, що треба примусово запустити новий запис
      state_dr_record = STATE_DR_FORCE_START_NEW_RECORD;
    }
      
  }
}
/*****************************************************/

/*****************************************************/
//Функція обробки логіки дискретного реєстратора
/*****************************************************/
inline void digital_registrator(volatile unsigned int* carrent_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];
  
  static unsigned int number_items_dr;
  static unsigned int number_changes_into_dr_record;
  static unsigned int time_from_start_record_dr;
  static unsigned int blocking_continue_monitoring_min_U;
  
  switch (state_dr_record)
  {
  case STATE_DR_NO_RECORD:
  case STATE_DR_FORCE_START_NEW_RECORD:
    {
      //Попередньо скидаємо невизначену помилку  роботи дискретного реєстратора
      _SET_BIT(clear_diagnostyka, ERROR_DR_UNDEFINED_BIT);
      if(number_records_dr_waiting_for_saving_operation < (WIGHT_OF_DR_WAITING - 1))
      {
        //Ця ситуація означає, що як мінімум на один новий запис у нас є вільне місц, тому скидаємо сигналізацію про втрату даних
        _SET_BIT(clear_diagnostyka, ERROR_DR_TEMPORARY_BUSY_BIT);
      }
      
      //На початок аналізу покищо ще дискретний реєстратор не запущений
      
      //Аналізуємо, чи стоїть умова запуску дискретного реєстратора
      if (
          (
           ((carrent_active_functions[0] & current_settings_prt.ranguvannja_digital_registrator[0]) != 0) ||
           ((carrent_active_functions[1] & current_settings_prt.ranguvannja_digital_registrator[1]) != 0) ||
           ((carrent_active_functions[2] & current_settings_prt.ranguvannja_digital_registrator[2]) != 0) ||
           ((carrent_active_functions[3] & current_settings_prt.ranguvannja_digital_registrator[3]) != 0) ||
           ((carrent_active_functions[4] & current_settings_prt.ranguvannja_digital_registrator[4]) != 0) ||
           ((carrent_active_functions[5] & current_settings_prt.ranguvannja_digital_registrator[5]) != 0) ||
           ((carrent_active_functions[6] & current_settings_prt.ranguvannja_digital_registrator[6]) != 0) ||
           (state_dr_record == STATE_DR_FORCE_START_NEW_RECORD)
          )   
         )
      {
        //Є умова запуску дискретного реєстратора
        
        //Перевіряємо, чи при початку нового запису ми не втратимо попередню інформацію
        if(number_records_dr_waiting_for_saving_operation < WIGHT_OF_DR_WAITING)
        {
          //Можна починати новий запис
          
          //Переводимо режим роботи із дискретним реєстратором у стан "Іде процес запису реєстратора"
          state_dr_record = STATE_DR_EXECUTING_RECORD;
          //Виставляємо активну функцію
          _SET_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
        
          //Записуємо мітку початку запису
          buffer_for_save_dr_record[FIRST_INDEX_START_START_RECORD_DR] = LABEL_START_RECORD_DR;
         
          //Записуємо час початку запису
          unsigned char *label_to_time_array;
          if (copying_time == 0) label_to_time_array = time;
          else label_to_time_array = time_copy;
          for(unsigned int i = 0; i < 7; i++) buffer_for_save_dr_record[FIRST_INDEX_DATA_TIME_DR + i] = *(label_to_time_array + i);
          
           //І'мя комірки
          for(unsigned int i=0; i< MAX_CHAR_IN_NAME_OF_CELL; i++) 
            buffer_for_save_dr_record[FIRST_INDEX_NAME_OF_CELL_DR + i] = current_settings_prt.name_of_cell[i] & 0xff;
          
          //Помічаємо скільки часу пройшло з початку запуску запису
          time_from_start_record_dr = 0;
          
          //Скидаємо кількість фіксацій максимальних струмів/напруг
          number_max_phase_dr = 0;
          number_min_U_dr = 0;
          number_max_U_dr = 0;
          number_max_ZOP_dr = 0;
          
      
          //Записуємо попередній cтан сигналів перед аварією
          //Мітка часу попереднього стану сигналів до моменту початку запису
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  0] = 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  1] = 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  2] = 0xff;
          //Попередній стан
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  3] =  previous_active_functions[0]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  4] = (previous_active_functions[0] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  5] = (previous_active_functions[0] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  6] = (previous_active_functions[0] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  7] =  previous_active_functions[1]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  8] = (previous_active_functions[1] >> 8)  & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR +  9] = (previous_active_functions[1] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 10] = (previous_active_functions[1] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 11] =  previous_active_functions[2]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 12] = (previous_active_functions[2] >> 8)  & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 13] = (previous_active_functions[2] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 14] = (previous_active_functions[2] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 15] =  previous_active_functions[3]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 16] = (previous_active_functions[3] >> 8)  & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 17] = (previous_active_functions[3] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 18] = (previous_active_functions[3] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 19] =  previous_active_functions[4]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 20] = (previous_active_functions[4] >> 8)  & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 21] = (previous_active_functions[4] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 22] = (previous_active_functions[4] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 23] =  previous_active_functions[5]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 24] = (previous_active_functions[5] >> 8)  & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 25] = (previous_active_functions[5] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 26] = (previous_active_functions[5] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 27] =  previous_active_functions[6]        & 0xff;
          //Нулем позначаємо у цій позиції кількість змін
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + 28] = 0;

          //Помічаємо кількість нових зрізів
          number_items_dr = 1;
      
          //Вираховуємо кількість змін сигналів
          number_changes_into_dr_record = 0;
          unsigned int number_changes_into_current_item;
          _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, carrent_active_functions, N_BIG, number_changes_into_current_item);
          number_changes_into_dr_record += number_changes_into_current_item;
      
          //Записуємо текучий cтан сигналів
          //Мітка часу
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  0] =  time_from_start_record_dr        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  2] = (time_from_start_record_dr >> 16) & 0xff;
          //Текучий стан сигналів
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  3] =  carrent_active_functions[0]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  4] = (carrent_active_functions[0] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  5] = (carrent_active_functions[0] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  6] = (carrent_active_functions[0] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  7] =  carrent_active_functions[1]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  8] = (carrent_active_functions[1] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  9] = (carrent_active_functions[1] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 10] = (carrent_active_functions[1] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 11] =  carrent_active_functions[2]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 12] = (carrent_active_functions[2] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 13] = (carrent_active_functions[2] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 14] = (carrent_active_functions[2] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 15] =  carrent_active_functions[3]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 16] = (carrent_active_functions[3] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 17] = (carrent_active_functions[3] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 18] = (carrent_active_functions[3] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 19] =  carrent_active_functions[4]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 20] = (carrent_active_functions[4] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 21] = (carrent_active_functions[4] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 22] = (carrent_active_functions[4] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 23] =  carrent_active_functions[5]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 24] = (carrent_active_functions[5] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 25] = (carrent_active_functions[5] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 26] = (carrent_active_functions[5] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 27] =  carrent_active_functions[6]        & 0xff;
          
          //Кількість змін сигналів у порівнянні із попереднім станом
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 28] = number_changes_into_current_item & 0xff;
        }
        else
        {
          //Виставляємо помилку, що є умова на роботу дискретного реєстратора тоді, як всі вільні буфери зайняті
          _SET_BIT(set_diagnostyka, ERROR_DR_TEMPORARY_BUSY_BIT);
          _SET_BIT(carrent_active_functions, RANG_DEFECT);
        }
      }
      else state_dr_record = STATE_DR_NO_RECORD;
      
      break;
    }
  case STATE_DR_EXECUTING_RECORD:
    {
      //Збільшуємо час з початку запуску запису
      time_from_start_record_dr++;
      //Включно до цього часу іде процес запису

      //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
      if(state_dr_record != STATE_DR_NO_RECORD)
      {
        //Перевіряємо, чи ще існує умова продовження запису
        //Якщо такої умови немає - то скидаємо сигнал запущеного дискретного реєстратора, що це зафіксувати у змінених сигналах
        if (stop_regisrator(carrent_active_functions, current_settings_prt.ranguvannja_digital_registrator) != 0)
        {
          //Скидаємо сигнал роботи дискретного реєстратора
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);

          //Переводимо режим роботи із дискретним реєстратором у стан "Виконується безпосередній запис у послідовну DataFlash"
          state_dr_record = STATE_DR_MAKE_RECORD;
        }
      
        //Перевіряємо чи відбуласа зміна сигналів у порівнянні із попереднім станом. Якщо така зміна є, то формуєм новий зріз сигналів у записі
        if (
            ((carrent_active_functions[0] != previous_active_functions[0])) ||
            ((carrent_active_functions[1] != previous_active_functions[1])) ||
            ((carrent_active_functions[2] != previous_active_functions[2])) ||
            ((carrent_active_functions[3] != previous_active_functions[3])) ||
            ((carrent_active_functions[4] != previous_active_functions[4])) ||
            ((carrent_active_functions[5] != previous_active_functions[5])) ||
            ((carrent_active_functions[6] != previous_active_functions[6]))
           )
        {
          //Теперішній стан сигналів не співпадає з попереднім станом сигналів

          //Збільшуємо на один кількість нових зрізів
          number_items_dr++;
      
          //Вираховуємо кількість змін сигналів
          unsigned int number_changes_into_current_item;
          _NUMBER_CHANGES_INTO_UNSIGNED_INT_ARRAY(previous_active_functions, carrent_active_functions, N_BIG, number_changes_into_current_item);
          number_changes_into_dr_record += number_changes_into_current_item;
      
          //Записуємо текучий cтан сигналів
          //Мітка часу
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  0] =  time_from_start_record_dr        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  1] = (time_from_start_record_dr >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  2] = (time_from_start_record_dr >> 16) & 0xff;
          //Текучий стан сигналів
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  3] =  carrent_active_functions[0]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  4] = (carrent_active_functions[0] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  5] = (carrent_active_functions[0] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  6] = (carrent_active_functions[0] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  7] =  carrent_active_functions[1]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  8] = (carrent_active_functions[1] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 +  9] = (carrent_active_functions[1] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 10] = (carrent_active_functions[1] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 11] =  carrent_active_functions[2]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 12] = (carrent_active_functions[2] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 13] = (carrent_active_functions[2] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 14] = (carrent_active_functions[2] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 15] =  carrent_active_functions[3]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 16] = (carrent_active_functions[3] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 17] = (carrent_active_functions[3] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 18] = (carrent_active_functions[3] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 19] =  carrent_active_functions[4]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 20] = (carrent_active_functions[4] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 21] = (carrent_active_functions[4] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 22] = (carrent_active_functions[4] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 23] =  carrent_active_functions[5]        & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 24] = (carrent_active_functions[5] >> 8 ) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 25] = (carrent_active_functions[5] >> 16) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 26] = (carrent_active_functions[5] >> 24) & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 27] =  carrent_active_functions[6]        & 0xff;

          //Кількість змін сигналів у порівнянні із попереднім станом
          buffer_for_save_dr_record[FIRST_INDEX_FIRST_DATA_DR + number_items_dr*29 + 28] = number_changes_into_current_item & 0xff;
        }
        
        //Перевіряємо, чи стоїть умова завершення запису
        if (
            (state_dr_record == STATE_DR_MAKE_RECORD)                  ||
            (time_from_start_record_dr >= MAX_TIME_OFFSET_FROM_START)  ||  
            ((number_items_dr + 1)     >= MAX_EVENTS_IN_ONE_RECORD  )  
           )
        {
          //Немає умови продовження запису, або є умова завершення запису - завершуємо формування запису і подаємо команду на запис
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR      ] = number_items_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR    ] =  number_changes_into_dr_record       & 0xff;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

          //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
          if(state_dr_record != STATE_DR_NO_RECORD)
          {
            //Записуємо кількість зафіксованих максимальних вимірювань всіх типів
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR  ] = number_max_phase_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR      ] = number_min_U_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR      ] = number_max_U_dr;
            buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR    ] = number_max_ZOP_dr;

            //Переводимо режим роботи із дискретним реєстратором у стан "Виконується безпосередній запис у DataFlash"
            if (state_dr_record != STATE_DR_MAKE_RECORD)
            {
              if (time_from_start_record_dr >= MAX_TIME_OFFSET_FROM_START)
              {
                //Якщо відбулося перевищення по часу запису, то подаємо команду завершити запис без продовження потім цього запису у наступному записі
                state_dr_record = STATE_DR_MAKE_RECORD;
              }
              else
              {
                //Якщо відбулося перевищення по досягнкнні максимальної кількості зрізів (або іншої причини, яка покищо не оговорена, але може з'явитися у майбутньому), то подаємо команду завершити запис але на наступному проході почати новий запис
                state_dr_record = STATE_DR_CUT_RECORD;
              }
            }
          }
        
          //Скидаємо сигнал роботи дискретного реєстратора
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
        }
      }

      break;
    }
  default:
    {
      //По ідеї сюди програма ніколи не мала б зайти
      fix_undefined_error_dr(carrent_active_functions);
      break;
    }
  }
  
  //Перевіряємо, чи стоїть умова сформований запис передати на запис у DataFlash
  routine_for_queue_dr();

  /*********************/
  //Формуємо попереденій стан сигналів для функції ввімкнення/вимкнення
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = carrent_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//Зафіксована невизначена помилка роботи аналогового реєстратора
/*****************************************************/
void fix_undefined_error_ar(volatile unsigned int* carrent_active_functions)
{
  //Виставляємо помилку з записом в дисретний реєстратор
  _SET_BIT(set_diagnostyka, ERROR_AR_UNDEFINED_BIT);
  _SET_BIT(carrent_active_functions, RANG_DEFECT);
  //Переводимо режим роботи з реєстратором у сатн "На даний момент ніких дій з дискретним реєстратором не виконується" 
  continue_previous_record_ar = 0; /*помічаємо, що ми не чикаємо деактивації всіх джерел активації аналогового реєстратора*/
  state_ar_record = STATE_AR_NO_RECORD;
  //Скидаєсо сигнал роботи аналогового реєстратора
  _CLEAR_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);

  //Виставляємо команду запису структуру для аналогового реєстратора у EEPROM
  _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
  //Відновлюємо інформаційну структуру для аналогового реєстратора
  /*
  Адресу залишаємо попередню, тобто така яка і була
  */
  info_rejestrator_ar.saving_execution = 0;
  /*
  оскільки скоріше всього якась частна запису відбулася, а це значить, що, якщо там були корисні дані
  якогось запису, то вони зіпсовані. Тому треба помітити, що якщо у аналоговому реєстраторі до цього часу була
  максимально можлива кількість записів, то зараз оснанній з них є зіпсований, тобто кількість записів стала 
  у такому випадку на одиницю менша
  */
  unsigned int max_number_records_ar_tmp = max_number_records_ar;
  if (info_rejestrator_ar.number_records >= max_number_records_ar_tmp) 
    info_rejestrator_ar.number_records = max_number_records_ar_tmp - 1; /*Умова мал аб бути ==, але щоб перестахуватися на невизначену помилку я поставив >=*/
}
/*****************************************************/

/*****************************************************/
//Функція обробки логіки дискретного реєстратора
/*****************************************************/
inline void analog_registrator(volatile unsigned int* carrent_active_functions)
{
  static unsigned int unsaved_bytes_of_header_ar;

  //Попередньо скидаємо невизначену помилку  роботи аналогового реєстратора
  _SET_BIT(clear_diagnostyka, ERROR_AR_UNDEFINED_BIT);

  if (continue_previous_record_ar != 0)
  {
    /*
    Ця ситуація означає, що були активними джерела аналогового реєстратора, які запустили
    в роботу аналоговий реєстратор, і тепер для розблокування можливості запускати новий запис ми 
    чекаємо ситуації, що всі джерела активації деактивуються (у будь-який час чи до
    завершення записування текучого запису аналогового реєстратора, чи вже після завершення
    записування. Це буде умовою розблокування можливості запису нового запису)
    */
    if(
       ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) == 0) &&
       ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) == 0) &&
       ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) == 0) &&
       ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) == 0) &&
       ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) == 0) &&
       ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) == 0) &&
       ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) == 0)
      ) 
    {
      //Умова розблокування можливості початку нового запису виконана
      continue_previous_record_ar = 0;
    }
  }

  switch (state_ar_record)
  {
  case STATE_AR_NO_RECORD:
    {
      if(semaphore_read_state_ar_record == 0)
      {
        /*
        Можливо була ситуація, що при попередній роботі модуля аналогового реєстратора відбувалося блокування роботи аналоговго реєстратора
        Знятий семафор semaphore_read_state_ar_record при умові, що стан роботи аналогового реєстратора STATE_AR_NO_RECORD означає,
        що зараз немає перешкод на його запуск, тому знімаємо теоретично можливу подію про тимчасову неготовність роботи аналогового реєстратора
        */
        _SET_BIT(clear_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
      }

      //Аналізуємо, чи стоїть умова запуску аналогового реєстратора
      if (
          (
           ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) != 0) ||
           ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) != 0) ||
           ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) != 0) ||
           ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) != 0) ||
           ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) != 0) ||
           ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) != 0) ||
           ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) != 0)
          )
          &&  
          (continue_previous_record_ar == 0) /*при попередній роботі ан.реєстротора (якщо така була) вже всі джерела активації були зняті і зароз вони знову виникли*/ 
         )
      {
        //Перевіряємо, чи при початку нового запису у нас не іде спроба переналаштвати аналоговий реєстратор
        if(semaphore_read_state_ar_record == 0)
        {
          //Є умова запуску аналогового реєстратора
          continue_previous_record_ar = 0xff; /*помічаємо будь-яким числом, що активувалися дзжерела ан.реєстратора, які запустили роботц аналогового реєстратора*/
    
          //Можна починати новий запис
          
          //Записуємо мітку початку запису
          header_ar.label_start_record = LABEL_START_RECORD_AR;
          //Записуємо час початку запису
          unsigned char *label_to_time_array;
          if (copying_time == 0) label_to_time_array = time;
          else label_to_time_array = time_copy;
          for(unsigned int i = 0; i < 7; i++) header_ar.time[i] = *(label_to_time_array + i);
          //Коефіцієнт трансформації TT
          header_ar.TCurrent = current_settings_prt.TCurrent;
          //Коефіцієнт трансформації TН
          header_ar.TVoltage = current_settings_prt.TVoltage;
          //І'мя ячейки
          for(unsigned int i=0; i<MAX_CHAR_IN_NAME_OF_CELL; i++)
            header_ar.name_of_cell[i] = current_settings_prt.name_of_cell[i] & 0xff;
          
          //Помічаємо, що ще ми ще не "відбирали" миттєві значення з масив для аналогового реєстратора
          copied_number_samples = 0;
          //Визначаємо загальну кількість миттєвих значень, які мають бути записані у мікросхему dataFlash2
          total_number_samples = ((current_settings_prt.prefault_number_periods + current_settings_prt.postfault_number_periods) << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar);

          //Визначаємо,що покищо заготовок аналогового реєстратора не скопійоманий у масив звідки будуть дані забирватися вже для запису у DataFlash
          unsaved_bytes_of_header_ar = sizeof(__HEADER_AR);

          //Визначаємо з якої адреси записувати
          temporary_address_ar = info_rejestrator_ar.next_address;

          //Визначаєом, що поки що немає підготовлених даних для запису
          count_to_save = 0;
          //Виставляємо будь-яким ненульовим числом дозвіл на підготовку нових даних для запису
          permit_copy_new_data = 0xff;

          //Робим спробу перекопіювати хоч частину заголовку аналогового реєстраторра і підготовлених даних у масив для запису в DataFlash
          if (making_buffer_for_save_ar_record(&unsaved_bytes_of_header_ar) != 0)
          {
            //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
            fix_undefined_error_ar(carrent_active_functions);
          }
          else
          {
            //Переводимо режим роботи із аналоговим реєстратором у стан "Запус нового запису"
            state_ar_record = STATE_AR_START;
            //Виставляємо активну функцію
            _SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);

            //У структурі по інформації стану реєстраторів виставляємо повідомлення, що почався запис і ще не закінчився
            _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
            info_rejestrator_ar.saving_execution = 1;
          }
        }
        else
        {
          //Виставляємо помилку, що тимчасово аналоговий реєстратор є занятий (черз те, що іде намаганні змінити часові витримки)
          _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
          _SET_BIT(carrent_active_functions, RANG_DEFECT);
        }
      }
      break;
    }
  case STATE_AR_START:
    {
      //Ніяких дій не виконуємо, поки не встанвиться режим STATE_AR_SAVE_SRAM_AND_SAVE_FLASH,  або STATE_AR_ONLY_SAVE_FLASH
      break;
    }
  case STATE_AR_SAVE_SRAM_AND_SAVE_FLASH:
  case STATE_AR_ONLY_SAVE_FLASH:
    {
      if (state_ar_record == STATE_AR_ONLY_SAVE_FLASH)
      {
        /*
        Весь післяаварійний масив підготовлений до запису
        */
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR) != 0)
        {
          //Знімаємо сигнал роботи аналогового реєстратора
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR);
        }
        
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_A_REJESTRATOR) == 0)
        {
          /*
          Враховуємо також той момент, коли сигнал запуску роботи аналогового реєсстратора був знятий
          */
          if  (continue_previous_record_ar == 0)
          {
            /*
            Перевіряємо, чи немає умови запуску нового заппису до моменту, 
            поки ще старий запис не закінчився повністю
            */
            if (
                ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) != 0) ||
                ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) != 0) ||
                ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) != 0) ||
                ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) != 0) ||
                ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) != 0) ||
                ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) != 0) ||
                ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) != 0)
               ) 
            {
              //Виставляємо помилку, що тимчасово аналоговий реєстратор є занятий (черз те, що завершується попередній запис)
              _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
              _SET_BIT(carrent_active_functions, RANG_DEFECT);
            }
          }
        }
      }
      
      if (permit_copy_new_data != 0)
      {
        /*
        Робим спробу перекопіювати хоч частину заголовку аналогового реєстраторра 
        і підготовлених даних у масив для запису в DataFlash тільки тоді, коли є дозвіл
        на цю операцію
        */
        if (making_buffer_for_save_ar_record(&unsaved_bytes_of_header_ar) != 0)
        {
          //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
          fix_undefined_error_ar(carrent_active_functions);
        }
      }
      else
      {
        if (
            (copied_number_samples == total_number_samples) &&
            (count_to_save == 0                           ) && 
            (
             (control_tasks_dataflash &
              (
               TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR | 
               TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR
              )
             ) == 0
            )   
           )
        {
          //Умова зупинки роботи анаалогового реєстратора
          unsigned int index_array_ar_heat_tmp = index_array_ar_heat;/*це робим для того, бо компілятор видає завуваження при порівнянні змінних з префіксом volatile*/
          unsigned int index_array_ar_tail_tmp = index_array_ar_tail;/*це робим для того, бо компілятор видає завуваження при порівнянні змінних з префіксом volatile*/
          if(
             (index_array_ar_tail_tmp == index_array_ar_heat_tmp) &&
             (state_ar_record == STATE_AR_ONLY_SAVE_FLASH)  
            )  
          {
            //Коректна умова зупинки роботи аналогового реєстратора
            state_ar_record = STATE_AR_NO_RECORD;

            //Виставляємо команду запису структури аналогового реєстратора у EEPROM
            _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
            //Визначаємо нову адресу наступного запису, нову кількість записів і знімаємо сигналізацію, що зараз іде запис
            if ((temporary_address_ar + size_one_ar_record) > (NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2))
              temporary_address_ar = 0; 
            info_rejestrator_ar.next_address = temporary_address_ar;
            info_rejestrator_ar.saving_execution = 0;
            unsigned int max_number_records_ar_tmp = max_number_records_ar;
            if (info_rejestrator_ar.number_records < max_number_records_ar_tmp) info_rejestrator_ar.number_records += 1;
            else info_rejestrator_ar.number_records = max_number_records_ar_tmp;
          }
          else
          {
            /*В процесі роботи аналогового реєстратора відбувся збій, який привів
            до непередбачуваного завершення роботи аналогового реєстратора
            
            Це скоріше всього виникло внаслідок того, що ми досягнули передчасно
            максимальної кількості зкопійованих миттєвих значень
            */
            fix_undefined_error_ar(carrent_active_functions);
          }
        }
        else
        {
          //Треба подати команду на запис підготовлених даних
          if (
              (count_to_save != 0 ) 
              && 
              (
               (control_tasks_dataflash &
                (
                 TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR | 
                 TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR
                )
               ) == 0
              )   
             )
          {
            /*
            Подаємо команду на запис нових даних тільки тоді коли не іде зараз запис
            попередньо підготованих даних і коли є нові дані для запису
            */
            
            if (((temporary_address_ar & 0x1ff) + count_to_save) <= SIZE_PAGE_DATAFLASH_2)
            {
              //Немає помилки при фомауванні кількості байт для запису (в одну сторінку дані поміщаються з текучої адреси)
              
              if (count_to_save == SIZE_PAGE_DATAFLASH_2) control_tasks_dataflash |= TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR;
              else control_tasks_dataflash |= TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR;
            }
            else
            {
              //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
              fix_undefined_error_ar(carrent_active_functions);
            }
          }
        }

      }
      break;
    }
  case STATE_AR_TEMPORARY_BLOCK:
    {
      //На даний момент певні внутрішні операції блокують роботу аналогового реєстратрора
      //Аналізуємо, чи стоїть умова запуску аналогового реєстратора
      if (
          ((carrent_active_functions[0] & current_settings_prt.ranguvannja_analog_registrator[0]) != 0) ||
          ((carrent_active_functions[1] & current_settings_prt.ranguvannja_analog_registrator[1]) != 0) ||
          ((carrent_active_functions[2] & current_settings_prt.ranguvannja_analog_registrator[2]) != 0) ||
          ((carrent_active_functions[3] & current_settings_prt.ranguvannja_analog_registrator[3]) != 0) ||
          ((carrent_active_functions[4] & current_settings_prt.ranguvannja_analog_registrator[4]) != 0) ||
          ((carrent_active_functions[5] & current_settings_prt.ranguvannja_analog_registrator[5]) != 0) ||
          ((carrent_active_functions[6] & current_settings_prt.ranguvannja_analog_registrator[6]) != 0)
         )
      {
        //Виставляємо помилку, що тимчасово аналоговий реєстратор є занятий
        _SET_BIT(set_diagnostyka, ERROR_AR_TEMPORARY_BUSY_BIT);
        _SET_BIT(carrent_active_functions, RANG_DEFECT);
      }
      break;
    }
  default:
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_ar(carrent_active_functions);
      break;
    }
  }
}
/*****************************************************/


/*****************************************************/
//Функція захистів з якої здійснюються всі інші операції
/*****************************************************/
inline void main_protection(void)
{
  copying_active_functions = 1; //Помічаємо, що зараз обновляємо значення активних функцій
  
  //Скижаємо ті сигнали, які відповідають за входи, фкнопки і активацію з інтерфейсу
  const unsigned int maska_input_signals[N_BIG] = 
  {
    MASKA_FOR_INPUT_SIGNALS_0, 
    MASKA_FOR_INPUT_SIGNALS_1, 
    MASKA_FOR_INPUT_SIGNALS_2, 
    MASKA_FOR_INPUT_SIGNALS_3, 
    MASKA_FOR_INPUT_SIGNALS_4, 
    MASKA_FOR_INPUT_SIGNALS_5, 
    MASKA_FOR_INPUT_SIGNALS_6 
  };
  for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_input_signals[i]);
  
  /**************************/
  //Перевірка, чи треба очистити трігерні функції
  /**************************/
  if (reset_trigger_function_from_interface !=0)
  {
    if ((reset_trigger_function_from_interface & (1 << USB_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_USB[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << USB_RECUEST));
    }
    if ((reset_trigger_function_from_interface & (1 << RS485_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_RS485[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << RS485_RECUEST));
    }
    
    //Помічаємо що ми виконали очистку по ВСІХ інтерфейсах
    reset_trigger_function_from_interface = 0;
  }
  /**************************/

  /**************************/
  //Опрацьовуємо натиснуті кнопки - Мають опрацьовуватися найпершими, бо тут іде аналіз чи активовується опреділювані функції з кнопок
  /**************************/
  if ((pressed_buttons !=0) || (activation_function_from_interface != 0))
  {
    unsigned int temp_value_for_activated_function = 0;
    
    //Активація з кнопуки
    if (pressed_buttons != 0)
    {
      for (unsigned int i = 0; i < NUMBER_DEFINED_BUTTONS; i++)
      {
        if ((pressed_buttons & (1 << i)) != 0)
          temp_value_for_activated_function |= current_settings_prt.ranguvannja_buttons[i];
      }

      //Очищаємо натиснуті кнопка, які ми вже опрацювали
      pressed_buttons =0;
    }
    
    //Активація з інтерфейсу
    if (activation_function_from_interface != 0)
    {
      temp_value_for_activated_function |= activation_function_from_interface;
      
      //Очищаємо помітку активації функцій з інетфейсу, які ми вже опрацювали
      activation_function_from_interface = 0;
    }
    
    //Якщо  функції активовувалися через кнопки то переносимо їх у тимчасовий масив функцій, які мають бути зараз активавані
    if(temp_value_for_activated_function != 0) 
    {
      //Опреділювані функції
      for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        unsigned int tmp_state_df = 0;
        switch (i)
        {
        case 0:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF1_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF1_IN);

            break;
          }
        case 1:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF2_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF2_IN);

            break;
          }
        case 2:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF3_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF3_IN);

            break;
          }
        case 3:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF4_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF4_IN);

            break;
          }
        case 4:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF5_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF5_IN);

            break;
          }
        case 5:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF6_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF6_IN);

            break;
          }
        case 6:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF7_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF7_IN);

            break;
          }
        case 7:
          {
            tmp_state_df = _GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DF8_IN);
            if (tmp_state_df) 
              _SET_BIT(active_functions, RANG_DF8_IN);

            break;
          }
        default: break;
        }
        
        if (tmp_state_df != 0)
        {
          //Зараз має активуватися ОФx, тому треба запустити таймер її утримування,
          //для того, щоб потім час цей можна було зрівняти з часом таймера павзи
          if (global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] <0)
          {
            //Запускаємо таймер таймер утримування цієї функції в активному стані (емітація активного входу)
            //Запуск робимо тільки ту тому випадкук, якщо він ще не почався
            global_timers[INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START + i] = 0;
          }
        }
        
      }
      
      //Опреділювані триґери
      for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        switch (i)
        {
        case 0:
          {
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT1_SET)) 
              _SET_BIT(active_functions, RANG_DT1_SET);
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT1_RESET)) 
              _SET_BIT(active_functions, RANG_DT1_RESET);

            break;
          }
        case 1:
          {
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT2_SET)) 
              _SET_BIT(active_functions, RANG_DT2_SET);
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT2_RESET)) 
              _SET_BIT(active_functions, RANG_DT2_RESET);

            break;
          }
        case 2:
          {
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT3_SET)) 
              _SET_BIT(active_functions, RANG_DT3_SET);
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT3_RESET)) 
              _SET_BIT(active_functions, RANG_DT3_RESET);

            break;
          }
        case 3:
          {
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT4_SET)) 
              _SET_BIT(active_functions, RANG_DT4_SET);
            if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_DT4_RESET)) 
              _SET_BIT(active_functions, RANG_DT4_RESET);

            break;
          }
        default: break;
        }
      }
      
      //Скидання світлодіодів і реле
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_RESET_LEDS)) 
        _SET_BIT(active_functions, RANG_RESET_LEDS);
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_RESET_RELES)) 
        _SET_BIT(active_functions, RANG_RESET_RELES);

      //"Місцеве/Дистанційне" управління
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_MISCEVE_DYSTANCIJNE))
      {
        //Активована команда з функціональної кнопки "Місцеве/Дистанційне"
        misceve_dystancijne = (misceve_dystancijne ^ 0x1) & 0x1;
        //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
        _SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
      }
      
      //Включення-виключення вимикача
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_VKL_VV)) 
        _SET_BIT(active_functions, RANG_VKL_VV);
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_OTKL_VV)) 
        _SET_BIT(active_functions, RANG_OTKL_VV);

      //скидання блокування готовності до ТУ
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_RESET_BLOCK_READY_TU_VID_ZAHYSTIV)) 
        _SET_BIT(active_functions, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV);
    }

    //Команди АВР
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_OTKL_AVR)) 
        _SET_BIT(active_functions, RANG_OTKL_AVR);
      if (_GET_OUTPUT_STATE(temp_value_for_activated_function, RANG_BUTTON_SBROS_BLOCK_AVR)) 
        _SET_BIT(active_functions, RANG_SBROS_BLOCK_AVR);
  }
  /**************************/

  //"Місц./Дистанц."  з функціональних кнопок меню
  active_functions[RANG_MISCEVE_DYSTANCIJNE >> 5] |= (misceve_dystancijne & 0x1) << (RANG_MISCEVE_DYSTANCIJNE & 0x1f);

  unsigned int blocking_commands_from_DI = 0;
  unsigned int active_inputs_grupa_ustavok = 0;
  /**************************/
  //Опрацьовуємо дискретні входи
  /**************************/
  //Перевіряємо чи є зараз активні входи
  if (active_inputs !=0)
  {
    //Є входи активні
    unsigned int temp_value_for_activated_function_2[N_SMALL] = {0,0};
    for (unsigned int i = 0; i < NUMBER_INPUTS; i++)
    {
      if ((active_inputs & (1 << i)) != 0)
      {
        temp_value_for_activated_function_2[0] |= current_settings_prt.ranguvannja_inputs[N_SMALL*i  ];
        temp_value_for_activated_function_2[1] |= current_settings_prt.ranguvannja_inputs[N_SMALL*i+1];
      }
    }
    
    //Якщо  функції активовувалися через ДВ то переносимо їх у тимчасовий масив функцій, які мають бути зараз активавані
    if(
       (temp_value_for_activated_function_2[0] != 0) ||
       (temp_value_for_activated_function_2[1] != 0)
      ) 
    {
      //Опреділювані функції
      for (unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
      {
        switch (i)
        {
        case 0:
          {
            active_functions[RANG_DF1_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF1_IN) != 0) << (RANG_DF1_IN & 0x1f);
            break;
          }
        case 1:
          {
            active_functions[RANG_DF2_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF2_IN) != 0) << (RANG_DF2_IN & 0x1f);
            break;
          }
        case 2:
          {
            active_functions[RANG_DF3_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF3_IN) != 0) << (RANG_DF3_IN & 0x1f);
            break;
          }
        case 3:
          {
            active_functions[RANG_DF4_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF4_IN) != 0) << (RANG_DF4_IN & 0x1f);
            break;
          }
        case 4:
          {
            active_functions[RANG_DF5_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF5_IN) != 0) << (RANG_DF5_IN & 0x1f);
            break;
          }
        case 5:
          {
            active_functions[RANG_DF6_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF6_IN) != 0) << (RANG_DF6_IN & 0x1f);
            break;
          }
        case 6:
          {
            active_functions[RANG_DF7_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF7_IN) != 0) << (RANG_DF7_IN & 0x1f);
            break;
          }
        case 7:
          {
            active_functions[RANG_DF8_IN >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DF8_IN) != 0) << (RANG_DF8_IN & 0x1f);
            break;
          }
        default: break;
        }
      }

      //Опреділювані триґери
      for (unsigned int i = 0; i < NUMBER_DEFINED_TRIGGERS; i++)
      {
        switch (i)
        {
        case 0:
          {
            active_functions[RANG_DT1_SET   >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT1_SET  ) != 0) << (RANG_DT1_SET   & 0x1f);
            active_functions[RANG_DT1_RESET >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT1_RESET) != 0) << (RANG_DT1_RESET & 0x1f);
            break;
          }
        case 1:
          {
            active_functions[RANG_DT2_SET   >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT2_SET  ) != 0) << (RANG_DT2_SET   & 0x1f);
            active_functions[RANG_DT2_RESET >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT2_RESET) != 0) << (RANG_DT2_RESET & 0x1f);
            break;
          }
        case 2:
          {
            active_functions[RANG_DT3_SET   >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT3_SET  ) != 0) << (RANG_DT3_SET   & 0x1f);
            active_functions[RANG_DT3_RESET >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT3_RESET) != 0) << (RANG_DT3_RESET & 0x1f);
            break;
          }
        case 3:
          {
            active_functions[RANG_DT4_SET   >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT4_SET  ) != 0) << (RANG_DT4_SET   & 0x1f);
            active_functions[RANG_DT4_RESET >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_DT4_RESET) != 0) << (RANG_DT4_RESET & 0x1f);
            break;
          }
        default: break;
        }
      }
      
      //Загальні функції (без ОФ-ій і функцій, які можуть блокуватися у місцевому управлінні)
      active_functions[RANG_BLOCK_VKL_VV                      >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_VKL_VV                     ) != 0) << (RANG_BLOCK_VKL_VV                      & 0x1f);
      active_functions[RANG_RESET_LEDS                        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_RESET_LEDS                       ) != 0) << (RANG_RESET_LEDS                        & 0x1f);
      active_functions[RANG_RESET_RELES                       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_RESET_RELES                      ) != 0) << (RANG_RESET_RELES                       & 0x1f);
      active_functions[RANG_MISCEVE_DYSTANCIJNE               >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_MISCEVE_DYSTANCIJNE              ) != 0) << (RANG_MISCEVE_DYSTANCIJNE               & 0x1f);
      active_functions[RANG_STATE_VV                          >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_STATE_VV                         ) != 0) << (RANG_STATE_VV                          & 0x1f);
      active_functions[RANG_PUSK_K1_AVR                       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_PUSK_K1_AVR                      ) != 0) << (RANG_PUSK_K1_AVR                       & 0x1f);
      active_functions[RANG_PUSK_K2_AVR                       >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_PUSK_K2_AVR                      ) != 0) << (RANG_PUSK_K2_AVR                       & 0x1f);
      active_functions[RANG_CTRL_VKL                          >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_CTRL_VKL                         ) != 0) << (RANG_CTRL_VKL                          & 0x1f);
      active_functions[RANG_CTRL_OTKL                         >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_CTRL_OTKL                        ) != 0) << (RANG_CTRL_OTKL                         & 0x1f);
      active_functions[RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0) << (RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV & 0x1f);
      active_functions[RANG_OTKL_VID_ZOVN_ZAHYSTIV            >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_OTKL_VID_ZOVN_ZAHYSTIV           ) != 0) << (RANG_OTKL_VID_ZOVN_ZAHYSTIV            & 0x1f);

      active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_1_GRUPA_USTAVOK    ) != 0) << (RANG_INPUT_1_GRUPA_USTAVOK - RANG_INPUT_1_GRUPA_USTAVOK);
      active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_2_GRUPA_USTAVOK    ) != 0) << (RANG_INPUT_2_GRUPA_USTAVOK - RANG_INPUT_1_GRUPA_USTAVOK);
      active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_3_GRUPA_USTAVOK    ) != 0) << (RANG_INPUT_3_GRUPA_USTAVOK - RANG_INPUT_1_GRUPA_USTAVOK);
      active_inputs_grupa_ustavok |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_4_GRUPA_USTAVOK    ) != 0) << (RANG_INPUT_4_GRUPA_USTAVOK - RANG_INPUT_1_GRUPA_USTAVOK);
      
      //Загальні функції (які блокувються у місцевому управлінні)
      //Ввімкнення ВВ
      if (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_VKL_VV ))
      {
        if (
            (_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE )) &&
            (current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_BLK_ON_CB_MISCEVE)
           ) 
        {
          //Умова блокування командви "Ввімкнення ВВ" від ДВх.
          blocking_commands_from_DI |= CTR_EXTRA_SETTINGS_1_BLK_ON_CB_MISCEVE;
        }
        else _SET_BIT(active_functions, RANG_VKL_VV);
      }
      if (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_OTKL_VV ))
      {
        if (
            (_CHECK_SET_BIT(active_functions, RANG_MISCEVE_DYSTANCIJNE )) &&
            (current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_BLK_OFF_CB_MISCEVE)
           ) 
        {
          //Умова блокування командви "Вимкнення ВВ" від ДВх.
          blocking_commands_from_DI |= CTR_EXTRA_SETTINGS_1_BLK_OFF_CB_MISCEVE;
        }
        else _SET_BIT(active_functions, RANG_OTKL_VV);
      }

      //Блок для МТЗ
      active_functions[RANG_BLOCK_MTZ1     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_MTZ1    ) != 0) << (RANG_BLOCK_MTZ1     & 0x1f);
      active_functions[RANG_BLOCK_MTZ2     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_MTZ2    ) != 0) << (RANG_BLOCK_MTZ2     & 0x1f);
      active_functions[RANG_BLOCK_USK_MTZ2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_USK_MTZ2) != 0) << (RANG_BLOCK_USK_MTZ2 & 0x1f);
      active_functions[RANG_BLOCK_MTZ3     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_MTZ3    ) != 0) << (RANG_BLOCK_MTZ3     & 0x1f);
      active_functions[RANG_BLOCK_USK_MTZ3 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_USK_MTZ3) != 0) << (RANG_BLOCK_USK_MTZ3 & 0x1f);
      active_functions[RANG_BLOCK_MTZ4     >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_MTZ4    ) != 0) << (RANG_BLOCK_MTZ4     & 0x1f);

      //Блок для ЗДЗ
      active_functions[RANG_PUSK_ZDZ_VID_DV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_PUSK_ZDZ_VID_DV) != 0) << (RANG_PUSK_ZDZ_VID_DV & 0x1f);

      //Блок АПВ
      active_functions[RANG_STAT_BLK_APV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_STAT_BLK_APV) != 0) << (RANG_STAT_BLK_APV & 0x1f);
      
      //ЧАПВ
//      active_functions[RANG_ACHR_CHAPV_VID_DV  >> 5] |=(_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_ACHR_CHAPV_VID_DV ) != 0) << (RANG_ACHR_CHAPV_VID_DV & 0x1f);

      //Блок для УРОВ
      active_functions[RANG_PUSK_UROV_VID_DV >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_PUSK_UROV_VID_DV) != 0) << (RANG_PUSK_UROV_VID_DV & 0x1f);

      //Блок ЗОП(КОФ)
      active_functions[RANG_BLOCK_ZOP >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_ZOP) != 0) << (RANG_BLOCK_ZOP & 0x1f);

      //Блок для Umin
      active_functions[RANG_BLOCK_UMIN1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_UMIN1) != 0) << (RANG_BLOCK_UMIN1 & 0x1f);
      active_functions[RANG_START_UMIN1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_START_UMIN1) != 0) << (RANG_START_UMIN1 & 0x1f);
      active_functions[RANG_BLOCK_UMIN2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_UMIN2) != 0) << (RANG_BLOCK_UMIN2 & 0x1f);
      active_functions[RANG_START_UMIN2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_START_UMIN2) != 0) << (RANG_START_UMIN2 & 0x1f);
      
      //Блок для Umax
      active_functions[RANG_BLOCK_UMAX1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_UMAX1) != 0) << (RANG_BLOCK_UMAX1 & 0x1f);
      active_functions[RANG_BLOCK_UMAX2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_BLOCK_UMAX2) != 0) << (RANG_BLOCK_UMAX2 & 0x1f);
    }
      
    //Блок для АВР
    active_functions[RANG_OTKL_AVR         >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_OTKL_AVR        ) != 0) << (RANG_OTKL_AVR         & 0x1f);
    active_functions[RANG_SBROS_BLOCK_AVR  >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_SBROS_BLOCK_AVR ) != 0) << (RANG_SBROS_BLOCK_AVR  & 0x1f);
    active_functions[RANG_STAT_BLOCK_AVR_1 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_STAT_BLOCK_AVR_1) != 0) << (RANG_STAT_BLOCK_AVR_1 & 0x1f);
    active_functions[RANG_STAT_BLOCK_AVR_2 >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_STAT_BLOCK_AVR_2) != 0) << (RANG_STAT_BLOCK_AVR_2 & 0x1f);
    active_functions[RANG_OZT_AVR_1        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_OZT_AVR_1       ) != 0) << (RANG_OZT_AVR_1        & 0x1f);
    active_functions[RANG_OZT_AVR_2        >> 5] |= (_CHECK_SET_BIT(temp_value_for_activated_function_2, RANG_INPUT_OZT_AVR_2       ) != 0) << (RANG_OZT_AVR_2        & 0x1f);
  }
  /**************************/

  /**************************/
  //Виконуємо фільтрацію переднього фронту для тих сигналів, які мають активуватися тільки по передньому фронтові
  /**************************/
  {
    static unsigned int previous_activating_functions[N_BIG];
    //Формуємо маску сигналів, які треба позначити, як активні тільки в момент переходу з "0" в "1"
    unsigned int temp_maska_filter_function[N_BIG] = {0, 0, 0, 0, 0, 0, 0};
    unsigned int temp_activating_functions[N_BIG] = {0, 0, 0, 0, 0, 0, 0};
  
    //Сигнал "Сблос индикации"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_LEDS);
  
    //Сигнал "Сблос реле"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_RELES);

    //Сигнал "Включить ВВ"
    _SET_BIT(temp_maska_filter_function, RANG_VKL_VV);
    
    //Сигнал "Отключить ВВ"
    _SET_BIT(temp_maska_filter_function, RANG_OTKL_VV);

    //Сигнал "Скидання блокування готовності до ТУ"
    _SET_BIT(temp_maska_filter_function, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV);
  
    //Сигнал "Внешний отключение АВР"
    _SET_BIT(temp_maska_filter_function, RANG_OTKL_AVR);
    //Сигнал "Внешний сброс блокировки АВР"
    _SET_BIT(temp_maska_filter_function, RANG_SBROS_BLOCK_AVR);
    
    for (unsigned int i = 0; i < N_BIG; i++)
    {
      //З масиву попередніх станів виділяємо тільки ті функції, якиї нас цікавить фронт змін і поміщаємо їх у тимчасовий масив
      temp_activating_functions[i] = previous_activating_functions[i] & temp_maska_filter_function[i];
  
      //У тимчасовому масиві виділяємо тільки ті функції, у яких зафіксовано або передній, або задній фронти
      temp_activating_functions[i] ^= (active_functions[i] & temp_maska_filter_function[i]);
  
      //Тепер виділяємо у тимчасовому масиві тільки ті функції у яких зараз значення стоять рівні "1" (тобто відбувся перехід з "0" в "1")
      temp_activating_functions[i] &= active_functions[i];

      /*
      Перед тим, як виділити ті функції, які у цьому циклі будуть відмічені як активні 
      з урахуванням того, що деякі функції активоються по передньому фронті
      копіюємо статичний стан функцій (без врахування фронтів) у тимчасовий масив
      щоб при наступному аналізі мати попередній статичний стан активних функцій
      */ 
      previous_activating_functions[i] = active_functions[i];

      //Обновляємо масив функцій, які зараз активуються з врахуванням того, що серед виділених функцій маскою активними мають юути тільки ті, у яких перехід був з "0" в "1"
      active_functions[i] = (active_functions[i] & (~temp_maska_filter_function[i])) | temp_activating_functions[i];
    }
  }
  /**************************/
  
  /***********************************************************/
  //Розрахунок вимірювань
  /***********************************************************/
  calc_measurement(number_group_stp);

#ifdef DEBUG_TEST
  /***/
  //Тільки для відладки
  /***/
  if (temp_value_3I0_1 != 0)
    measurement[IM_3I0]         = temp_value_3I0_1;
  if (temp_value_3I0_other != 0)
    measurement[IM_3I0_other_g] = temp_value_3I0_other;
  if (temp_value_IA != 0)
    measurement[IM_IA]          = temp_value_IA;
  if (temp_value_IC != 0)
    measurement[IM_IC]          = temp_value_IC;
  if (temp_value_UA != 0)
    measurement[IM_UA]          = temp_value_UA;
  if (temp_value_UB != 0)
    measurement[IM_UB]          = temp_value_UB;
  if (temp_value_UC != 0)
    measurement[IM_UC]          = temp_value_UC;
  if (temp_value_3U0 != 0)
    measurement[IM_3U0]         = temp_value_3U0;
  if (temp_value_I2 != 0)
    measurement[IM_I2]          = temp_value_I2;
  if (temp_value_I1 != 0)
    measurement[IM_I1]          = temp_value_I1;
  /***/
#endif
    
      
  //Діагностика справності раз на період
  diagnostyca_adc_execution();
  
  //Копіюємо вимірювання для низькопріоритетних і високопріоритетних завдань
  unsigned int bank_measurement_high_tmp = (bank_measurement_high ^ 0x1) & 0x1;
  if(semaphore_measure_values_low1 == 0)
  {
    for (unsigned int i = 0; i < (NUMBER_ANALOG_CANALES + 8); i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement_middle[i] = measurement[i];
    }
  }
  else
  {
    for (unsigned int i = 0; i < (NUMBER_ANALOG_CANALES + 8); i++) 
    {
      measurement_high[bank_measurement_high_tmp][i] = measurement[i];
    }
  }
  bank_measurement_high = bank_measurement_high_tmp;
  /***********************************************************/
  
  
  /**************************/
  //Сигнал "Несправность Общая"
  /**************************/
  unsigned int diagnostyka_tmp[3];
  diagnostyka_tmp[0] = diagnostyka[0];
  diagnostyka_tmp[1] = diagnostyka[1];
  diagnostyka_tmp[2] = diagnostyka[2];

  diagnostyka_tmp[0] &= (unsigned int)(~clear_diagnostyka[0]); 
  diagnostyka_tmp[0] |= set_diagnostyka[0]; 

  diagnostyka_tmp[1] &= (unsigned int)(~clear_diagnostyka[1]); 
  diagnostyka_tmp[1] |= set_diagnostyka[1]; 

  diagnostyka_tmp[2] &= (unsigned int)(~clear_diagnostyka[2]); 
  diagnostyka_tmp[2] |= set_diagnostyka[2]; 
  
  diagnostyka_tmp[2] &= USED_BITS_IN_LAST_INDEX; 

  _CLEAR_BIT(diagnostyka_tmp, EVENT_START_SYSTEM_BIT);
  _CLEAR_BIT(diagnostyka_tmp, EVENT_DROP_POWER_BIT);
  if (
      (diagnostyka_tmp[0] != 0) ||
      (diagnostyka_tmp[1] != 0) ||
      (diagnostyka_tmp[2] != 0)
     )   
  {
    _SET_BIT(active_functions, RANG_DEFECT);
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_DEFECT);
  }
  /**************************/

  /**************************/
  //Сигнал "Несправность Аварийная"
  /**************************/
  if (
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_BIT                     ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_EMPTY_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_COMPARISON_BIT          ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_SETTINGS_EEPROM_CONTROL_BIT             ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_BIT                   ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_EMPTY_BIT             ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_COMPARISON_BIT        ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_CONTROL_BIT           ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_USTUVANNJA_EEPROM_ADJUSTMENT_ID_FAIL_BIT) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_BIT                     ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_EMPTY_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_COMPARISON_BIT          ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_TRG_FUNC_EEPROM_CONTROL_BIT             ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC1_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC1_TEST_BIT                      ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC1_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC1_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC1_TEST_COARSE_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC1_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC2_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC2_TEST_BIT                      ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC2_TEST_BIT                       ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_GND_ADC2_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VREF_ADC2_TEST_COARSE_BIT               ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_VDD_ADC2_TEST_COARSE_BIT                ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_DIGITAL_OUTPUTS_BIT                     ) != 0) ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_INTERNAL_FLASH_BIT                     ) != 0)/* ||
      (_CHECK_SET_BIT(diagnostyka, ERROR_EXTERNAL_SRAM_BIT                       ) != 0)*/
     )   
  {
    _SET_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  else
  {
    _CLEAR_BIT(active_functions, RANG_AVAR_DEFECT);
  }
  /**************************/
  
  //Логічні схеми мають працювати тільки у тому випадку, якщо немє сигналу "Аварийная неисправность"
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //Аварійна ситуація не зафіксована

    /**************************/
    //"Перевірка фазування"
    /**************************/
    if ((current_settings_prt.configuration & (1 << CTRL_PHASE_BIT_CONFIGURATION)) != 0) 
    {
      ctrl_phase_handler(active_functions, number_group_stp);
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_ctrl_phase_signals[N_BIG] = 
      {
        MASKA_CTRL_PHASE_SIGNALS_0, 
        MASKA_CTRL_PHASE_SIGNALS_1, 
        MASKA_CTRL_PHASE_SIGNALS_2,
        MASKA_CTRL_PHASE_SIGNALS_3, 
        MASKA_CTRL_PHASE_SIGNALS_4, 
        MASKA_CTRL_PHASE_SIGNALS_5, 
        MASKA_CTRL_PHASE_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_ctrl_phase_signals[i]);
      
      global_timers[INDEX_TIMER_CTRL_PHASE_U] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_U_D] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_PHI] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_PHI_D] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_F] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_F_D] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_TMP1_100MS] = -1;
      global_timers[INDEX_TIMER_CTRL_PHASE_TMP2_100MS] = -1;
    }
    /**************************/
    
    /**************************/
    //Розширена логіка
    /**************************/
    if ((current_settings_prt.configuration & (1 << EL_BIT_CONFIGURATION)) != 0)
    {
      unsigned int active_functions_tmp[NUMBER_ITERATION_EL_MAX][N_BIG];
      unsigned int iteration = 0;
	  unsigned int repeat_state = false;
      unsigned int df_changed_state_with_start_new_timeout = 0;
      do
      {
        for (unsigned int i = 0; i < iteration; i++)
        {
          if (
              (active_functions_tmp[i][0] == active_functions[0]) &&
              (active_functions_tmp[i][1] == active_functions[1]) &&
              (active_functions_tmp[i][2] == active_functions[2]) &&
              (active_functions_tmp[i][3] == active_functions[3]) &&
              (active_functions_tmp[i][4] == active_functions[4]) &&
              (active_functions_tmp[i][5] == active_functions[5]) &&
              (active_functions_tmp[i][6] == active_functions[6])
             )
          {
            repeat_state = true;
            break;
          }
        }
        if (repeat_state != false ) break;
        
        active_functions_tmp[iteration][0] = active_functions[0];
        active_functions_tmp[iteration][1] = active_functions[1];
        active_functions_tmp[iteration][2] = active_functions[2];
        active_functions_tmp[iteration][3] = active_functions[3];
        active_functions_tmp[iteration][4] = active_functions[4];
        active_functions_tmp[iteration][5] = active_functions[5];
        active_functions_tmp[iteration][6] = active_functions[6];

        d_and_handler(active_functions);
        d_or_handler(active_functions);
        d_xor_handler(active_functions);
        d_not_handler(active_functions);
        df_handler(active_functions, &df_changed_state_with_start_new_timeout);
        dt_handler(active_functions);
        
        iteration++;
      }
      while (
             (iteration < current_settings_prt.number_iteration_el)
             &&
             (
              (active_functions_tmp[iteration - 1][0] != active_functions[0]) ||
              (active_functions_tmp[iteration - 1][1] != active_functions[1]) ||
              (active_functions_tmp[iteration - 1][2] != active_functions[2]) ||
              (active_functions_tmp[iteration - 1][3] != active_functions[3]) ||
              (active_functions_tmp[iteration - 1][4] != active_functions[4]) ||
              (active_functions_tmp[iteration - 1][5] != active_functions[5]) ||
              (active_functions_tmp[iteration - 1][6] != active_functions[6])
             ) 
            );
      
      if (
          (repeat_state != false ) ||
          (iteration >= current_settings_prt.number_iteration_el)
         )
      {
        _SET_BIT(active_functions, RANG_ERROR_CONF_EL);
      }
      else
      {
        _CLEAR_BIT(active_functions, RANG_ERROR_CONF_EL);
      }
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_el_signals[N_BIG] = 
      {
        MASKA_EL_SIGNALS_0, 
        MASKA_EL_SIGNALS_1, 
        MASKA_EL_SIGNALS_2,
        MASKA_EL_SIGNALS_3, 
        MASKA_EL_SIGNALS_4, 
        MASKA_EL_SIGNALS_5, 
        MASKA_EL_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_el_signals[i]);
      
      //Скидаємо всі таймери, які відповідають за розширену логіку
      for(unsigned int i = INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START; i < (INDEX_TIMER_DF_WORK_START + NUMBER_DEFINED_FUNCTIONS); i++)
        global_timers[i] = -1;
    }
    /**************************/
  }
  else
  {
    //Аварійна ситуація зафіксована
    
    //Скидаємо всі активні функції, крім інформативних
    active_functions[0] &= MASKA_INFO_SIGNALES_0;
    active_functions[1] &= MASKA_INFO_SIGNALES_1;
    active_functions[2] &= MASKA_INFO_SIGNALES_2;
    active_functions[3] &= MASKA_INFO_SIGNALES_3;
    active_functions[4] &= MASKA_INFO_SIGNALES_4;
    active_functions[5] &= MASKA_INFO_SIGNALES_5;
    active_functions[6] &= MASKA_INFO_SIGNALES_6;
    
    //Деактивовуємо всі реле
    state_outputs = 0;
    
    //Переводимо у початковий стан деякі глобальні змінні
    previous_states_APV_0 = 0;
    trigger_APV_0 = 0;

    previous_states_AVR_0 = 0;
    trigger_AVR_0 = 0;

    previous_states_ready_tu = 0;
    trigger_ready_tu = 0;
    
    //Скидаємо всі таймери, які присутні у лозіці
    for(unsigned int i = INDEX_TIMER_DF_PROLONG_SET_FOR_BUTTON_INTERFACE_START; i < MAX_NUMBER_GLOBAL_TIMERS; i++)
      global_timers[i] = -1;
    
    //Стан виконання ОФ переводимо у початковий
    for(unsigned int i = 0; i < NUMBER_DEFINED_FUNCTIONS; i++)
    {
      etap_execution_df[i] = NONE_DF;
    }
  }

  /**************************/
  //Обробка аналогового реєстратора
  /**************************/
  analog_registrator(active_functions);
  /**************************/

  /**************************/
  //Обробка дискретного реєстратора
  /**************************/
  digital_registrator(active_functions);
  /**************************/

  /**************************/
  //Робота з функціями, які мають записуватися у енергонезалежну пам'ять
  /**************************/
  const unsigned int maska_trg_func_array[N_BIG] = {MASKA_TRIGGER_SIGNALES_0, MASKA_TRIGGER_SIGNALES_1, MASKA_TRIGGER_SIGNALES_2, MASKA_TRIGGER_SIGNALES_3, MASKA_TRIGGER_SIGNALES_4, MASKA_TRIGGER_SIGNALES_5, MASKA_TRIGGER_SIGNALES_6};
  unsigned int comparison_true = true;
  for (unsigned int i = 0; i < N_BIG; i++)
  {
    unsigned int tmp_data;
    if (trigger_active_functions[i] != (tmp_data = (active_functions[i] & maska_trg_func_array[i])))
    {
      comparison_true = false;
      trigger_active_functions[i] = tmp_data;
    }
  }
  if (comparison_true != true)
  {
    /*
    Сигнали, значення яких записується у енергонезалежну пам'1ять змінилися.
    Подаємо команду на їх запис у енергонезалежну пам'ять
    */
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT);
  }
  /**************************/

  /**************************/
  //Перекидання інфомації у масиви активних і тригерних функцій
  /**************************/
  for (unsigned int i = 0; i < N_BIG; i++)
  {
    unsigned int temp_data = active_functions[i];
    trigger_functions_USB[i]   |= temp_data;
    trigger_functions_RS485[i] |= temp_data;
  }

  copying_active_functions = 0; //Помічаємо, що обновлення значення активних функцій завершене
  
  /*
  Робимо копію значення активних функцій для того, щоб коли ці знаення будуть
  обновлятися, то можна було б іншим модулям  (запис у об'єднаний аналоговий
  реєстратор) взяти попереднє, але достовірне значення
  */
  for (unsigned int i = 0; i < N_BIG; i++) active_functions_copy[i] = active_functions[i];
  /**************************/

  /**************************/
  //Вивід інформації на виходи
  /**************************/
  //Спочатку перевіряємо, чи не активовувалвся команда "Сблос реле" - і якщо так, то попередньо скидаємо всі реле
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_RELES) !=0)
  {
    state_outputs = 0;
  }
  
  if (_CHECK_SET_BIT(active_functions, RANG_AVAR_DEFECT) == 0)
  {
    //Не зафіксовано аварійної ситуації, тому встановлювати реле можна
    
    //Визначаємо, які реле зараз мають бути замкнутими
    for (unsigned int i = 0; i < NUMBER_OUTPUTS; i++)
    {
      //У тимчасовий масив поміщаємо ЛОГІЧНЕ І ранжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
      unsigned int temp_array_of_outputs[N_BIG];
    
      for (unsigned int j = 0; j < N_BIG; j++) temp_array_of_outputs[j] = current_settings_prt.ranguvannja_outputs[N_BIG*i + j] & active_functions[j];

      //Сигнал "Аварійна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_AVAR_DEFECT) !=0)
      {
        //Сигнал "Aварийная неисправность"  справді зранжовано на даний вихід
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT) == 0)
        {
          //Сигнал "Aварийная неисправность" не є активним
          //Приимусово встановлюємо його у активний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _SET_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
        else
        {
          //Сигнал "Aварийная неисправность" є активним
          //Приимусово переводимо його у пасивний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _CLEAR_BIT(temp_array_of_outputs, RANG_AVAR_DEFECT);
        }
      }
      
      //Сигнал "Загальна несправність" працює у інверсному режимі: замикає реле на якому зранжована у випадку, коли даний сигнал не активинй
      if(_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_DEFECT) !=0)
      {
        //Сигнал "Загальна несправність"  справді зранжовано на даний вихід
        if (_CHECK_SET_BIT(temp_array_of_outputs, RANG_DEFECT) ==0)
        {
          //Сигнал "Загальна несправність" не є активним
          //Приимусово встановлюємо його у активний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _SET_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
        else
        {
          //Сигнал "Загальна несправність" є активним
          //Приимусово переводимо його у пасивний стан у масиві, який є  ЛОГІЧНИМ І анжування виходу, який індексується інедексом "i" і функцій, які зараз є активними
          _CLEAR_BIT(temp_array_of_outputs, RANG_DEFECT);
        }
      }

      //Перевіряємо, чи є співпадіння між ранжованими функціями на цьому виході і функціями, які зараз є активними - умова активації виходу
      if (
          (temp_array_of_outputs[0] !=0 ) ||
          (temp_array_of_outputs[1] != 0) ||
          (temp_array_of_outputs[2] != 0) ||
          (temp_array_of_outputs[3] != 0) ||
          (temp_array_of_outputs[4] != 0) ||
          (temp_array_of_outputs[5] != 0) ||
          (temp_array_of_outputs[6] != 0)
         )
      {
        //Для сигнального реле виконуємо його замикання, а для командного перевіряємо чи нема спроби активувати реле при умові що на нього заведено блок включення, причому він блокований
        if ((current_settings_prt.type_of_output & (1 << i)) != 0)
        {
          //Вихід сигнальний, тому у буль якому разі замикаємо реле
          //Відмічаємо, що даний вихід - ЗАМКНУТИЙ
          state_outputs |= (1 << i);
        }
        else
        {
          //Вихід командний, тому перевіряємо чи не йде спроба активувати реле, на яке заведено БВ, причому блок БВ з пеквних причин блокований (неактивний)
          if (_CHECK_SET_BIT((current_settings_prt.ranguvannja_outputs + N_BIG*i), RANG_WORK_BV) == 0)
          {
            //На дане реле не заводиться сигнал БВ (блок включення)
          
            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ
            state_outputs |= (1 << i);
          }
          else
          {
            //На дане реле заводиться сигнал БВ (блок включення)
          
            //Відмічаємо, що даний вихід - ЗАМКНУТИЙ тільки тоді, коли функція БВ активна зараз
            if (_CHECK_SET_BIT(active_functions, RANG_WORK_BV) != 0)
              state_outputs |= (1 << i);
            else
              state_outputs &= ~(1 << i);
          }
        }
      }
      else
      {
        //Перевіряємо, чи вихід командний, чи сигнальний
        if ((current_settings_prt.type_of_output & (1 << i)) == 0)
        {
          //Вихід командний
        
          //Відмічаємо, що даний вихід - РОЗІМКНУТИЙ
          state_outputs &= ~(1 << i);
        }
      }
    }
  }
  else
  {
    //Зафіксовано аварійнe ситуацію, тому деактивуємо всі реле!!!

    //Деактивовуємо всі реле
    state_outputs = 0;
  }
  
  //Перевіряємо чи треба записувати стан сигнальних виходів у EEPROM
  unsigned int temp_value_char_for_volatile = state_signal_outputs;
  if((state_outputs  & current_settings_prt.type_of_output) != temp_value_char_for_volatile)
  {
    state_signal_outputs = state_outputs  & current_settings_prt.type_of_output;
    //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }
  
  //Стан виходу з урахуванням імпульсного режиму роботи сигнальних виходів
  state_outputs_raw = ( state_outputs & ((unsigned int)(~current_settings_prt.type_of_output_modif)) ) | ((state_outputs & current_settings_prt.type_of_output_modif)*output_timer_prt_signal_output_mode_2);
  
  //Виводимо інформацію по виходах на піни процесора (у зворотньому порядку)
  unsigned int temp_state_outputs = 0;
  for (unsigned int index = 0; index < NUMBER_OUTPUTS; index++)
  {
    if ((state_outputs_raw & (1 << index)) != 0)
    {
      if (index < NUMBER_OUTPUTS_1)
        temp_state_outputs |= 1 << (NUMBER_OUTPUTS_1 - index - 1);
      else
        temp_state_outputs |= 1 << index;
    }
  }
  unsigned int temp_state_outputs_1 =  temp_state_outputs                      & ((1 << NUMBER_OUTPUTS_1) - 1);
  unsigned int temp_state_outputs_2 = (temp_state_outputs >> NUMBER_OUTPUTS_1) & ((1 << NUMBER_OUTPUTS_2) - 1);
  _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_1) = temp_state_outputs_1;
  _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_2) = temp_state_outputs_2;
  /**************************/

  /**************************/
  //Вивід інформації на світлодіоди
  /**************************/
  //Спочатку перевіряємо, чи не активовувалвся команда "Сблос индикации" - і якщо так, то попередньо скидаємо всю індикацію
  if (_CHECK_SET_BIT(active_functions, RANG_RESET_LEDS) !=0)
  {
    state_leds = 0;
  }
  
  //Визначаємо, які світлоіндикатори зараз мають бути активними
  for (unsigned int i = 0; i < NUMBER_LEDS; i++)
  {
    //У тимчасовий масив копіюємо ранжування світлоіндикатора, який індексується інедексом "i"
    unsigned int temp_array_of_leds[N_BIG];
    
    //Перевіряємо, чи є співпадіння між ранжованими функціями на цьому світлоіндикаторі і функціями, які зараз є активними - умова активації виходу
    for (unsigned int j = 0; j < N_BIG; j++ ) temp_array_of_leds[j] = current_settings_prt.ranguvannja_leds[N_BIG*i + j] & active_functions[j];

    if (
        (temp_array_of_leds[0] != 0) ||
        (temp_array_of_leds[1] != 0) ||
        (temp_array_of_leds[2] != 0) ||
        (temp_array_of_leds[3] != 0) ||
        (temp_array_of_leds[4] != 0) ||
        (temp_array_of_leds[5] != 0) ||
        (temp_array_of_leds[6] != 0)
       )
    {
      //Відмічаємо, що даний світлоіндикатор - ГОРИТЬ
      state_leds |= (1 << i);
    }
    else
    {
      //Перевіряємо, чи даний світлоіндикатор нормальний, чи тригерний
      if ((current_settings_prt.type_of_led & (1 << i)) == 0)
      {
        //Світлоіндикатор нормальний

        //Відмічаємо, що даний світлоіндикатор - ПОГАШЕНИЙ
        state_leds &= ~(1 << i);
      }
    }
  }
  //Перевіряємо чи треба записувати стан тригерних світлоіндикаторів у EEPROM
  temp_value_char_for_volatile = state_trigger_leds;
  if((state_leds  & current_settings_prt.type_of_led) != temp_value_char_for_volatile)
  {
    state_trigger_leds = state_leds  & current_settings_prt.type_of_led;
    //Виставляємо повідомлення про те, що в EEPROM треба записати нові значення сигнальних виходів і тригерних світлоіндикаторів
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_STATE_LEDS_OUTPUTS_EEPROM_BIT);
  }

  //Виводимо інформацію по світлоіндикаторах на світлодіоди
  _DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_LEDS) = state_leds;
  /**************************/

  /**************************/
  //
  /**************************/
  /**************************/
}
/*****************************************************/

/*****************************************************/
//Переривання від таймеру TIM2, який обслуговує систему захистів
/*****************************************************/
void TIM2_IRQHandler(void)
{
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordEnterISR();
#endif
  
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    /***********************************************************************************************/
    //Переривання відбулося вік каналу 1, який генерує переривання кожні 1 мс, для опраціьовування таймерів і систем захистів
    /***********************************************************************************************/
    TIM2->SR = (uint16_t)((~(uint32_t)TIM_IT_CC1) & 0xffff);
    uint32_t current_tick = TIM2->CCR1;
    
    /***********************************************************/
    //Перевіряємо, чи відбувалися зміни настройок
    /***********************************************************/
    if (
        (changed_settings == CHANGED_ETAP_ENDED) && /*Це є умова, що нові дані підготовлені для передачі їх у роботу системою захистів (і при цьому зараз дані не змінюються)*/
        (state_ar_record  != STATE_AR_START    )    /*Це є умова, що на даний момент не може виникнути переривання від вимірювальної системи (з вищим пріоритетом за пріоритет системи захистів) з умовою початку формування запису аналогового реєстратора. де треба буде взяти ширину доаварійного і післяаварійного масивів*/ 
       )   
    {
      //Копіюємо таблицю настройок у копію цієї таблиці але з якою працює (читає і змінює) тільки система захистів
      current_settings_prt = current_settings;
      
      //Помічаємо, що зміни прийняті системою захистів
      changed_settings = CHANGED_ETAP_NONE;
    }
    /***********************************************************/

    /***********************************************************/
    //Перевіряємо необхідність очистки аналогового і дискретного реєстраторів
    /***********************************************************/
    //Аналоговий реєстратор
    if (
        ((clean_rejestrators & CLEAN_AR) != 0 )
        &&  
        (state_ar_record == STATE_AR_NO_RECORD)
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR |
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR      |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB                         |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485                       |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU
                                    )
         ) == 0
        )   
       )
    {
      //Виставлено каманда очистити аналогового реєстратора

      //Виставляємо команду запису цієї структури у EEPROM
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT);
    
      //Очищаємо структуру інформації по дискретному реєстраторі
      info_rejestrator_ar.next_address = MIN_ADDRESS_AR_AREA;
      info_rejestrator_ar.saving_execution = 0;
      info_rejestrator_ar.number_records = 0;
    
      //Помічаємо, що номер запису не вибраний
      number_record_of_ar_for_menu = 0xffff;
      number_record_of_ar_for_USB = 0xffff;
      number_record_of_ar_for_RS485 = 0xffff;

      //Знімаємо команду очистки аналогового реєстратора
      clean_rejestrators &= (unsigned int)(~CLEAN_AR);
    }
    
    //Дискретний реєстратор
    if (
        ((clean_rejestrators & CLEAN_DR) != 0)
        &&  
        (
         (control_tasks_dataflash & (
                                     TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR | 
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB                    |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485                  |
                                     TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU
                                    )
         ) == 0
        )
       )   
    {
      //Виставлено каманда очистити дискретного реєстратор

      //Виставляємо команду запису цієї структури у EEPROM
      _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);

      //Очищаємо структуру інформації по дискретному реєстраторі
      info_rejestrator_dr.next_address = MIN_ADDRESS_DR_AREA;
      info_rejestrator_dr.saving_execution = 0;
      info_rejestrator_dr.number_records = 0;

      //Помічаємо, що номер запису не вибраний
      number_record_of_dr_for_menu  = 0xffff;
      number_record_of_dr_for_USB   = 0xffff;
      number_record_of_dr_for_RS485 = 0xffff;

      //Знімаємо команду очистки дискретного реєстратора
      clean_rejestrators &= (unsigned int)(~CLEAN_DR);
    }
    /***********************************************************/

    /***********************************************************/
    //Опрцювання логічних тайменрів і дискретних входів тільки коли настройки успішно прочитані
    /***********************************************************/
    clocking_global_timers();
    /***********************************************************/
    
    /***********************************************************/
    //Опрцювання функцій захистів
    /***********************************************************/
    //Діагностика вузлів, яку треба проводити кожен раз перед початком опрацьовуванням логіки пристрою
    unsigned int control_state_outputs_1 = (( (~((unsigned int)(_DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_1)))) >> 8) & ((1 << NUMBER_OUTPUTS_1) - 1));
    unsigned int control_state_outputs_2 = (( (~((unsigned int)(_DEVICE_REGISTER(Bank1_SRAM2_ADDR, OFFSET_OUTPUTS_2)))) >> 8) & ((1 << NUMBER_OUTPUTS_2) - 1));
    unsigned int control_state_outputs = control_state_outputs_1 | (control_state_outputs_2 << NUMBER_OUTPUTS_1);
    //Формуємо стани виходів у відповідності до зміненої нумерації
    unsigned int temp_state_outputs = 0;
    for (unsigned int index = 0; index < NUMBER_OUTPUTS; index++)
    {
      if ((state_outputs_raw & (1 << index)) != 0) 
      {
        if (index < NUMBER_OUTPUTS_1)
          temp_state_outputs |= 1 << (NUMBER_OUTPUTS_1 - index - 1);
        else
          temp_state_outputs |= 1 << index;
      }
    }
    if (control_state_outputs != temp_state_outputs) _SET_BIT(set_diagnostyka, ERROR_DIGITAL_OUTPUTS_BIT);
//    else _SET_BIT(clear_diagnostyka, ERROR_DIGITAL_OUTPUTS_BIT);
    
    //Перевіряємо достовірність значень для аналогового реєстратора
    if (
        (state_ar_record  != STATE_AR_TEMPORARY_BLOCK) &&
        (changed_settings == CHANGED_ETAP_NONE       )  
       )   
    {
      //Перевірку здійснюємо тільки тоді, коли не іде зміна часових параметрів
      unsigned int size_one_ar_record_tmp = size_one_ar_record, max_number_records_ar_tmp = max_number_records_ar;
      if (
          ((number_word_digital_part_ar*8*sizeof(short int)) < NUMBER_TOTAL_SIGNAL_FOR_RANG)
          ||  
          (size_one_ar_record_tmp != (sizeof(__HEADER_AR) + ((current_settings_prt.prefault_number_periods + current_settings_prt.postfault_number_periods) << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int)))
          ||
          (
           !(
             (size_one_ar_record_tmp* max_number_records_ar_tmp      <= ((NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2))) &&
             (size_one_ar_record_tmp*(max_number_records_ar_tmp + 1) >  ((NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2)))
            ) 
          ) 
         )
      {
        //Теоретично ця помилка ніколи не малаб реєструватися
        /*Якщо виникла така ситуація то треба зациклити ропаграму, щоб вона пішла на перезапуск - 
        бо відбулася недопустима незрозуміла помилка у розраховуваних параметрах аналогового реєстратора.
        Не зрозумілу чого вона виникла, коли і де, коректність роботи пригоамного забезпечення під питанням!*/
        total_error_sw_fixed(5);
      }
    }

    //Функції захистів
    main_protection();
    /***********************************************************/

    /***********************************************************/
    //Перевірка на необхідність зроботи резервні копії даних для самоконтролю
    /***********************************************************/
    //Триґерна інформація
    if (periodical_tasks_TEST_TRG_FUNC != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_i2c_task & STATE_TRG_FUNC_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли триґерна інформація успішно зчитана і сформована контрольна сума
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_TRG_FUNC_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_TRG_FUNC_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_TRG_FUNC_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_TRG_FUNC_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис триґерної інформації, тому можна здійснити копіювання
          misceve_dystancijne_ctrl = misceve_dystancijne;
          for (unsigned int i = 0; i < N_BIG; i++) trigger_active_functions_ctrl[i] = trigger_active_functions[i];
          crc_trg_func_ctrl = crc_trg_func;

          //Скидаємо активну задачу формування резервної копії 
          periodical_tasks_TEST_TRG_FUNC = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії 
          periodical_tasks_TEST_TRG_FUNC_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії 
        periodical_tasks_TEST_TRG_FUNC = false;
      }
    }

    //Аналоговий реєстратор
    if (periodical_tasks_TEST_INFO_REJESTRATOR_AR != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_i2c_task & STATE_INFO_REJESTRATOR_AR_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли структура інформації реєстратора успішно зчитана і сформована контрольна сума
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_AR_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_INFO_REJESTRATOR_AR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_INFO_REJESTRATOR_AR_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис структури інформації реєстратора, тому можна здійснити копіювання
          info_rejestrator_ar_ctrl = info_rejestrator_ar;
          crc_info_rejestrator_ar_ctrl = crc_info_rejestrator_ar;

          //Скидаємо активну задачу формування резервної копії 
          periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії 
          periodical_tasks_TEST_INFO_REJESTRATOR_AR_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії 
        periodical_tasks_TEST_INFO_REJESTRATOR_AR = false;
      }
    }

    //Дискретний реєстратор
    if (periodical_tasks_TEST_INFO_REJESTRATOR_DR != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_i2c_task & STATE_INFO_REJESTRATOR_DR_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли структура інформації реєстратора успішно зчитана і сформована контрольна сума
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_INFO_REJESTRATOR_DR_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_INFO_REJESTRATOR_DR_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис структури інформації реєстратора, тому можна здійснити копіювання
          info_rejestrator_dr_ctrl = info_rejestrator_dr;
          crc_info_rejestrator_dr_ctrl = crc_info_rejestrator_dr;

          //Скидаємо активну задачу формування резервної копії 
          periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії 
          periodical_tasks_TEST_INFO_REJESTRATOR_DR_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії 
        periodical_tasks_TEST_INFO_REJESTRATOR_DR = false;
      }
    }
    /***********************************************************/

    /***********************************************************/
    //Встановлюємо "значення лічильника для наступного переривання"
    /***********************************************************/
    uint32_t capture_new;
    unsigned int delta;
    TIM2->CCR1 = (capture_new = (current_tick + (delta = TIM2_CCR1_VAL)));
    
    unsigned int repeat;
    unsigned int previous_tick;
    do
    {
      repeat = 0;
      current_tick = TIM2->CNT;

      uint64_t delta_time = 0;
      if (capture_new < current_tick)
        delta_time = capture_new + 0x100000000 - current_tick;
      else delta_time = capture_new - current_tick;

      if ((delta_time > delta) || (delta_time == 0))
      {
        if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == RESET)
        {
          if (delta < TIM2_CCR1_VAL)
          {
            uint64_t delta_tick;
            if (current_tick < previous_tick)
              delta_tick = current_tick + 0x100000000 - previous_tick;
            else delta_tick = current_tick - previous_tick;
              
            delta = delta_tick + 1;
          }
          else if (delta == TIM2_CCR1_VAL)
            delta = 1; /*Намагаємося, щоб нове переивання запустилося як омога скоріше*/
          else
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed(83);
          }
          TIM2->CCR1 = (capture_new = (TIM2->CNT +  delta));
          previous_tick = current_tick;
          repeat = 0xff;
        }
      }
    }
    while (repeat != 0);
    /***********************************************************/
    
    /***********************************************************/
    //Виставляємо повідомлення про те, система захисів праціює
    /***********************************************************/
    control_word_of_watchdog |= WATCHDOG_PROTECTION;
    /***********************************************************/
    /***********************************************************************************************/
  }
  else
  {
    total_error_sw_fixed(23);
  }
  
#ifdef SYSTEM_VIEWER_ENABLE
  SEGGER_SYSVIEW_RecordExitISR();
#endif
}
/*****************************************************/

