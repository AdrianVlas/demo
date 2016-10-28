#ifndef __CONST_MESUREMENT__
#define __CONST_MESUREMENT__

#define USTUVANNJA_VAGA 12
#define DEFAULT_USTUVANNJA_VALUE ((1<<USTUVANNJA_VAGA) - 1)
#define AMPLITUDA_SECTOR_VAGA 7
#define AMPLITUDA_SECTOR      (1 << AMPLITUDA_SECTOR_VAGA)
#define AMPLITUDA_FI_VAGA     10
#define AMPLITUDA_FI          (1 << AMPLITUDA_FI_VAGA)

#define SECTOR1_MTZ_TZNP   7
#define POPRAVKA_MTZ_TZNP  0/*3*/
#define SECTOR2_MTZ_TZNP   2

#define MEASUREMENT_TIM_FREQUENCY       60000000 //МГц

#define MAIN_FREQUENCY  50      //Гц
#define MIN_FREQUENCY   45      //Гц
#define MAX_FREQUENCY   55      //Гц

#define MIN_TICK_PERIOD (MEASUREMENT_TIM_FREQUENCY/MAX_FREQUENCY - 1)
#define MAX_TICK_PERIOD (MEASUREMENT_TIM_FREQUENCY/MIN_FREQUENCY + 1)

#define NUMBER_ADCs                     2
#define NUMBER_CANALs_ADC               16
#define NUMBER_INPUTs_ADCs              (NUMBER_ADCs*NUMBER_CANALs_ADC)

#define VAGA_NUMBER_POINT                5
#define NUMBER_POINT                     (1 << VAGA_NUMBER_POINT)

#define NUMBER_PERIOD_TRANSMIT           10

/*
Коефіцієнт, який переводить число з АЦП у мА
          K*2500
Koef = ------------
        R*1*0xFFF
де:
K - коефіцієнт трансформації і він рівний 500 для 3I0, або 2500 для фазних струмів
2500 - максимальне значення напруги у мілівольтах, яке може оцифрувати АЦП (у нас це є 2,5 В = 2500 мВ)
R - резистор, який перетворює трансформований струм у напругу. Зараз стоїть 12,7 Ом
1 - після перетворення струму в напругу як мінімум раз сигнвал проходить через підсилювач на операційному підсилювачі з коефіцієнтом підсилення "1"
0xFFF - максимальне число, яке може видати АЦП

Koef = 120,17728553161624027765760049225 для фазних струмів (7691/64 = 120,171875) 

Множення на ці коефіцієнти переводять одиниці АЦП у мА

Але коли ми проводимо розрахунки з допомогою перетворення Фур'є, то результатом є амплітуда 1-ої  агмоніки (або інших гармонік. якщо розрахунок би йшов по них)
Тому крім цього коефіцієнта треба ще поділити на корінб з двох.
для простоти зробимо узагальнений коефіцієнт, що є результатом Koef_1 = Koef/sqrt(2)

Koef_1 = 84,978173543997808495193432804655 для фазних струмів (5439/64 = 84,984375)
*/

#define MNOGNYK_I                     7691           
#define VAGA_DILENNJA_I               6
#define MNOGNYK_I_DIJUCHE             5439//6451          
#define VAGA_DILENNJA_I_DIJUCHE       6
#define MNOGNYK_I_DIJUCHE_DOUBLE       84.984375


/*
Коефіцієнт, який переводить число з АЦП у мB
          Ku*2500*R1
Koef = --------------
        R2*1*0xFFF
де:
Ku - коефіцієнт трансформації і він рівний 1
2500 - максимальне значення напруги у мілівольтах, яке може оцифрувати АЦП (у нас це є 2,5 В = 2500 мВ)
R1 -  резистор, який визначає струм у первинній обмотці трансвформатора струму. Зараз стоїть 75 кОм
R2 - резистор, який перетворює трансформований струм у напругу. Зараз стоїть 499 Ом
1 - після перетворення струму в напругу як мінімум раз сигнвал проходить через підсилювач на операційному підсилювачі з коефіцієнтом підсилення "1"
0xFFF - максимальне число, яке може видати АЦП

Koef = 91,7586087926769289494740396544 (5872/64 = 91,75)

Множення на ці коефіцієнти переводять одиниці АЦП у мВ

Але коли ми проводимо розрахунки з допомогою перетворення Фур'є, то результатом є амплітуда 1-ої  агмоніки (або інших гармонік. якщо розрахунок би йшов по них)
Тому крім цього коефіцієнта треба ще поділити на корінь з двох.
для простоти зробимо узагальнений коефіцієнт, що є результатом Koef_1 = Koef/sqrt(2)

Koef_1 = 64,883134509545420915167731259667 (4152/64 = 64,875)
*/
#define MNOGNYK_U                     5896//5872           
#define VAGA_DILENNJA_U               6
#define MNOGNYK_U_DIJUCHE             4185//4153           
#define VAGA_DILENNJA_U_DIJUCHE       6
#define MNOGNYK_U_DIJUCHE_DOUBLE       65.390625


