#include "header.h"

/*****************************************************/
//Формуємо екран відображення уставок Umin
/*****************************************************/
void make_ekran_setpoint_Umin(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_SETPOINT_UMIN][MAX_COL_LCD] = 
  {
    {
      " Уставка ЗНмин1 ",
      "Блок.ЗНмин1 по I",
      " Уставка ЗНмин2 ",
      "Блок.ЗНмин2 по I"
    },
    {
      " Уставка ЗНмін1 ",
      "Блок.ЗНмін1 по I",
      " Уставка ЗНмін2 ",
      "Блок.ЗНмін2 по I"
    },
    {
      " Уставка Umin1  ",
      " Блок.Umin1 по I",
      " Уставка Umin2  ",
      " Блок.Umin2 по I"
    },
    {
      " Уставка ЗНмин1 ",
      "Блок.ЗНмин1 по I",
      " Уставка ЗНмин2 ",
      "Блок.ЗНмин2 по I"
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
    if (index_of_ekran < (MAX_ROW_FOR_SETPOINT_UMIN<<1))//Множення на два константи MAX_ROW_FOR_SETPOINT_UMIN потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_STPUMIN1)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки Umin1
          if (current_ekran.edition == 0) value = current_settings.setpoint_Umin1[group]; //у змінну value поміщаємо значення уставки Umin1
          else value = edition_settings.setpoint_Umin1[group];
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPUMIN1_IBLK)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки блокування Umin1 по струму
          if (current_ekran.edition == 0) value = current_settings.setpoint_Umin1_Iblk[group]; //у змінну value поміщаємо значення уставки блокування Umin1 по струму
          else value = edition_settings.setpoint_Umin1_Iblk[group];
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPUMIN2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки Umin2
          if (current_ekran.edition == 0) value = current_settings.setpoint_Umin2[group]; //у змінну value поміщаємо значення уставки Umin2
          else value = edition_settings.setpoint_Umin2[group];
        }
        else if ((index_of_ekran>>1) == INDEX_ML_STPUMIN2_IBLK)
        {
          vaga = 1000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для уставки блокування Umin2 по струму
          if (current_ekran.edition == 0) value = current_settings.setpoint_Umin2_Iblk[group]; //у змінну value поміщаємо значення уставки блокування Umin2 по струму
          else value = edition_settings.setpoint_Umin2_Iblk[group];
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_STPUMIN1)
          {
            if (
                ((j < COL_SETPOINT_UMIN1_BEGIN) ||  (j > COL_SETPOINT_UMIN1_END ))  &&
                (j != (COL_SETPOINT_UMIN1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_UMIN1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_UMIN1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_UMIN1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPUMIN1_IBLK)
          {
            if (
                ((j < COL_SETPOINT_UMIN1_IBLK_BEGIN) ||  (j > COL_SETPOINT_UMIN1_IBLK_END ))  &&
                (j != (COL_SETPOINT_UMIN1_IBLK_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_UMIN1_IBLK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_UMIN1_IBLK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_UMIN1_IBLK_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPUMIN2)
          {
            if (
                ((j < COL_SETPOINT_UMIN2_BEGIN) ||  (j > COL_SETPOINT_UMIN2_END ))  &&
                (j != (COL_SETPOINT_UMIN2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_UMIN2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_UMIN2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_V];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_UMIN2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_STPUMIN2_IBLK)
          {
            if (
                ((j < COL_SETPOINT_UMIN2_IBLK_BEGIN) ||  (j > COL_SETPOINT_UMIN2_IBLK_END ))  &&
                (j != (COL_SETPOINT_UMIN2_IBLK_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_SETPOINT_UMIN2_IBLK_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_SETPOINT_UMIN2_IBLK_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_A];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_SETPOINT_UMIN2_IBLK_COMMA, 0);
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
    if (current_ekran.index_position == INDEX_ML_STPUMIN1)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_UMIN1_BEGIN;
      last_position_cursor_x = COL_SETPOINT_UMIN1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPUMIN1_IBLK)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_UMIN1_IBLK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_UMIN1_IBLK_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPUMIN2)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_UMIN2_BEGIN;
      last_position_cursor_x = COL_SETPOINT_UMIN2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_STPUMIN2_IBLK)
    {
      current_ekran.position_cursor_x = COL_SETPOINT_UMIN2_IBLK_BEGIN;
      last_position_cursor_x = COL_SETPOINT_UMIN2_IBLK_END;
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
//Формуємо екран відображення витримок Umin
/*****************************************************/
void make_ekran_timeout_Umin(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_UMIN][MAX_COL_LCD] = 
  {
    {
      " Выдержка ЗНмин1",
      " Выдержка ЗНмин2"
    },
    {
      " Витримка ЗНмін1",
      " Витримка ЗНмін2"
    },
    {
      " Выдержка Umin1 ",
      " Выдержка Umin2 "
    },
    {
      " Выдержка ЗНмин1",
      " Выдержка ЗНмин2"
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
    if (index_of_ekran < (MAX_ROW_FOR_TIMEOUT_UMIN<<1))//Множення на два константи MAX_ROW_FOR_TIMEOUT_UNIN потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_TMOUMIN1)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки Umin1
          if (current_ekran.edition == 0) value = current_settings.timeout_Umin1[group]; //у змінну value поміщаємо значення витримки Umin1
          else value = edition_settings.timeout_Umin1[group];
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOUMIN2)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки Umin2
          if (current_ekran.edition == 0) value = current_settings.timeout_Umin2[group]; //у змінну value поміщаємо значення витримки Umin2
          else value = edition_settings.timeout_Umin2[group];
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_TMOUMIN1)
          {
            if (
                ((j < COL_TMO_UMIN1_BEGIN) ||  (j > COL_TMO_UMIN1_END )) &&
                (j != (COL_TMO_UMIN1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_UMIN1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_UMIN1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_UMIN1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOUMIN2)
          {
            if (
                ((j < COL_TMO_UMIN2_BEGIN) ||  (j > COL_TMO_UMIN2_END )) &&
                (j != (COL_TMO_UMIN2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_UMIN2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_UMIN2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_UMIN2_COMMA, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOUMIN1) 
    {
      current_ekran.position_cursor_x = COL_TMO_UMIN1_BEGIN;
      last_position_cursor_x = COL_TMO_UMIN1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOUMIN2) 
    {
      current_ekran.position_cursor_x = COL_TMO_UMIN2_BEGIN;
      last_position_cursor_x = COL_TMO_UMIN2_END;
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
//Формуємо екран відображення значення управлінської інформації для Umin
/*****************************************************/
void make_ekran_control_Umin()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_UMIN][MAX_COL_LCD] = 
  {
    {
      "     ЗНмин1     ",
      "   ПО ЗНмин1    ",
      "Блок.ЗНмин1 по U",
      "Блок.ЗНмин1 по I",
      "     ЗНмин2     ",
      "   ПО ЗНмин2    ",
      "Блок.ЗНмин2 по U",
      "Блок.ЗНмин2 по I"
    },
    {
      "     ЗНмін1     ",
      "   ПО ЗНмін1    ",
      "Блок.ЗНмін1 по U",
      "Блок.ЗНмін1 по I",
      "     ЗНмін2     ",
      "   ПО ЗНмін2    ",
      "Блок.ЗНмін2 по U",
      "Блок.ЗНмін2 по I"
    },
    {
      "     Umin1      ",
      "    ПО Umin1    ",
      " Блок.Umin1 по U",
      " Блок.Umin1 по I",
      "     Umin2      ",
      "    ПО Umin2    ",
      " Блок.Umin2 по U",
      " Блок.Umin2 по I"
    },
    {
      "     ЗНмин1     ",
      "   ПО ЗНмин1    ",
      "Блок.ЗНмин1 по U",
      "Блок.ЗНмин1 по I",
      "     ЗНмин2     ",
      "   ПО ЗНмин2    ",
      "Блок.ЗНмин2 по U",
      "Блок.ЗНмин2 по I"
    }
  };
  int index_language = index_language_in_array(current_settings.language);

  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_CONTROL_UMIN<<1))//Множення на два константи MAX_ROW_FOR_CONTROL_UMIN потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        const unsigned char information[MAX_ROW_FOR_CONTROL_UMIN][MAX_NAMBER_LANGUAGE][2][MAX_COL_LCD] = 
        {
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"      ИЛИ       ", "       И        "},
            {"      АБО       ", "       І        "},
            {"       OR       ", "      AND       "},
            {"      ИЛИ       ", "       И        "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"      ИЛИ       ", "       И        "},
            {"      АБО       ", "       І        "},
            {"       OR       ", "      AND       "},
            {"      ИЛИ       ", "       И        "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          },
          {
            {"     Откл.      ", "      Вкл.      "},
            {"     Вимк.      ", "     Ввімк.     "},
            {"      Off       ", "       On       "},
            {"     Сљнд.      ", "     Косу.      "}
          }
        };
        const unsigned int cursor_x[MAX_ROW_FOR_CONTROL_UMIN][MAX_NAMBER_LANGUAGE][2] = 
        {
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {5, 6},
            {5, 6},
            {6, 5},
            {5, 6}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {5, 6},
            {5, 6},
            {6, 5},
            {5, 6}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          },
          {
            {4, 5},
            {4, 4},
            {5, 6},
            {4, 4}
          }
        };

        unsigned int index_ctr = (index_of_ekran>>1);

        unsigned int temp_data;
        if(current_ekran.edition == 0) temp_data = current_settings.control_Umin;
        else temp_data = edition_settings.control_Umin;
  
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = information[index_ctr][index_language][(temp_data >> index_ctr) & 0x1][j];
        current_ekran.position_cursor_x = cursor_x[index_ctr][index_language][(temp_data >> index_ctr) & 0x1];
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
