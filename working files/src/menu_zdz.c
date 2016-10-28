#include "header.h"

/*****************************************************/
//Формуємо екран відображення значення управлінської інформації для ЗДЗ
/*****************************************************/
void make_ekran_control_zdz()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_ZDZ][MAX_COL_LCD] = 
  {
    {
      "      ЗДЗ       ",
      "  Пуск от МТЗ1  ",
      "  Пуск от МТЗ2  ",
      "  Пуск от МТЗ3  ",
      "  Пуск от МТЗ4  "
    },
    {
      "      ЗДЗ       ",
      " Пуск від МСЗ1  ",
      " Пуск від МСЗ2  ",
      " Пуск від МСЗ3  ",
      " Пуск від МСЗ4  "
    },
    {
      "      ЗДЗ       ",
      " Start from OCP1",
      " Start from OCP2",
      " Start from OCP3",
      " Start from OCP4"
    },
    {
      "      ЗДЗ       ",
      "  Пуск от МТЗ1  ",
      "  Пуск от МТЗ2  ",
      "  Пуск от МТЗ3  ",
      "  Пуск от МТЗ4  "
    }
  };
  unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_ZDZ][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_ZDZ; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  unsigned int temp_data;
  if(current_ekran.edition == 0) temp_data = current_settings.control_zdz;
  else temp_data = edition_settings.control_zdz;
        
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current_mtz = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  for (int current_index = 0; current_index < (MAX_ROW_FOR_CONTROL_ZDZ - additional_current_mtz); current_index++ )
  {

    if (
        (
         (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ1) ||
         (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ2) ||
         (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ3) ||
         (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ4)
        )   
        &&
        ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
       )   
    {
      int i = current_index - additional_current_mtz;
      unsigned int maska_1, maska_2;
      maska_1 = (1 << i) - 1;
      maska_2 = (unsigned int)(~maska_1);
    
      if ((i+1) <= position_temp) position_temp--;
      do
      {
        for(unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((i+1) < MAX_ROW_FOR_CONTROL_ZDZ) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
          else name_string_tmp[i][j] = ' ';
        }
        i++;
      }
      while (i< (MAX_ROW_FOR_CONTROL_ZDZ - additional_current_mtz));
    
      unsigned int temp_data_1 = (temp_data >> 1) & maska_2;
      temp_data = (temp_data & maska_1) | temp_data_1;

      if (
          (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ1) ||
          (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ2) ||
          (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ3) ||
          (current_index == INDEX_ML_CTRZDZ_STARTED_FROM_MTZ4)
         )   
        additional_current_mtz++;
    }
  }
  /******************************************/
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_CONTROL_ZDZ<<1))//Множення на два константи MAX_ROW_FOR_CONTROL_ZDZ потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran>>1][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        const unsigned char information[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {"     Откл.      ", "      Вкл.      "},
          {"     Вимк.      ", "     Ввімк.     "},
          {"      Off       ", "       On       "},
          {"     Сљнд.      ", "     Косу.      "}
        };
        const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
        {
         {4, 5},
         {4, 4},
         {5, 6},
         {4, 4}
        };
        
        unsigned int index_ctr = (index_of_ekran>>1);

        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_language][(temp_data >> index_ctr) & 0x1][j];
        current_ekran.position_cursor_x = cursor_x[index_language][(temp_data >> index_ctr) & 0x1];
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  if(current_ekran.edition == 0)current_ekran.cursor_blinking_on = 0;
  else current_ekran.cursor_blinking_on = 1;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
