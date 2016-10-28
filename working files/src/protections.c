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
//Розрахунок струму зворотньої послідовності
/*****************************************************/
inline void velychyna_zvorotnoi_poslidovnosti(int ortogonal_local_calc[])
{
  int ortogonal_tmp[2];
  int mul_x1, mul_x2, mul_x3, mul_x4, mul_y1, mul_y2, mul_y3, mul_y4;
  
  mul_x1 = (      ortogonal_local_calc[2*FULL_ORT_Ib    ]>>1 );
  mul_y1 = (0x376*ortogonal_local_calc[2*FULL_ORT_Ib    ]>>10);

  mul_x2 = (0x376*ortogonal_local_calc[2*FULL_ORT_Ib + 1]>>10);
  mul_y2 = (      ortogonal_local_calc[2*FULL_ORT_Ib + 1]>>1 );

  mul_x3 = (      ortogonal_local_calc[2*FULL_ORT_Ic    ]>>1 );
  mul_y3 = (0x376*ortogonal_local_calc[2*FULL_ORT_Ic    ]>>10);

  mul_x4 = (0x376*ortogonal_local_calc[2*FULL_ORT_Ic + 1]>>10);
  mul_y4 = (      ortogonal_local_calc[2*FULL_ORT_Ic + 1]>>1 );

  //Зворотня послідовність
  ortogonal_tmp[0] = ((int)(0x155*(ortogonal_local_calc[2*FULL_ORT_Ia    ] - mul_x1  + mul_x2  - mul_x3 - mul_x4)))>>10;
  ortogonal_tmp[1] = ((int)(0x155*(ortogonal_local_calc[2*FULL_ORT_Ia + 1] - mul_y1  - mul_y2  + mul_y3 - mul_y4)))>>10;
  measurement[IM_I2] = (unsigned int)((unsigned long long)( MNOGNYK_I_DIJUCHE  *(sqrt_32((unsigned int)ortogonal_tmp[0]*ortogonal_tmp[0] + (unsigned int)ortogonal_tmp[1]*ortogonal_tmp[1])) ) >> (VAGA_DILENNJA_I_DIJUCHE   + 4));

  //Пряма послідовність
  ortogonal_tmp[0] = ((int)(0x155*(ortogonal_local_calc[2*FULL_ORT_Ia    ] - mul_x1  - mul_x2  - mul_x3 + mul_x4)))>>10;
  ortogonal_tmp[1] = ((int)(0x155*(ortogonal_local_calc[2*FULL_ORT_Ia + 1] + mul_y1  - mul_y2  - mul_y3 - mul_y4)))>>10;
  measurement[IM_I1] = (unsigned int)((unsigned long long)( MNOGNYK_I_DIJUCHE  *(sqrt_32((unsigned int)ortogonal_tmp[0]*ortogonal_tmp[0] + (unsigned int)ortogonal_tmp[1]*ortogonal_tmp[1])) ) >> (VAGA_DILENNJA_I_DIJUCHE   + 4));
}
/*****************************************************/

/*****************************************************/
//Направленість МТЗ з визначенням секторів
/*****************************************************/
inline void directional_mtz(int ortogonal_local_calc[], unsigned int number_group_stp) 
{
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  unsigned int index_FULL_ORT_Uab, index_FULL_ORT_Ubc, index_FULL_ORT_Uca;
  if (TN1_TN2 == 1)
  {
    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
    
    index_FULL_ORT_Uab = FULL_ORT_Uab2;
    index_FULL_ORT_Ubc = FULL_ORT_Ubc2;
    index_FULL_ORT_Uca = FULL_ORT_Uca2;
  }
  else
  {
    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
    
    index_FULL_ORT_Uab = FULL_ORT_Uab1;
    index_FULL_ORT_Ubc = FULL_ORT_Ubc1;
    index_FULL_ORT_Uca = FULL_ORT_Uca1;
  }

  for (unsigned int mtz = 0; mtz < 4; mtz++)
  {
    int a_cos_fi, a_sin_fi;
    switch (mtz)
    {
    case 0:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_1_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_1_angle_sin[number_group_stp];
        
        break;
      }
    case 1:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_2_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_2_angle_sin[number_group_stp];
        
        break;
      }
    case 2:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_3_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_3_angle_sin[number_group_stp];
        
        break;
      }
    case 3:
      {
        a_cos_fi = current_settings_prt.setpoint_mtz_4_angle_cos[number_group_stp];
        a_sin_fi = current_settings_prt.setpoint_mtz_4_angle_sin[number_group_stp];
        
        break;
      }
    default:
      {
        //Теоретично цього ніколи не мало б бути
        total_error_sw_fixed(54);
      }
    }
    a_sin_fi = -a_sin_fi; //З розділу: "Неймовірно, але факт", тобто що я не можу пояснити

    //Робимо доповорот на встановлений кут
    /*
    За розрахунком описаним при розрахунку діючих значень наші ортогональні є у ворматі (15 біт + знак) = 16-розряжне число
    */
    int amp_cos_U_fi;
    int amp_sin_U_fi;
  
    for (unsigned int i = 0; i < 3; i++)
    {
      unsigned int index_I, index_U;
      unsigned int index_I_ort, index_U_ort;
      switch (i)
      {
      case 0:
        {
          index_I = IM_IA;
          index_U = index_IM_UBC;

          index_I_ort = FULL_ORT_Ia;
          index_U_ort = index_FULL_ORT_Ubc;

          break;
        }
      case 1:
        {
          index_I = IM_IB;
          index_U = index_IM_UCA;

          index_I_ort = FULL_ORT_Ib;
          index_U_ort = index_FULL_ORT_Uca;

          break;
        }
      case 2:
        {
          index_I = IM_IC;
          index_U = index_IM_UAB;

          index_I_ort = FULL_ORT_Ic;
          index_U_ort = index_FULL_ORT_Uab;

          break;
        }
      default:
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed(60);
        }
      }

      unsigned int porig_Uxy;
      if (Uxy_bilshe_porogu[i] == 0) porig_Uxy = PORIG_CHUTLYVOSTI_VOLTAGE*KOEF_POVERNENNJA_SECTOR_BLK/100;
      else porig_Uxy = PORIG_CHUTLYVOSTI_VOLTAGE;
      unsigned int Uxy_bilshe_porogu_tmp = Uxy_bilshe_porogu[i] = (measurement[index_U] >= porig_Uxy);
      
      unsigned int porig_Ix;
      if (Ix_bilshe_porogu[i] == 0) porig_Ix = PORIG_CHUTLYVOSTI_CURRENT*KOEF_POVERNENNJA_SECTOR_BLK/100;
      else porig_Ix = PORIG_CHUTLYVOSTI_CURRENT;
      unsigned int Ix_bilshe_porogu_tmp = Ix_bilshe_porogu[i]  = (measurement[index_I] >= porig_Ix );

      if (
          (Uxy_bilshe_porogu_tmp != 0) &&
          (Ix_bilshe_porogu_tmp  != 0)
         )
      {
#define SIN_I   ortogonal_local_calc[2*index_I_ort    ]
#define COS_I   ortogonal_local_calc[2*index_I_ort + 1]
#define SIN_U   ortogonal_local_calc[2*index_U_ort    ]
#define COS_U   ortogonal_local_calc[2*index_U_ort + 1]

        amp_cos_U_fi = (COS_U*a_cos_fi - SIN_U*a_sin_fi) >> AMPLITUDA_FI_VAGA; //З поверненням до початкової розрядності ортогональних
        amp_sin_U_fi = (SIN_U*a_cos_fi + COS_U*a_sin_fi) >> AMPLITUDA_FI_VAGA; //З поверненням до початкової розрядності ортогональних

        //Вираховуємо COS і SIN кута різниці між (U[i]+fi) і I[i]
        int cos_fi, sin_fi;
        /*
        З наведених теоретичних роззрахунків у функції обрахунку діючих значень (calc_measurement())
        випливає, що максимальне значення ортогональних для струму може бути 0x6E51, для лінійної напруги 
        0x120FC, то добуток їх має дати 0x7C87B7BC  - 31 бітне число (плюс знак біту)
        Тобто ми акладаємося у тип int.
        */
        cos_fi = amp_cos_U_fi*COS_I + amp_sin_U_fi*SIN_I;
        sin_fi = -(amp_sin_U_fi*COS_I - amp_cos_U_fi*SIN_I);//З розділу: "Неймовірно, але факт", тобто що я не можу пояснити

#undef SIN_I
#undef COS_I
#undef SIN_U
#undef COS_U

        /*
        З вище наведених розрахункыв виходить, що COS і SIN різниці струму і напруги може бути 31-бітне число
        Векторні координати границь секторів задано 7-бінтими числами + знак
        Тоді щоб не отримати переповнення треба гарантувати, що розрядність різниці векторів
        COS і SIN буде = 31 - (7 + 1) = 23
        7 - це розрядність координат векторів границь секторів
        1 - це врахування що ми використовіємо формулу (ab+cd)
        */
        unsigned int order_cos, order_sin, max_order, shift = 0;
        order_cos = get_order(cos_fi);
        order_sin = get_order(sin_fi);
        if (order_cos > order_sin) max_order = order_cos; else max_order = order_sin;
        if (max_order > 23) shift = max_order - 23;
  
        /*
        Сам зсув я роблю після визначення квадранта - це хоч і втрата на розмірі програмного коду,
        але мало б підвищити точність визначення квадранту.
        Хоч може тут я перемудровую?..
        */

        //Визначення сектору
        int sin_fi1_minus_fi2;
        int *sector;
        if ((sin_fi >= 0) && (cos_fi >= 0))
        {
          //1-ий квадрант
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;
          
          if (sector_directional_mtz[mtz][i] != 1)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;
          
#define COS_SECTOR sector[0]    
#define SIN_SECTOR sector[1]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 <= 0) ?  1 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR   
        }
        else if ((sin_fi >= 0) && (cos_fi < 0))
        {
          //2-ий квадрант
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;

          if (sector_directional_mtz[mtz][i] != 2)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[2]    
#define SIN_SECTOR sector[3]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 >= 0) ?  2 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
        }
        else if ((sin_fi < 0) && (cos_fi < 0))
        {
          //3-ий квадрант
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;

          if (sector_directional_mtz[mtz][i] != 2)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[4]    
#define SIN_SECTOR sector[5]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 <= 0) ?  2 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
        }
        else
        {
          //4-ий квадрант
          cos_fi = cos_fi >> shift;
          sin_fi = sin_fi >> shift;

          if (sector_directional_mtz[mtz][i] != 1)
            sector = sector_1_mtz_tznp;
          else
            sector = sector_2_mtz_tznp;

#define COS_SECTOR sector[6]    
#define SIN_SECTOR sector[7]    

          sin_fi1_minus_fi2 = sin_fi*COS_SECTOR - cos_fi*SIN_SECTOR;
          sector_directional_mtz[mtz][i] = (sin_fi1_minus_fi2 >= 0) ?  1 : 0;
    
#undef COS_SECTOR    
#undef SIN_SECTOR    
        }
      }
      else
      {
        sector_directional_mtz[mtz][i] = 0;
      }
    }
  }
}
/*****************************************************/

/*****************************************************/
//Визначенням миттєвої потужності
/*****************************************************/
inline void calc_power(int ortogonal_local_calc[]) 
{
  unsigned int index_FULL_ORT_Uab, index_FULL_ORT_Ubc/*, index_FULL_ORT_Uca*/;
  if (TN1_TN2 == 1)
  {
    index_FULL_ORT_Uab = FULL_ORT_Uab2;
    index_FULL_ORT_Ubc = FULL_ORT_Ubc2;
//    index_FULL_ORT_Uca = FULL_ORT_Uca2;
  }
  else
  {
    index_FULL_ORT_Uab = FULL_ORT_Uab1;
    index_FULL_ORT_Ubc = FULL_ORT_Ubc1;
//    index_FULL_ORT_Uca = FULL_ORT_Uca1;
  }

  /*
  Розраховуємо дійсну і уявну частину потужності у компдексній площині
  
  .    .  .     .  .
  S = UabIa* - UbcIc*
  */
  
#define IA_SIN          ortogonal_local_calc[2*FULL_ORT_Ia + 1]
#define IA_COS          ortogonal_local_calc[2*FULL_ORT_Ia + 0]
#define UAB_SIN         ortogonal_local_calc[2*index_FULL_ORT_Uab + 1]
#define UAB_COS         ortogonal_local_calc[2*index_FULL_ORT_Uab + 0]
  
#define IC_SIN          ortogonal_local_calc[2*FULL_ORT_Ic + 1]
#define IC_COS          ortogonal_local_calc[2*FULL_ORT_Ic + 0]
#define UBC_SIN         ortogonal_local_calc[2*index_FULL_ORT_Ubc + 1]
#define UBC_COS         ortogonal_local_calc[2*index_FULL_ORT_Ubc + 0]
  
  long long Re_IaUab, Im_IaUab;
  if (measurement[IM_IA] >= PORIG_I_ENERGY)
  {
    Re_IaUab = UAB_COS*IA_COS + UAB_SIN*IA_SIN;
    Im_IaUab = UAB_SIN*IA_COS - UAB_COS*IA_SIN;
  
  }
  else
  {
    Re_IaUab = 0;
    Im_IaUab = 0;
  }

  long long Re_IcUbc, Im_IcUbc;
  if (measurement[IM_IC] >= PORIG_I_ENERGY)
  {  
    Re_IcUbc = UBC_COS*IC_COS + UBC_SIN*IC_SIN;
    Im_IcUbc = UBC_SIN*IC_COS - UBC_COS*IC_SIN;
  }
  else
  {
    Re_IcUbc = 0;
    Im_IcUbc = 0;
  }
#undef IA_SIN
#undef IA_COS
#undef UAB_SIN
#undef UAB_COS
  
#undef IC_SIN
#undef IC_COS
#undef UBC_SIN
#undef UBC_COS
  
  long long P_adc_x16 = Re_IaUab - Re_IcUbc; /*  активна потужність у поділках АЦП і з вхідними сигналами, які є підсилені у 16 разів*/
  long long Q_adc_x16 = Im_IaUab - Im_IcUbc; /*реактивна потужність у поділках АЦП і з вхідними сигналами, які є підсилені у 16 разів*/
  
  /*
  Коли перемножимо на коефіцієнти MNOGNYK_I_DIJUCHE_FLOAT і MNOGNYK_U_DIJUCHE_FLOAT,
  а потім поділемо на 16x16 - то отримаємо мАхмВ = мкВт/мкВАр/мкВА
  
  Ми хочемо тримати число у мВт/мВАр/мВА. 
  
  Тоді треба поділити ще на 1000
  */
  
  //Активна потужність
  double P_float = ((double)P_adc_x16) * ((double)(MNOGNYK_I_DIJUCHE_DOUBLE*MNOGNYK_U_DIJUCHE_DOUBLE/(1000.0*16.0*16.0)));
  int P_tmp = (int)(P_float); // з точністю до мВт

  //Реактивна потужність
  double Q_float = ((double)Q_adc_x16) * ((double)(MNOGNYK_I_DIJUCHE_DOUBLE*MNOGNYK_U_DIJUCHE_DOUBLE/(1000.0*16.0*16.0)));
  int Q_tmp  = (int)(Q_float); // з точністю до мВАр

  unsigned int bank_for_enegry_tmp = bank_for_enegry;  
  int *energy_array;
  
  //Активна потужність
  if (P_tmp >= 0) energy_array = P_plus;
  else energy_array = P_minus;
  *(energy_array + bank_for_enegry_tmp) += abs(P_tmp);
  
  //Реактивна потужність
  if     ((Q_tmp >= 0) && (P_tmp >= 0)) energy_array = Q_1q;
  else if((Q_tmp >= 0) && (P_tmp <  0)) energy_array = Q_2q;
  else if((Q_tmp <  0) && (P_tmp <  0)) energy_array = Q_3q;
  else                                  energy_array = Q_4q;
  *(energy_array + bank_for_enegry_tmp) += abs(Q_tmp);
  
  if (++lichylnyk_1s_po_20ms >= MAIN_FREQUENCY)
  {
    if (lichylnyk_1s_po_20ms > MAIN_FREQUENCY)
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed(64);
    }

    //Переключаємо банк для збереження нових даних s обнулюємо значення у новому банку
    bank_for_enegry = bank_for_enegry_tmp = (bank_for_enegry_tmp + 1) & 0x1;
    P_plus[bank_for_enegry_tmp] = 0;
    P_minus[bank_for_enegry_tmp] = 0;
    Q_1q[bank_for_enegry_tmp] = 0;
    Q_2q[bank_for_enegry_tmp] = 0;
    Q_3q[bank_for_enegry_tmp] = 0;
    Q_4q[bank_for_enegry_tmp] = 0;
    
    lichylnyk_1s_po_20ms = 0;
    if (periodical_tasks_CALC_ENERGY_DATA == 0)
    {
      //Скидаємо повідомлення у слові діагностики
      _SET_BIT(clear_diagnostyka, LOSE_ENERGY_DATA);
    }
    else
    {
      //Виствляємо повідомлення у слові діагностики
      _SET_BIT(set_diagnostyka, LOSE_ENERGY_DATA);
    }
    periodical_tasks_CALC_ENERGY_DATA = true;
  }
  
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

  /***/
  //Вибір звідки брати напругу (ТН1 чи ТН2)
  /***/
  TN1_TN2 = choose_tn1_tn2(active_functions);
  /***/

  /***/
  //Фазочутливий елемент для МТЗ (всіх ступенів)
  /***/
  directional_mtz(ortogonal_calc, number_group_stp);
  /***/

  /***/
  //Розраховуємо струм зворотньої послідовності
  /***/
  velychyna_zvorotnoi_poslidovnosti(ortogonal_calc);
  /***/
  
  if(++number_inputs_for_fix_one_period >= 20)
  {
    /***/
    //Розрахунок миттєвих потужностей (раз на 20мс)
    /***/
    calc_power(ortogonal_calc);
    /***/
    
    number_inputs_for_fix_one_period = 0;
  }
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
//Вираховування часової витримки  для залежної МТЗ2 (універсальна формула)
/*
-------------------------------------------------
Т(c) = K*Tу/( ((I/Iу)^alpha) - 1 )
-------------------------------------------------
*/
/*****************************************************/
inline int timeout_dependent_general(unsigned int i, unsigned int number_group_stp)
{
  int timeout_result = 0;
  int type_mtz2_tmp = current_settings_prt.type_mtz2;
  
  if (
      (type_mtz2_tmp >= TYPE_MTZ_DEPENDENT_A) &&
      (type_mtz2_tmp <= TYPE_MTZ_DEPENDENT_RTV_I)
     )   
  {
    unsigned int Iust = current_settings_prt.setpoint_mtz_2[number_group_stp];
    if (i > Iust)
    {
      register float timeout = (((float)current_settings_prt.timeout_mtz_2[number_group_stp])/1000.0f);
      register float timeout_dependent = 0;
      
      if (
          (type_mtz2_tmp >= TYPE_MTZ_DEPENDENT_A) &&
          (type_mtz2_tmp <= TYPE_MTZ_DEPENDENT_C)
         )   
      {
        register float K;
        register float alpha;

        switch (type_mtz2_tmp)
        {
          case TYPE_MTZ_DEPENDENT_A:
          {
            K = 0.14f;
            alpha = 0.02f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_B:
          {
            K = 13.5f;
            alpha = 1.0f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_C:
          {
            K = 80.0f;
            alpha = 2.0f;
            break;
          }
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed(42);
          }
        }
    
        register float I_div_Iusy = ((float)i)/((float)Iust);
        register float rising_to_power = powf(I_div_Iusy, alpha);
        if (
            (isnan(rising_to_power) != 0) || 
            (isinf(rising_to_power) != 0) ||
            (rising_to_power <= 1)
          )
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed(57);
        }
  
        timeout_dependent = K*timeout/(rising_to_power - 1);
      }
      else if (
               (type_mtz2_tmp >= TYPE_MTZ_DEPENDENT_RT_80) &&
               (type_mtz2_tmp <= TYPE_MTZ_DEPENDENT_RTV_I)
              )   
      {
        register float I_div_Iusy = ((float)i)/((float)Iust);
        I_div_Iusy = I_div_Iusy - 1.0f;

        register float K;
        register float alpha;

        switch (type_mtz2_tmp)
        {
        case TYPE_MTZ_DEPENDENT_RT_80:
          {
            K = 20.0f;
            alpha = 1.8f;
          
            I_div_Iusy /= 6.0f;
            break;
          }
        case TYPE_MTZ_DEPENDENT_RTV_I:
          {
            K = 30.0f;
            alpha = 3.0f;
            break;
          } 
        default:
          {
            //Теоретично цього ніколи не мало б бути
            total_error_sw_fixed(87);
          }
        }
      
        register float rising_to_power = powf(I_div_Iusy, alpha);
        if (
            (isnan(rising_to_power) != 0) || 
            (isinf(rising_to_power) != 0) ||
            (rising_to_power <= 0)
           )
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed(86);
        }
      
        timeout_dependent = 1.0f/(K*rising_to_power)+ timeout;
      }

      int timeout_dependent_int = (int)timeout_dependent;
      if (timeout_dependent_int > (TIMEOUT_MTZ2_MAX/1000)) timeout_dependent = (TIMEOUT_MTZ2_MAX/1000);
      else if (timeout_dependent_int < 0) timeout_dependent = 0;
    
      timeout_result = (int)(timeout_dependent*1000.0f);
    }
    else timeout_result = TIMEOUT_MTZ2_MAX;
  }
  
  return timeout_result;
}
/*****************************************************/

