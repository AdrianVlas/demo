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
      " Выходы         ",
      " Светоиндикаторы",
      " Выключатель    ",
      " Трансформаторы ",
      " УВВ            ",
      " Коммуникация   ",
      " Регистраторы   ",
      " Расш.логика    ",
      " Пароли         "
    },
    {
      " Версія ПЗ і КП ",
      " Мітка налашт.  ",
      " Виходи         ",
      " Світлоіндикат. ",
      " Вимикач        ",
      " Трансформатори ",
      " УВВ            ",
      " Комунікація    ",
      " Реєстратори    ",
      " Розш.логіка    ",
      " Паролі         "
    },
    {
      " VER.of F/W & MM",
      " Settings Mark  ",
      " Outputs        ",
      " LED            ",
      " Circuit-Breaker",
      " Transformers   ",
      " BIOS           ",
      " Communication  ",
      " Recorders      ",
      " Extended logic ",
      " Passwords      "
    },
    {
      " Версия ПО и КП ",
      " Метка настроек ",
      " Выходы         ",
      " Светоиндикаторы",
      " Выключатель    ",
      " Трансформаторы ",
      " УВВ            ",
      " Коммуникация   ",
      " Регистраторы   ",
      " Расш.логика    ",
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
  
  int position_temp = current_ekran.index_position;
  int index_of_ekran;
  index_of_ekran = (position_temp >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  
  //Копіюємо  рядки у робочий екран
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    //Наступні рядки треба перевірити, чи їх требе відображати у текучій коффігурації
    if (index_of_ekran < MAX_ROW_FOR_CHOSE_SETTINGS)
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