#define MAX_INDEX_DATA_FOR_OSCYLOGRAPH  10

#define GND_NORMAL_VALUE                0x0
#define VREF_NORMAL_VALUE               0x800
#define VDD_NORMAL_VALUE                0xb00

//Канали
#define C_GND_ADC1_1            0
#define C_Uc2_1                 1
#define C_Uc2_16                2
#define C_Ia_1                  3
#define C_Ia_16                 4
#define C_Ib_1                  5
#define C_Ib_16                 6
#define C_Ic_1                  7
#define C_Ic_16                 8
#define C_Ic_256                9
#define C_GND_ADC1_2            10
#define C_GND_ADC1_3            11
#define C_GND_ADC1_4            12
#define C_GND_ADC1_5            13
#define C_VREF_ADC1             14
#define C_VDD_ADC1              15
//#define C_GND_ADC2_1            (0  + NUMBER_CANALs_ADC)
//#define C_Ub2_1                 (1  + NUMBER_CANALs_ADC)
//#define C_Ub2_16                (2  + NUMBER_CANALs_ADC)
//#define C_Uc1_1                 (3  + NUMBER_CANALs_ADC)
//#define C_Uc1_16                (4  + NUMBER_CANALs_ADC)
//#define C_Ub1_1                 (5  + NUMBER_CANALs_ADC)
//#define C_Ub1_16                (6  + NUMBER_CANALs_ADC)
//#define C_Ua1_1                 (7  + NUMBER_CANALs_ADC)
//#define C_Ua1_16                (8  + NUMBER_CANALs_ADC)
//#define C_Ub2_256               (9  + NUMBER_CANALs_ADC)
//#define C_Ua2_16                (10 + NUMBER_CANALs_ADC)
//#define C_Ua2_1                 (11 + NUMBER_CANALs_ADC)
//#define C_GND_ADC2_2            (12 + NUMBER_CANALs_ADC)
//#define C_GND_ADC2_3            (13 + NUMBER_CANALs_ADC)
//#define C_VREF_ADC2             (14 + NUMBER_CANALs_ADC)
//#define C_VDD_ADC2              (15 + NUMBER_CANALs_ADC)
#define C_GND_ADC2_1            (0  + NUMBER_CANALs_ADC)
#define C_Ua1_1                 (1  + NUMBER_CANALs_ADC)
#define C_Ua1_16                (2  + NUMBER_CANALs_ADC)
#define C_Ub1_1                 (3  + NUMBER_CANALs_ADC)
#define C_Ub1_16                (4  + NUMBER_CANALs_ADC)
#define C_Uc1_1                 (5  + NUMBER_CANALs_ADC)
#define C_Uc1_16                (6  + NUMBER_CANALs_ADC)
#define C_Ua2_1                 (7  + NUMBER_CANALs_ADC)
#define C_Ua2_16                (8  + NUMBER_CANALs_ADC)
#define C_Ub2_1                 (9  + NUMBER_CANALs_ADC)
#define C_Ub2_16                (10 + NUMBER_CANALs_ADC)
#define C_Ub2_256               (11 + NUMBER_CANALs_ADC)
#define C_GND_ADC2_2            (12 + NUMBER_CANALs_ADC)
#define C_GND_ADC2_3            (13 + NUMBER_CANALs_ADC)
#define C_VREF_ADC2             (14 + NUMBER_CANALs_ADC)
#define C_VDD_ADC2              (15 + NUMBER_CANALs_ADC)

#define READ_Ua1 (                     \
                  (1 << C_Ua1_1  ) |   \
                  (1 << C_Ua1_16 )     \
                 )

#define READ_Ub1 (                     \
                  (1 << C_Ub1_1  ) |   \
                  (1 << C_Ub1_16 )     \
                 )

#define READ_Uc1 (                     \
                  (1 << C_Uc1_1  ) |   \
                  (1 << C_Uc1_16 )     \
                 )

#define READ_Ua2 (                     \
                  (1 << C_Ua2_1  ) |   \
                  (1 << C_Ua2_16 )     \
                 )

#define READ_Ub2 (                     \
                  (1 << C_Ub2_1  ) |   \
                  (1 << C_Ub2_16 )     \
                 )