/*****************************************************/
// МТЗ
/*****************************************************/
inline void mtz_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  unsigned int tmp_value;
  
  unsigned int po_mtz_x;
  unsigned int direction_ABC_tmp;
  unsigned int po_mtzn_x_vpered_setpoint;
  unsigned int po_mtzn_x_nazad_setpoint;
  unsigned int po_block_u_mtzn_x_setpoint;
  unsigned int po_i_ncn_setpoint;
  unsigned int po_u_ncn_setpoint;
  unsigned int po_u_mtzpn_x_setpoint;
  unsigned int po_mtzpn_x_setpoint;
  
  /******Неисправность цепей напряжения для 4-х ступеней*******/
  _Bool ctr_mtz_nespr_kil_napr = (current_settings_prt.control_mtz & CTR_MTZ_NESPR_KIL_NAPR) != 0; //Неиспр. Напр. Вкл. (М)
  
  //ПО Iнцн
  po_i_ncn_setpoint = previous_state_mtz_po_incn ?
            i_nom_const * KOEF_POVERNENNJA_MTZ_I_DOWN / 100 :
            i_nom_const;
  
  previous_state_mtz_po_incn = ((measurement[IM_IA] <= po_i_ncn_setpoint) &&
                                (measurement[IM_IB] <= po_i_ncn_setpoint) &&
                                (measurement[IM_IC] <= po_i_ncn_setpoint));
  
  //ПО Uнцн
  po_u_ncn_setpoint = previous_state_mtz_po_uncn ?
            u_linear_nom_const * U_DOWN / 100 :
            u_linear_nom_const;
  
  previous_state_mtz_po_uncn = ((measurement[index_IM_UAB] <= po_u_ncn_setpoint) ||
                                (measurement[index_IM_UBC] <= po_u_ncn_setpoint) ||
                                (measurement[index_IM_UCA] <= po_u_ncn_setpoint));
  
  ctr_mtz_nespr_kil_napr = ctr_mtz_nespr_kil_napr && previous_state_mtz_po_incn && previous_state_mtz_po_uncn; //Неисправность цепей напряжения (_AND3)
  
  //Неисправность цепей напряжения
  if (ctr_mtz_nespr_kil_napr)
    _SET_BIT(p_active_functions, RANG_NCN_MTZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_NCN_MTZ);
  /******Неисправность цепей напряжения для 4-х ступеней*******/
  
  for (int mtz_level = 0; mtz_level < NUMBER_LEVEL_MTZ; mtz_level++) 
  {
    //М
    /*Проверяем тип МТЗ*/
    tmp_value = (*type_mtz_arr[mtz_level] == 1)                                  << 0; //Направленная
    tmp_value |= (*type_mtz_arr[mtz_level] == 2)                                 << 1; //С пуском
    tmp_value |= (*type_mtz_arr[mtz_level] == 0)                                 << 2; //Простая
    tmp_value |= (
                  (mtz_level == MTZ_LEVEL2)
                  && (*type_mtz_arr[mtz_level] >= TYPE_MTZ_DEPENDENT_A)
                  && (*type_mtz_arr[mtz_level] <= TYPE_MTZ_DEPENDENT_RTV_I) 
                 )                                                               << 3; //Зависимая (если mtz_level == 1 (2-я ступень МТЗ))
    
    /******ПО МТЗх***********************/
    //Уставка ПО МТЗх с учетом гистерезиса
    po_mtz_x = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]) != 0) ?
            *(setpoint_mtz[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            *(setpoint_mtz[mtz_level] + number_group_stp);
    
    tmp_value |= ((measurement[IM_IA] >= po_mtz_x) ||
          (measurement[IM_IB] >= po_mtz_x) ||
          (measurement[IM_IC] >= po_mtz_x)) << 4; //ПО МТЗх
    /******ПО МТЗх***********************/
    
    //М
    tmp_value |= ((current_settings_prt.control_mtz & mtz_const_menu_settings_prt[mtz_level][CTR_MTZ]) != 0) << 5; //МТЗх Вкл.
    //ДВ
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_BLOCK_MTZ]) == 0) << 6; //Блокировка МТЗх
    //М
    tmp_value |= ((current_settings_prt.control_mtz & mtz_const_menu_settings_prt[mtz_level][CTR_MTZ_VPERED]) != 0) << 7; //МТЗНх: Вкл. прям
    tmp_value |= ((current_settings_prt.control_mtz & mtz_const_menu_settings_prt[mtz_level][CTR_MTZ_NAZAD]) != 0) << 8; //МТЗНх: Вкл. обр
    
    /******Сектор МТЗНх Вперед/Назад**********/
    //Проверка направленности фаз А,В,С
    direction_ABC_tmp = (sector_directional_mtz[mtz_level][PHASE_A_INDEX] == MTZ_VPERED) << 0; //Проверка направленности фазы А вперед
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_B_INDEX] == MTZ_VPERED) << 1; //Проверка направленности фазы B вперед
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_C_INDEX] == MTZ_VPERED) << 2; //Проверка направленности фазы C вперед
    
    _OR3(direction_ABC_tmp, 0, direction_ABC_tmp, 1, direction_ABC_tmp, 2, direction_ABC_tmp, 3);
    
    //Сектор МТЗНх Вперед
    if (_GET_OUTPUT_STATE(direction_ABC_tmp, 3))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_VPERED_MTZN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_VPERED_MTZN]);
    
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_A_INDEX] == MTZ_NAZAD) << 4; //Проверка направленности фазы А назад
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_B_INDEX] == MTZ_NAZAD) << 5; //Проверка направленности фазы B назад
    direction_ABC_tmp |= (sector_directional_mtz[mtz_level][PHASE_C_INDEX] == MTZ_NAZAD) << 6; //Проверка направленности фазы C назад
    
    _OR3(direction_ABC_tmp, 4, direction_ABC_tmp, 5, direction_ABC_tmp, 6, direction_ABC_tmp, 7);
    
    //Сектор МТЗНх Назад
    if (_GET_OUTPUT_STATE(direction_ABC_tmp, 7))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_NAZAD_MTZN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_SECTOR_NAZAD_MTZN]);
    
    //Уставка ПО МТЗН1 прям. с учетом гистерезиса
    po_mtzn_x_vpered_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]) != 0) ?
            *(setpoint_mtz_n_vpered[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            *(setpoint_mtz_n_vpered[mtz_level] + number_group_stp);
    
    direction_ABC_tmp |= (measurement[IM_IA] >= po_mtzn_x_vpered_setpoint) << 8; //Сравниваем с уставкой тока по фазе А (вперед)
    direction_ABC_tmp |= (measurement[IM_IB] >= po_mtzn_x_vpered_setpoint) << 9; //Сравниваем с уставкой тока по фазе B (вперед)
    direction_ABC_tmp |= (measurement[IM_IC] >= po_mtzn_x_vpered_setpoint) << 10; //Сравниваем с уставкой тока по фазе C (вперед)
    
    //Уставка ПО МТЗН1 прям. с учетом гистерезиса
    po_mtzn_x_nazad_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]) != 0) ?
            *(setpoint_mtz_n_nazad[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100 :
            *(setpoint_mtz_n_nazad[mtz_level] + number_group_stp);
    
    direction_ABC_tmp |= (measurement[IM_IA] >= po_mtzn_x_nazad_setpoint) << 11; //Сравниваем с уставкой тока по фазе А (назад)
    direction_ABC_tmp |= (measurement[IM_IB] >= po_mtzn_x_nazad_setpoint) << 12; //Сравниваем с уставкой тока по фазе B (назад)
    direction_ABC_tmp |= (measurement[IM_IC] >= po_mtzn_x_nazad_setpoint) << 13; //Сравниваем с уставкой тока по фазе C (назад)
    
    _AND2(direction_ABC_tmp, 8, direction_ABC_tmp, 0, direction_ABC_tmp, 14);
    _AND2(direction_ABC_tmp, 9, direction_ABC_tmp, 1, direction_ABC_tmp, 15);
    _AND2(direction_ABC_tmp, 10, direction_ABC_tmp, 2, direction_ABC_tmp, 16);
    
    _AND2(direction_ABC_tmp, 11, direction_ABC_tmp, 4, direction_ABC_tmp, 17);
    _AND2(direction_ABC_tmp, 12, direction_ABC_tmp, 5, direction_ABC_tmp, 18);
    _AND2(direction_ABC_tmp, 13, direction_ABC_tmp, 6, direction_ABC_tmp, 19);
    
    _OR3(direction_ABC_tmp, 14, direction_ABC_tmp, 15, direction_ABC_tmp, 16, tmp_value, 12);
    _OR3(direction_ABC_tmp, 17, direction_ABC_tmp, 18, direction_ABC_tmp, 19, tmp_value, 13);
    /******Сектор МТЗНх Вперед/Назад**********/
    
    /******ПО U блок. МТЗНх***********************/
    //Уставка ПО U блок. МТЗНх с учетом гистерезиса
    po_block_u_mtzn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN) != 0) ?
           PORIG_CHUTLYVOSTI_VOLTAGE :
           PORIG_CHUTLYVOSTI_VOLTAGE * U_DOWN / 100;
    
    tmp_value |= ((measurement[index_IM_UAB] <= po_block_u_mtzn_x_setpoint) ||
                  (measurement[index_IM_UBC] <= po_block_u_mtzn_x_setpoint) ||
                  (measurement[index_IM_UCA] <= po_block_u_mtzn_x_setpoint)) << 14; //ПО U блок. МТЗНх
    
    //ПО U блок. МТЗНх
    if (_GET_OUTPUT_STATE(tmp_value, 14))
      _SET_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_BLOCK_U_MTZN);
    /******ПО U блок. МТЗНх***********************/
    
    //Неисправность цепей напряжения для ступени МТЗх
    tmp_value |= (
                   !(_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]) != 0 ||
                     _CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD])  != 0 ||
                     _CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN])     != 0)
                   &&
                   ctr_mtz_nespr_kil_napr
                 ) << 15;
    
    /******ПО U МТЗПНх***********************/
    po_u_mtzpn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_U_MTZPN]) != 0) ?
            *(setpoint_mtz_U[mtz_level] + number_group_stp) * U_DOWN / 100:
            *(setpoint_mtz_U[mtz_level] + number_group_stp) ;
    
    tmp_value |= ((measurement[index_IM_UAB] <= po_u_mtzpn_x_setpoint) ||
                  (measurement[index_IM_UBC] <= po_u_mtzpn_x_setpoint) ||
                  (measurement[index_IM_UCA] <= po_u_mtzpn_x_setpoint)) << 16; //ПО U МТЗПНх
    
    //ПО U МТЗПНх
    if (_GET_OUTPUT_STATE(tmp_value, 16))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_U_MTZPN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_U_MTZPN]);
    /******ПО U МТЗПНх***********************/
    
    /******ПО МТЗПНх***********************/
    po_mtzpn_x_setpoint = (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]) != 0) ?
            *(setpoint_mtz_po_napruzi[mtz_level] + number_group_stp) * KOEF_POVERNENNJA_MTZ_I_UP / 100:
            *(setpoint_mtz_po_napruzi[mtz_level] + number_group_stp);
    
    tmp_value |= ((measurement[IM_IA] >= po_mtzpn_x_setpoint)   ||
                  (measurement[IM_IB] >= po_mtzpn_x_setpoint) ||
                  (measurement[IM_IC] >= po_mtzpn_x_setpoint)) << 17; //ПО МТЗПНх
    /******ПО МТЗПНх***********************/
    
    if ((mtz_level == MTZ_LEVEL2) || (mtz_level == MTZ_LEVEL3))
    { 
      //тільки для 2-ої або 3-ої ступеней
      //ДВ
      tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_BLOCK_USK_MTZ]) == 0) << 9; //Блокировка ускорения МТЗ 2
      //М
      tmp_value |= ((current_settings_prt.control_mtz & mtz_prysk_const_menu_settings_prt[mtz_level - MTZ_LEVEL2][CTR_MTZ_PRYSKORENNJA]/*CTR_MTZ_2_PRYSKORENA*/) != 0) << 10; //МТЗ2 Ускоренная
      //ДВ
      tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) << 18; //Положение ВВ
      //M
      tmp_value |= ((current_settings_prt.control_mtz & mtz_prysk_const_menu_settings_prt[mtz_level - MTZ_LEVEL2][CTR_MTZ_PRYSKORENA]/*CTR_MTZ_2_PRYSKORENNJA*/) != 0) << 11; //Ускорение МТЗ2 вкл.
    }
    
    if (_GET_OUTPUT_STATE(tmp_value, 15)) 
    {
      //Если зафиксирована неисправность цепей напряжения для ступени МТЗх
      _CLEAR_STATE(tmp_value, 16); //Очистка ПО U МТЗПНх
    }
    
    _OR2_INVERTOR(tmp_value, 14, tmp_value, 15, tmp_value, 14);
    
    if (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]) != 0) 
    {
      _SET_STATE(tmp_value, 19);
    } 
    else 
    {
      if (mtz_level == 1)
      {
        //только для 2-ой ступени при формировании 19-го сигнала будет учитываться
        //сигнал "Зависимая" (tmp_value 3)
        if (_GET_OUTPUT_STATE(tmp_value, 14)) 
        {
          _OR3(tmp_value, 2, tmp_value, 3, tmp_value, 15, tmp_value, 19);
        }
        else 
        {
          _OR4(tmp_value, 0, tmp_value, 2, tmp_value, 3, tmp_value, 15, tmp_value, 19);
        }
      }
      else 
      {
        if (_GET_OUTPUT_STATE(tmp_value, 14))
        {
          _OR2(tmp_value, 2, tmp_value, 15, tmp_value, 19);
        } 
        else
        {
          _OR3(tmp_value, 0, tmp_value, 2, tmp_value, 15, tmp_value, 19);
        }
      }
    }
    
    _AND2(tmp_value, 5, tmp_value, 6, tmp_value, 20);
    
    //ПО МТЗх
    _AND3(tmp_value, 19, tmp_value, 4, tmp_value, 20, tmp_value, 21);
    if (_GET_OUTPUT_STATE(tmp_value, 21))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]);
    
    //ПО МТЗНх вперед
    _AND5(tmp_value, 0, tmp_value, 20, tmp_value, 7, tmp_value, 12, tmp_value, 14, tmp_value, 22);
    if (_GET_OUTPUT_STATE(tmp_value, 22))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]);
    
    //ПО МТЗНх назад
    _AND5(tmp_value, 0, tmp_value, 20, tmp_value, 8, tmp_value, 13, tmp_value, 14, tmp_value, 23);
    if (_GET_OUTPUT_STATE(tmp_value, 23)) 
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]);
    
    //ПО МТЗПНх
    _AND4(tmp_value, 1, tmp_value, 20, tmp_value, 16, tmp_value, 17, tmp_value, 24);
    if (_GET_OUTPUT_STATE(tmp_value, 24))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]);
    
    if ((mtz_level != MTZ_LEVEL2) && (mtz_level != MTZ_LEVEL3))
    { 
      //Для всіх ступенів крів 2-ої і 3-ої
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ], *(timeout_mtz[mtz_level] + number_group_stp), tmp_value, 21, tmp_value, 25);
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ_N_VPERED], *(timeout_mtz_n_vpered[mtz_level] + number_group_stp), tmp_value, 22, tmp_value, 26);
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ_N_NAZAD], *(timeout_mtz_n_nazad[mtz_level] + number_group_stp), tmp_value, 23, tmp_value, 27);
      _TIMER_T_0(mtz_tmr_const[mtz_level][INDEX_TIMER_MTZ_PO_NAPRUZI], *(timeout_mtz_po_napruzi[mtz_level] + number_group_stp), tmp_value, 24, tmp_value, 28);
      _OR4(tmp_value, 25, tmp_value, 26, tmp_value, 27, tmp_value, 28, tmp_value, 31);
    }
    else 
    {
      unsigned int tmp = 0;
      _AND2(tmp_value, 18, tmp_value, 11, tmp, 0);
      _TIMER_IMPULSE(mtz_prysk_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_VVID_PR]/*INDEX_TIMER_MTZ2_VVID_PR*/, *(timeout_mtz_vvid_prysk[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_vvid_pr[number_group_stp]*/, temp_states_for_mtz, (mtz_level - MTZ_LEVEL2), tmp, 0, tmp, 1);
      _OR2(tmp, 1, tmp_value, 10, tmp, 2);
      _AND2(tmp_value, 9, tmp, 2, tmp, 3);
      _INVERTOR(tmp, 3, tmp, 4);
      _INVERTOR(tmp_value, 3, tmp, 23); /*Добавлено Тарасом у Богданову програму*/
      _AND3(tmp, 4, tmp_value, 3, tmp_value, 21, tmp, 5);
      _AND2(tmp, 3, tmp_value, 21, tmp, 6);
      _AND3(tmp, 4, tmp_value, 21, tmp, 23, tmp, 7); /*Модифіковано Тарасом у Богдановій програмі*/
      _AND2(tmp, 4, tmp_value, 22, tmp, 8);
      _AND2(tmp, 3, tmp_value, 22, tmp, 9);
      _AND2(tmp, 4, tmp_value, 23, tmp, 10);
      _AND2(tmp, 3, tmp_value, 23, tmp, 11);
      _AND2(tmp, 4, tmp_value, 24, tmp, 12);
      _AND2(tmp, 3, tmp_value, 24, tmp, 13);
      
      unsigned int i_max = measurement[IM_IA];
      if (i_max < measurement[IM_IB]) i_max = measurement[IM_IB];
      if (i_max < measurement[IM_IC]) i_max = measurement[IM_IC];
      _TIMER_T_0_LOCK(INDEX_TIMER_MTZ2_DEPENDENT, timeout_dependent_general(i_max, number_group_stp), tmp, 5, p_global_trigger_state_mtz2, 0);
      _TIMER_T_0(mtz_prysk_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_PR]/*INDEX_TIMER_MTZ2_PR*/, *(timeout_mtz_prysk[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_pr[number_group_stp]*/, tmp, 6, tmp, 15);
      _TIMER_T_0(mtz_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ]/*INDEX_TIMER_MTZ2*/, *(timeout_mtz[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2[number_group_stp]*/, tmp, 7, tmp, 16);
      _TIMER_T_0(mtz_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_N_VPERED]/*INDEX_TIMER_MTZ2_N_VPERED*/, *(timeout_mtz_n_vpered[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_n_vpered[number_group_stp]*/, tmp, 8, tmp, 17);
      _TIMER_T_0(mtz_prysk_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_N_VPERED_PR]/*INDEX_TIMER_MTZ2_N_VPERED_PR*/, *(timeout_mtz_n_vpered_prysk[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_n_vpered_pr[number_group_stp]*/, tmp, 9, tmp, 18);
      _TIMER_T_0(mtz_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_N_NAZAD]/*INDEX_TIMER_MTZ2_N_NAZAD*/, *(timeout_mtz_n_nazad[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_n_nazad[number_group_stp]*/, tmp, 10, tmp, 19);
      _TIMER_T_0(mtz_prysk_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_N_NAZAD_PR]/*INDEX_TIMER_MTZ2_N_NAZAD_PR*/, *(timeout_mtz_n_nazad_prysk[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_n_nazad_pr[number_group_stp]*/, tmp, 11, tmp, 20);
      _TIMER_T_0(mtz_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_PO_NAPRUZI]/*INDEX_TIMER_MTZ2_PO_NAPRUZI*/, *(timeout_mtz_po_napruzi[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_po_napruzi[number_group_stp]*/, tmp, 12, tmp, 21);
      _TIMER_T_0(mtz_prysk_tmr_const[mtz_level - MTZ_LEVEL2][INDEX_TIMER_MTZ_PO_NAPRUZI_PR]/*INDEX_TIMER_MTZ2_PO_NAPRUZI_PR*/, *(timeout_mtz_po_napruzi_prysk[mtz_level - MTZ_LEVEL2] + number_group_stp)/*current_settings_prt.timeout_mtz_2_po_napruzi_pr[number_group_stp]*/, tmp, 13, tmp, 22);
      
      _OR6(p_global_trigger_state_mtz2, 0, tmp, 15, tmp, 16, tmp, 17, tmp, 18, tmp, 19, tmp_value, 29);
      _OR3(tmp, 20, tmp, 21, tmp, 22, tmp_value, 30);
      _OR2(tmp_value, 29, tmp_value, 30, tmp_value, 31);
    }
    
    //Сраб.МТЗх
    if (_GET_OUTPUT_STATE(tmp_value, 31))
      _SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_MTZ]);
    else
      _CLEAR_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_MTZ]);
  }
}
/*****************************************************/

/*****************************************************/
// ЗДЗ
/*****************************************************/
inline void zdz_handler(volatile unsigned int *p_active_functions)
{
  unsigned int tmp_value;
  //M
  tmp_value = ((current_settings_prt.control_zdz & CTR_ZDZ_STARTED_FROM_MTZ1) != 0) << 0; //Пуск от МТЗ1 : Вкл/Откл
  tmp_value |= ((current_settings_prt.control_zdz & CTR_ZDZ_STARTED_FROM_MTZ2) != 0) << 1; //Пуск от МТЗ2 : Вкл/Откл
  tmp_value |= ((current_settings_prt.control_zdz & CTR_ZDZ_STARTED_FROM_MTZ3) != 0) << 2; //Пуск от МТЗ3 : Вкл/Откл
  tmp_value |= ((current_settings_prt.control_zdz & CTR_ZDZ_STARTED_FROM_MTZ4) != 0) << 3; //Пуск от МТЗ4 : Вкл/Откл
  
  for (int mtz_level = 0; mtz_level < NUMBER_LEVEL_MTZ; mtz_level++) {
    //берем не из active потому что сигналы сформировались на предыдущем шаге mtz_handler()
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZ]) != 0) << (4 + mtz_level); //ПО МТЗx
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_VPERED]) != 0) << (8 + mtz_level); //ПО МТЗНх вперед
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZN_NAZAD]) != 0) << (12 + mtz_level);  //ПО МТЗНх назад
    tmp_value |= (_CHECK_SET_BIT(p_active_functions, mtz_settings_prt[mtz_level][RANG_PO_MTZPN]) != 0) << (16 + mtz_level); //ПО МТЗПНх
  }
  //М
  tmp_value |= ((current_settings_prt.control_zdz & CTR_ZDZ_STATE) != 0) << 20;
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_ZDZ_VID_DV) != 0) << 21; //Пуск ЗДЗ от ДВ
  
  _OR4(tmp_value, 4, tmp_value, 8, tmp_value, 12, tmp_value, 16, tmp_value, 22);
  _OR4(tmp_value, 5, tmp_value, 9, tmp_value, 13, tmp_value, 17, tmp_value, 23);
  _OR4(tmp_value, 6, tmp_value, 10, tmp_value, 14, tmp_value, 18, tmp_value, 24);
  _OR4(tmp_value, 7, tmp_value, 11, tmp_value, 15, tmp_value, 19, tmp_value, 25);
  
  _AND2(tmp_value, 0, tmp_value, 22, tmp_value, 26);
  _AND2(tmp_value, 1, tmp_value, 23, tmp_value, 27);
  _AND2(tmp_value, 2, tmp_value, 24, tmp_value, 28);
  _AND2(tmp_value, 3, tmp_value, 25, tmp_value, 29);
  
  _OR4(tmp_value, 26, tmp_value, 27, tmp_value, 28, tmp_value, 29, tmp_value, 30);
  
  unsigned int tmp_value2 = 0;
  _AND3(tmp_value, 30, tmp_value, 21, tmp_value, 20, tmp_value2, 0);
  _OR4_INVERTOR(tmp_value, 0, tmp_value, 1, tmp_value, 2, tmp_value, 3, tmp_value2, 1);
  _AND3(tmp_value2, 1, tmp_value, 21, tmp_value, 20, tmp_value2, 2);
  
  _OR2(tmp_value2, 0, tmp_value2, 2, tmp_value2, 3);
  
  //Сраб. ЗДЗ
  if (_GET_OUTPUT_STATE(tmp_value2, 3))
    _SET_BIT(p_active_functions, RANG_ZDZ);
  else
    _CLEAR_BIT(p_active_functions, RANG_ZDZ);
}
/*****************************************************/

/*****************************************************/
// ЗОП(КОФ)
/*****************************************************/
inline void zop_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /*******************************/
  //Фуксуємо у локальній змінній текучі струми зворотньої послідовності і прямоїпослідовності
  /*******************************/
  unsigned int i2_current = measurement[IM_I2], i1_current = measurement[IM_I1];
  /*******************************/
  
  /*******************************/
  //1 ступінь ЗОП(КОФ)
  /*******************************/
  if ((current_settings_prt.control_zop & CTR_ZOP_STATE) != 0)
  {
    //1 ступінь ЗОП(КОФ) включена
    unsigned int setpoint; //уставка - з якою зрівнюється вимірювальна величина
    unsigned int setpoint_i1, setpoint_i2;
    unsigned int previous_state_po_zop1;
    
    if (i1_bilshe_porogu == 0) setpoint_i1 = MIN_LIMIT_FOR_I1_AND_I2*KOEF_POVERNENNJA_ZOP_BLK/100;
    else setpoint_i1 = MIN_LIMIT_FOR_I1_AND_I2;
    unsigned int i1_bilshe_porogu_tmp = i1_bilshe_porogu = (i1_current >= setpoint_i1);
    
    if (i2_bilshe_porogu == 0) setpoint_i2 = MIN_LIMIT_FOR_I1_AND_I2*KOEF_POVERNENNJA_ZOP_BLK/100;
    else setpoint_i2 = MIN_LIMIT_FOR_I1_AND_I2;
    unsigned int i2_bilshe_porogu_tmp = i2_bilshe_porogu = (i2_current >= setpoint_i2);
    
    //Якщо ПО ЗОП(КОФ) ще не активне, то треба працювати по устаці спацювання - уставці, яка вводиться як основна з системи меню чи верхнього рівня
    //Якщо ПО ЗОП(КОФ) вже спрацювало, то треба працювати по уставці відпускання - береться процент від основної утанки по коефіцієнту повернення
    if(( previous_state_po_zop1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_ZOP) ) == 0 )
    {
      //Працюємо по утавці спрацювання
      setpoint = current_settings_prt.setpoint_zop[number_group_stp];
    }
    else
    {
      //Працюємо по утавці відпускання
      setpoint = current_settings_prt.setpoint_zop[number_group_stp]*KOEF_POVERNENNJA_GENERAL/100;
    }
    
    //Виставляємо, або скидаємо сигнал "ПО КОФ"
    /*
    I2
    -- >= K
    I1
    оскільки величина устаки у нас є перемножена на 1000, ,бо 0,010 відповідає 10, а 1,000 відповідає 1000,
    то K*1000 = SETPOINT =>
        SETPOINT
    K = --------  =>
         1000
    I2    SETPOINT
    -- >= ---------
    I1     1000
    
    струми зворотньої і прямої послідовності не треба ні нащо перемножувати, бо вони є у однакових величинах (міліампери), а нас цікавить їх відношення
    
    (I2*1000) >= (SETPOINT*I1)
    */
    if (
        (i1_bilshe_porogu_tmp != 0) &&
        (i2_bilshe_porogu_tmp != 0) &&
        ((i2_current*1000) >= (i1_current*setpoint))                            && 
        (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_ZOP) == 0)
       )
    {
      //Існує умова активного пускового органу зворотньої послідовності
      if(previous_state_po_zop1 == 0)
      {
        //Встановлюємо сигнал "ПО КОФ"
        _SET_BIT(p_active_functions, RANG_PO_ZOP);
      
        //Запускаємо таймер ЗОП(КОФ), якщо він ще не запущений
        global_timers[INDEX_TIMER_ZOP] = 0;
      }
    }
    else 
    {
      //Не існує умови активного пускового органу зворотньої послідовності
      if(previous_state_po_zop1 != 0)
      {
        //Скидаємо сигнал "ПО КОФ"
        _CLEAR_BIT(p_active_functions, RANG_PO_ZOP);
        //Це є умовою також скидання сигналу "Сраб. КОФ"
        _CLEAR_BIT(p_active_functions, RANG_ZOP);
        //Якщо таймер ще не скинутий? то скидаємо його
        if ( global_timers[INDEX_TIMER_ZOP] >=0) global_timers[INDEX_TIMER_ZOP] = -1;
      }
    }
    
    if(global_timers[INDEX_TIMER_ZOP] >= current_settings_prt.timeout_zop[number_group_stp])
    {
      //Якщо витримана Витримка ЗОП(КОФ) то встановлюємо сигнал "Сраб. КОФ"
      _SET_BIT(p_active_functions, RANG_ZOP);

      //Скидаємо таймер ЗОП(КОФ)
      global_timers[INDEX_TIMER_ZOP] = -1;
    }
  }
  else
  {
    //Треба скинути всі таймери і сигнали, які за 1 ступінь ЗОП(КОФ) відповідають
    _CLEAR_BIT(p_active_functions, RANG_PO_ZOP);
    _CLEAR_BIT(p_active_functions, RANG_ZOP);
    global_timers[INDEX_TIMER_ZOP] = -1;

  }  
}
/*****************************************************/

