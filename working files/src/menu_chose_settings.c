#include "header.h"

/*****************************************************/
//Формуємо екран відображення заголовків настройок
/*****************************************************/
void make_ekran_chose_settings(void)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CHOSE_SETTINGS][MAX_COL_LCD] = 
  {
    {
      " Версия ПО и КП ",
      " Метка настроек ",
      " Входы          ",
      " Выходы         ",
      " Светоиндикаторы",
      " Выключатель    ",
      " Трансформаторы ",
      " УВВ            ",
      " Коммуникация   ",
      " Регистраторы   ",
      " Расш.логика    ",
      " Ф-кнопки       ",
      " Группа уставок ",
      " Доп.настройки  ",
      " Пароли         "
    },
    {
      " Версія ПЗ і КП ",
      " Мітка налашт.  ",
      " Входи          ",
      " Виходи         ",
      " Світлоіндикат. ",
      " Вимикач        ",
      " Трансформатори ",
      " УВВ            ",
      " Комунікація    ",
      " Реєстратори    ",
      " Розш.логіка    ",
      " Ф-кнопки       ",
      " Група уставок  ",
      " Дод.налашт.    ",
      " Паролі         "
    },
    {
      " VER.of F/W & MM",
      " Settings Mark  ",
      " Inputs         ",
      " Outputs        ",
      " LED            ",
      " Circuit-Breaker",
      " Transformers   ",
      " BIOS           ",
      " Communication  ",
      " Recorders      ",
      " Extended logic ",
      " F-Keys         ",
      " Pick-up Set    ",
      " Addit.Settings ",
      " Passwords      "
    },
    {
      " Версия ПО и КП ",
      " Метка настроек ",
      " Входы          ",
      " Выходы         ",
      " Светоиндикаторы",
      " Выключатель    ",
      " Трансформаторы ",
      " УВВ            ",
      " Коммуникация   ",
      " Регистраторы   ",
      " Расш.логика    ",
      " Ф-кнопки       ",
      " Группа уставок ",
      " Доп.настройки  ",
      " Пароли         "
    }
  };
  unsigned char name_string_tmp[MAX_ROW_FOR_CHOSE_SETTINGS][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_CHOSE_SETTINGS; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;

  if ((current_settings.configuration & (1 << EL_BIT_CONFIGURATION)) == 0)
  {
    int i = INDEX_OF_EXTENDED_LOGIC - additional_current;
    if ((i+1) <= position_temp) position_temp--;
    do
    {
      for(unsigned int j = 0; j < MAX_COL_LCD; j++)
      {
        if ((i+1) < (MAX_ROW_FOR_CHOSE_SETTINGS - additional_current)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
        else name_string_tmp[i][j] = ' ';
      }

      i++;
    }
    while (i< (MAX_ROW_FOR_CHOSE_SETTINGS - additional_current));
    additional_current++;
  }
  /******************************************/
  
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < (MAX_ROW_FOR_CHOSE_SETTINGS - additional_current))
    {
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string_tmp[index_of_ekran][j];
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
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