#define READ_Uc2 (                     \
                  (1 << C_Uc2_1  ) |   \
                  (1 << C_Uc2_16 )     \
                 )

#define READ_I   (                     \
                  (1 << C_Ia_1   ) |   \
                  (1 << C_Ia_16  ) |   \
                  (1 << C_Ib_1   ) |   \
                  (1 << C_Ib_16  ) |   \
                  (1 << C_Ic_1   ) |   \
                  (1 << C_Ic_16  )     \
                 )

#define READ_DATA_VAL_1 (           \
                         READ_I   | \
                         READ_Ua1 | \
                         READ_Ub1 | \
                         READ_Uc1   \
                       )

#define READ_DATA_VAL_2 (           \
                         READ_Ua2 | \
                         READ_Ub2 | \
                         READ_Uc2   \
                       )

#define READ_TEST_VAL  (                                  \
                        (1 << C_GND_ADC1_1)             | \
                        (1 << C_GND_ADC1_2)             | \
                        (1 << C_GND_ADC1_3)             | \
                        (1 << C_GND_ADC1_4)             | \
                        (1 << C_GND_ADC1_5)             | \
                        (1 << C_VREF_ADC1 )             | \
                        (1 << C_VDD_ADC1  )             | \
                        (1 << C_GND_ADC2_1)             | \
                        (1 << C_GND_ADC2_2)             | \
                        (1 << C_GND_ADC2_3)             | \
                        (1 << C_VREF_ADC2 )             | \
          (unsigned int)(1 << C_VDD_ADC2  )               \
                       )

#define DATA_VAL_1_READ_BIT       0
#define DATA_VAL_1_READ           (1 << DATA_VAL_1_READ_BIT)
#define DATA_VAL_2_READ_BIT       1
#define DATA_VAL_2_READ           (1 << DATA_VAL_2_READ_BIT)
#define TEST_VAL_READ_BIT         2
#define TEST_VAL_READ             (1 << TEST_VAL_READ_BIT)

#define NUMBER_GND_ADC1         5
#define NUMBER_GND_ADC2         3

#define N_VAL_1                 0
#define N_VAL_2                 1

#define I_Ia          0
#define I_Ib          1
#define I_Ic          2
#define I_Ua1         3
#define I_Ub1         4
#define I_Uc1         5
#define I_Ua2         6
#define I_Ub2         7
#define I_Uc2         8

#define IM_IA          0
#define IM_IB          1
#define IM_IC          2
#define IM_I2          3
#define IM_I1          4
#define IM_UA1         5
#define IM_UB1         6
#define IM_UC1         7
#define IM_UA2         8
#define IM_UB2         9
#define IM_UC2         10
#define IM_UAB1        11
#define IM_UBC1        12
#define IM_UCA1        13
#define IM_UAB2        14
#define IM_UBC2        15
#define IM_UCA2        16

#define PORIG_FOR_FAPCH 10000

/*****************************************/
//Константи для фазової корекції між ТН1 і ТН2
/*****************************************/
enum _PhK
{
INDEX_PhK_UA1 = 0,
INDEX_PhK_UB1,
INDEX_PhK_UC1,
INDEX_PhK_UA2,
INDEX_PhK_UB2,
INDEX_PhK_UC2,

MAX_INDEX_PhK
};
/*****************************************/

enum _full_ort_index
{
FULL_ORT_Ua1 = 0,
FULL_ORT_Ub1,
FULL_ORT_Uc1,
FULL_ORT_Ua2,
FULL_ORT_Ub2,
FULL_ORT_Uc2,
FULL_ORT_Uab1,
FULL_ORT_Ubc1,
FULL_ORT_Uca1,
FULL_ORT_Uab2,
FULL_ORT_Ubc2,
FULL_ORT_Uca2,
FULL_ORT_Ia,
FULL_ORT_Ib,
FULL_ORT_Ic,

FULL_ORT_MAX
};

#define DIV_kWh         3600000000.0
#define PERIOD_SAVE_ENERGY_IN_MINUTES           30 /*хв.*/
enum _index_energy
{
  INDEX_EA_PLUS = 0,
  INDEX_EA_MINUS,
  INDEX_ER_1,
  INDEX_ER_2,
  INDEX_ER_3,
  INDEX_ER_4,
  
  MAX_NUMBER_INDEXES_ENERGY
};

#define UNDEF_PHI                              (1 << 31)
#define UNDEF_SPEED_PHI                        (1 << 31)

enum _const_sequence_TN
{
  CONST_SEQ_UNDEF = 0,
  CONST_SEQ_OK,
  CONST_SEQ_FAIL,
  CONST_SEQ_UNREAL
};

#endif