/*****************************************************/
// ЗНМИН1
/*****************************************************/
void umin1_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  _Bool previous_state_po_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN1) > 0;
  _Bool previous_state_po_ublk_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1) > 0;
  _Bool previous_state_po_iblk_umin1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN1) > 0;
  
  unsigned int setpoint1 = previous_state_po_umin1 ?
          current_settings_prt.setpoint_Umin1[number_group_stp] * U_DOWN / 100 :
          current_settings_prt.setpoint_Umin1[number_group_stp];
  
  _Bool Uab_is_smaller_than_Umin1 = measurement[index_IM_UAB] <= setpoint1;
  _Bool Ubc_is_smaller_than_Umin1 = measurement[index_IM_UBC] <= setpoint1;
  _Bool Uca_is_smaller_than_Umin1 = measurement[index_IM_UCA] <= setpoint1;
  
  _Bool Ua_is_smaller_than_Umin1 = measurement[index_IM_UA] <= setpoint1;
  _Bool Ub_is_smaller_than_Umin1 = measurement[index_IM_UB] <= setpoint1;
  _Bool Uc_is_smaller_than_Umin1 = measurement[index_IM_UC] <= setpoint1;
  
  unsigned int setpoint2 = previous_state_po_ublk_umin1 ?
          KOEF_MIN_UMIN * U_DOWN / 100 : KOEF_MIN_UMIN;
          
  _Bool Ua_or_Ub_or_Uc_is_smaller_than_250mV = (measurement[index_IM_UA] <= setpoint2) || (measurement[index_IM_UB] <= setpoint2) || (measurement[index_IM_UC] <= setpoint2);
  _Bool Uab_or_Ubc_or_Uca_is_smaller_than_250mV = (measurement[index_IM_UAB] <= setpoint2) || (measurement[index_IM_UBC] <= setpoint2) || (measurement[index_IM_UCA] <= setpoint2);
  
  unsigned int setpoint3 = previous_state_po_iblk_umin1 ?
          current_settings_prt.setpoint_Umin1_Iblk[number_group_stp] * KOEF_POVERNENNJA_GENERAL / 100 :
          current_settings_prt.setpoint_Umin1_Iblk[number_group_stp];
  
  _Bool Ia_or_Ic_is_larger_than_Iust = (measurement[IM_IA] >= setpoint3) ||
                                       (measurement[IM_IB] >= setpoint3) || 
                                       (measurement[IM_IC] >= setpoint3);
  //М
  unsigned int tmp_value = ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) != 0)                    << 0;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1) != 0)                                                            << 2;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_PO_UMIN1_OR_AND) != 0)                                                  << 3;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1_UBLK) != 0)                                                       << 4;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN1_IBLK) != 0)                                                       << 5;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMIN1) != 0)                                     << 6;
  _INVERTOR(tmp_value, 6, tmp_value, 6);
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_START_UMIN1) != 0)                                     << 7;
  
  if (_GET_OUTPUT_STATE(tmp_value, 0)) 
  {
    //Фазные
    _AND4(Ua_is_smaller_than_Umin1, 0, Ub_is_smaller_than_Umin1, 0, Uc_is_smaller_than_Umin1, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Ua_is_smaller_than_Umin1, 0, Ub_is_smaller_than_Umin1, 0, Uc_is_smaller_than_Umin1, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_or_Ic_is_larger_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Ua_or_Ub_or_Uc_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin1
    if (Ua_or_Ub_or_Uc_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
  } 
  else 
  {
    //Линейные
    _AND4(Uab_is_smaller_than_Umin1, 0, Ubc_is_smaller_than_Umin1, 0, Uca_is_smaller_than_Umin1, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Uab_is_smaller_than_Umin1, 0, Ubc_is_smaller_than_Umin1, 0, Uca_is_smaller_than_Umin1, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_or_Ic_is_larger_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Uab_or_Ubc_or_Uca_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin1
    if (Uab_or_Ubc_or_Uca_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN1);
  }
  _INVERTOR(tmp_value, 12, tmp_value, 12);
  _INVERTOR(tmp_value, 13, tmp_value, 13);
  _OR3(tmp_value, 7, tmp_value, 9, tmp_value, 11, tmp_value, 14);
  
  _AND5(tmp_value, 6, tmp_value, 2, tmp_value, 14, tmp_value, 13, tmp_value, 12, tmp_value, 15);
  
  //ПО Iблк. Umin1
  if (Ia_or_Ic_is_larger_than_Iust)
    _SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_IBLK_UMIN1);
  
  //ПО Umin1
  if (_GET_OUTPUT_STATE(tmp_value, 15))
    _SET_BIT(p_active_functions, RANG_PO_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMIN1);
  
  _TIMER_T_0(INDEX_TIMER_UMIN1, current_settings_prt.timeout_Umin1[number_group_stp], tmp_value, 15, tmp_value, 16);
  
  //Сраб. Umin1
  if (_GET_OUTPUT_STATE(tmp_value, 16))
    _SET_BIT(p_active_functions, RANG_UMIN1);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMIN1);
}
/*****************************************************/

/*****************************************************/
// ЗНМИН2
/*****************************************************/
void umin2_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  _Bool previous_state_po_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMIN2) > 0;
  _Bool previous_state_po_ublk_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2) > 0;
  _Bool previous_state_po_iblk_umin2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN2) > 0;
  
  unsigned int setpoint1 = previous_state_po_umin2 ?
          current_settings_prt.setpoint_Umin2[number_group_stp] * U_DOWN / 100 :
          current_settings_prt.setpoint_Umin2[number_group_stp];
  
  _Bool Uab_is_smaller_than_Umin2 = measurement[index_IM_UAB] <= setpoint1;
  _Bool Ubc_is_smaller_than_Umin2 = measurement[index_IM_UBC] <= setpoint1;
  _Bool Uca_is_smaller_than_Umin2 = measurement[index_IM_UCA] <= setpoint1;
  
  _Bool Ua_is_smaller_than_Umin2 = measurement[index_IM_UA] <= setpoint1;
  _Bool Ub_is_smaller_than_Umin2 = measurement[index_IM_UB] <= setpoint1;
  _Bool Uc_is_smaller_than_Umin2 = measurement[index_IM_UC] <= setpoint1;
  
  unsigned int setpoint2 = previous_state_po_ublk_umin2 ?
          KOEF_MIN_UMIN * U_DOWN / 100 : KOEF_MIN_UMIN;
          
  _Bool Ua_or_Ub_or_Uc_is_smaller_than_250mV = (measurement[index_IM_UA] <= setpoint2) || (measurement[index_IM_UB] <= setpoint2) || (measurement[index_IM_UC] <= setpoint2);
  _Bool Uab_or_Ubc_or_Uca_is_smaller_than_250mV = (measurement[index_IM_UAB] <= setpoint2) || (measurement[index_IM_UBC] <= setpoint2) || (measurement[index_IM_UCA] <= setpoint2);
  
  unsigned int setpoint3 = previous_state_po_iblk_umin2 ?
          current_settings_prt.setpoint_Umin2_Iblk[number_group_stp] * KOEF_POVERNENNJA_GENERAL / 100 :
          current_settings_prt.setpoint_Umin2_Iblk[number_group_stp];
  
  _Bool Ia_or_Ic_is_larger_than_Iust = (measurement[IM_IA] >= setpoint3) ||
                                       (measurement[IM_IB] >= setpoint3) || 
                                       (measurement[IM_IC] >= setpoint3);
  //М
  unsigned int tmp_value = ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) != 0)                    << 0;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2) != 0)                                                            << 2;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_PO_UMIN2_OR_AND) != 0)                                                  << 3;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2_UBLK) != 0)                                                       << 4;
  tmp_value |= ((current_settings_prt.control_Umin & CTR_UMIN2_IBLK) != 0)                                                       << 5;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMIN2) != 0)                                    << 6;
  _INVERTOR(tmp_value, 6, tmp_value, 6);
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_START_UMIN2) != 0)                                    << 7;
  
  if (_GET_OUTPUT_STATE(tmp_value, 0))
  {
    //Фазные
    _AND4(Ua_is_smaller_than_Umin2, 0, Ub_is_smaller_than_Umin2, 0, Uc_is_smaller_than_Umin2, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Ua_is_smaller_than_Umin2, 0, Ub_is_smaller_than_Umin2, 0, Uc_is_smaller_than_Umin2, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_or_Ic_is_larger_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Ua_or_Ub_or_Uc_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin2
    if (Ua_or_Ub_or_Uc_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    
  } 
  else 
  {
    //Линейные
    _AND4(Uab_is_smaller_than_Umin2, 0, Ubc_is_smaller_than_Umin2, 0, Uca_is_smaller_than_Umin2, 0, tmp_value, 3, tmp_value, 9);
    _OR3(Uab_is_smaller_than_Umin2, 0, Ubc_is_smaller_than_Umin2, 0, Uca_is_smaller_than_Umin2, 0, tmp_value, 10);
    _INVERTOR(tmp_value, 3, tmp_value, 3);
    _AND2(tmp_value, 10, tmp_value, 3, tmp_value, 11);
    _AND2(tmp_value, 5, Ia_or_Ic_is_larger_than_Iust, 0, tmp_value, 12);
    _INVERTOR(tmp_value, 5, tmp_value, 5);
    _AND3(Uab_or_Ubc_or_Uca_is_smaller_than_250mV, 0, tmp_value, 4, tmp_value, 5, tmp_value, 13);
    
    //ПО Uблк. Umin2
    if (Uab_or_Ubc_or_Uca_is_smaller_than_250mV)
      _SET_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
    else
      _CLEAR_BIT(p_active_functions, RANG_PO_UBLK_UMIN2);
  }
  _INVERTOR(tmp_value, 12, tmp_value, 12);
  _INVERTOR(tmp_value, 13, tmp_value, 13);
  _OR3(tmp_value, 7, tmp_value, 9, tmp_value, 11, tmp_value, 14);
  
  _AND5(tmp_value, 6, tmp_value, 2, tmp_value, 14, tmp_value, 13, tmp_value, 12, tmp_value, 15);
  
  //ПО Iблк. Umin2
  if (Ia_or_Ic_is_larger_than_Iust)
    _SET_BIT(p_active_functions, RANG_PO_IBLK_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_IBLK_UMIN2);
  
  //ПО Umin2
  if (_GET_OUTPUT_STATE(tmp_value, 15))
    _SET_BIT(p_active_functions, RANG_PO_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMIN2);
  
  _TIMER_T_0(INDEX_TIMER_UMIN2, current_settings_prt.timeout_Umin2[number_group_stp], tmp_value, 15, tmp_value, 16);
  
  //Сраб. Umin2
  if (_GET_OUTPUT_STATE(tmp_value, 16))
    _SET_BIT(p_active_functions, RANG_UMIN2);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMIN2);
}
/*****************************************************/

/*****************************************************/
// ЗНМАКС1
/*****************************************************/
void umax1_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  _Bool previous_state_po_umax1 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMAX1) > 0;
  
  unsigned int setpoint1 = previous_state_po_umax1 ?
          current_settings_prt.setpoint_Umax1[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_Umax1[number_group_stp];
  
  _Bool Uab_is_larger_than_Umax1 = measurement[index_IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_Umax1 = measurement[index_IM_UBC] >= setpoint1;
  _Bool Uca_is_larger_than_Umax1 = measurement[index_IM_UCA] >= setpoint1;
  
  _Bool Ua_is_larger_than_Umax1 = measurement[index_IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_Umax1 = measurement[index_IM_UB] >= setpoint1;
  _Bool Uc_is_larger_than_Umax1 = measurement[index_IM_UC] >= setpoint1;
  
  //М
  unsigned int tmp_value = ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) != 0)                    << 0;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_PO_UMAX1_OR_AND) != 0)                                                  << 2;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_UMAX1) != 0)                                                            << 3;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMAX1) != 0)                                    << 4;
  
  if (_GET_OUTPUT_STATE(tmp_value, 0)) 
  {
    _AND4(Ua_is_larger_than_Umax1, 0, Ub_is_larger_than_Umax1, 0, Uc_is_larger_than_Umax1, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Ua_is_larger_than_Umax1, 0, Ub_is_larger_than_Umax1, 0, Uc_is_larger_than_Umax1, 0, tmp_value, 6);
  } 
  else 
  {
    _AND4(Uab_is_larger_than_Umax1, 0, Ubc_is_larger_than_Umax1, 0, Uca_is_larger_than_Umax1, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Uab_is_larger_than_Umax1, 0, Ubc_is_larger_than_Umax1, 0, Uca_is_larger_than_Umax1, 0, tmp_value, 6);
  }
  
  _INVERTOR(tmp_value, 2, tmp_value, 2);
  _AND2(tmp_value, 6, tmp_value, 2, tmp_value, 7);
  _OR2(tmp_value, 5, tmp_value, 7, tmp_value, 8);
  _INVERTOR(tmp_value, 4, tmp_value, 4);
  _AND3(tmp_value, 8, tmp_value, 4, tmp_value, 3, tmp_value, 9);
  
  //ПО Umax1
  if (_GET_OUTPUT_STATE(tmp_value, 9))
    _SET_BIT(p_active_functions, RANG_PO_UMAX1);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMAX1);
  
  _TIMER_T_0(INDEX_TIMER_UMAX1, current_settings_prt.timeout_Umax1[number_group_stp], tmp_value, 9, tmp_value, 10);
  
  //Сраб. Umax1
  if (_GET_OUTPUT_STATE(tmp_value, 10))
    _SET_BIT(p_active_functions, RANG_UMAX1);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMAX1);
}
/*****************************************************/

/*****************************************************/
// ЗНМАКС2
/*****************************************************/
void umax2_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  _Bool previous_state_po_umax2 = _CHECK_SET_BIT(p_active_functions, RANG_PO_UMAX2) > 0;
  
  unsigned int setpoint1 = previous_state_po_umax2 ?
          current_settings_prt.setpoint_Umax2[number_group_stp] * U_UP / 100 :
          current_settings_prt.setpoint_Umax2[number_group_stp];
  
  _Bool Uab_is_larger_than_Umax2 = measurement[index_IM_UAB] >= setpoint1;
  _Bool Ubc_is_larger_than_Umax2 = measurement[index_IM_UBC] >= setpoint1;
  _Bool Uca_is_larger_than_Umax2 = measurement[index_IM_UCA] >= setpoint1;
  
  _Bool Ua_is_larger_than_Umax2 = measurement[index_IM_UA] >= setpoint1;
  _Bool Ub_is_larger_than_Umax2 = measurement[index_IM_UB] >= setpoint1;
  _Bool Uc_is_larger_than_Umax2 = measurement[index_IM_UC] >= setpoint1;
  
  //М
  unsigned int tmp_value = ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) != 0)                    << 0;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_PO_UMAX2_OR_AND) != 0)                                                  << 2;
  tmp_value |= ((current_settings_prt.control_Umax & CTR_UMAX2) != 0)                                                            << 3;
  
  //ДВ
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_UMAX2) != 0)                                    << 4;
  
  if (_GET_OUTPUT_STATE(tmp_value, 0)) 
  {
    _AND4(Ua_is_larger_than_Umax2, 0, Ub_is_larger_than_Umax2, 0, Uc_is_larger_than_Umax2, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Ua_is_larger_than_Umax2, 0, Ub_is_larger_than_Umax2, 0, Uc_is_larger_than_Umax2, 0, tmp_value, 6);
  } 
  else 
  {
    _AND4(Uab_is_larger_than_Umax2, 0, Ubc_is_larger_than_Umax2, 0, Uca_is_larger_than_Umax2, 0, tmp_value, 2, tmp_value, 5);
    _OR3(Uab_is_larger_than_Umax2, 0, Ubc_is_larger_than_Umax2, 0, Uca_is_larger_than_Umax2, 0, tmp_value, 6);
  }
  
  _INVERTOR(tmp_value, 2, tmp_value, 2);
  _AND2(tmp_value, 6, tmp_value, 2, tmp_value, 7);
  _OR2(tmp_value, 5, tmp_value, 7, tmp_value, 8);
  _INVERTOR(tmp_value, 4, tmp_value, 4);
  _AND3(tmp_value, 8, tmp_value, 4, tmp_value, 3, tmp_value, 9);
  
  //ПО Umax1
  if (_GET_OUTPUT_STATE(tmp_value, 9))
    _SET_BIT(p_active_functions, RANG_PO_UMAX2);
  else
    _CLEAR_BIT(p_active_functions, RANG_PO_UMAX2);
  
  _TIMER_T_0(INDEX_TIMER_UMAX2, current_settings_prt.timeout_Umax2[number_group_stp], tmp_value, 9, tmp_value, 10);
  
  //Сраб. Umax1
  if (_GET_OUTPUT_STATE(tmp_value, 10))
    _SET_BIT(p_active_functions, RANG_UMAX2);
  else
    _CLEAR_BIT(p_active_functions, RANG_UMAX2);
}
/*****************************************************/

/*****************************************************/
// Вибір ТН1/ТН2
/*****************************************************/
uint32_t choose_tn1_tn2(volatile unsigned int *p_active_functions)
{
  uint32_t logic_tn1_tn2_0 = 0;
  static uint32_t static_logic_tn1_tn2_0; 
  
  logic_tn1_tn2_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_K1_AVR) != 0) << 0;
  logic_tn1_tn2_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_K2_AVR) != 0) << 1;
  logic_tn1_tn2_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) << 2;
  
  _INVERTOR(logic_tn1_tn2_0, 0, logic_tn1_tn2_0, 3);
  _INVERTOR(logic_tn1_tn2_0, 1, logic_tn1_tn2_0, 4);
  _INVERTOR(logic_tn1_tn2_0, 2, logic_tn1_tn2_0, 5);
  
  _AND2(logic_tn1_tn2_0, 3, logic_tn1_tn2_0, 4, logic_tn1_tn2_0, 6); /*1к- 2к-*/
  _AND2(logic_tn1_tn2_0, 0, logic_tn1_tn2_0, 5, logic_tn1_tn2_0, 7);
  _AND2(logic_tn1_tn2_0, 3, logic_tn1_tn2_0, 1, logic_tn1_tn2_0, 8); /*1к- 2к+*/
  _AND2(logic_tn1_tn2_0, 5, logic_tn1_tn2_0, 8, logic_tn1_tn2_0, 9);
  
  uint32_t stp_0 = _GET_OUTPUT_STATE(static_logic_tn1_tn2_0, 0) ?  KOEF_MIN_UMIN * U_DOWN / 100 : KOEF_MIN_UMIN;
  uint32_t stp_1 = _GET_OUTPUT_STATE(static_logic_tn1_tn2_0, 1) ?  KOEF_MIN_UMIN * U_DOWN / 100 : KOEF_MIN_UMIN;
          
  if ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0)
  {
    //Лінійні
    if (
        (measurement[IM_UAB1] >= stp_0) &&
        (measurement[IM_UBC1] >= stp_0) &&
        (measurement[IM_UCA1] >= stp_0)
       )
    {
      static_logic_tn1_tn2_0 |= (1 << 0);
    }
    else
    {
      static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 0));
    }

    if (
        (measurement[IM_UAB2] >= stp_1) &&
        (measurement[IM_UBC2] >= stp_1) &&
        (measurement[IM_UCA2] >= stp_1)
       )
    {
      static_logic_tn1_tn2_0 |= (1 << 1);
    }
    else
    {
      static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 1));
    }
  }
  else
  {
    //Фазні
    if (
        (measurement[IM_UA1] >= stp_0) &&
        (measurement[IM_UB1] >= stp_0) &&
        (measurement[IM_UC1] >= stp_0)
       )
    {
      static_logic_tn1_tn2_0 |= (1 << 0);
    }
    else
    {
      static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 0));
    }

    if (
        (measurement[IM_UA2] >= stp_1) &&
        (measurement[IM_UB2] >= stp_1) &&
        (measurement[IM_UC2] >= stp_1)
       )
    {
      static_logic_tn1_tn2_0 |= (1 << 1);
    }
    else
    {
      static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 1));
    }
  }

  /*Неисправність кіл напруг*/
  //ПО Iнцн
  uint32_t stp_tmp = _GET_OUTPUT_STATE(static_logic_tn1_tn2_0, 2) ?  i_nom_const * KOEF_POVERNENNJA_MTZ_I_DOWN / 100 : i_nom_const;
  if(
     (measurement[IM_IA] <= stp_tmp) &&
     (measurement[IM_IB] <= stp_tmp) &&
     (measurement[IM_IC] <= stp_tmp)
    )
  {
    static_logic_tn1_tn2_0 |= (1 << 2);
  }
  else
  {
    static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 2));
  }
  
  //ПО U1нцн
  stp_tmp = _GET_OUTPUT_STATE(static_logic_tn1_tn2_0, 3) ?  u_linear_nom_const * U_DOWN / 100 : u_linear_nom_const;
  if(
     (measurement[IM_UAB1] <= stp_tmp) ||
     (measurement[IM_UBC1] <= stp_tmp) ||
     (measurement[IM_UCA1] <= stp_tmp)
    )
  {
    static_logic_tn1_tn2_0 |= (1 << 3);
  }
  else
  {
    static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 3));
  }
  
  //ПО U2нцн
  stp_tmp = _GET_OUTPUT_STATE(static_logic_tn1_tn2_0, 4) ?  u_linear_nom_const * U_DOWN / 100 : u_linear_nom_const;
  if(
     (measurement[IM_UAB2] <= stp_tmp) ||
     (measurement[IM_UBC2] <= stp_tmp) ||
     (measurement[IM_UCA2] <= stp_tmp)
    )
  {
    static_logic_tn1_tn2_0 |= (1 << 4);
  }
  else
  {
    static_logic_tn1_tn2_0 &= (uint32_t)(~(1 << 4));
  }

  _AND2(static_logic_tn1_tn2_0, 2, static_logic_tn1_tn2_0, 3, logic_tn1_tn2_0, 10); /*НЦН1к*/
  _AND2(static_logic_tn1_tn2_0, 2, static_logic_tn1_tn2_0, 4, logic_tn1_tn2_0, 11); /*НЦН2к*/
  /*Несправність кіл напруг*/
  
  _INVERTOR(logic_tn1_tn2_0, 10, logic_tn1_tn2_0, 12);
  _INVERTOR(logic_tn1_tn2_0, 11, logic_tn1_tn2_0, 13);
  
  _INVERTOR(static_logic_tn1_tn2_0, 1, logic_tn1_tn2_0, 14);
  _INVERTOR(static_logic_tn1_tn2_0, 0, logic_tn1_tn2_0, 15);
  
  _OR2(logic_tn1_tn2_0, 14, logic_tn1_tn2_0, 11, logic_tn1_tn2_0, 16);
  _OR2(logic_tn1_tn2_0, 15, logic_tn1_tn2_0, 10, logic_tn1_tn2_0, 17);
  
  _AND2(static_logic_tn1_tn2_0, 0, logic_tn1_tn2_0, 12, logic_tn1_tn2_0, 18);
  _AND2(static_logic_tn1_tn2_0, 1, logic_tn1_tn2_0, 13, logic_tn1_tn2_0, 19);
  
  _AND3(logic_tn1_tn2_0, 0, logic_tn1_tn2_0, 18, logic_tn1_tn2_0, 2, logic_tn1_tn2_0, 20);
  _AND3(logic_tn1_tn2_0, 8, logic_tn1_tn2_0, 19, logic_tn1_tn2_0, 2, logic_tn1_tn2_0, 21);
  
  _AND4(logic_tn1_tn2_0, 18, logic_tn1_tn2_0, 2, logic_tn1_tn2_0,  8, logic_tn1_tn2_0, 16, logic_tn1_tn2_0, 22);
  _AND4(logic_tn1_tn2_0, 19, logic_tn1_tn2_0, 2, logic_tn1_tn2_0, 17, logic_tn1_tn2_0,  0, logic_tn1_tn2_0, 23);
  
  _AND3(logic_tn1_tn2_0, 18, logic_tn1_tn2_0,  2, logic_tn1_tn2_0, 16, logic_tn1_tn2_0, 24);
  _AND3(logic_tn1_tn2_0,  8, logic_tn1_tn2_0, 17, logic_tn1_tn2_0,  2, logic_tn1_tn2_0, 25);
  
  _OR3(logic_tn1_tn2_0, 20, logic_tn1_tn2_0, 22, logic_tn1_tn2_0, 24, logic_tn1_tn2_0, 26);
  _OR3(logic_tn1_tn2_0, 21, logic_tn1_tn2_0, 23, logic_tn1_tn2_0, 25, logic_tn1_tn2_0, 27);
  
  _OR3(logic_tn1_tn2_0, 26, logic_tn1_tn2_0, 6, logic_tn1_tn2_0, 7, logic_tn1_tn2_0, 28);
  _OR2(logic_tn1_tn2_0, 27, logic_tn1_tn2_0, 9, logic_tn1_tn2_0, 29);

  uint32_t tn1_tn2 = 0;
  if (_GET_OUTPUT_STATE(logic_tn1_tn2_0, 28)) tn1_tn2 = 0;
  else if (_GET_OUTPUT_STATE(logic_tn1_tn2_0, 29))  tn1_tn2 = 1;
  else
  {
    //у всіх інших випадках беремо tn1_tn2 = 0
  }
  
  if (tn1_tn2 == 0) 
  {
    _SET_BIT(p_active_functions, RANG_TN1);
    _CLEAR_BIT(p_active_functions, RANG_TN2);
  }
  else
  {
    _SET_BIT(p_active_functions, RANG_TN2);
    _CLEAR_BIT(p_active_functions, RANG_TN1);
  }

  return tn1_tn2;
}
/*****************************************************/

