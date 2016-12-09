#include "header.h"

/*****************************************************/
//Формуємо екран відображення списку сигналізацій
/*****************************************************/
void make_ekran_choose_list_alarms(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_LIST_ALARMS][MAX_COL_LCD] = 
  {
    {
      " Сигнализация 1 ",
      " Сигнализация 2 "
    },
    {
      " Сигналізація 1 ",
      " Сигналізація 2 "
    },
    {
      " Alarms 1       ",
      " Alarms 2       "
    },
    {
      " Сигнализация 1 ",
      " Сигнализация 2 "
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
    if (index_of_ekran < MAX_ROW_FOR_LIST_ALARMS)
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

/*****************************************************/
//Формуємо екран відображення заголовків настроювання для сигналізації
/*****************************************************/
void make_ekran_choose_settings_alarms(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOOSE_SETTINGS_ALARMS][MAX_COL_LCD] = 
  {
    {
      " Выдержки       ",
      " Управление     ",
      " Ранжирование   "
    },
    {
      " Витримки       ",
      " Управління     ",
      " Ранжування     "
    },
    {
      " Delay          ",
      " Control        ",
      " Ранжирование   "
    },
    {
      " Кyрулар        ",
      " Беріктік       ",
      " Ранжирование   "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій кофігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOOSE_SETTINGS_ALARMS)
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

/*****************************************************/
//Формуємо екран відображення витримок Виключателя
/*****************************************************/
void make_ekran_timeout_alarms(unsigned int alarms)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_ALARMS][MAX_COL_LCD] = 
  {
    {
      "  Длит.сигнала  "
    },
    {
      "  Трив.сигналу  "
    },
    {
      " Alarm duration "
    },
    {
      "  Длит.сигнала  "
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
    if (index_of_ekran < (MAX_ROW_FOR_TIMEOUT_ALARMS<<1))//Множення на два константи MAX_ROW_FOR_TIMEOUT_ALARMS потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_ALARM_SOUND_PERIOD)
        {
          vaga = 10000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки
          if (current_ekran.edition == 0) value = current_settings.timeout_alarm_sound_period[alarms]; //у змінну value поміщаємо значення витримки.
          else value = edition_settings.timeout_alarm_sound_period[alarms];
        }
        first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_ALARM_SOUND_PERIOD)
          {
            if (
                ((j < COL_TMO_ALARM_SOUND_PERIOD_BEGIN) ||  (j > COL_TMO_ALARM_SOUND_PERIOD_END )) &&
                (j != (COL_TMO_ALARM_SOUND_PERIOD_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_ALARM_SOUND_PERIOD_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_ALARM_SOUND_PERIOD_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_ALARM_SOUND_PERIOD_COMMA, 0);
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
    if (current_ekran.index_position == INDEX_ML_ALARM_SOUND_PERIOD)
    {
      current_ekran.position_cursor_x = COL_TMO_ALARM_SOUND_PERIOD_BEGIN;
      last_position_cursor_x = COL_TMO_ALARM_SOUND_PERIOD_END;
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
//Формуємо екран відображення значення управлінської інформації для вимикача
/*****************************************************/
void make_ekran_control_alarms(unsigned int alarms)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_ALARMS][MAX_COL_LCD] = 
  {
    {
      "     Режим      "
    },
    {
      "     Режим      "
    },
    {
      "      Mode      "
    },
    {
      "     Режим      "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < (MAX_ROW_FOR_CONTROL_ALARMS<<1))//Множення на два потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
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
          {"   Нормальный   ", " Заданое время  "},
          {"   Нормальний   ", "  Заданий час   "},
          {"     Normal     ", " Specif.period  "},
          {"   Нормальный   ", " Заданое время  "}
        };
        const unsigned int cursor_x[MAX_NAMBER_LANGUAGE][2] = 
        {
          {2, 0},
          {2, 1},
          {4, 0},
          {2, 0}
        };
        
        unsigned int index_ctr = (index_of_ekran>>1) + (ALARMS_SEPARATOR*alarms);

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.control_alarm;
        else temp_data = edition_settings.control_alarm;
          
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
