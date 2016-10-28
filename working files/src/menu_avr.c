#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок АВР Umin
/*****************************************************/
void make_ekran_setpoint_avr_Umin(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_AVR_UMIN][MAX_COL_LCD] = 
  {
    {
      "  U1min < U1x   ",
      "  U2min > U2x   ",
      "  U2min < U2x   ",
      "  U1min > U1x   "
    },
    {
      "  U1min < U1x   ",
      "  U2min > U2x   ",
      "  U2min < U2x   ",
      "  U1min > U1x   "
    },
    {
      "  U1min < U1x   ",
      "  U2min > U2x   ",
      "  U2min < U2x   ",
      "  U1min > U1x   "
    },
    {
      "  U1min < U1x   ",
      "  U2min > U2x   ",
      "  U2min < U2x   ",
      "  U1min > U1x   "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_SETPOINT_AVR_UMIN<<1))//Множення на два константи MAX_ROW_FOR_SETPOINT_AVR_UMIN потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U1_UMIN)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn1_U1_Umin[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn1_U1_Umin[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U2_LOW_WORK)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn1_U2_low_work[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn1_U2_low_work[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U2_UMIN)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn2_U2_Umin[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn2_U2_Umin[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U1_LOW_WORK)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn2_U1_low_work[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn2_U1_low_work[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U1_UMIN)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN1_U1_UMIN_BEGIN) ||  (j > COL_SETPOINT_AVR_TN1_U1_UMIN_END ))  &&
                (j != (COL_SETPOINT_AVR_TN1_U1_UMIN_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN1_U1_UMIN_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN1_U1_UMIN_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN1_U1_UMIN_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U2_LOW_WORK)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN1_U2_LOW_WORK_BEGIN) ||  (j > COL_SETPOINT_AVR_TN1_U2_LOW_WORK_END ))  &&
                (j != (COL_SETPOINT_AVR_TN1_U2_LOW_WORK_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN1_U2_LOW_WORK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN1_U2_LOW_WORK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN1_U2_LOW_WORK_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U2_UMIN)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN2_U2_UMIN_BEGIN) ||  (j > COL_SETPOINT_AVR_TN2_U2_UMIN_END )) &&
                (j != (COL_SETPOINT_AVR_TN2_U2_UMIN_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN2_U2_UMIN_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN2_U2_UMIN_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN2_U2_UMIN_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U1_LOW_WORK)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN2_U1_LOW_WORK_BEGIN) ||  (j > COL_SETPOINT_AVR_TN2_U1_LOW_WORK_END ))  &&
                (j != (COL_SETPOINT_AVR_TN2_U1_LOW_WORK_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN2_U1_LOW_WORK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN2_U1_LOW_WORK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN2_U1_LOW_WORK_COMMA, 0);
          }
        }
      }
        
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_STPAVR_TN1_U1_UMIN)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN1_U1_UMIN_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN1_U1_UMIN_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPAVR_TN1_U2_LOW_WORK)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN1_U2_LOW_WORK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN1_U2_LOW_WORK_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPAVR_TN2_U2_UMIN) 
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN2_U2_UMIN_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN2_U2_UMIN_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPAVR_TN2_U1_LOW_WORK)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN2_U1_LOW_WORK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN2_U1_LOW_WORK_END;
    }
    
    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;
    
    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
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
//Формуємо екран відображення уставок АВР Umin
/*****************************************************/
void make_ekran_setpoint_avr_Umax(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_AVR_UMAX][MAX_COL_LCD] = 
  {
    {
      "  U1max > U1x   ",
      "  U2max < U2x   ",
      "  U2max > U2x   ",
      "  U1max < U1x   "
    },
    {
      "  U1max > U1x   ",
      "  U2max < U2x   ",
      "  U2max > U2x   ",
      "  U1max < U1x   "
    },
    {
      "  U1max > U1x   ",
      "  U2max < U2x   ",
      "  U2max > U2x   ",
      "  U1max < U1x   "
    },
    {
      "  U1max > U1x   ",
      "  U2max < U2x   ",
      "  U2max > U2x   ",
      "  U1max < U1x   "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_SETPOINT_AVR_UMAX<<1))//Множення на два константи MAX_ROW_FOR_SETPOINT_AVR_UMAX потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U1_UMAX)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn1_U1_Umax[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn1_U1_Umax[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U2_HIGH_WORK)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn1_U2_high_work[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn1_U2_high_work[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U2_UMAX)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn2_U2_Umax[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn2_U2_Umax[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U1_HIGH_WORK)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки
          if (current_ekran.edition == 0) value = current_settings.setpoint_avr_tn2_U1_high_work[group]; //у змінну value поміщаємо значення уставки
          else value = edition_settings.setpoint_avr_tn2_U1_high_work[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U1_UMAX)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN1_U1_UMAX_BEGIN) ||  (j > COL_SETPOINT_AVR_TN1_U1_UMAX_END ))  &&
                (j != (COL_SETPOINT_AVR_TN1_U1_UMAX_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN1_U1_UMAX_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN1_U1_UMAX_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN1_U1_UMAX_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN1_U2_HIGH_WORK)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_BEGIN) ||  (j > COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_END ))  &&
                (j != (COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U2_UMAX)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN2_U2_UMAX_BEGIN) ||  (j > COL_SETPOINT_AVR_TN2_U2_UMAX_END ))  &&
                (j != (COL_SETPOINT_AVR_TN2_U2_UMAX_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN2_U2_UMAX_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN2_U2_UMAX_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN2_U2_UMAX_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPAVR_TN2_U1_HIGH_WORK)
          {
            if (
                ((j < COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_BEGIN) ||  (j > COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_END ))  &&
                (j != (COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_COMMA, 0);
          }
        }
      }
        
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_STPAVR_TN1_U1_UMAX)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN1_U1_UMAX_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN1_U1_UMAX_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPAVR_TN1_U2_HIGH_WORK)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN1_U2_HIGH_WORK_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPAVR_TN2_U2_UMAX)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN2_U2_UMAX_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN2_U2_UMAX_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPAVR_TN2_U1_HIGH_WORK)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_AVR_TN2_U1_HIGH_WORK_END;
    }
    
    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;
    
    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
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
//Формуємо екран відображення витримок АВР (загальні)
/*****************************************************/
void make_ekran_timeout_avr_general(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_AVR_GENERAL][MAX_COL_LCD] = 
  {
    {
      " Блок.АВР кан.1 ",
      "   Пуск кан.1   ",
      "Разр.деств.кан.1",
      " Вкл.рез.кан.1  ",
      "   Вкл.кан.1    ",
      "   Откл.кан.1   ",
      " Блок.АВР кан.2 ",
      "   Пуск кан.2   ",
      "Разр.деств.кан.2",
      " Вкл.рез.кан.2  ",
      "   Вкл.кан.2    ",
      "   Откл.кан.2   "
    },
    {
      " Блок.АВР кан.1 ",
      "   Пуск кан.1   ",
      " Дозв.дії.кан.1 ",
      " Ввімк.рез.кан.1",
      "  Ввімк.кан.1   ",
      "   Вимк.кан.1   ",
      " Блок.АВР кан.2 ",
      "   Пуск кан.2   ",
      " Дозв.дії.кан.2 ",
      " Ввімк.рез.кан.2",
      "  Ввімк.кан.2   ",
      "   Вимк.кан.2   "
    },
    {
      " Блок.АВР кан.1 ",
      "   Пуск кан.1   ",
      "Разр.деств.кан.1",
      " Вкл.рез.кан.1  ",
      "   Вкл.кан.1    ",
      "   Откл.кан.1   ",
      " Блок.АВР кан.2 ",
      "   Пуск кан.2   ",
      "Разр.деств.кан.2",
      " Вкл.рез.кан.2  ",
      "   Вкл.кан.2    ",
      "   Откл.кан.2   "
    },
    {
      " Блок.АВР кан.1 ",
      "   Пуск кан.1   ",
      "Разр.деств.кан.1",
      " Вкл.рез.кан.1  ",
      "   Вкл.кан.1    ",
      "   Откл.кан.1   ",
      " Блок.АВР кан.2 ",
      "   Пуск кан.2   ",
      "Разр.деств.кан.2",
      " Вкл.рез.кан.2  ",
      "   Вкл.кан.2    ",
      "   Откл.кан.2   "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
 
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_TIMEOUT_AVR_GENERAL<<1))//Множення на два константи MAX_ROW_FOR_TIMEOUT_AVR_GENERAL потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_BLK_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_blk_k1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_blk_k1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_PUSK_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_pusk_k1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_pusk_k1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_D_DIJI_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_d_diji_k1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_d_diji_k1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_REZ_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vvimk_rez_k1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vvimk_rez_k1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vvimk_k1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vvimk_k1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VYMK_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vymk_k1[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vymk_k1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_BLK_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_blk_k2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_blk_k2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_PUSK_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_pusk_k2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_pusk_k2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_D_DIJI_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_d_diji_k2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_d_diji_k2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_REZ_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vvimk_rez_k2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vvimk_rez_k2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vvimk_k2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vvimk_k2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VYMK_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vymk_k2[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vymk_k2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_BLK_K1)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_BLK_K1_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_BLK_K1_END )) &&
                (j != (COL_TMO_AVR_GENERAL_BLK_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_BLK_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_BLK_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_BLK_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_PUSK_K1)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_PUSK_K1_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_PUSK_K1_END )) &&
                (j != (COL_TMO_AVR_GENERAL_PUSK_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_PUSK_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_PUSK_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_PUSK_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_D_DIJI_K1)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_D_DIJI_K1_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_D_DIJI_K1_END )) &&
                (j != (COL_TMO_AVR_GENERAL_D_DIJI_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_D_DIJI_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_D_DIJI_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_D_DIJI_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_REZ_K1)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_END )) &&
                (j != (COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_K1)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_VVIMK_K1_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_VVIMK_K1_END )) &&
                (j != (COL_TMO_AVR_GENERAL_VVIMK_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_VVIMK_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_VVIMK_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_VVIMK_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VYMK_K1)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_VYMK_K1_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_VYMK_K1_END )) &&
                (j != (COL_TMO_AVR_GENERAL_VYMK_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_VYMK_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_VYMK_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_VYMK_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_BLK_K2)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_BLK_K2_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_BLK_K2_END )) &&
                (j != (COL_TMO_AVR_GENERAL_BLK_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_BLK_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_BLK_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_BLK_K2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_PUSK_K2)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_PUSK_K2_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_PUSK_K2_END )) &&
                (j != (COL_TMO_AVR_GENERAL_PUSK_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_PUSK_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_PUSK_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_PUSK_K2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_D_DIJI_K2)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_D_DIJI_K2_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_D_DIJI_K2_END )) &&
                (j != (COL_TMO_AVR_GENERAL_D_DIJI_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_D_DIJI_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_D_DIJI_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_D_DIJI_K2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_REZ_K2)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_END )) &&
                (j != (COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VVIMK_K2)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_VVIMK_K2_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_VVIMK_K2_END )) &&
                (j != (COL_TMO_AVR_GENERAL_VVIMK_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_VVIMK_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_VVIMK_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_VVIMK_K2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_GENERAL_VYMK_K2)
          {
            if (
                ((j < COL_TMO_AVR_GENERAL_VYMK_K2_BEGIN) ||  (j > COL_TMO_AVR_GENERAL_VYMK_K2_END )) &&
                (j != (COL_TMO_AVR_GENERAL_VYMK_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_GENERAL_VYMK_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_GENERAL_VYMK_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_GENERAL_VYMK_K2_COMMA, 0);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_BLK_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_BLK_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_BLK_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_PUSK_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_PUSK_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_PUSK_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_D_DIJI_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_D_DIJI_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_D_DIJI_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_VVIMK_REZ_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_REZ_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_VVIMK_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_VYMK_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_VYMK_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_VYMK_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_BLK_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_BLK_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_BLK_K2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_PUSK_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_PUSK_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_PUSK_K2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_D_DIJI_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_D_DIJI_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_D_DIJI_K2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_VVIMK_REZ_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_REZ_K2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_VVIMK_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_VVIMK_K2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_GENERAL_VYMK_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_GENERAL_VYMK_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_GENERAL_VYMK_K2_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
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
//Формуємо екран відображення витримок АВР Umin
/*****************************************************/
void make_ekran_timeout_avr_Umin(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_AVR_UMIN][MAX_COL_LCD] = 
  {
    {
      " Откл.р.к.1 Umin",
      " Откл.р.к.2 Umin"
    },
    {
      " Вимк.р.к.1 Umin",
      " Вимк.р.к.2 Umin"
    },
    {
      " Откл.р.к.1 Umin",
      " Откл.р.к.2 Umin"
    },
    {
      " Откл.р.к.1 Umin",
      " Откл.р.к.2 Umin"
    }
  };
  int index_language = index_language_in_array(current_settings.language);
 
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_TIMEOUT_AVR_UMIN<<1))//Множення на два константи MAX_ROW_FOR_TIMEOUT_AVR_UMIN потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMIN_VYMK_ROB_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vymk_rob_k1_Umin[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vymk_rob_k1_Umin[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMIN_VYMK_ROB_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vymk_rob_k2_Umin[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vymk_rob_k2_Umin[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMIN_VYMK_ROB_K1)
          {
            if (
                ((j < COL_TMO_AVR_UMIN_VYMK_ROB_K1_BEGIN) ||  (j > COL_TMO_AVR_UMIN_VYMK_ROB_K1_END )) &&
                (j != (COL_TMO_AVR_UMIN_VYMK_ROB_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_UMIN_VYMK_ROB_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_UMIN_VYMK_ROB_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_UMIN_VYMK_ROB_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMIN_VYMK_ROB_K2)
          {
            if (
                ((j < COL_TMO_AVR_UMIN_VYMK_ROB_K2_BEGIN) ||  (j > COL_TMO_AVR_UMIN_VYMK_ROB_K2_END )) &&
                (j != (COL_TMO_AVR_UMIN_VYMK_ROB_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_UMIN_VYMK_ROB_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_UMIN_VYMK_ROB_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_UMIN_VYMK_ROB_K2_COMMA, 0);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_TMOAVR_UMIN_VYMK_ROB_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_UMIN_VYMK_ROB_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_UMIN_VYMK_ROB_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_UMIN_VYMK_ROB_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_UMIN_VYMK_ROB_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_UMIN_VYMK_ROB_K2_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
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
//Формуємо екран відображення витримок АВР Umax
/*****************************************************/
void make_ekran_timeout_avr_Umax(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_AVR_UMAX][MAX_COL_LCD] = 
  {
    {
      " Откл.р.к.1 Umax",
      " Откл.р.к.2 Umax"
    },
    {
      " Вимк.р.к.1 Umax",
      " Вимк.р.к.2 Umax"
    },
    {
      " Откл.р.к.1 Umax",
      " Откл.р.к.2 Umax"
    },
    {
      " Откл.р.к.1 Umax",
      " Откл.р.к.2 Umax"
    }
  };
  int index_language = index_language_in_array(current_settings.language);
 
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  unsigned int vaga, value, first_symbol;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_TIMEOUT_AVR_UMAX<<1))//Множення на два константи MAX_ROW_FOR_TIMEOUT_AVR_UMAX потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMAX_VYMK_ROB_K1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vymk_rob_k1_Umax[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vymk_rob_k1_Umax[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMAX_VYMK_ROB_K2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_avr_vymk_rob_k2_Umax[group]; //у змінну value поміщаємо значення витримки
          else value = edition_settings.timeout_avr_vymk_rob_k2_Umax[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMAX_VYMK_ROB_K1)
          {
            if (
                ((j < COL_TMO_AVR_UMAX_VYMK_ROB_K1_BEGIN) ||  (j > COL_TMO_AVR_UMAX_VYMK_ROB_K1_END )) &&
                (j != (COL_TMO_AVR_UMAX_VYMK_ROB_K1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_UMAX_VYMK_ROB_K1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_UMAX_VYMK_ROB_K1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_UMAX_VYMK_ROB_K1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAVR_UMAX_VYMK_ROB_K2)
          {
            if (
                ((j < COL_TMO_AVR_UMAX_VYMK_ROB_K2_BEGIN) ||  (j > COL_TMO_AVR_UMAX_VYMK_ROB_K2_END )) &&
                (j != (COL_TMO_AVR_UMAX_VYMK_ROB_K2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_AVR_UMAX_VYMK_ROB_K2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_AVR_UMAX_VYMK_ROB_K2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_AVR_UMAX_VYMK_ROB_K2_COMMA, 0);
          }
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = MAX_COL_LCD;
    if (current_ekran.index_position == INDEX_ML_TMOAVR_UMAX_VYMK_ROB_K1) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_UMAX_VYMK_ROB_K1_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_UMAX_VYMK_ROB_K1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAVR_UMAX_VYMK_ROB_K2) 
    {
      current_ekran.position_cursor_x = COL_TMO_AVR_UMAX_VYMK_ROB_K2_BEGIN;
      last_position_cursor_x = COL_TMO_AVR_UMAX_VYMK_ROB_K2_END;
    }

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;

    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
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
//Формуємо екран відображення значення управлінської інформації для АВР
/*****************************************************/
void make_ekran_control_avr()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_AVR][MAX_COL_LCD] = 
  {
    {
      "      АВР       ",
      "  АВР от Umax   ",
      "     Бл.АВР     "
    },
    {
      "      АВР       ",
      "  АВР від Umax  ",
      "     Бл.АВР     "
    },
    {
      "      АВР       ",
      "  АВР от Umax   ",
      "     Бл.АВР     "
    },
    {
      "      АВР       ",
      "  АВР от Umax   ",
      "     Бл.АВР     "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_CONTROL_AVR<<1))//Множення на два константи MAX_ROW_FOR_CONTROL_AVR потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
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

        unsigned int temp_data;
        if(current_ekran.edition == 0) temp_data = current_settings.control_avr;
        else temp_data = edition_settings.control_avr;
          
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