/*****************************************************/
// АВР
/*****************************************************/
void avr_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int logic_AVR_0 = 0;
  unsigned int logic_AVR_1 = 0;
//  unsigned int logic_AVR_2 = 0;
  unsigned int logic_AVR_3 = 0;
//  unsigned int logic_AVR_4 = 0;

  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV) != 0) << 0;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_APV_WORK) != 0) << 1;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_OTKL_VV) != 0) << 2;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_OTKL_AVR) != 0) << 3;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_OTKL_VID_ZOVN_ZAHYSTIV) != 0) << 4;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_PRYVID_VV) != 0) << 5;
  _OR6(logic_AVR_0, 0, logic_AVR_0, 1, logic_AVR_0, 2, logic_AVR_0, 3, logic_AVR_0, 4, logic_AVR_0, 5, logic_AVR_0, 6);
  
  logic_AVR_0 |= ((current_settings_prt.control_avr & CTR_AVR_BLK) != 0) << 7;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_SBROS_BLOCK_AVR) != 0) << 8;
  _OR2(logic_AVR_0, 7, logic_AVR_0, 8, logic_AVR_0, 9);

  logic_AVR_0 |= ((current_settings_prt.control_avr & CTR_AVR) != 0) << 11;
  logic_AVR_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) == 0) << 12;

  logic_AVR_0 |= ((current_settings_prt.control_avr & CTR_AVR_UMAX) != 0) << 16;
  _INVERTOR(logic_AVR_0, 16, logic_AVR_0, 17);
  
  unsigned int setpoint_avr_tn1_U1_min = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U1_MIN) == 0) ?
    current_settings_prt.setpoint_avr_tn1_U1_Umin[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn1_U1_Umin[number_group_stp]*U_DOWN/100);
  logic_AVR_1 |= (
                  (measurement[IM_UAB1] <= setpoint_avr_tn1_U1_min) &&
                  (measurement[IM_UBC1] <= setpoint_avr_tn1_U1_min) &&
                  (measurement[IM_UCA1] <= setpoint_avr_tn1_U1_min)
                 ) << 0;   
  //"ПО U1min < U1x"
  if (_GET_OUTPUT_STATE(logic_AVR_1, 0)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U1_MIN);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN1_U1_MIN);
    
  unsigned int setpoint_avr_tn2_U1_low_work = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U1_LOW_WORK) == 0) ?
    current_settings_prt.setpoint_avr_tn2_U1_low_work[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn2_U1_low_work[number_group_stp]*U_UP/100);
  logic_AVR_1 |= (
                  (measurement[IM_UAB2] >= setpoint_avr_tn2_U1_low_work) &&
                  (measurement[IM_UBC2] >= setpoint_avr_tn2_U1_low_work) &&
                  (measurement[IM_UCA2] >= setpoint_avr_tn2_U1_low_work)
                 ) << 1;   
  //"ПО U2min > U2x"
  if (_GET_OUTPUT_STATE(logic_AVR_1, 1)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U1_LOW_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN2_U1_LOW_WORK);

  unsigned int setpoint_avr_tn1_U1_max = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U1_MAX) == 0) ?
    current_settings_prt.setpoint_avr_tn1_U1_Umax[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn1_U1_Umax[number_group_stp]*U_UP/100);
  logic_AVR_1 |= (
                  (measurement[IM_UAB1] >= setpoint_avr_tn1_U1_max) &&
                  (measurement[IM_UBC1] >= setpoint_avr_tn1_U1_max) &&
                  (measurement[IM_UCA1] >= setpoint_avr_tn1_U1_max)
                 ) << 21;   

  //"ПО U1max > U1x"
  if (_GET_OUTPUT_STATE(logic_AVR_1, 21)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U1_MAX);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN1_U1_MAX);

  unsigned int setpoint_avr_tn1_U2_high_work = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U2_HIGH_WORK) == 0) ?
    current_settings_prt.setpoint_avr_tn1_U2_high_work[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn1_U2_high_work[number_group_stp]*U_DOWN/100);
  logic_AVR_1 |= (
                  (measurement[IM_UAB2] <= setpoint_avr_tn1_U2_high_work) &&
                  (measurement[IM_UBC2] <= setpoint_avr_tn1_U2_high_work) &&
                  (measurement[IM_UCA2] <= setpoint_avr_tn1_U2_high_work)
                 ) << 22;   
  //"ПО U2max < U2x"
  if (_GET_OUTPUT_STATE(logic_AVR_1, 22)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U2_HIGH_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN1_U2_HIGH_WORK);

  
  _AND2(logic_AVR_1, 21, logic_AVR_0, 16, logic_AVR_1, 24);
  _OR2(logic_AVR_1, 0, logic_AVR_1, 24, logic_AVR_1, 26);
  _OR2(logic_AVR_1, 22, logic_AVR_0, 17, logic_AVR_1, 27);
  
  logic_AVR_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_K1_AVR) == 0) << 2;
  _TIMER_IMPULSE(INDEX_TIMER_AVR_PUSK_K1, current_settings_prt.timeout_avr_d_diji_k1[number_group_stp], previous_states_AVR_0, 1, logic_AVR_1, 2, logic_AVR_1, 3);
  
  logic_AVR_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_STAT_BLOCK_AVR_1) != 0) << 4;
  
  logic_AVR_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_OZT_AVR_1) != 0) << 16;
  _AND2(logic_AVR_1, 3, logic_AVR_1, 0, logic_AVR_1, 28);

  unsigned int setpoint_avr_tn2_U2_min = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U2_MIN) == 0) ?
    current_settings_prt.setpoint_avr_tn2_U2_Umin[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn2_U2_Umin[number_group_stp]*U_DOWN/100);
  logic_AVR_3 |= (
                  (measurement[IM_UAB2] <= setpoint_avr_tn2_U2_min) &&
                  (measurement[IM_UBC2] <= setpoint_avr_tn2_U2_min) &&
                  (measurement[IM_UCA2] <= setpoint_avr_tn2_U2_min)
                 ) << 0;   
  //"ПО U2min < U2x"
  if (_GET_OUTPUT_STATE(logic_AVR_3, 0)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U2_MIN);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN2_U2_MIN);
    
  unsigned int setpoint_avr_tn1_U2_low_work = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U2_LOW_WORK) == 0) ?
    current_settings_prt.setpoint_avr_tn1_U2_low_work[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn1_U2_low_work[number_group_stp]*U_UP/100);
  logic_AVR_3 |= (
                  (measurement[IM_UAB1] >= setpoint_avr_tn1_U2_low_work) &&
                  (measurement[IM_UBC1] >= setpoint_avr_tn1_U2_low_work) &&
                  (measurement[IM_UCA1] >= setpoint_avr_tn1_U2_low_work)
                 ) << 1;   
  //"ПО U1min > U1x"
  if (_GET_OUTPUT_STATE(logic_AVR_3, 1)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN1_U2_LOW_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN1_U2_LOW_WORK);

  unsigned int setpoint_avr_tn2_U2_max = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U2_MAX) == 0) ?
    current_settings_prt.setpoint_avr_tn2_U2_Umax[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn2_U2_Umax[number_group_stp]*U_UP/100);
  logic_AVR_3 |= (
                  (measurement[IM_UAB2] >= setpoint_avr_tn2_U2_max) &&
                  (measurement[IM_UBC2] >= setpoint_avr_tn2_U2_max) &&
                  (measurement[IM_UCA2] >= setpoint_avr_tn2_U2_max)
                 ) << 21;   

  //"ПО U2max > U2x"
  if (_GET_OUTPUT_STATE(logic_AVR_3, 21)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U2_MAX);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN2_U2_MAX);

  unsigned int setpoint_avr_tn2_U1_high_work = (_CHECK_SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U1_HIGH_WORK) == 0) ?
    current_settings_prt.setpoint_avr_tn2_U1_high_work[number_group_stp] :
    (current_settings_prt.setpoint_avr_tn2_U1_high_work[number_group_stp]*U_DOWN/100);
  logic_AVR_3 |= (
                  (measurement[IM_UAB1] <= setpoint_avr_tn2_U1_high_work) &&
                  (measurement[IM_UBC1] <= setpoint_avr_tn2_U1_high_work) &&
                  (measurement[IM_UCA1] <= setpoint_avr_tn2_U1_high_work)
                 ) << 22;   
  //"ПО U1max < U1x"
  if (_GET_OUTPUT_STATE(logic_AVR_3, 22)) _SET_BIT(p_active_functions, RANG_PO_AVR_TN2_U1_HIGH_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_TN2_U1_HIGH_WORK);

  _AND2(logic_AVR_3, 21, logic_AVR_0, 16, logic_AVR_3, 24);
  _OR2(logic_AVR_3, 0, logic_AVR_3, 24, logic_AVR_3, 26);
  _OR2(logic_AVR_3, 22, logic_AVR_0, 17, logic_AVR_3, 27);
  
  logic_AVR_3 |= (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_K2_AVR) == 0) << 2;
  _TIMER_IMPULSE(INDEX_TIMER_AVR_PUSK_K2, current_settings_prt.timeout_avr_d_diji_k2[number_group_stp], previous_states_AVR_0, 2, logic_AVR_3, 2, logic_AVR_3, 3);
  
  logic_AVR_3 |= (_CHECK_SET_BIT(p_active_functions, RANG_STAT_BLOCK_AVR_2) != 0) << 4;
  
  logic_AVR_3 |= (_CHECK_SET_BIT(p_active_functions, RANG_OZT_AVR_2) != 0) << 16;
  _AND2(logic_AVR_3, 3, logic_AVR_3, 0, logic_AVR_3, 28);

  logic_AVR_1 |= (_CHECK_SET_BIT(p_active_functions, RANG_KOM3_ON_AVR) != 0) << 5;
  logic_AVR_3 |= (_CHECK_SET_BIT(p_active_functions, RANG_KOM4_ON_AVR) != 0) << 5;
  do
  {
    //"Ком.3 Вкл.АВР" (ця операція не має значення для першої ітерації, але потрібна для наступних ітерацій)
    if (_GET_OUTPUT_STATE(logic_AVR_1, 5)) _SET_BIT(p_active_functions, RANG_KOM3_ON_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_KOM3_ON_AVR);
    
    //"Ком.4 Вкл.АВР" (ця операція не має значення для першої ітерації, але потрібна для наступних ітерацій)
    if (_GET_OUTPUT_STATE(logic_AVR_3, 5)) _SET_BIT(p_active_functions, RANG_KOM4_ON_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_KOM4_ON_AVR);
    
    _OR2(logic_AVR_1, 5, logic_AVR_3, 5, logic_AVR_0, 14);
    _OR2(logic_AVR_0, 14, logic_AVR_0, 6, logic_AVR_0, 15);
    
    _D_TRIGGER(1,  0, _GET_OUTPUT_STATE(logic_AVR_0, 9), previous_states_AVR_0, 0, logic_AVR_0, 15, trigger_AVR_0, 0);
    _INVERTOR(trigger_AVR_0, 0, logic_AVR_0, 10);
    //"Блок.АВР"
    if (_GET_OUTPUT_STATE(trigger_AVR_0, 0)) _SET_BIT(p_active_functions, RANG_BLOCK_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_BLOCK_AVR);

    _AND3(logic_AVR_0, 10, logic_AVR_0, 11, logic_AVR_0, 12, logic_AVR_0, 13);
    
    _AND2(logic_AVR_1, 5, logic_AVR_0, 7, logic_AVR_1, 29);
    _AND2(logic_AVR_3, 5, logic_AVR_0, 7, logic_AVR_3, 29);
  
    _OR2(logic_AVR_1, 29, logic_AVR_1, 4, logic_AVR_1, 6);
    _TIMER_0_T(INDEX_TIMER_AVR_BLK_K1, current_settings_prt.timeout_avr_blk_k1[number_group_stp], logic_AVR_1, 6, logic_AVR_1, 7);
    _INVERTOR(logic_AVR_1, 7, logic_AVR_1, 8);
    
    _OR2(logic_AVR_3, 29, logic_AVR_3, 4, logic_AVR_3, 6);
    _TIMER_0_T(INDEX_TIMER_AVR_BLK_K2, current_settings_prt.timeout_avr_blk_k2[number_group_stp], logic_AVR_3, 6, logic_AVR_3, 7);
    _INVERTOR(logic_AVR_3, 7, logic_AVR_3, 8);
    
    _AND5(logic_AVR_0, 13, logic_AVR_1, 8, logic_AVR_1, 26, logic_AVR_1, 1, logic_AVR_1, 27, logic_AVR_1, 9);
    //"ПО АВР к.1"
    if (_GET_OUTPUT_STATE(logic_AVR_1, 9)) _SET_BIT(p_active_functions, RANG_PO_AVR_K1);
    else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_K1);
    
    _AND5(logic_AVR_0, 13, logic_AVR_3, 8, logic_AVR_3, 26, logic_AVR_3, 1, logic_AVR_3, 27, logic_AVR_3, 9);
    //"ПО АВР к.2"
    if (_GET_OUTPUT_STATE(logic_AVR_3, 9)) _SET_BIT(p_active_functions, RANG_PO_AVR_K2);
    else  _CLEAR_BIT(p_active_functions, RANG_PO_AVR_K2);

    _AND2(logic_AVR_1, 9, logic_AVR_1, 16, logic_AVR_1, 17);
    _INVERTOR(logic_AVR_1, 17, logic_AVR_1, 18);
    _AND2(logic_AVR_1, 9, logic_AVR_1, 18, logic_AVR_1, 19);

    _AND2(logic_AVR_3, 9, logic_AVR_3, 16, logic_AVR_3, 17);
    _INVERTOR(logic_AVR_3, 17, logic_AVR_3, 18);
    _AND2(logic_AVR_3, 9, logic_AVR_3, 18, logic_AVR_3, 19);
    
    _AND2(logic_AVR_1, 0, logic_AVR_1, 19, logic_AVR_1, 30);
    _AND3(logic_AVR_1, 19, logic_AVR_0, 16, logic_AVR_1, 21, logic_AVR_1, 25);
    
    _AND2(logic_AVR_3, 0, logic_AVR_3, 19, logic_AVR_3, 30);
    _AND3(logic_AVR_3, 19, logic_AVR_0, 16, logic_AVR_3, 21, logic_AVR_3, 25);

    _TIMER_T_0(INDEX_TIMER_AVR_VYMK_ROB_K1_UMIN, current_settings_prt.timeout_avr_vymk_rob_k1_Umin[number_group_stp], logic_AVR_1, 30, logic_AVR_1, 10);
    _TIMER_T_0(INDEX_TIMER_AVR_VYMK_ROB_K1_UMAX, current_settings_prt.timeout_avr_vymk_rob_k1_Umax[number_group_stp], logic_AVR_1, 25, logic_AVR_1, 23);
    _OR3(logic_AVR_1, 10, logic_AVR_1, 23, logic_AVR_1, 17, logic_AVR_1, 20);
    _TIMER_0_T(INDEX_TIMER_AVR_VYMK_K1, current_settings_prt.timeout_avr_vymk_k1[number_group_stp], logic_AVR_1, 20, logic_AVR_1, 11);
    //"Ком.1 Откл.АВР"
    if (_GET_OUTPUT_STATE(logic_AVR_1, 11)) _SET_BIT(p_active_functions, RANG_KOM1_OFF_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_KOM1_OFF_AVR);
    
    _TIMER_T_0(INDEX_TIMER_AVR_VYMK_ROB_K2_UMIN, current_settings_prt.timeout_avr_vymk_rob_k2_Umin[number_group_stp], logic_AVR_3, 30, logic_AVR_3, 10);
    _TIMER_T_0(INDEX_TIMER_AVR_VYMK_ROB_K2_UMAX, current_settings_prt.timeout_avr_vymk_rob_k2_Umax[number_group_stp], logic_AVR_3, 25, logic_AVR_3, 23);
    _OR3(logic_AVR_3, 10, logic_AVR_3, 23, logic_AVR_3, 17, logic_AVR_3, 20);
    _TIMER_0_T(INDEX_TIMER_AVR_VYMK_K2, current_settings_prt.timeout_avr_vymk_k2[number_group_stp], logic_AVR_3, 20, logic_AVR_3, 11);
    //"Ком.2 Откл.АВР"
    if (_GET_OUTPUT_STATE(logic_AVR_3, 11)) _SET_BIT(p_active_functions, RANG_KOM2_OFF_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_KOM2_OFF_AVR);
    
    _AND2(logic_AVR_1, 28, logic_AVR_1, 9, logic_AVR_1, 12);
    _AND2(logic_AVR_3, 28, logic_AVR_3, 9, logic_AVR_3, 12);

    _TIMER_T_0(INDEX_TIMER_AVR_VVIMK_REZ_K1, current_settings_prt.timeout_avr_vvimk_rez_k1[number_group_stp], logic_AVR_1, 12, logic_AVR_1, 13);
    _TIMER_0_T(INDEX_TIMER_AVR_VVIMK_K1, current_settings_prt.timeout_avr_vvimk_k1[number_group_stp], logic_AVR_1, 13, logic_AVR_1, 14);
    //"Ком.1 Вкл.АВР"
    if (_GET_OUTPUT_STATE(logic_AVR_1, 14)) _SET_BIT(p_active_functions, RANG_KOM1_ON_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_KOM1_ON_AVR);

    _TIMER_T_0(INDEX_TIMER_AVR_VVIMK_REZ_K2, current_settings_prt.timeout_avr_vvimk_rez_k2[number_group_stp], logic_AVR_3, 12, logic_AVR_3, 13);
    _TIMER_0_T(INDEX_TIMER_AVR_VVIMK_K2, current_settings_prt.timeout_avr_vvimk_k2[number_group_stp], logic_AVR_3, 13, logic_AVR_3, 14);
    //"Ком.2 Вкл.АВР"
    if (_GET_OUTPUT_STATE(logic_AVR_3, 14)) _SET_BIT(p_active_functions, RANG_KOM2_ON_AVR);
    else  _CLEAR_BIT(p_active_functions, RANG_KOM2_ON_AVR);

    _TIMER_T_0(INDEX_TIMER_AVR_D_DIJI_K1, current_settings_prt.timeout_avr_d_diji_k1[number_group_stp], logic_AVR_1, 12, logic_AVR_1, 15);
    _TIMER_0_T(INDEX_TIMER_AVR_TMP_1MS_K1, 1, logic_AVR_1, 15, logic_AVR_1, 5);


    _TIMER_T_0(INDEX_TIMER_AVR_D_DIJI_K2, current_settings_prt.timeout_avr_d_diji_k2[number_group_stp], logic_AVR_3, 12, logic_AVR_3, 15);
    _TIMER_0_T(INDEX_TIMER_AVR_TMP_1MS_K2, 1, logic_AVR_3, 15, logic_AVR_3, 5);
  }
  while(
        (_GET_OUTPUT_STATE(logic_AVR_1, 5) != (_CHECK_SET_BIT(p_active_functions, RANG_KOM3_ON_AVR) != 0)) ||
        (_GET_OUTPUT_STATE(logic_AVR_3, 5) != (_CHECK_SET_BIT(p_active_functions, RANG_KOM4_ON_AVR) != 0))
       );
  /*Якщо ми вийшли з цього циклу, то гарантовано біт logic_AVR_1.5 рівний стану команди "Ком.3 Вкл.АВР" і logic_AVR_3.5 рівний стану команди "Ком.4 Вкл.АВР"*/
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
// Готовность к ТУ
/*****************************************************/
void ready_tu(volatile unsigned int *p_active_functions)
{
  unsigned int tmp_value = (_CHECK_SET_BIT(p_active_functions, RANG_PRYVID_VV) == 0)                  << 0;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV) != 0)                    << 1;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_RESET_BLOCK_READY_TU_VID_ZAHYSTIV) != 0)      << 2;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_AVAR_DEFECT) == 0)                            << 3;
  tmp_value |= (_CHECK_SET_BIT(p_active_functions, RANG_MISCEVE_DYSTANCIJNE) == 0)                    << 6;
  
  _Bool ctrl_ready_tu = ((current_settings_prt.control_extra_settings_1 & CTR_EXTRA_SETTINGS_1_CTRL_READY_TU) == 0);
  
  if (!previous_states_ready_tu && _GET_OUTPUT_STATE(tmp_value, 1)) {
    previous_states_ready_tu = 0;
  }
  
  _OR2(tmp_value, 2, ctrl_ready_tu, 0, tmp_value, 7);
  _D_TRIGGER(1, 0, _GET_OUTPUT_STATE(tmp_value, 7), previous_states_ready_tu, 0, tmp_value, 1, trigger_ready_tu, 0);
  
  _AND4(tmp_value, 0, tmp_value, 3, !trigger_ready_tu, 0, tmp_value, 6, tmp_value, 5);
  
  //Готовность к ТУ
  if (_GET_OUTPUT_STATE(tmp_value, 5))
    _SET_BIT(p_active_functions, RANG_READY_TU);
  else
    _CLEAR_BIT(p_active_functions, RANG_READY_TU);
}
/*****************************************************/

