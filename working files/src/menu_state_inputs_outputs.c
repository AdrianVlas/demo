#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків станів входів-виходів
/*****************************************************/
void make_ekran_list_inputs_outputs(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_INPUTS_OUTPUTS][MAX_COL_LCD] = 
  {
    {
      " Сост.входов    ",
      " Сост.выходов   "
    },
    {
      " Стан входів    ",
      " Стан виходів   "
    },
    {
      " Inputs  state  ",
      " Outputs state  "
    },
    {
      " Кіріс жаfдайы  ",
      " Шыfыс жаfдайы  "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_LIST_INPUTS_OUTPUTS)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran][j];
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/

/*******************************************************/
//Формуємо екрвн станів входів або виходів
/*
--------------------------------------------------------
input_output
    0 - входи
    1 - виходи
--------------------------------------------------------
*/
/*******************************************************/
void make_ekran_state_inputs_or_outputs(unsigned int input_output)
{
#define MAX_COL_LCD_PART1 7

  const unsigned char title_input_output[MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD_PART1] = 
  {
    {
      " Двх   ",
      " Двых  "
    },
    {
      " Двх   ",
      " Двих  "
    },
    {
      " DI    ",
      " DO    "
    },
    {
      " Дк    ",
      " Дшыf  "
    }
  };
  
  const unsigned int index_of_number_di_do[MAX_NAMBER_LANGUAGE][2] = 
  {
    {4, 5},
    {4, 5},
    {3, 3},
    {3, 5}
  };
    
  const unsigned char information_active[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] = 
  {
    " Активный",
    " Активний",
    "   Active",
    "  Активті"
  };
  const unsigned char information_pasive[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1] = 
  {
    "Паcсивный",
    " Пасивний",
    "  Passive",
    " Пассивті"
  };
  const unsigned char information_close[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1]  = 
  {
    "  Замкнут",
    "   Замкн.",
    "   Closed",
    "  Замкнут"
  };
  const unsigned char information_open[MAX_NAMBER_LANGUAGE][MAX_COL_LCD - MAX_COL_LCD_PART1]   = 
  {
    "Разомкнут",
    " Разімкн.",
    "   Opened",
    "Разомкнут"
  };
  
  int index_language = index_language_in_array(current_settings.language);
  unsigned int index_of_number_di_do_tmp = index_of_number_di_do[index_language][input_output];
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int max_row, state;
  
  //Визначаємо скільки рядків має бути у вікні
  if (input_output == 0)
  {
    max_row = NUMBER_INPUTS;
    state = active_inputs;
  }
  else
  {
    max_row = NUMBER_OUTPUTS;
    state = state_outputs_raw;
  }
  
  /*******************************************************/
  //Формуємо назви і значення. Копіюємо  рядки у робочий екран
  /*******************************************************/
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int number = index_of_ekran + 1;
    unsigned int tmp_1 = (number / 10), tmp_2 = number - tmp_1*10;
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < max_row)
      for (unsigned int j = 0; j<MAX_COL_LCD; j++)
      {
        if (j < MAX_COL_LCD_PART1)
        {
          if ((j < index_of_number_di_do_tmp) || (j > (index_of_number_di_do_tmp + 1)))
            working_ekran[i][j] = title_input_output[index_language][input_output][j];
          else if (j == index_of_number_di_do_tmp)
          {
            if (tmp_1 > 0 ) working_ekran[i][j] = tmp_1 + 0x30;
          }
          else     
          {
            if (tmp_1 > 0 )
            {
              working_ekran[i][j] = tmp_2 + 0x30;
            }
            else
            {
              working_ekran[i][j - 1] = tmp_2 + 0x30;
              working_ekran[i][j] = ' ';
            }
          }
        }
        else
        {
          if (input_output == 0)
          {
            if ((state & (1<<index_of_ekran)) == 0)
              working_ekran[i][j] = information_pasive[index_language][j - MAX_COL_LCD_PART1];
            else
              working_ekran[i][j] = information_active[index_language][j - MAX_COL_LCD_PART1];
          }
          else
          {
            if ((state & (1<<index_of_ekran)) == 0)
              working_ekran[i][j] = information_open[index_language][j - MAX_COL_LCD_PART1];
            else
              working_ekran[i][j] = information_close[index_language][j - MAX_COL_LCD_PART1];
          }
        }
      }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }  

  //Курсор по горизонталі відображається на першій позиції
  current_ekran.position_cursor_x = 0;
  //Відображення курору по вертикалі
  current_ekran.position_cursor_y = position_temp & (MAX_ROW_LCD - 1);
  //Курсор видимий
  current_ekran.cursor_on = 1;
  //Курсор не мигає
  current_ekran.cursor_blinking_on = 0;

  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
  
#undef MAX_COL_LCD_PART1  
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