/*****************************************************/
// УРОВ
/*****************************************************/
inline void urov_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  /*******************************/
  //Визначаємо максимальний фазовий струм для УРОВ
  /*******************************/
  unsigned int max_faze_current = measurement[IM_IA];
  if (max_faze_current < measurement[IM_IB]) max_faze_current = measurement[IM_IB];
  if (max_faze_current < measurement[IM_IC]) max_faze_current = measurement[IM_IC];
  /*******************************/
  
  /*******************************/
  if(
     (( current_settings_prt.control_urov & CTR_UROV_STATE) != 0) &&
     (
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_MTZ1   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ1             ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_MTZ2   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ2             ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_MTZ3   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ3             ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_MTZ4   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_MTZ4             ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_ZDZ    ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ZDZ              ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_ZOP1   ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_ZOP              ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_UMIN1  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMIN1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_UMIN2  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMIN2            ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_UMAX1  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMAX1            ) != 0)) ||
      ( ((current_settings_prt.control_urov & CTR_UROV_STARTED_FROM_UMAX2  ) != 0) && (_CHECK_SET_BIT(p_active_functions, RANG_UMAX2            ) != 0)) ||
      (                                                                               (_CHECK_SET_BIT(p_active_functions, RANG_PUSK_UROV_VID_DV ) != 0))
     )     
    )
  {
    //Є умова запуску УРОВ
    
    unsigned int setpoint; //уставка - з якою зрівнюється вимірювальна величина
    unsigned int previous_state_po_urov;
    
    //Якщо ПО УРОВ ще не активне, то треба працювати по устаці спацювання - уставці, яка вводиться як основна з системи меню чи верхнього рівня
    //Якщо ПО УРОВ вже спрацювало, то треба працювати по уставці відпускання - береться процент від основної утанки по коефіцієнту повернення
    if(( previous_state_po_urov = _CHECK_SET_BIT(p_active_functions, RANG_PO_UROV) ) ==0 )
    {
      //Працюємо по утавці спрацювання
      setpoint = current_settings_prt.setpoint_urov[number_group_stp];
    }
    else
    {
      //Працюємо по утавці відпускання
      setpoint = current_settings_prt.setpoint_urov[number_group_stp]*KOEF_POVERNENNJA_GENERAL/100;
    }
    
    //Виставляємо, або скидаємо сигнал "ПО УРОВ"
    if (max_faze_current >= setpoint)
    {
      //Максимальний фазний стум перевищує свою уставку
      if(previous_state_po_urov == 0)
      {
        //Встановлюємо сигнал "ПО УРОВ"
        _SET_BIT(p_active_functions, RANG_PO_UROV);
      
        //Запускаємо таймери УРОВ1 і УРОВ2, якщо вони ще не запущені
        global_timers[INDEX_TIMER_UROV1] = 0;
        global_timers[INDEX_TIMER_UROV2] = 0;
      }
    }
    else 
    {
      //Максимальний фазний стум нижче уставки
      if(previous_state_po_urov != 0)
      {
        //Скидаємо сигнал "ПО УРОВ"
        _CLEAR_BIT(p_active_functions, RANG_PO_UROV);
        //Це є умовою також скидання сигналів "Сраб. УРОВ1" і "Сраб. УРОВ2"
        _CLEAR_BIT(p_active_functions, RANG_UROV1);
        _CLEAR_BIT(p_active_functions, RANG_UROV2);
        //Якщо таймери ще не скинуті, то скидаємо їх
        if ( global_timers[INDEX_TIMER_UROV1] >=0) global_timers[INDEX_TIMER_UROV1] = -1;
        if ( global_timers[INDEX_TIMER_UROV2] >=0) global_timers[INDEX_TIMER_UROV2] = -1;
      }
    }
    
    //Перевіряємо чи таймер УРОВ1 досягнув значення своєї витримки
    if(global_timers[INDEX_TIMER_UROV1] >= current_settings_prt.timeout_urov_1[number_group_stp])
    {
      //Якщо витримана Витримка УРОВ1 то встановлюємо сигнал "Сраб. УРОВ1"
      _SET_BIT(p_active_functions, RANG_UROV1);

      //Скидаємо таймер УРОВ1
      global_timers[INDEX_TIMER_UROV1] = -1;
    }

    //Перевіряємо чи таймер УРОВ2 досягнув значення своєї витримки
    if(global_timers[INDEX_TIMER_UROV2] >= current_settings_prt.timeout_urov_2[number_group_stp])
    {
      //Якщо витримана Витримка УРОВ2 то встановлюємо сигнал "Сраб. УРОВ2"
      _SET_BIT(p_active_functions, RANG_UROV2);

      //Скидаємо таймер УРОВ2
      global_timers[INDEX_TIMER_UROV2] = -1;
    }
  }
  else
  {
    //Треба скинути всі таймери і сигнали, які за УРОВ відповідають
    _CLEAR_BIT(p_active_functions, RANG_PO_UROV);
    _CLEAR_BIT(p_active_functions, RANG_UROV1);
    _CLEAR_BIT(p_active_functions, RANG_UROV2);
    global_timers[INDEX_TIMER_UROV1] = -1;
    global_timers[INDEX_TIMER_UROV2] = -1;
  }  
}
  /*******************************/
/*****************************************************/

/*****************************************************/
//АПВ
/*****************************************************/
inline void apv_handler(volatile unsigned int *p_active_functions, unsigned int number_group_stp)
{
  unsigned int logic_APV_0 = 0;
  unsigned int logic_APV_1 = 0;

  //Рестарт пристрою
  logic_APV_1 |= (start_restart != 0) << 19;
  //Очищаємо повідомлення про рестрат пристрою
  start_restart = 0;

  //"Статичне блокування"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STAT_BLK_APV) != 0) << 0;
  _INVERTOR(logic_APV_0, 0, logic_APV_0, 1);

  //М:"Пуск від МТЗ1"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ1) != 0) << 2;
  //"МТЗ1"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ1) != 0) << 3;
  _AND2(logic_APV_0, 2, logic_APV_0, 3, logic_APV_0, 4);

  //М:"Пуск від МТЗ2"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ2) != 0) << 5;
  //"МТЗ2"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ2) != 0) << 6;
  _AND2(logic_APV_0, 5, logic_APV_0, 6, logic_APV_0, 7);

  //М:"Пуск від МТЗ3"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ3) != 0) << 8;
  //"МТЗ3"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ3) != 0) << 9;
  _AND2(logic_APV_0, 8, logic_APV_0, 9, logic_APV_0, 10);

  //М:"Пуск від МТЗ4"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STARTED_FROM_MTZ4) != 0) << 11;
  //"МТЗ4"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_MTZ4) != 0) << 12;
  _AND2(logic_APV_0, 11, logic_APV_0, 12, logic_APV_0, 13);
  
  _OR4(logic_APV_0, 4, logic_APV_0, 7, logic_APV_0, 10, logic_APV_0, 13, logic_APV_0, 14);
  _TIMER_0_T(INDEX_TIMER_APV_TMP1, 1, logic_APV_0, 14, logic_APV_0, 15);

  //"АЧР/ЧАПВ від ДВ"
  logic_APV_0 |= 0/*(_CHECK_SET_BIT(p_active_functions, RANG_ACHR_CHAPV_VID_DV) != 0)*/ << 17;
  //М:"ЧАПВ від ДВ"
  logic_APV_1 |= 0/*((current_settings_prt.control_achr_chapv & CTR_CHAPV_VID_DV) != 0)*/ << 22;
  
  _AND2(logic_APV_0, 17, logic_APV_1, 22, logic_APV_0, 18);
  
  _TIMER_0_T(INDEX_TIMER_ACHR_CHAPV, MAX_VALUE_TIMER_FOR_COUNT_SIGNAL_ACHR_CHAPV, logic_APV_0, 18, logic_APV_0, 19);

  _OR2_INVERTOR(logic_APV_0, 15, logic_APV_0, 19, logic_APV_0, 20);

  //"Стан вимикача"
  logic_APV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_STATE_VV) != 0) << 21;
  _TIMER_IMPULSE(INDEX_TIMER_APV_BLOCK_VID_VV, current_settings_prt.timeout_apv_block_vid_VV[number_group_stp], previous_states_APV_0, 0, logic_APV_0, 21, logic_APV_0, 22);

  //М:"АПВ"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_1) != 0) << 23;
  _INVERTOR(logic_APV_0, 23, logic_APV_0, 24);

  //М:"АПВ2"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_2) != 0) << 25;
  _INVERTOR(logic_APV_0, 25, logic_APV_0, 26);

  //М:"АПВ3"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_3) != 0) << 27;
  _INVERTOR(logic_APV_0, 27, logic_APV_0, 28);

  //М:"АПВ4"
  logic_APV_0 |= ((current_settings_prt.control_apv & CTR_APV_STAGE_4) != 0) << 29;
  _INVERTOR(logic_APV_0, 29, logic_APV_0, 30);

  //Наступні операції виконуються ітераційно поки всі сигнали не встановляться у фіксований стан
  logic_APV_0 |= _GET_OUTPUT_STATE(previous_states_APV_0, 1) << 31;
  logic_APV_1 |= _GET_OUTPUT_STATE(previous_states_APV_0, 2) <<  0;
  logic_APV_1 |= _GET_OUTPUT_STATE(previous_states_APV_0, 3) <<  1;
  logic_APV_1 |= _GET_OUTPUT_STATE(previous_states_APV_0, 4) <<  2;
  
  unsigned int previous_trigger_APV_0;
  
  /*
  Скидаємо спочатку ці сигнали, бо у циклі do-while будемо їх виставляти,
  але після виставлення їх піде сигнал на очищення триґера, який і, фактично, буде
  означати очищення цих сигналів.
  Щоб зафіксувати спрацювання цих сигналів, ми їх очищуємо всіх перед циклом, а
  у циклі будемо тільки виставляти
  */
  _CLEAR_BIT(p_active_functions, RANG_APV1);
  _CLEAR_BIT(p_active_functions, RANG_APV2);
  _CLEAR_BIT(p_active_functions, RANG_APV3);
  _CLEAR_BIT(p_active_functions, RANG_APV4);
  do
  {
    /***
    Фіксуємо теперішній стан сигналів, які потім, можливо, зміняться і від зміни
    стану яких залежить завершення ітераційного процесу
    ***/
    _CLEAR_STATE(previous_states_APV_0, 1);
    previous_states_APV_0 |= _GET_OUTPUT_STATE(logic_APV_0, 31) << 1;
    _CLEAR_STATE(previous_states_APV_0, 2);
    previous_states_APV_0 |= _GET_OUTPUT_STATE(logic_APV_1,  0) << 2;
    _CLEAR_STATE(previous_states_APV_0, 3);
    previous_states_APV_0 |= _GET_OUTPUT_STATE(logic_APV_1,  1) << 3;
    _CLEAR_STATE(previous_states_APV_0, 4);
    previous_states_APV_0 |= _GET_OUTPUT_STATE(logic_APV_1,  2) << 4;
    
    previous_trigger_APV_0 = trigger_APV_0;
    /***/
    
    _OR2(logic_APV_1, 2, logic_APV_1, 1, logic_APV_1, 3);
    
    
    //АПВ1
    _OR6(logic_APV_1, 19, logic_APV_0, 14,                   trigger_APV_0, 1, trigger_APV_0, 2, trigger_APV_0, 3, logic_APV_0, 31, logic_APV_1, 4);
    _OR6(logic_APV_0, 18, logic_APV_0, 22, logic_APV_0, 24, logic_APV_1, 3, logic_APV_1, 0, logic_APV_1, 4, logic_APV_1, 8);
    _D_TRIGGER(_GET_OUTPUT_STATE(logic_APV_0, 1),  0, _GET_OUTPUT_STATE(logic_APV_1, 8), previous_states_APV_0, 5, logic_APV_0, 20, trigger_APV_0, 0);                 
    _TIMER_T_0(INDEX_TIMER_APV_1, current_settings_prt.timeout_apv_1[number_group_stp], trigger_APV_0, 0, logic_APV_1, 9);
    /*
    Цей сигнал встановлюмо в масив p_active_functions в циклі з післяумовою
    do-while, бо нас цікавить зафіксувати цей сигнал, а коли він виставиться, то
    піде сигнал на очистку триґера і цей сигнал очиститься. Але встановлення по АБО "0"
    після встановлення "1" не мало б зняти цей сигнал і ми його маємо зафіксувати до 
    наступної роботи системи захистів (скидання цих сигналів здійснюється перед
    циклом do-while).
    */
    if (_GET_OUTPUT_STATE(logic_APV_1, 9)) _SET_BIT(p_active_functions, RANG_APV1);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV1, current_settings_prt.timeout_apv_block_vid_apv1[number_group_stp], logic_APV_1, 9, logic_APV_0, 31);

    //АПВ2
    _AND2_INVERTOR(logic_APV_0, 31, logic_APV_0, 15, logic_APV_1, 10);
    _OR6(logic_APV_1, 19, logic_APV_0, 14, trigger_APV_0, 0,                   trigger_APV_0, 2, trigger_APV_0, 3, logic_APV_1,  0, logic_APV_1, 5);
    _OR6(logic_APV_0, 18, logic_APV_0, 24, logic_APV_0, 26, logic_APV_1, 2, logic_APV_1, 1, logic_APV_1, 5, logic_APV_1, 11);
    _D_TRIGGER(1,  0, _GET_OUTPUT_STATE(logic_APV_1, 11), previous_states_APV_0, 6, logic_APV_1, 10, trigger_APV_0, 1);    
    _TIMER_T_0(INDEX_TIMER_APV_2, current_settings_prt.timeout_apv_2[number_group_stp], trigger_APV_0, 1, logic_APV_1, 12);
    /*
    Цей сигнал встановлюмо в масив p_active_functions в циклі з післяумовою
    do-while, бо нас цікавить зафіксувати цей сигнал, а коли він виставиться, то
    піде сигнал на очистку триґера і цей сигнал очиститься. Але встановлення по АБО "0"
    після встановлення "1" не мало б зняти цей сигнал і ми його маємо зафіксувати до 
    наступної роботи системи захистів (скидання цих сигналів здійснюється перед
    циклом do-while).
    */
    if (_GET_OUTPUT_STATE(logic_APV_1, 12)) _SET_BIT(p_active_functions, RANG_APV2);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV2, current_settings_prt.timeout_apv_block_vid_apv2[number_group_stp], logic_APV_1, 12, logic_APV_1, 0);

    //АПВ3
    _AND2_INVERTOR(logic_APV_1, 0, logic_APV_0, 15, logic_APV_1, 13);
    _OR6(logic_APV_1, 19, logic_APV_0, 14, trigger_APV_0, 0, trigger_APV_0, 1,                   trigger_APV_0, 3, logic_APV_1,  1, logic_APV_1, 6);
    _OR5(logic_APV_0, 18, logic_APV_0, 24, logic_APV_0, 28, logic_APV_1, 2, logic_APV_1, 6, logic_APV_1, 14);
    _D_TRIGGER(1,  0, _GET_OUTPUT_STATE(logic_APV_1, 14), previous_states_APV_0, 7, logic_APV_1, 13, trigger_APV_0, 2);                 
    _TIMER_T_0(INDEX_TIMER_APV_3, current_settings_prt.timeout_apv_3[number_group_stp], trigger_APV_0, 2, logic_APV_1, 15);
    /*
    Цей сигнал встановлюмо в масив p_active_functions в циклі з післяумовою
    do-while, бо нас цікавить зафіксувати цей сигнал, а коли він виставиться, то
    піде сигнал на очистку триґера і цей сигнал очиститься. Але встановлення по АБО "0"
    після встановлення "1" не мало б зняти цей сигнал і ми його маємо зафіксувати до 
    наступної роботи системи захистів (скидання цих сигналів здійснюється перед
    циклом do-while).
    */
    if (_GET_OUTPUT_STATE(logic_APV_1, 15)) _SET_BIT(p_active_functions, RANG_APV3);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV3, current_settings_prt.timeout_apv_block_vid_apv3[number_group_stp], logic_APV_1, 15, logic_APV_1, 1);

    //АПВ4
    _AND2_INVERTOR(logic_APV_1, 1, logic_APV_0, 15, logic_APV_1, 16);
    _OR6(logic_APV_1, 19, logic_APV_0, 14, trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2,                   logic_APV_1,  2, logic_APV_1, 7);
    _OR4(logic_APV_0, 18, logic_APV_0, 24, logic_APV_0, 30, logic_APV_1, 7, logic_APV_1, 17);
    _D_TRIGGER(1,  0, _GET_OUTPUT_STATE(logic_APV_1, 17), previous_states_APV_0, 8, logic_APV_1, 16, trigger_APV_0, 3);                 
    _TIMER_T_0(INDEX_TIMER_APV_4, current_settings_prt.timeout_apv_4[number_group_stp], trigger_APV_0, 3, logic_APV_1, 18);
    /*
    Цей сигнал встановлюмо в масив p_active_functions в циклі з післяумовою
    do-while, бо нас цікавить зафіксувати цей сигнал, а коли він виставиться, то
    піде сигнал на очистку триґера і цей сигнал очиститься. Але встановлення по АБО "0"
    після встановлення "1" не мало б зняти цей сигнал і ми його маємо зафіксувати до 
    наступної роботи системи захистів (скидання цих сигналів здійснюється перед
    циклом do-while).
    */
    if (_GET_OUTPUT_STATE(logic_APV_1, 18)) _SET_BIT(p_active_functions, RANG_APV4);
    _TIMER_0_T(INDEX_TIMER_APV_BLOCK_VID_APV4, current_settings_prt.timeout_apv_block_vid_apv4[number_group_stp], logic_APV_1, 18, logic_APV_1, 2);
  }
  while (
         (_GET_OUTPUT_STATE(logic_APV_0, 31) != _GET_OUTPUT_STATE(previous_states_APV_0, 1)) ||
         (_GET_OUTPUT_STATE(logic_APV_1,  0) != _GET_OUTPUT_STATE(previous_states_APV_0, 2)) ||
         (_GET_OUTPUT_STATE(logic_APV_1,  1) != _GET_OUTPUT_STATE(previous_states_APV_0, 3)) ||
         (_GET_OUTPUT_STATE(logic_APV_1,  2) != _GET_OUTPUT_STATE(previous_states_APV_0, 4)) ||
         ( trigger_APV_0                     !=  previous_trigger_APV_0                    )  
        );
  
  _Bool work_apv = 0;
  _OR4(trigger_APV_0, 0, trigger_APV_0, 1, trigger_APV_0, 2, trigger_APV_0, 3, work_apv, 0);
  
  //Работа АПВ
  if (work_apv) _SET_BIT(p_active_functions, RANG_APV_WORK);
  else  _CLEAR_BIT(p_active_functions, RANG_APV_WORK);
  
}
/*****************************************************/

/*****************************************************/
//Функція управління блоками включення і відключення
/*****************************************************/
inline void on_off_handler(volatile unsigned int *p_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];
  unsigned int maska[N_BIG] = {0, 0, 0, 0, 0, 0, 0};

  /*********************/
  //Спочатку опрацьовуємо таймери
  /*********************/
  //Таймер  відключення
  if (global_timers[INDEX_TIMER_VIDKL_VV] >= 0)
  {
    //Таймер БО зараз активний і як мінімум тільки зараз завершить свою роботу
    if (global_timers[INDEX_TIMER_VIDKL_VV] >= current_settings_prt.timeout_swch_off)
    {
      //Таймер досягнув свого максимального значення
      global_timers[INDEX_TIMER_VIDKL_VV] = -1;
      //Відмічаємо у масиві функцій, які зараз активуються, що блок БО має бути деативованим
      _CLEAR_BIT(p_active_functions, RANG_WORK_BO);
    }
    //Незавершена робота блоку БО означає, що таймер блокування БВ має бути запущений і знаходитися у свому початковому значенні,
    //щоб як тільки блок БО відпрацює, щоб блокування включення почалося на весь час з моменту закінчення роботи блоку БО
    global_timers[INDEX_TIMER_BLK_VKL_VV] = 0;
  }
    
  //Таймер  блокування включення
  if (global_timers[INDEX_TIMER_BLK_VKL_VV] >= 0)
  {
    //Таймер блокування включення БВ зараз активний і як мінімум тільки зараз завершить свою роботу
    if (global_timers[INDEX_TIMER_BLK_VKL_VV] >= current_settings_prt.timeout_swch_udl_blk_on)
    {
      //Таймер досягнув свого максимального значення
      global_timers[INDEX_TIMER_BLK_VKL_VV] = -1;
    }
  }

  //Таймер  включення
  if (global_timers[INDEX_TIMER_VKL_VV] >= 0)
  {
    //Таймер БВ зараз активний і як мінімум тільки зараз завершить свою роботу

    //Якщо по якійсь причині таймер включення працює, при умові, що таймери БО і блокування включення ще не скинуті, то таймер включення треба скинути
    if ((global_timers[INDEX_TIMER_VIDKL_VV] >= 0) || (global_timers[INDEX_TIMER_BLK_VKL_VV] >=0))
    {
      global_timers[INDEX_TIMER_VKL_VV] = -1;
      //Відмічаємо у масиві функцій, які зараз активуються, що блок БB має бути деативованим
      _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
    }
    else
    {
      //Перевіряємо, чи таймер включення не досягнув свого масимального значення
      if (global_timers[INDEX_TIMER_VKL_VV] >= current_settings_prt.timeout_swch_on)
      {
        //Таймер досягнув свого максимального значення
        global_timers[INDEX_TIMER_VKL_VV] = -1;
        //Відмічаємо у масиві функцій, які зараз активуються, що блок БB має бути деативованим
        _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
      }
    }
  }
  /*********************/

  /*********************/
  //Першим розглядається блок відключення, бо він може блокувати включення вимикача
  /*********************/
  /*
  Цей сигнал встановлюється тільки у певних випадках, тому по замовчуванню його треба скинута,
  а коли буде потрібно - він встановиться
  */
  _CLEAR_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV);
  if (
      ((p_active_functions[0] & current_settings_prt.ranguvannja_off_cb[0]) != 0) ||
      ((p_active_functions[1] & current_settings_prt.ranguvannja_off_cb[1]) != 0) ||
      ((p_active_functions[2] & current_settings_prt.ranguvannja_off_cb[2]) != 0) ||
      ((p_active_functions[3] & current_settings_prt.ranguvannja_off_cb[3]) != 0) ||
      ((p_active_functions[4] & current_settings_prt.ranguvannja_off_cb[4]) != 0) ||
      ((p_active_functions[5] & current_settings_prt.ranguvannja_off_cb[5]) != 0) ||
      ((p_active_functions[6] & current_settings_prt.ranguvannja_off_cb[6]) != 0)
     )
  {
    //Є умова активації блку вимкнення
    _SET_BIT(p_active_functions, RANG_WORK_BO);

    //Запускаємо (або продовжуємо утримувати у 0, поки не пропаде сигнал активації БО) таймери: блоку БО, блокуванння БВ.
    global_timers[INDEX_TIMER_VIDKL_VV  ] = 0;
    global_timers[INDEX_TIMER_BLK_VKL_VV] = 0;
    
    //Скидаємо активацію блоку ввімкнення
    _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
    //Скидаємо таймер блку вимкнення
    global_timers[INDEX_TIMER_VKL_VV] = -1;  

    /*
    Формуємо сигнал "Відключення від захистів" (він рівний наявності умови команди
    активації команди "Робота БО" будь-якою командою за виключенняв "Вимкн. ВВ")
    */
    //Формуємо інвертовану маску для виключення команди "Вимк.ВВ"
    for (unsigned int i = 0; i < N_BIG; i++ )  maska[i] = (unsigned int)(~0);
    _CLEAR_BIT(maska, RANG_OTKL_VV);
    if (
        ((p_active_functions[0] & maska[0]) != 0) ||
        ((p_active_functions[1] & maska[1]) != 0) ||
        ((p_active_functions[2] & maska[2]) != 0) ||
        ((p_active_functions[3] & maska[3]) != 0) ||
        ((p_active_functions[4] & maska[4]) != 0) ||
        ((p_active_functions[5] & maska[5]) != 0) ||
        ((p_active_functions[6] & maska[6]) != 0)
       )
    {
      //Вимкнення від захистів
      _SET_BIT(p_active_functions, RANG_VIDKL_VID_ZAKHYSTIV);
      
      unsigned int temp_array_of_outputs[N_BIG];
      for (unsigned int i = 0; i < N_BIG; i++ ) temp_array_of_outputs[i] = (p_active_functions[i] & maska[i]);
          
      /*****************************************************
      Формуванні інформації про причину відключення для меню
      *****************************************************/
      unsigned char *label_to_time_array;
      if (copying_time == 0) label_to_time_array = time;
      else label_to_time_array = time_copy;
          
      //МТЗ1
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_MTZ1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_MTZ1);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ1][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_MTZ1);
      }
      
      //МТЗ2
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_MTZ2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_MTZ2);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ2][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_MTZ2);
      }
      
      //МТЗ3
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_MTZ3) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ3) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_MTZ3);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ3][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_MTZ3);
      }
      
      //МТЗ4
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_MTZ4) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_MTZ4) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_MTZ4);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_MTZ4][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_MTZ4);
      }
      
      //ЗДЗ
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_ZDZ) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ZDZ) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_ZDZ);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZDZ][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_ZDZ);
      }
      
      //УРОВ1
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_UROV1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UROV1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_UROV1);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UROV1][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_UROV1);
      }
      
      //УРОВ2
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_UROV2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UROV2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_UROV2);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UROV2][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_UROV2);
      }
      
      //ЗОП
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_ZOP) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_ZOP) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_ZOP);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZOP][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_ZOP);
      }
      
      //Umin1
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_UMIN1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMIN1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_UMIN1);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMIN1][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_UMIN1);
      }
      
      //Umin2
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_UMIN2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMIN2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_UMIN2);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMIN2][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_UMIN2);
      }
      
      //Umax1
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_UMAX1) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMAX1) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_UMAX1);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMAX1][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_UMAX1);
      }
      
      //Umax2
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_UMAX2) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_UMAX2) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_UMAX2);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_UMAX2][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_UMAX2);
      }
      
      //Відключення від зовнішніх захистів
      if(
         (_CHECK_SET_BIT(temp_array_of_outputs, RANG_OTKL_VID_ZOVN_ZAHYSTIV) != 0) &&
         (_CHECK_SET_BIT(previous_active_functions, RANG_OTKL_VID_ZOVN_ZAHYSTIV) == 0) /*умова, що сигнал тільки активується (щоб зафіксувати час старту)*/
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_ZOVNISHNIKH_ZAKHYSTIV][i] = *(label_to_time_array + i);

        _CLEAR_BIT(temp_array_of_outputs, RANG_OTKL_VID_ZOVN_ZAHYSTIV);
      }
      
      //Відключення від інших сигналів (крім відключення від сигналу "Вимк.ВВ")
      if(
         (
          (temp_array_of_outputs[0] != 0) ||
          (temp_array_of_outputs[1] != 0) ||
          (temp_array_of_outputs[2] != 0) ||
          (temp_array_of_outputs[3] != 0) ||
          (temp_array_of_outputs[4] != 0) ||
          (temp_array_of_outputs[5] != 0) ||
          (temp_array_of_outputs[6] != 0)
         )
         &&
         (
          ((previous_active_functions[0] & temp_array_of_outputs[0])!= temp_array_of_outputs[0]) ||
          ((previous_active_functions[1] & temp_array_of_outputs[1])!= temp_array_of_outputs[1]) ||
          ((previous_active_functions[2] & temp_array_of_outputs[2])!= temp_array_of_outputs[2]) ||
          ((previous_active_functions[3] & temp_array_of_outputs[3])!= temp_array_of_outputs[3]) ||
          ((previous_active_functions[4] & temp_array_of_outputs[4])!= temp_array_of_outputs[4]) ||
          ((previous_active_functions[5] & temp_array_of_outputs[5])!= temp_array_of_outputs[5]) ||
          ((previous_active_functions[6] & temp_array_of_outputs[6])!= temp_array_of_outputs[6])
         ) 
        )   
      {
        info_vidkluchennja_vymykacha |= (1 << VYMKNENNJA_VID_INSHYKH_SYGNALIV);
        for(unsigned int i = 0; i < 7; i++) info_vidkluchennja_vymykachatime[VYMKNENNJA_VID_INSHYKH_SYGNALIV][i] = *(label_to_time_array + i);
      }
      /*****************************************************/
    }
  }

  /*********************/
  //Потім розглядається блок включення
  /*********************/
  if (
      (global_timers[INDEX_TIMER_VIDKL_VV  ] < 0) && 
      (global_timers[INDEX_TIMER_BLK_VKL_VV] < 0) &&
      (_CHECK_SET_BIT(p_active_functions, RANG_BLOCK_VKL_VV) == 0)
     )
  {
    //Оскільки не працюють таймери БО і блокування включення БВ, а також немає сигналу блокування включення ВВ
    //тому перевіряємо, чи немає умови запуску БВ

    if (
        ((p_active_functions[0] & current_settings_prt.ranguvannja_on_cb[0]) != 0) ||
        ((p_active_functions[1] & current_settings_prt.ranguvannja_on_cb[1]) != 0) ||
        ((p_active_functions[2] & current_settings_prt.ranguvannja_on_cb[2]) != 0) ||
        ((p_active_functions[3] & current_settings_prt.ranguvannja_on_cb[3]) != 0) ||
        ((p_active_functions[4] & current_settings_prt.ranguvannja_on_cb[4]) != 0) ||
        ((p_active_functions[5] & current_settings_prt.ranguvannja_on_cb[5]) != 0) ||
        ((p_active_functions[6] & current_settings_prt.ranguvannja_on_cb[6]) != 0)
      )
    {
      //Відмічаємо у масиві функцій, які зараз активуються, що ще треба активувати блок БВ (якщо він ще не активний)
      _SET_BIT(p_active_functions, RANG_WORK_BV);

      //Запускаємо (або продовжуємо утримувати у 0, поки не пропаде сигнал активації БВ) таймер роботи БВ
      global_timers[INDEX_TIMER_VKL_VV] = 0;
    }
  }
  else
  {
    //На даний момент існує одна або більше умов блокування БВ
    global_timers[INDEX_TIMER_VKL_VV] = -1;
    _CLEAR_BIT(p_active_functions, RANG_WORK_BV);
  }
  /*********************/

  /*********************/
  //Формуємо попереденій стан сигналів для функції ввімкнення/вимкнення
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = p_active_functions[i];
  /*********************/
}
/*****************************************************/

/*****************************************************/
//Контроль приводу вимикача
/*****************************************************/
void control_VV(volatile unsigned int *p_active_functions)
{
  unsigned int logic_control_VV_0 = 0;

  //"Контроль Вкл."
  logic_control_VV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_CTRL_VKL ) != 0) << 0;
  //"Контроль Откл."
  logic_control_VV_0 |= (_CHECK_SET_BIT(p_active_functions, RANG_CTRL_OTKL) != 0) << 1;
  
  _XOR_INVERTOR(logic_control_VV_0, 0, logic_control_VV_0, 1, logic_control_VV_0, 2);

  _TIMER_T_0(INDEX_TIMER_PRYVOD_VV, current_settings_prt.timeout_pryvoda_VV, logic_control_VV_0, 2, logic_control_VV_0, 3);

  //М:"Контроль ВВ"
  logic_control_VV_0 |= ((current_settings_prt.control_switch & CTR_PRYVOD_VV) != 0) << 4;
  
  _AND2(logic_control_VV_0, 3, logic_control_VV_0, 4, logic_control_VV_0, 5);
  
  if (_GET_OUTPUT_STATE(logic_control_VV_0, 5))
    _SET_BIT(p_active_functions, RANG_PRYVID_VV);
  else
    _CLEAR_BIT(p_active_functions, RANG_PRYVID_VV);
}
/*****************************************************/

/*****************************************************/
//Розрахунок коефіцієнтрів для підрахунку ресурсу вимикача
/*****************************************************/
void make_koef_for_resurs(void)
{
  float tmp_1, tmp_2;
  
  tmp_1 = (float)current_settings.setpoint_r_kom_st_Inom/(float)current_settings.setpoint_r_kom_st_Inom_vymk;
  tmp_1  = log10f(tmp_1);

  tmp_2 = (float)current_settings.setpoint_Inom_vymk/(float)current_settings.setpoint_Inom;
  tmp_2  = log10f(tmp_2);
  
  //Помічаємо, що коефіцієнти для розрахунку ресурсу вимикача зараз будуть змінені
  koef_resurs_changed = CHANGED_ETAP_EXECUTION;

  K_resurs = tmp_1/tmp_2;
  
  //Помічаємо, що коефіцієнти для розрахунку ресурсу вимикача змінені і чекають на прийом системою захистів
  koef_resurs_changed = CHANGED_ETAP_ENDED;
}
/*****************************************************/

/*****************************************************/
//Лічильник ресурсу
/*****************************************************/
inline void resurs_vymykacha_handler(volatile unsigned int *p_active_functions)
{
  static unsigned int previous_active_functions[N_BIG];
  
  if (restart_counter != 0)
  {
    //Треба подати команду очистки ресурсу вимикача
    resurs_vymykacha = current_settings_prt.setpoint_pochatkovyj_resurs;
    resurs_vidkljuchennja = current_settings_prt.setpoint_pochatkova_k_vymk;
    
     restart_counter = 0;

    //Запускаємо запис у EEPROM
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);

    //Помічаємо, що відбулася очистка ресурсу вимикача
    information_about_restart_counter |= ((1 << USB_RECUEST)|(1 << RS485_RECUEST));
  }
  
  /*******************************/
  //Визначаємо максимальний фазовий струм під час роботи Блоку Вимкнення (БО)
  /*******************************/
  static unsigned int max_faze_current_vv;
  if (_CHECK_SET_BIT(p_active_functions, RANG_WORK_BO) != 0)
  {
    unsigned int max_faze_current_tmp = measurement[IM_IA];
    if (max_faze_current_tmp < measurement[IM_IB]) max_faze_current_tmp = measurement[IM_IB];
    if (max_faze_current_tmp < measurement[IM_IC]) max_faze_current_tmp = measurement[IM_IC];
    max_faze_current_tmp = max_faze_current_tmp*current_settings_prt.TCurrent/1000;
  
    if (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) == 0)
    {
      //Зафіксовано старт роботи БО
      max_faze_current_vv = max_faze_current_tmp;
    }
    else if (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) != 0)
    {
      //БО продовжує свою роботу
      if (max_faze_current_vv < max_faze_current_tmp) max_faze_current_vv = max_faze_current_tmp;
    }
    
    if (max_faze_current_vv > current_settings_prt.setpoint_Inom_vymk) 
    {
      _SET_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
    }
    else
    {
      _CLEAR_BIT(p_active_functions, RANG_PEREVYSHCHENNJA_Inom_VYMK);
    }
  }
  /*******************************/
  
  if (
      ((current_settings_prt.control_switch & CTR_RESURS_VV) != 0) &&
      (_CHECK_SET_BIT(previous_active_functions, RANG_WORK_BO) != 0) &&
      (_CHECK_SET_BIT(p_active_functions , RANG_WORK_BO) == 0)
     ) 
  {
    //Зафіксовано завершення "Робота БО"
    
    /*******************************/
    //Обчислюється ресурс вимикачa
    /*******************************/
    unsigned int N = 0;
    if (max_faze_current_vv >= current_settings_prt.setpoint_Inom_vymk) {
      N = (unsigned int) ceilf((float)current_settings_prt.setpoint_r_kom_st_Inom/(float)current_settings_prt.setpoint_r_kom_st_Inom_vymk);
    } else if (max_faze_current_vv <= current_settings_prt.setpoint_Inom) {
      N = 1;
    } else {
      float I_div_Inom = (float)max_faze_current_vv / (float)current_settings_prt.setpoint_Inom;
      float lg_I_div_Inom = log10f(I_div_Inom);
      float lg_N = K_resurs_prt * lg_I_div_Inom;
      float N_ = powf(10, lg_N);
      N = (unsigned int) ceilf(N_);
      if ((((float)N) - N_) >= 0.5f)
      {
        N--;
      }
      
    }
    
    if (resurs_vymykacha <= N) {
      resurs_vymykacha = 0;
    } else {
      resurs_vymykacha -= N;
    }
    
    //Загальне число переключень
    if ((0xffffffff - resurs_vidkljuchennja) >= 1)
    {
      //Додавання одиниці не викличе переповнення
      resurs_vidkljuchennja++;
    }

    //Запускаємо запис у EEPROM
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT);
  }
  
  if (resurs_vymykacha <= current_settings_prt.setpoint_krytychnyj_resurs) 
  {
    _SET_BIT(p_active_functions, RANG_KRYTYCHNYJ_RESURS_VYMYKACHA);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_KRYTYCHNYJ_RESURS_VYMYKACHA);
  }

  if (resurs_vymykacha == 0) 
  {
    _SET_BIT(p_active_functions, RANG_VYCHERPANYJ_RESURS_VYMYKACHA);
  }
  else
  {
    _CLEAR_BIT(p_active_functions, RANG_VYCHERPANYJ_RESURS_VYMYKACHA);
  }
 

  /*********************/
  //Формуємо попереденій стан сигналів для функції ввімкнення/вимкнення
  /*********************/
  for (unsigned int i = 0; i < N_BIG; i++) previous_active_functions[i] = p_active_functions[i];
  /*********************/
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
//Початок моніторингу максимального фазного струму 
/*****************************************************/
inline void start_monitoring_max_phase_current(unsigned int time_tmp)
{
  //Збільшуємо кількість фіксованих значень максимального фазного струму
  number_max_phase_dr++;
  
  //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
  int frequency_val_1_int = (int)frequency_val_1;
  if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
  int frequency_val_2_int = (int)frequency_val_2;
  if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

  measurements_phase_max_dr[ 0] = measurement[IM_IA];
  measurements_phase_max_dr[ 1] = measurement[IM_IB];
  measurements_phase_max_dr[ 2] = measurement[IM_IC];
  measurements_phase_max_dr [3] = measurement[IM_I2];
  measurements_phase_max_dr[ 4] = measurement[IM_I1];
  measurements_phase_max_dr[ 5] = measurement[IM_UA1];
  measurements_phase_max_dr[ 6] = measurement[IM_UB1];
  measurements_phase_max_dr[ 7] = measurement[IM_UC1];
  measurements_phase_max_dr[ 8] = measurement[IM_UA2];
  measurements_phase_max_dr[ 9] = measurement[IM_UB2];
  measurements_phase_max_dr[10] = measurement[IM_UC2];
  measurements_phase_max_dr[11] = measurement[IM_UAB1];
  measurements_phase_max_dr[12] = measurement[IM_UBC1];
  measurements_phase_max_dr[13] = measurement[IM_UCA1];
  measurements_phase_max_dr[14] = measurement[IM_UAB2];
  measurements_phase_max_dr[15] = measurement[IM_UBC2];
  measurements_phase_max_dr[16] = measurement[IM_UCA2];
  measurements_phase_max_dr[17] = (unsigned int)frequency_val_1_int;
  measurements_phase_max_dr[18] = (unsigned int)frequency_val_2_int;
  
  //Визначаємо макисальний фазний струм між трьома фазами
  max_phase_current_dr = measurements_phase_max_dr[0];
  if (max_phase_current_dr < measurements_phase_max_dr[1]) max_phase_current_dr = measurements_phase_max_dr[1];
  if (max_phase_current_dr < measurements_phase_max_dr[2]) max_phase_current_dr = measurements_phase_max_dr[2];

  //Фіксуємо час з моменту початку аварійного запису
  measurements_phase_max_dr[19] = time_tmp;

  //Помічаємо, що ми на стадії моніторингу максимального фазного струму
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE);
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального фазного струму
/*****************************************************/
inline void continue_monitoring_max_phase_current(unsigned int time_tmp)
{
  //Перевірка, чи не є зарза фазний струм більший, ніж той що помічений максимальним
  if (
      (max_phase_current_dr < measurement[IM_IA]) ||
      (max_phase_current_dr < measurement[IM_IB]) ||
      (max_phase_current_dr < measurement[IM_IC])
     )
  {
    //Зафіксовано зріз при найвищому фазовому струмі з моменту початку спостереження за ним
    int frequency_val_1_int = (int)frequency_val_1;
    if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
    int frequency_val_2_int = (int)frequency_val_2;
    if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

    measurements_phase_max_dr[ 0] = measurement[IM_IA];
    measurements_phase_max_dr[ 1] = measurement[IM_IB];
    measurements_phase_max_dr[ 2] = measurement[IM_IC];
    measurements_phase_max_dr [3] = measurement[IM_I2];
    measurements_phase_max_dr[ 4] = measurement[IM_I1];
    measurements_phase_max_dr[ 5] = measurement[IM_UA1];
    measurements_phase_max_dr[ 6] = measurement[IM_UB1];
    measurements_phase_max_dr[ 7] = measurement[IM_UC1];
    measurements_phase_max_dr[ 8] = measurement[IM_UA2];
    measurements_phase_max_dr[ 9] = measurement[IM_UB2];
    measurements_phase_max_dr[10] = measurement[IM_UC2];
    measurements_phase_max_dr[11] = measurement[IM_UAB1];
    measurements_phase_max_dr[12] = measurement[IM_UBC1];
    measurements_phase_max_dr[13] = measurement[IM_UCA1];
    measurements_phase_max_dr[14] = measurement[IM_UAB2];
    measurements_phase_max_dr[15] = measurement[IM_UBC2];
    measurements_phase_max_dr[16] = measurement[IM_UCA2];
    measurements_phase_max_dr[17] = (unsigned int)frequency_val_1_int;
    measurements_phase_max_dr[18] = (unsigned int)frequency_val_2_int;

    max_phase_current_dr = measurements_phase_max_dr[0];
    if (max_phase_current_dr < measurements_phase_max_dr[1]) max_phase_current_dr = measurements_phase_max_dr[1];
    if (max_phase_current_dr < measurements_phase_max_dr[2]) max_phase_current_dr = measurements_phase_max_dr[2];

    //Фіксуємо час з моменту початку аварійного запису
    measurements_phase_max_dr[19] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу мінімальної фазної/лінійної напруги 
/*****************************************************/
inline void start_monitoring_min_U(unsigned int time_tmp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  //Збільшуємо кількість фіксованих значень мінімальної фазної/лінійної напруги
  number_min_U_dr++;
  
  //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найменші
  int frequency_val_1_int = (int)frequency_val_1;
  if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
  int frequency_val_2_int = (int)frequency_val_2;
  if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

  measurements_U_min_dr[ 0] = measurement[IM_IA];
  measurements_U_min_dr[ 1] = measurement[IM_IB];
  measurements_U_min_dr[ 2] = measurement[IM_IC];
  measurements_U_min_dr [3] = measurement[IM_I2];
  measurements_U_min_dr[ 4] = measurement[IM_I1];
  measurements_U_min_dr[ 5] = measurement[IM_UA1];
  measurements_U_min_dr[ 6] = measurement[IM_UB1];
  measurements_U_min_dr[ 7] = measurement[IM_UC1];
  measurements_U_min_dr[ 8] = measurement[IM_UA2];
  measurements_U_min_dr[ 9] = measurement[IM_UB2];
  measurements_U_min_dr[10] = measurement[IM_UC2];
  measurements_U_min_dr[11] = measurement[IM_UAB1];
  measurements_U_min_dr[12] = measurement[IM_UBC1];
  measurements_U_min_dr[13] = measurement[IM_UCA1];
  measurements_U_min_dr[14] = measurement[IM_UAB2];
  measurements_U_min_dr[15] = measurement[IM_UBC2];
  measurements_U_min_dr[16] = measurement[IM_UCA2];
  measurements_U_min_dr[17] = (unsigned int)frequency_val_1_int;
  measurements_U_min_dr[18] = (unsigned int)frequency_val_2_int;
  
  if ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0)
  {
    //Визначаємо мінімальної лінійну напругу між трьома фазами
    min_voltage_dr = measurement[index_IM_UAB];
    if (min_voltage_dr > measurement[index_IM_UBC]) min_voltage_dr = measurement[index_IM_UBC];
    if (min_voltage_dr > measurement[index_IM_UCA]) min_voltage_dr = measurement[index_IM_UCA];
  }
  else
  {
    //Визначаємо мінімальної фазну напругу між трьома фазами
    min_voltage_dr = measurement[index_IM_UA];
    if (min_voltage_dr > measurement[index_IM_UB]) min_voltage_dr = measurement[index_IM_UB];
    if (min_voltage_dr > measurement[index_IM_UC]) min_voltage_dr = measurement[index_IM_UC];
  }

  //Фіксуємо час з моменту початку аварійного запису
  measurements_U_min_dr[19] = time_tmp;

  //Помічаємо, що ми на стадії моніторингу мінімальної фазної/лінійної напруги
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE);
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу мінімальної фазної/лінійної напруги
/*****************************************************/
inline void continue_monitoring_min_U(unsigned int time_tmp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  //Перевірка, чи не є зарза досліджувана напуга менша, ніж та що помічена мінімальною
  if (
      (
       ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0) &&
       (  
        (min_voltage_dr > measurement[index_IM_UAB]) ||
        (min_voltage_dr > measurement[index_IM_UBC]) ||
        (min_voltage_dr > measurement[index_IM_UCA])
       )
      )   
      || 
      (
       ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) != 0) &&
       (  
        (min_voltage_dr > measurement[index_IM_UA]) ||
        (min_voltage_dr > measurement[index_IM_UB]) ||
        (min_voltage_dr > measurement[index_IM_UC])
       )
     )
    )    
  {
    //Зафіксовано зріз при найнижчій досліджуваній напрузі з моменту початку спостереження за нею
    int frequency_val_1_int = (int)frequency_val_1;
    if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
    int frequency_val_2_int = (int)frequency_val_2;
    if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

    measurements_U_min_dr[ 0] = measurement[IM_IA];
    measurements_U_min_dr[ 1] = measurement[IM_IB];
    measurements_U_min_dr[ 2] = measurement[IM_IC];
    measurements_U_min_dr [3] = measurement[IM_I2];
    measurements_U_min_dr[ 4] = measurement[IM_I1];
    measurements_U_min_dr[ 5] = measurement[IM_UA1];
    measurements_U_min_dr[ 6] = measurement[IM_UB1];
    measurements_U_min_dr[ 7] = measurement[IM_UC1];
    measurements_U_min_dr[ 8] = measurement[IM_UA2];
    measurements_U_min_dr[ 9] = measurement[IM_UB2];
    measurements_U_min_dr[10] = measurement[IM_UC2];
    measurements_U_min_dr[11] = measurement[IM_UAB1];
    measurements_U_min_dr[12] = measurement[IM_UBC1];
    measurements_U_min_dr[13] = measurement[IM_UCA1];
    measurements_U_min_dr[14] = measurement[IM_UAB2];
    measurements_U_min_dr[15] = measurement[IM_UBC2];
    measurements_U_min_dr[16] = measurement[IM_UCA2];
    measurements_U_min_dr[17] = (unsigned int)frequency_val_1_int;
    measurements_U_min_dr[18] = (unsigned int)frequency_val_2_int;

    if ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0)
    {
      //Визначаємо мінімальної лінійну напругу між трьома фазами
      min_voltage_dr = measurement[index_IM_UAB];
      if (min_voltage_dr > measurement[index_IM_UBC]) min_voltage_dr = measurement[index_IM_UBC];
      if (min_voltage_dr > measurement[index_IM_UCA]) min_voltage_dr = measurement[index_IM_UCA];
    }
    else
    {
      //Визначаємо мінімальної фазну напругу між трьома фазами
      min_voltage_dr = measurement[index_IM_UA];
      if (min_voltage_dr > measurement[index_IM_UB]) min_voltage_dr = measurement[index_IM_UB];
      if (min_voltage_dr > measurement[index_IM_UC]) min_voltage_dr = measurement[index_IM_UC];
    }

    //Фіксуємо час з моменту початку аварійного запису
    measurements_U_min_dr[19] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу максимальної фазної/лінійної напруги 
/*****************************************************/
inline void start_monitoring_max_U(unsigned int time_tmp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  //Збільшуємо кількість фіксованих значень максимальної фазної/лінійної напруги
  number_max_U_dr++;
  
  //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
  int frequency_val_1_int = (int)frequency_val_1;
  if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
  int frequency_val_2_int = (int)frequency_val_2;
  if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

  measurements_U_max_dr[ 0] = measurement[IM_IA];
  measurements_U_max_dr[ 1] = measurement[IM_IB];
  measurements_U_max_dr[ 2] = measurement[IM_IC];
  measurements_U_max_dr [3] = measurement[IM_I2];
  measurements_U_max_dr[ 4] = measurement[IM_I1];
  measurements_U_max_dr[ 5] = measurement[IM_UA1];
  measurements_U_max_dr[ 6] = measurement[IM_UB1];
  measurements_U_max_dr[ 7] = measurement[IM_UC1];
  measurements_U_max_dr[ 8] = measurement[IM_UA2];
  measurements_U_max_dr[ 9] = measurement[IM_UB2];
  measurements_U_max_dr[10] = measurement[IM_UC2];
  measurements_U_max_dr[11] = measurement[IM_UAB1];
  measurements_U_max_dr[12] = measurement[IM_UBC1];
  measurements_U_max_dr[13] = measurement[IM_UCA1];
  measurements_U_max_dr[14] = measurement[IM_UAB2];
  measurements_U_max_dr[15] = measurement[IM_UBC2];
  measurements_U_max_dr[16] = measurement[IM_UCA2];
  measurements_U_max_dr[17] = (unsigned int)frequency_val_1_int;
  measurements_U_max_dr[18] = (unsigned int)frequency_val_2_int;
  
  if ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0)
  {
    //Визначаємо макисальну лінійну напругу між трьома фазами
    max_voltage_dr = measurement[index_IM_UAB];
    if (max_voltage_dr < measurement[index_IM_UBC]) max_voltage_dr = measurement[index_IM_UBC];
    if (max_voltage_dr <measurement[index_IM_UCA]) max_voltage_dr = measurement[index_IM_UCA];
  }
  else
  {
    //Визначаємо макисальну фазну напругу між трьома фазами
    max_voltage_dr = measurement[index_IM_UA];
    if (max_voltage_dr < measurement[index_IM_UB]) max_voltage_dr = measurement[index_IM_UB];
    if (max_voltage_dr <measurement[index_IM_UC]) max_voltage_dr = measurement[index_IM_UC];
  }

  //Фіксуємо час з моменту початку аварійного запису
  measurements_U_max_dr[19] = time_tmp;

  //Помічаємо, що ми на стадії моніторингу максимальної фазної/лінійної напруги
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE);
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимальної фазної/лінійної напруги
/*****************************************************/
inline void continue_monitoring_max_U(unsigned int time_tmp)
{
  unsigned int index_IM_UA, index_IM_UB, index_IM_UC;
  unsigned int index_IM_UAB, index_IM_UBC, index_IM_UCA;
  if (TN1_TN2 == 1)
  {
    index_IM_UA = IM_UA2;
    index_IM_UB = IM_UB2;
    index_IM_UC = IM_UC2;

    index_IM_UAB = IM_UAB2;
    index_IM_UBC = IM_UBC2;
    index_IM_UCA = IM_UCA2;
  }
  else
  {
    index_IM_UA = IM_UA1;
    index_IM_UB = IM_UB1;
    index_IM_UC = IM_UC1;

    index_IM_UAB = IM_UAB1;
    index_IM_UBC = IM_UBC1;
    index_IM_UCA = IM_UCA1;
  }

  //Перевірка, чи не є зарза досліджувана напуга більша, ніж та що помічена максимальною
  if (
      (
       ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0) &&
       (  
        (max_voltage_dr < measurement[index_IM_UAB]) ||
        (max_voltage_dr < measurement[index_IM_UBC]) ||
        (max_voltage_dr < measurement[index_IM_UCA])
       )
      )   
      || 
      (
       ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) != 0) &&
       (  
        (max_voltage_dr < measurement[index_IM_UA]) ||
        (max_voltage_dr < measurement[index_IM_UB]) ||
        (max_voltage_dr < measurement[index_IM_UC])
       )
     )
    )    
  {
    //Зафіксовано зріз при найвищійу досліджуваній напрузі з моменту початку спостереження за нею
    int frequency_val_1_int = (int)frequency_val_1;
    if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
    int frequency_val_2_int = (int)frequency_val_2;
    if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

    measurements_U_max_dr[ 0] = measurement[IM_IA];
    measurements_U_max_dr[ 1] = measurement[IM_IB];
    measurements_U_max_dr[ 2] = measurement[IM_IC];
    measurements_U_max_dr [3] = measurement[IM_I2];
    measurements_U_max_dr[ 4] = measurement[IM_I1];
    measurements_U_max_dr[ 5] = measurement[IM_UA1];
    measurements_U_max_dr[ 6] = measurement[IM_UB1];
    measurements_U_max_dr[ 7] = measurement[IM_UC1];
    measurements_U_max_dr[ 8] = measurement[IM_UA2];
    measurements_U_max_dr[ 9] = measurement[IM_UB2];
    measurements_U_max_dr[10] = measurement[IM_UC2];
    measurements_U_max_dr[11] = measurement[IM_UAB1];
    measurements_U_max_dr[12] = measurement[IM_UBC1];
    measurements_U_max_dr[13] = measurement[IM_UCA1];
    measurements_U_max_dr[14] = measurement[IM_UAB2];
    measurements_U_max_dr[15] = measurement[IM_UBC2];
    measurements_U_max_dr[16] = measurement[IM_UCA2];
    measurements_U_max_dr[17] = (unsigned int)frequency_val_1_int;
    measurements_U_max_dr[18] = (unsigned int)frequency_val_2_int;

    if ((current_settings_prt.control_transformator & CTR_TRANSFORMATOR_LINE_PHASE) == 0)
    {
      //Визначаємо макисальну лінійну напругу між трьома фазами
      max_voltage_dr = measurement[index_IM_UAB];
      if (max_voltage_dr < measurement[index_IM_UBC]) max_voltage_dr = measurement[index_IM_UBC];
      if (max_voltage_dr <measurement[index_IM_UCA]) max_voltage_dr = measurement[index_IM_UCA];
    }
    else
    {
      //Визначаємо макисальну фазну напругу між трьома фазами
      max_voltage_dr = measurement[index_IM_UA];
      if (max_voltage_dr < measurement[index_IM_UB]) max_voltage_dr = measurement[index_IM_UB];
      if (max_voltage_dr <measurement[index_IM_UC]) max_voltage_dr = measurement[index_IM_UC];
    }

    //Фіксуємо час з моменту початку аварійного запису
    measurements_U_max_dr[19] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//Початок моніторингу максимального струму  зворотньої послідовності
/*****************************************************/
inline void start_monitoring_max_ZOP(unsigned int time_tmp)
{
  //Збільшуємо кількість фіксованих значень максимального 3I0
  number_max_ZOP_dr++;
  
  //Помічаємо, що будем виходити з того, що зараз значення тільки починають моніторитися, тому приймаємо їх за найбільші
  int frequency_val_1_int = (int)frequency_val_1;
  if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
  int frequency_val_2_int = (int)frequency_val_2;
  if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

  measurements_ZOP_max_dr[ 0] = measurement[IM_IA];
  measurements_ZOP_max_dr[ 1] = measurement[IM_IB];
  measurements_ZOP_max_dr[ 2] = measurement[IM_IC];
  measurements_ZOP_max_dr [3] = measurement[IM_I2];
  measurements_ZOP_max_dr[ 4] = measurement[IM_I1];
  measurements_ZOP_max_dr[ 5] = measurement[IM_UA1];
  measurements_ZOP_max_dr[ 6] = measurement[IM_UB1];
  measurements_ZOP_max_dr[ 7] = measurement[IM_UC1];
  measurements_ZOP_max_dr[ 8] = measurement[IM_UA2];
  measurements_ZOP_max_dr[ 9] = measurement[IM_UB2];
  measurements_ZOP_max_dr[10] = measurement[IM_UC2];
  measurements_ZOP_max_dr[11] = measurement[IM_UAB1];
  measurements_ZOP_max_dr[12] = measurement[IM_UBC1];
  measurements_ZOP_max_dr[13] = measurement[IM_UCA1];
  measurements_ZOP_max_dr[14] = measurement[IM_UAB2];
  measurements_ZOP_max_dr[15] = measurement[IM_UBC2];
  measurements_ZOP_max_dr[16] = measurement[IM_UCA2];
  measurements_ZOP_max_dr[17] = (unsigned int)frequency_val_1_int;
  measurements_ZOP_max_dr[18] = (unsigned int)frequency_val_2_int;
  
  //Фіксуємо час з моменту початку аварійного запису
  measurements_ZOP_max_dr[19] = time_tmp;

  //Помічаємо, що ми на стадії моніторингу максимального струму зворотньої послідовності
  state_current_monitoring |= (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP);
}
/*****************************************************/

/*****************************************************/
//Продовження моніторингу максимального струму зворотньої послідовності
/*****************************************************/
inline void continue_monitoring_max_ZOP(unsigned int time_tmp)
{
  //Перевірка, чи не є зарза струм зворотньої послідовності більший, ніж той що помічений максимальним
/*
      _I2 - струм зворотньої послідовності при попередньому зафіксованому максимальному відношенні I2/I1
      _I1 - струм прямої послідовності при попередньому зафіксованому максимальному відношенні I2/I1
      
      I2 -  текучий струм зворотньої послідовності
      I1 -  текучий струм прямої послідовності
      
      Умова перезапису параметрів:
      _I2     I2
      ---  < --- /x на _I1*I1  
      _I1     I1
      
      (_I2*I1 < I2*_I1)  - це є умова, що зараз є більше ЗОП(КОФ) ніж попередній раз
*/
  unsigned int I2, I1, _I1, _I2;
   I2 = measurement[IM_I2];
   I1 = measurement[IM_I1];
  _I2 = measurements_ZOP_max_dr[3];
  _I1 = measurements_ZOP_max_dr[4];
  if (
      ( (_I1 >  0) && ( ((_I2*I1) < (I2*_I1)) || (I1 == 0) ) ) ||
      ( (_I1 == 0) && (I1 == 0) && ( _I2 < I2 ) ) 
     )
  {
    //Зафіксовано зріз при найвищому струмі зворотньої послідовності з моменту початку запису
    int frequency_val_1_int = (int)frequency_val_1;
    if (frequency_val_1_int >= 0) frequency_val_1_int = (int)(frequency_val_1*1000);
  
    int frequency_val_2_int = (int)frequency_val_2;
    if (frequency_val_2_int >= 0) frequency_val_2_int = (int)(frequency_val_2*1000);

    measurements_ZOP_max_dr[ 0] = measurement[IM_IA];
    measurements_ZOP_max_dr[ 1] = measurement[IM_IB];
    measurements_ZOP_max_dr[ 2] = measurement[IM_IC];
    measurements_ZOP_max_dr [3] = measurement[IM_I2];
    measurements_ZOP_max_dr[ 4] = measurement[IM_I1];
    measurements_ZOP_max_dr[ 5] = measurement[IM_UA1];
    measurements_ZOP_max_dr[ 6] = measurement[IM_UB1];
    measurements_ZOP_max_dr[ 7] = measurement[IM_UC1];
    measurements_ZOP_max_dr[ 8] = measurement[IM_UA2];
    measurements_ZOP_max_dr[ 9] = measurement[IM_UB2];
    measurements_ZOP_max_dr[10] = measurement[IM_UC2];
    measurements_ZOP_max_dr[11] = measurement[IM_UAB1];
    measurements_ZOP_max_dr[12] = measurement[IM_UBC1];
    measurements_ZOP_max_dr[13] = measurement[IM_UCA1];
    measurements_ZOP_max_dr[14] = measurement[IM_UAB2];
    measurements_ZOP_max_dr[15] = measurement[IM_UBC2];
    measurements_ZOP_max_dr[16] = measurement[IM_UCA2];
    measurements_ZOP_max_dr[17] = (unsigned int)frequency_val_1_int;
    measurements_ZOP_max_dr[18] = (unsigned int)frequency_val_2_int;

    //Фіксуємо час з моменту початку аварійного запису
    measurements_ZOP_max_dr[19] = time_tmp;
  }
}
/*****************************************************/

/*****************************************************/
//Завершення моніторингу максимального струму
/*
  type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE        - завершення моніторингу максимального фазного струму
  type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE              - завершення моніторингу мінімальної напруги
  type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE              - завершення моніторингу максимальної напруги
  type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP          - завершення моніторингу максимального струму зворотньої послідовності
*/
/*****************************************************/
inline void end_monitoring_min_max_measurement(unsigned int type_current, volatile unsigned int* carrent_active_functions)
{
  if(
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE   ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE         ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE         ) ||
     (type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP     )
    )
  {
    int step = number_max_phase_dr   + 
               number_min_U_dr       +
               number_max_U_dr       +
               number_max_ZOP_dr;
    
    //Перевірка на коректність роботи програмного забеспечення
    if(
       ( (number_max_phase_dr   > 0) || ( (number_max_phase_dr   == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  )) == 0) ) ) &&
       ( (number_min_U_dr       > 0) || ( (number_min_U_dr       == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE        )) == 0) ) ) &&
       ( (number_max_U_dr       > 0) || ( (number_max_U_dr       == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE        )) == 0) ) ) &&
       ( (number_max_ZOP_dr     > 0) || ( (number_max_ZOP_dr     == 0) && ((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP    )) == 0) ) ) &&
       ( (step > 0) && (step <= MAX_NUMBER_FIX_MAX_MEASUREMENTS)) 
      )
    {
      unsigned char *input_data_point, *output_data_point;
      
      //Визначаємо джерело звідки  будемо копіювати дані
      if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)
      {
        input_data_point = (unsigned char *)(measurements_phase_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)
      {
        input_data_point = (unsigned char *)(measurements_U_min_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)
      {
        input_data_point = (unsigned char *)(measurements_U_max_dr);
      }
      else if(type_current == IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)
      {
        input_data_point = (unsigned char *)(measurements_ZOP_max_dr);
      }
      
      //Визначаємо адресу киди дані треба копіювати
      step -= 1; //Тому що  нумерація починається з 0, а не з 1 (step гарантовано не менше 1(це перевірено вище), тому від'ємного числа не може бути)
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE  ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE        )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE        ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE        )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE        ))
        step -= 1;
      if(((state_current_monitoring & (1<< IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP    )) != 0) && (type_current != IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP    ))
        step -= 1;
      
      if(step >= 0)
      {
        output_data_point = (buffer_for_save_dr_record + FIRST_INDEX_FIRST_BLOCK_DR +step*sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS);
        
        for(unsigned int i = 0; i < (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS); i++)
        {
          if(i != (sizeof(unsigned int)*SIZE_ARRAY_FIX_MAX_MEASUREMENTS - 1))
          {
            *(output_data_point + i) = *(input_data_point + i);
          }
          else
          {
            //У останній байт масиву, що відповідає згідно з Little-endian формату старшову байту останнього елементу масиву, мітку типу струму, по якому фіксувався максимальний струм
            *(output_data_point + i) = type_current;
          }
        }

        //Знімаємо помітку, що ми на стадії моніторингу 
        state_current_monitoring &= ~(1<<type_current);
      }
      else
      {
        //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
        fix_undefined_error_dr(carrent_active_functions);
      }
    }
    else
    {
      //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
      fix_undefined_error_dr(carrent_active_functions);
    }
  }
  else
  {
    //Відбулася незрозуміла ситуація - сюди програма теоретично ніколи не мала б заходити
    fix_undefined_error_dr(carrent_active_functions);
  }
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
  
  //Цю перевірку виконуємо тільки у тому випадку, коли іде процес формування нового запису
  if(state_dr_record == STATE_DR_EXECUTING_RECORD)
  {
    //Перевіряємо чи не виникла умова, що зарараз буде перебір фіксації максимальних струмів
    unsigned int temp_value_for_max_min_fix_measurement = (
                                                            number_max_phase_dr   + 
                                                            number_min_U_dr       +
                                                            number_max_U_dr       +
                                                            number_max_ZOP_dr
                                                          );
    if(temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
    {
      //Сюди, теоретично програма нікол не мала б заходити, але якщо зайшла, тоиреба перервати роботу дискретного реєстратора
      fix_undefined_error_dr(carrent_active_functions);

      /*
      Скидаємо сигнал роботи дискретного реєстратора у масиві попередніх активних функцій
      Це ми робимо для того, щоб у першому записі нового запису було зафіксовано активацію роботу дискретного реєстратора
      */
      _CLEAR_BIT(previous_active_functions, RANG_WORK_D_REJESTRATOR);
    }
    else
    {
      //Перевіряємо чи стоїть умова почати моніторити максимальний фазний струм
      if(
         ((carrent_active_functions[0] & MASKA_MONITOTYNG_PHASE_SIGNALES_0) != 0) ||
         ((carrent_active_functions[1] & MASKA_MONITOTYNG_PHASE_SIGNALES_1) != 0) ||
         ((carrent_active_functions[2] & MASKA_MONITOTYNG_PHASE_SIGNALES_2) != 0) ||
         ((carrent_active_functions[3] & MASKA_MONITOTYNG_PHASE_SIGNALES_3) != 0) ||
         ((carrent_active_functions[4] & MASKA_MONITOTYNG_PHASE_SIGNALES_4) != 0) ||
         ((carrent_active_functions[5] & MASKA_MONITOTYNG_PHASE_SIGNALES_5) != 0) ||
         ((carrent_active_functions[6] & MASKA_MONITOTYNG_PHASE_SIGNALES_6) != 0)
        )
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) == 0)
        {
          //Є умова почати новий моніторинг максимального фазного струму
          temp_value_for_max_min_fix_measurement++;
        }
      }

      //Перевіряємо чи стоїть умова почати моніторити мінімальну напругу
      if (
          (blocking_continue_monitoring_min_U == 0) ||
          (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BV) != 0)  
         )
      {
        if(
           ((carrent_active_functions[0] & MASKA_MONITOTYNG_UMIN_SIGNALES_0) != 0) ||
           ((carrent_active_functions[1] & MASKA_MONITOTYNG_UMIN_SIGNALES_1) != 0) ||
           ((carrent_active_functions[2] & MASKA_MONITOTYNG_UMIN_SIGNALES_2) != 0) ||
           ((carrent_active_functions[3] & MASKA_MONITOTYNG_UMIN_SIGNALES_3) != 0) ||
           ((carrent_active_functions[4] & MASKA_MONITOTYNG_UMIN_SIGNALES_4) != 0) ||
           ((carrent_active_functions[5] & MASKA_MONITOTYNG_UMIN_SIGNALES_5) != 0) ||
           ((carrent_active_functions[6] & MASKA_MONITOTYNG_UMIN_SIGNALES_6) != 0)
          )
        {
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) == 0)
          {
            //Є умова почати новий моніторинг мінімальну фазну напругу
            temp_value_for_max_min_fix_measurement++;
          }
        }
      }
      
      //Перевіряємо чи стоїть умова почати моніторити максимальну напругу
      if(
         ((carrent_active_functions[0] & MASKA_MONITOTYNG_UMAX_SIGNALES_0) != 0) ||
         ((carrent_active_functions[1] & MASKA_MONITOTYNG_UMAX_SIGNALES_1) != 0) ||
         ((carrent_active_functions[2] & MASKA_MONITOTYNG_UMAX_SIGNALES_2) != 0) ||
         ((carrent_active_functions[3] & MASKA_MONITOTYNG_UMAX_SIGNALES_3) != 0) ||
         ((carrent_active_functions[4] & MASKA_MONITOTYNG_UMAX_SIGNALES_4) != 0) ||
         ((carrent_active_functions[5] & MASKA_MONITOTYNG_UMAX_SIGNALES_5) != 0) ||
         ((carrent_active_functions[6] & MASKA_MONITOTYNG_UMAX_SIGNALES_6) != 0)
      )
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) == 0)
        {
          //Є умова почати новий моніторинг максимальну фазну напругу
          temp_value_for_max_min_fix_measurement++;
        }
      }
      
      //Перевіряємо чи стоїть умова почати моніторити максимальний струм зворотньої послідовності
      if(
         ((carrent_active_functions[0] & MASKA_MONITOTYNG_ZOP_SIGNALES_0) != 0) ||
         ((carrent_active_functions[1] & MASKA_MONITOTYNG_ZOP_SIGNALES_1) != 0) ||
         ((carrent_active_functions[2] & MASKA_MONITOTYNG_ZOP_SIGNALES_2) != 0) ||
         ((carrent_active_functions[3] & MASKA_MONITOTYNG_ZOP_SIGNALES_3) != 0) ||
         ((carrent_active_functions[4] & MASKA_MONITOTYNG_ZOP_SIGNALES_4) != 0) ||
         ((carrent_active_functions[5] & MASKA_MONITOTYNG_ZOP_SIGNALES_5) != 0) ||
         ((carrent_active_functions[6] & MASKA_MONITOTYNG_ZOP_SIGNALES_6) != 0)
        )
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) == 0)
        {
          //Є умова почати новий моніторинг максимального струму зворотнньої послідовності
          temp_value_for_max_min_fix_measurement++;
        }
      }
    
      if(temp_value_for_max_min_fix_measurement > MAX_NUMBER_FIX_MAX_MEASUREMENTS)
      {
        //Виникла ситуація, що зарараз буде перебір фіксації максимальних вимірювань
        //Треба примусово завершити текучий запис і почати наступний запис

        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_ITEMS_DR      ] = number_items_dr;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR    ] = number_changes_into_dr_record        & 0xff;
        buffer_for_save_dr_record[FIRST_INDEX_NUMBER_CHANGES_DR + 1] = (number_changes_into_dr_record >> 8) & 0xff;

        //Перевіряємо чи треба завершити моніторинг максимального фазного струму
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг мінімальної напруги
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимальної напруги
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

        //Перевіряємо чи треба завершити моніторинг максимального струму звороної послідовності
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
        
        //Дальші дії виконуємо тіьлки у тому випадку, якщо функція end_monitoring_min_max_measurement не зафіксувала помилку і не скинула state_dr_record у STATE_DR_NO_RECORD
        if(state_dr_record != STATE_DR_NO_RECORD)
        {
          //Записуємо кількість зафіксованих максимальних струмів всіх типів
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_PHASE_DR  ] = number_max_phase_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MIN_U_DR      ] = number_min_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_U_DR      ] = number_max_U_dr;
          buffer_for_save_dr_record[FIRST_INDEX_NUMBER_MAX_ZOP_DR    ] = number_max_ZOP_dr;

          //Помічаємо, що треба при першій же нагоді почати новий запис, бо попередній запис був примусово зупинений
          state_dr_record = STATE_DR_CUT_RECORD;
        
          //Скидаємо сигнал роботи дискретного реєстратора
          _CLEAR_BIT(carrent_active_functions, RANG_WORK_D_REJESTRATOR);
        
          //Сформований запис ставим в чергу для запису
          routine_for_queue_dr();
        }
        /*
        Скидаємо сигнал роботи дискретного реєстратора у масиві попередніх активних функцій
        Це ми робимо для того, щоб у першому записі нового запису було зафіксовано активацію роботу дискретного реєстратора
        */
        _CLEAR_BIT(previous_active_functions, RANG_WORK_D_REJESTRATOR);
        
      }
    }
  }
  
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
          
          //Знімаємо повідомлення про моніторинг максимальних струмів
          state_current_monitoring = 0;
          
          //Перевіряємо чи стоїть умова моніторити максимальний фазний струм
          if(
             ((carrent_active_functions[0] & MASKA_MONITOTYNG_PHASE_SIGNALES_0) != 0) ||
             ((carrent_active_functions[1] & MASKA_MONITOTYNG_PHASE_SIGNALES_1) != 0) ||
             ((carrent_active_functions[2] & MASKA_MONITOTYNG_PHASE_SIGNALES_2) != 0) ||
             ((carrent_active_functions[3] & MASKA_MONITOTYNG_PHASE_SIGNALES_3) != 0) ||
             ((carrent_active_functions[4] & MASKA_MONITOTYNG_PHASE_SIGNALES_4) != 0) ||
             ((carrent_active_functions[5] & MASKA_MONITOTYNG_PHASE_SIGNALES_5) != 0) ||
             ((carrent_active_functions[6] & MASKA_MONITOTYNG_PHASE_SIGNALES_6) != 0)
            )
          {
            start_monitoring_max_phase_current(time_from_start_record_dr);
          }

          //Знімаємо блокування моніторингу мінімальної напруги
          blocking_continue_monitoring_min_U = 0;
          //Перевіряємо чи стоїть умова моніторити мінімальну напругу
          if(
             ((carrent_active_functions[0] & MASKA_MONITOTYNG_UMIN_SIGNALES_0) != 0) ||
             ((carrent_active_functions[1] & MASKA_MONITOTYNG_UMIN_SIGNALES_1) != 0) ||
             ((carrent_active_functions[2] & MASKA_MONITOTYNG_UMIN_SIGNALES_2) != 0) ||
             ((carrent_active_functions[3] & MASKA_MONITOTYNG_UMIN_SIGNALES_3) != 0) ||
             ((carrent_active_functions[4] & MASKA_MONITOTYNG_UMIN_SIGNALES_4) != 0) ||
             ((carrent_active_functions[5] & MASKA_MONITOTYNG_UMIN_SIGNALES_5) != 0) ||
             ((carrent_active_functions[6] & MASKA_MONITOTYNG_UMIN_SIGNALES_6) != 0)
            )
          {
            start_monitoring_min_U(time_from_start_record_dr);
          }
          //Перевіряємо, чи стоїть умова припинити моніторинг мінімальної напруги після вимкнення вимикача
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
          {
            if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BO) != 0)
            {
              blocking_continue_monitoring_min_U = 0xff;
              end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
            }
          }

          //Перевіряємо чи стоїть умова моніторити максимальну напругу
          if(
             ((carrent_active_functions[0] & MASKA_MONITOTYNG_UMAX_SIGNALES_0) != 0) ||
             ((carrent_active_functions[1] & MASKA_MONITOTYNG_UMAX_SIGNALES_1) != 0) ||
             ((carrent_active_functions[2] & MASKA_MONITOTYNG_UMAX_SIGNALES_2) != 0) ||
             ((carrent_active_functions[3] & MASKA_MONITOTYNG_UMAX_SIGNALES_3) != 0) ||
             ((carrent_active_functions[4] & MASKA_MONITOTYNG_UMAX_SIGNALES_4) != 0) ||
             ((carrent_active_functions[5] & MASKA_MONITOTYNG_UMAX_SIGNALES_5) != 0) ||
             ((carrent_active_functions[6] & MASKA_MONITOTYNG_UMAX_SIGNALES_6) != 0)
            )
          {
            start_monitoring_max_U(time_from_start_record_dr);
          }

          //Перевіряємо чи стоїть умова моніторити максимальний струм зворотньої послідовності
          if(
             ((carrent_active_functions[0] & MASKA_MONITOTYNG_ZOP_SIGNALES_0) != 0) ||
             ((carrent_active_functions[1] & MASKA_MONITOTYNG_ZOP_SIGNALES_1) != 0) ||
             ((carrent_active_functions[2] & MASKA_MONITOTYNG_ZOP_SIGNALES_2) != 0) ||
             ((carrent_active_functions[3] & MASKA_MONITOTYNG_ZOP_SIGNALES_3) != 0) ||
             ((carrent_active_functions[4] & MASKA_MONITOTYNG_ZOP_SIGNALES_4) != 0) ||
             ((carrent_active_functions[5] & MASKA_MONITOTYNG_ZOP_SIGNALES_5) != 0) ||
             ((carrent_active_functions[6] & MASKA_MONITOTYNG_ZOP_SIGNALES_6) != 0)
            )
          {
            start_monitoring_max_ZOP(time_from_start_record_dr);
          }
      
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

      //Контроль-фіксація максимальних аналогових сигналів
      
      //Перевіряємо чи стоїть умова моніторити максимальний фазний струм
      if(
         ((carrent_active_functions[0] & MASKA_MONITOTYNG_PHASE_SIGNALES_0) != 0) ||
         ((carrent_active_functions[1] & MASKA_MONITOTYNG_PHASE_SIGNALES_1) != 0) ||
         ((carrent_active_functions[2] & MASKA_MONITOTYNG_PHASE_SIGNALES_2) != 0) ||
         ((carrent_active_functions[3] & MASKA_MONITOTYNG_PHASE_SIGNALES_3) != 0) ||
         ((carrent_active_functions[4] & MASKA_MONITOTYNG_PHASE_SIGNALES_4) != 0) ||
         ((carrent_active_functions[5] & MASKA_MONITOTYNG_PHASE_SIGNALES_5) != 0) ||
         ((carrent_active_functions[6] & MASKA_MONITOTYNG_PHASE_SIGNALES_6) != 0)
        )
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
          continue_monitoring_max_phase_current(time_from_start_record_dr);
        else
          start_monitoring_max_phase_current(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);
      }

      //Перевіряємо чи стоїть умова відновити моніторинг мінімальної напруги
      if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BV) != 0)
      {
        //Знімаємо блокування моніторингу мінімальної напруги
        blocking_continue_monitoring_min_U = 0;
      }
      //Перевіряємо чи стоїть умова моніторити мінімальну напругу
      if (blocking_continue_monitoring_min_U == 0)
      {
        if(
           ((carrent_active_functions[0] & MASKA_MONITOTYNG_UMIN_SIGNALES_0) != 0) ||
           ((carrent_active_functions[1] & MASKA_MONITOTYNG_UMIN_SIGNALES_1) != 0) ||
           ((carrent_active_functions[2] & MASKA_MONITOTYNG_UMIN_SIGNALES_2) != 0) ||
           ((carrent_active_functions[3] & MASKA_MONITOTYNG_UMIN_SIGNALES_3) != 0) ||
           ((carrent_active_functions[4] & MASKA_MONITOTYNG_UMIN_SIGNALES_4) != 0) ||
           ((carrent_active_functions[5] & MASKA_MONITOTYNG_UMIN_SIGNALES_5) != 0) ||
           ((carrent_active_functions[6] & MASKA_MONITOTYNG_UMIN_SIGNALES_6) != 0)
          )
        {
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
            continue_monitoring_min_U(time_from_start_record_dr);
          else
            start_monitoring_min_U(time_from_start_record_dr);
        }
        else
        {
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
        }
      }
      //Перевіряємо, чи стоїть умова припинити моніторинг мінімальної напруги після вимкнення вимикача
      if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
      {
        if (_CHECK_SET_BIT(carrent_active_functions, RANG_WORK_BO) != 0)
        {
          blocking_continue_monitoring_min_U = 0xff;
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);
        }
      }
      
      //Перевіряємо чи стоїть умова моніторити максимальну напругу
      if(
         ((carrent_active_functions[0] & MASKA_MONITOTYNG_UMAX_SIGNALES_0) != 0) ||
         ((carrent_active_functions[1] & MASKA_MONITOTYNG_UMAX_SIGNALES_1) != 0) ||
         ((carrent_active_functions[2] & MASKA_MONITOTYNG_UMAX_SIGNALES_2) != 0) ||
         ((carrent_active_functions[3] & MASKA_MONITOTYNG_UMAX_SIGNALES_3) != 0) ||
         ((carrent_active_functions[4] & MASKA_MONITOTYNG_UMAX_SIGNALES_4) != 0) ||
         ((carrent_active_functions[5] & MASKA_MONITOTYNG_UMAX_SIGNALES_5) != 0) ||
         ((carrent_active_functions[6] & MASKA_MONITOTYNG_UMAX_SIGNALES_6) != 0)
        )
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
          continue_monitoring_max_U(time_from_start_record_dr);
        else
          start_monitoring_max_U(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);
      }
      
      //Перевіряємо чи стоїть умова моніторити максимальний струм зворотньої послідовності
      if(
         ((carrent_active_functions[0] & MASKA_MONITOTYNG_ZOP_SIGNALES_0) != 0) ||
         ((carrent_active_functions[1] & MASKA_MONITOTYNG_ZOP_SIGNALES_1) != 0) ||
         ((carrent_active_functions[2] & MASKA_MONITOTYNG_ZOP_SIGNALES_2) != 0) ||
         ((carrent_active_functions[3] & MASKA_MONITOTYNG_ZOP_SIGNALES_3) != 0) ||
         ((carrent_active_functions[4] & MASKA_MONITOTYNG_ZOP_SIGNALES_4) != 0) ||
         ((carrent_active_functions[5] & MASKA_MONITOTYNG_ZOP_SIGNALES_5) != 0) ||
         ((carrent_active_functions[6] & MASKA_MONITOTYNG_ZOP_SIGNALES_6) != 0)
        )
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
          continue_monitoring_max_ZOP(time_from_start_record_dr);
        else
          start_monitoring_max_ZOP(time_from_start_record_dr);
      }
      else
      {
        if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
          end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
      }
      
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

          //Перевіряємо чи треба завершити моніторинг максимального фазного струму
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_PHASE, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг мінімальну напругу
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MIN_VOLTAGE, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимальну напругу
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_VOLTAGE, carrent_active_functions);

          //Перевіряємо чи треба завершити моніторинг максимального струму послідовності
          if((state_current_monitoring & (1<<IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP)) != 0)
            end_monitoring_min_max_measurement(IDENTIFIER_BIT_ARRAY_MAX_CURRENT_ZOP, carrent_active_functions);
        
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
      information_about_clean_energy     &= (unsigned int)(~(1 << USB_RECUEST));
    }
    if ((reset_trigger_function_from_interface & (1 << RS485_RECUEST)) != 0)
    {
      for (unsigned int i = 0; i < N_BIG; i++) trigger_functions_RS485[i] = 0;
      
      information_about_restart_counter  &= (unsigned int)(~(1 << RS485_RECUEST));
      information_about_clean_energy     &= (unsigned int)(~(1 << RS485_RECUEST));
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
  
  /**************************/
  //Вибір групи уставок
  /**************************/
  if (count_number_set_bit(&active_inputs_grupa_ustavok, NUMBER_GROUP_USTAVOK) > 1)
    _SET_BIT(set_diagnostyka, ERROR_SELECT_GRUPY_USRAVOK);
  else
    _SET_BIT(clear_diagnostyka, ERROR_SELECT_GRUPY_USRAVOK);
   
  if (
      ((active_functions[0] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_0) != 0) ||
      ((active_functions[1] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_1) != 0) ||
      ((active_functions[2] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_2) != 0) ||
      ((active_functions[3] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_3) != 0) ||
      ((active_functions[4] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_4) != 0) ||
      ((active_functions[5] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_5) != 0) ||
      ((active_functions[6] & MASKA_SIGNALES_FOR_LOCK_GROUP_PICKUP_6) != 0)
    )
  {
    //Іде блокування груп уставок - група уставок залишається тою, яка вибрана попередньо
    _SET_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);

    unsigned int number_group_stp_tmp = 0;
    if(( _CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    if(( _CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK) ) !=0 ) number_group_stp_tmp++;
    
    if (number_group_stp_tmp != 1)
    {
      //Теоретично цього ніколи не мало б бути
      total_error_sw_fixed(52);
    }
  }
  else
  {
    //Можна вибирати групу уставок - захисти цю операцію не блокують
    _CLEAR_BIT(active_functions, RANG_BLK_GRUP_USTAVOK_VID_ZACHYSTIV);
    setpoints_selecting(active_functions, active_inputs_grupa_ustavok);
  }

  unsigned int number_group_stp;
  if     (( _CHECK_SET_BIT(active_functions, RANG_1_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 0;
  else if(( _CHECK_SET_BIT(active_functions, RANG_2_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 1;
  else if(( _CHECK_SET_BIT(active_functions, RANG_3_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 2;
  else if(( _CHECK_SET_BIT(active_functions, RANG_4_GRUPA_USTAVOK) ) !=0 ) number_group_stp = 3;
  else
  {
    //Теоретично цього ніколи не мало б бути
    total_error_sw_fixed(51);
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
    //Контроль привода ВВ
    /**************************/
    control_VV(active_functions);
    /**************************/

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
    //МТЗ
    /**************************/
    if ((current_settings_prt.configuration & (1 << MTZ_BIT_CONFIGURATION)) != 0)
    {
      mtz_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_mtz_signals[N_BIG] = 
      {
        MASKA_MTZ_SIGNALS_0,
        MASKA_MTZ_SIGNALS_1,
        MASKA_MTZ_SIGNALS_2, 
        MASKA_MTZ_SIGNALS_3, 
        MASKA_MTZ_SIGNALS_4, 
        MASKA_MTZ_SIGNALS_5,
        MASKA_MTZ_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_mtz_signals[i]);
      
      global_timers[INDEX_TIMER_MTZ1] = -1;
      global_timers[INDEX_TIMER_MTZ1_N_VPERED] = -1;
      global_timers[INDEX_TIMER_MTZ1_N_NAZAD] = -1;
      global_timers[INDEX_TIMER_MTZ1_PO_NAPRUZI] = -1;
      global_timers[INDEX_TIMER_MTZ2] = -1;
      global_timers[INDEX_TIMER_MTZ2_DEPENDENT] = -1;
      global_timers[INDEX_TIMER_MTZ2_PR] = -1;
      global_timers[INDEX_TIMER_MTZ2_N_VPERED] = -1;
      global_timers[INDEX_TIMER_MTZ2_N_VPERED_PR] = -1;
      global_timers[INDEX_TIMER_MTZ2_N_NAZAD] = -1;
      global_timers[INDEX_TIMER_MTZ2_N_NAZAD_PR] = -1;
      global_timers[INDEX_TIMER_MTZ2_PO_NAPRUZI] = -1;
      global_timers[INDEX_TIMER_MTZ2_PO_NAPRUZI_PR] = -1;
      global_timers[INDEX_TIMER_MTZ2_VVID_PR] = -1;
      global_timers[INDEX_TIMER_MTZ3] = -1;
      global_timers[INDEX_TIMER_MTZ3_N_VPERED] = -1;
      global_timers[INDEX_TIMER_MTZ3_N_NAZAD] = -1;
      global_timers[INDEX_TIMER_MTZ3_PO_NAPRUZI] = -1;
      global_timers[INDEX_TIMER_MTZ4] = -1;
      global_timers[INDEX_TIMER_MTZ4_N_VPERED] = -1;
      global_timers[INDEX_TIMER_MTZ4_N_NAZAD] = -1;
      global_timers[INDEX_TIMER_MTZ4_PO_NAPRUZI] = -1;
    }
    /**************************/
    
    /**************************/
    //ЗДЗ
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZDZ_BIT_CONFIGURATION)) != 0)
    {
      zdz_handler(active_functions);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_zdz_signals[N_BIG] = 
      {
        MASKA_ZDZ_SIGNALS_0, 
        MASKA_ZDZ_SIGNALS_1, 
        MASKA_ZDZ_SIGNALS_2,
        MASKA_ZDZ_SIGNALS_3, 
        MASKA_ZDZ_SIGNALS_4, 
        MASKA_ZDZ_SIGNALS_5, 
        MASKA_ZDZ_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_zdz_signals[i]);
    }
    /**************************/

    /**************************/
    //ЗОП(КОФ)
    /**************************/
    if ((current_settings_prt.configuration & (1 << ZOP_BIT_CONFIGURATION)) != 0)
    {
      zop_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_zop_signals[N_BIG] = 
      {
        MASKA_ZOP_SIGNALS_0, 
        MASKA_ZOP_SIGNALS_1, 
        MASKA_ZOP_SIGNALS_2,
        MASKA_ZOP_SIGNALS_3, 
        MASKA_ZOP_SIGNALS_4, 
        MASKA_ZOP_SIGNALS_5, 
        MASKA_ZOP_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_zop_signals[i]);
      global_timers[INDEX_TIMER_ZOP] = -1;
    }
    /**************************/
    
    if ((current_settings_prt.configuration & (1 << UMIN_BIT_CONFIGURATION)) != 0) 
    {
      /**************************/
      //ЗНМИН1
      /**************************/
      umin1_handler(active_functions, number_group_stp);
      /**************************/
      
      /**************************/
      //ЗНМИН2
      /**************************/
      umin2_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_umin_signals[N_BIG] = 
      {
        MASKA_UMIN_SIGNALS_0, 
        MASKA_UMIN_SIGNALS_1, 
        MASKA_UMIN_SIGNALS_2,
        MASKA_UMIN_SIGNALS_3, 
        MASKA_UMIN_SIGNALS_4, 
        MASKA_UMIN_SIGNALS_5, 
        MASKA_UMIN_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_umin_signals[i]);

      global_timers[INDEX_TIMER_UMIN1] = -1;
      global_timers[INDEX_TIMER_UMIN2] = -1;
    }
    
    if ((current_settings_prt.configuration & (1 << UMAX_BIT_CONFIGURATION)) != 0) 
    {
      /**************************/
      //ЗНМАКС1
      /**************************/
      umax1_handler(active_functions, number_group_stp);
      /**************************/
      
      /**************************/
      //ЗНМАКС2
      /**************************/
      umax2_handler(active_functions, number_group_stp);
      /**************************/
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_umax_signals[N_BIG] = 
      {
        MASKA_UMAX_SIGNALS_0, 
        MASKA_UMAX_SIGNALS_1, 
        MASKA_UMAX_SIGNALS_2,
        MASKA_UMAX_SIGNALS_3, 
        MASKA_UMAX_SIGNALS_4, 
        MASKA_UMAX_SIGNALS_5, 
        MASKA_UMAX_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_umax_signals[i]);

      global_timers[INDEX_TIMER_UMAX1] = -1;
      global_timers[INDEX_TIMER_UMAX2] = -1;
    }
    /**************************/

    /**************************/
    //АВР
    /**************************/
    if ((current_settings_prt.configuration & (1 << AVR_BIT_CONFIGURATION)) != 0) 
    {
      avr_handler(active_functions, number_group_stp);
    } 
    else 
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_avr_signals[N_BIG] = 
      {
        MASKA_AVR_SIGNALS_0, 
        MASKA_AVR_SIGNALS_1, 
        MASKA_AVR_SIGNALS_2,
        MASKA_AVR_SIGNALS_3, 
        MASKA_AVR_SIGNALS_4, 
        MASKA_AVR_SIGNALS_5, 
        MASKA_AVR_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_avr_signals[i]);
      
      global_timers[INDEX_TIMER_AVR_BLK_K1] = -1;
      global_timers[INDEX_TIMER_AVR_PUSK_K1] = -1;
      global_timers[INDEX_TIMER_AVR_D_DIJI_K1] = -1;
      global_timers[INDEX_TIMER_AVR_VVIMK_REZ_K1] = -1;
      global_timers[INDEX_TIMER_AVR_VVIMK_K1] = -1;
      global_timers[INDEX_TIMER_AVR_VYMK_ROB_K1_UMIN] = -1;
      global_timers[INDEX_TIMER_AVR_VYMK_K1] = -1;
      global_timers[INDEX_TIMER_AVR_TMP_1MS_K1] = -1;
      global_timers[INDEX_TIMER_AVR_BLK_K2] = -1;
      global_timers[INDEX_TIMER_AVR_PUSK_K2] = -1;
      global_timers[INDEX_TIMER_AVR_D_DIJI_K2] = -1;
      global_timers[INDEX_TIMER_AVR_VVIMK_REZ_K2] = -1;
      global_timers[INDEX_TIMER_AVR_VVIMK_K2] = -1;
      global_timers[INDEX_TIMER_AVR_VYMK_ROB_K2_UMIN] = -1;
      global_timers[INDEX_TIMER_AVR_VYMK_K2] = -1;
      global_timers[INDEX_TIMER_AVR_TMP_1MS_K2] = -1;

      previous_states_AVR_0 = 0;
      trigger_AVR_0 = 0;
    }
    /**************************/
    
    /**************************/
    //УРОВ
    /**************************/
    if ((current_settings_prt.configuration & (1 << UROV_BIT_CONFIGURATION)) != 0)
    {
      urov_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_urov_signals[N_BIG] = 
      {
        MASKA_UROV_SIGNALS_0, 
        MASKA_UROV_SIGNALS_1, 
        MASKA_UROV_SIGNALS_2,
        MASKA_UROV_SIGNALS_3, 
        MASKA_UROV_SIGNALS_4, 
        MASKA_UROV_SIGNALS_5, 
        MASKA_UROV_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_urov_signals[i]);

      global_timers[INDEX_TIMER_UROV1] = -1;
      global_timers[INDEX_TIMER_UROV2] = -1;
    }
    /**************************/

    /**************************/
    //АПВ
    /**************************/
    if ((current_settings_prt.configuration & (1 << APV_BIT_CONFIGURATION)) != 0)
    {
      apv_handler(active_functions, number_group_stp);
    }
    else
    {
      //Очищуємо сигнали, які не можуть бути у даній конфігурації
      const unsigned int maska_apv_signals[N_BIG] = 
      {
        MASKA_APV_SIGNALS_0, 
        MASKA_APV_SIGNALS_1, 
        MASKA_APV_SIGNALS_2,
        MASKA_APV_SIGNALS_3, 
        MASKA_APV_SIGNALS_4, 
        MASKA_APV_SIGNALS_5, 
        MASKA_APV_SIGNALS_6
      };
      for (unsigned int i = 0; i < N_BIG; i++) active_functions[i] &= (unsigned int)(~maska_apv_signals[i]);

      global_timers[INDEX_TIMER_APV_1] = -1;
      global_timers[INDEX_TIMER_APV_2] = -1;
      global_timers[INDEX_TIMER_APV_3] = -1;
      global_timers[INDEX_TIMER_APV_4] = -1;
      global_timers[INDEX_TIMER_APV_BLOCK_VID_APV1] = -1;
      global_timers[INDEX_TIMER_APV_BLOCK_VID_APV2] = -1;
      global_timers[INDEX_TIMER_APV_BLOCK_VID_APV3] = -1;
      global_timers[INDEX_TIMER_APV_BLOCK_VID_APV4] = -1;
      global_timers[INDEX_TIMER_APV_BLOCK_VID_VV] = -1;
      global_timers[INDEX_TIMER_APV_TMP1] = -1;

      previous_states_APV_0 = 0;
      trigger_APV_0 = 0;
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

    /**************************/
    //Включення-Відключення
    /**************************/
    on_off_handler(active_functions);
    /**************************/
    
    /**************************/
    //Готовность к ТУ
    /**************************/
    ready_tu(active_functions);
    /**************************/

    /**************************/
    //Ресурс вимикача (завжди пісяля логіки Блоку вимикання)
    /**************************/
    resurs_vymykacha_handler(active_functions);
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

    if (koef_resurs_changed == CHANGED_ETAP_ENDED)
    {
      //Коефіцієнти для підрахунку ресурсу вимикача
      K_resurs_prt = K_resurs;

      //Помічаємо, що зміни прийняті системою захистів
      koef_resurs_changed = CHANGED_ETAP_NONE;
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

    //Лічильник ресурсу
    if (periodical_tasks_TEST_RESURS != 0)
    {
      //Стоїть у черзі активна задача зроботи резервні копії даних
      if ((state_i2c_task & STATE_RESURS_EEPROM_GOOD) != 0)
      {
        //Робимо копію тільки тоді, коли інформаціz успішно зчитана і сформована контрольна сума
        if (
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_WRITE_RESURS_EEPROM_BIT) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_WRITING_RESURS_EEPROM_BIT    ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_START_READ_RESURS_EEPROM_BIT ) == 0) &&
            (_CHECK_SET_BIT(control_i2c_taskes, TASK_READING_RESURS_EEPROM_BIT    ) == 0)
           ) 
        {
          //На даний моммент не іде читання-запис ресурсу вимикача, тому можна здійснити копіювання
          resurs_vymykacha_ctrl = resurs_vymykacha;          
          resurs_vidkljuchennja_ctrl = resurs_vidkljuchennja;
          crc_resurs_ctrl = crc_resurs;

          //Скидаємо активну задачу формування резервної копії 
          periodical_tasks_TEST_RESURS = false;
          //Виставляємо активну задачу контролю достовірності по резервній копії 
          periodical_tasks_TEST_RESURS_LOCK = true;
        }
      }
      else
      {
        //Скидаємо активну задачу формування резервної копії 
        periodical_tasks_TEST_RESURS = false;
      }
    }

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

/*****************************************************/
//Вибір групи уставок
/*****************************************************/
void setpoints_selecting(volatile unsigned int *p_active_functions, unsigned int act_inp_gr_ustavok) {
  unsigned int grupa_ustavok = 0;
  act_inp_gr_ustavok &= 0xf;
  if (current_settings_prt.grupa_ustavok < SETPOINT_GRUPA_USTAVOK_MIN ||
      current_settings_prt.grupa_ustavok > SETPOINT_GRUPA_USTAVOK_MAX) {
    //Неопределенная ошибка
    while (1);
  } else {
    grupa_ustavok = 1 << (current_settings_prt.grupa_ustavok - 1);
  }
  
  _OR4_INVERTOR(act_inp_gr_ustavok, 0, act_inp_gr_ustavok, 1, act_inp_gr_ustavok, 2, act_inp_gr_ustavok, 3, act_inp_gr_ustavok, 4);
  if (_GET_OUTPUT_STATE(act_inp_gr_ustavok, 4)) 
    _SET_BIT(p_active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_INVERS_DV_GRUPA_USTAVOK);
  
  _AND2(grupa_ustavok, 0, act_inp_gr_ustavok, 4, grupa_ustavok, 4);
  _AND2(grupa_ustavok, 1, act_inp_gr_ustavok, 4, grupa_ustavok, 5);
  _AND2(grupa_ustavok, 2, act_inp_gr_ustavok, 4, grupa_ustavok, 6);
  _AND2(grupa_ustavok, 3, act_inp_gr_ustavok, 4, grupa_ustavok, 7);
  
  unsigned int tmp = gr_ustavok_tmp;
  do {
    _AND4(act_inp_gr_ustavok, 0, gr_ustavok_tmp, 1, gr_ustavok_tmp, 2, gr_ustavok_tmp, 3, act_inp_gr_ustavok, 5);
    _INVERTOR(act_inp_gr_ustavok, 5, tmp, 0);
    
    _AND4(tmp, 0, act_inp_gr_ustavok, 1, tmp, 2, tmp, 3, act_inp_gr_ustavok, 6);
    _INVERTOR(act_inp_gr_ustavok, 6, tmp, 1);
    
    _AND4(tmp, 0, tmp, 1, act_inp_gr_ustavok, 2, tmp, 3, act_inp_gr_ustavok, 7);
    _INVERTOR(act_inp_gr_ustavok, 7, tmp, 2);
    
    _AND4(tmp, 0, tmp, 1, tmp, 2, act_inp_gr_ustavok, 3, act_inp_gr_ustavok, 8);
    _INVERTOR(act_inp_gr_ustavok, 8, tmp, 3);
    if (tmp == gr_ustavok_tmp) {
      break;
    }
    gr_ustavok_tmp = tmp;
  } while (1);
  
  _OR2(grupa_ustavok, 4, act_inp_gr_ustavok, 5, grupa_ustavok, 8);
  _OR2(grupa_ustavok, 5, act_inp_gr_ustavok, 6, grupa_ustavok, 9);
  _OR2(grupa_ustavok, 6, act_inp_gr_ustavok, 7, grupa_ustavok, 10);
  _OR2(grupa_ustavok, 7, act_inp_gr_ustavok, 8, grupa_ustavok, 11);
  
  if (_GET_OUTPUT_STATE(grupa_ustavok, 8)) 
    _SET_BIT(p_active_functions, RANG_1_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_1_GRUPA_USTAVOK);

  if (_GET_OUTPUT_STATE(grupa_ustavok, 9))
    _SET_BIT(p_active_functions, RANG_2_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_2_GRUPA_USTAVOK);

  if (_GET_OUTPUT_STATE(grupa_ustavok, 10))
      _SET_BIT(p_active_functions, RANG_3_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_3_GRUPA_USTAVOK);

  if (_GET_OUTPUT_STATE(grupa_ustavok, 11))
      _SET_BIT(p_active_functions, RANG_4_GRUPA_USTAVOK);
  else
    _CLEAR_BIT(p_active_functions, RANG_4_GRUPA_USTAVOK);
}
/*****************************************************/

/*****************************************************/
