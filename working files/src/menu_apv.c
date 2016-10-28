#include "header.h"

/*****************************************************/
//Формуємо екран відображення витримок АПВ
/*****************************************************/
void make_ekran_timeout_apv(unsigned int group)
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_TIMEOUT_APV][MAX_COL_LCD] = 
  {
    {
      "   1 Цикл АПВ   ",
      "   2 Цикл АПВ   ",
      "   3 Цикл АПВ   ",
      "   4 Цикл АПВ   ",
      "    Блк.АПВ1    ",
      "    Блк.АПВ2    ",
      "    Блк.АПВ3    ",
      "    Блк.АПВ4    ",
      " Блк.АПВ от ВВ  "
    },
    {
      "   1 Цикл АПВ   ",
      "   2 Цикл АПВ   ",
      "   3 Цикл АПВ   ",
      "   4 Цикл АПВ   ",
      "    Бл.АПВ1     ",
      "    Бл.АПВ2     ",
      "    Бл.АПВ3     ",
      "    Бл.АПВ4     ",
      " Бл.АПВ від ВВ  "
    },
    {
      "   AR Shot 1    ",
      "   AR Shot 2    ",
      "   AR Shot 3    ",
      "   AR Shot 4    ",
      "  AR1 Blocking  ",
      "  AR2 Blocking  ",
      "  AR3 Blocking  ",
      "  AR4 Blocking  ",
      "AR Block from CB"
    },
    {
      "   1 Цикл АПВ   ",
      "   2 Цикл АПВ   ",
      "   3 Цикл АПВ   ",
      "   4 Цикл АПВ   ",
      "    Блк.АПВ1    ",
      "    Блк.АПВ2    ",
      "    Блк.АПВ3    ",
      "    Блк.АПВ4    ",
      " Блк.АПВ от ВВ  "
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
    if (index_of_ekran < (MAX_ROW_FOR_TIMEOUT_APV<<1))//Множення на два константи MAX_ROW_FOR_TIMEOUT_APV потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran>>1][j];
        if ((index_of_ekran>>1) == INDEX_ML_TMOAPV1)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 1 Циклу АПВ
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_1[group]; //у змінну value поміщаємо значення витримки 1 Циклу АПВ
          else value = edition_settings.timeout_apv_1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 2 Циклу АПВ
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_2[group]; //у змінну value поміщаємо значення витримки 2 Циклу АПВ
          else value = edition_settings.timeout_apv_2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV3)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 3 Циклу АПВ
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_3[group]; //у змінну value поміщаємо значення витримки 3 Циклу АПВ
          else value = edition_settings.timeout_apv_3[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV4)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки 4 Циклу АПВ
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_4[group]; //у змінну value поміщаємо значення витримки 4 Циклу АПВ
          else value = edition_settings.timeout_apv_4[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV1)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_block_vid_apv1[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv1[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV2)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_block_vid_apv2[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv2[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV3)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_block_vid_apv3[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv3[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV4)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ підготовки до повторної роботи
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_block_vid_apv4[group]; //у змінну value поміщаємо значення витримки АПВ підготовки до повторної роботи
          else value = edition_settings.timeout_apv_block_vid_apv4[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
        else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_VV)
        {
          vaga = 100000; //максимальний ваговий коефіцієнт для вилілення старшого розряду для витримки АПВ подачі вихідного сигналу
          if (current_ekran.edition == 0) value = current_settings.timeout_apv_block_vid_VV[group]; //у змінну value поміщаємо значення итримки АПВ подачі вихідного сигналу
          else value = edition_settings.timeout_apv_block_vid_VV[group];
          first_symbol = 0; //помічаємо, що ще ніодин значущий символ не виведений
        }
      }
      else
      {
        //У парному номері рядку виводимо значення уставки
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((index_of_ekran>>1) == INDEX_ML_TMOAPV1)
          {
            if (
                ((j < COL_TMO_APV_1_BEGIN) ||  (j > COL_TMO_APV_1_END )) &&
                (j != (COL_TMO_APV_1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV2)
          {
            if (
                ((j < COL_TMO_APV_2_BEGIN) ||  (j > COL_TMO_APV_2_END )) &&
                (j != (COL_TMO_APV_2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV3)
          {
            if (
                ((j < COL_TMO_APV_3_BEGIN) ||  (j > COL_TMO_APV_3_END )) &&
                (j != (COL_TMO_APV_3_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_3_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_3_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_3_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV4)
          {
            if (
                ((j < COL_TMO_APV_4_BEGIN) ||  (j > COL_TMO_APV_4_END )) &&
                (j != (COL_TMO_APV_4_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_4_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_4_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_4_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV1)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV1_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV1_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV1_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV1_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV1_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV1_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV2)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV2_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV2_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV2_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV2_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV2_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV2_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV3)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV3_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV3_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV3_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV3_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV3_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV3_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_APV4)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_APV4_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_APV4_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_APV4_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_APV4_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_APV4_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_APV4_COMMA, 0);
          }
          else if ((index_of_ekran>>1) == INDEX_ML_TMOAPV_BLOCK_VID_VV)
          {
            if (
                ((j < COL_TMO_APV_BLOCK_VID_VV_BEGIN) ||  (j > COL_TMO_APV_BLOCK_VID_VV_END )) &&
                (j != (COL_TMO_APV_BLOCK_VID_VV_END + 2))  
               )working_ekran[i][j] = ' ';
            else if (j == COL_TMO_APV_BLOCK_VID_VV_COMMA )working_ekran[i][j] = ',';
            else if (j == (COL_TMO_APV_BLOCK_VID_VV_END + 2)) working_ekran[i][j] = odynyci_vymirjuvannja[index_language][INDEX_SECOND];
            else
              calc_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, j, COL_TMO_APV_BLOCK_VID_VV_COMMA, 0);
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
    if (current_ekran.index_position == INDEX_ML_TMOAPV1) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_1_BEGIN;
      last_position_cursor_x = COL_TMO_APV_1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV2) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_2_BEGIN;
      last_position_cursor_x = COL_TMO_APV_2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV3) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_3_BEGIN;
      last_position_cursor_x = COL_TMO_APV_3_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV4) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_4_BEGIN;
      last_position_cursor_x = COL_TMO_APV_4_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV1) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV1_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV1_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV2) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV2_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV2_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV3) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV3_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV3_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_APV4) 
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_APV4_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_APV4_END;
    }
    else if (current_ekran.index_position == INDEX_ML_TMOAPV_BLOCK_VID_VV)
    {
      current_ekran.position_cursor_x = COL_TMO_APV_BLOCK_VID_VV_BEGIN;
      last_position_cursor_x = COL_TMO_APV_BLOCK_VID_VV_END;
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
//Формуємо екран відображення значення управлінської інформації для АПВ
/*****************************************************/
void make_ekran_control_apv()
{
  const unsigned char name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_CONTROL_APV][MAX_COL_LCD] = 
  {
    {
      "      АПВ       ",
      "      АПВ2      ",
      "      АПВ3      ",
      "      АПВ4      ",
      "  Пуск от МТЗ1  ",
      "  Пуск от МТЗ2  ",
      "  Пуск от МТЗ3  ",
      "  Пуск от МТЗ4  "
    },
    {
      "      АПВ       ",
      "      АПВ2      ",
      "      АПВ3      ",
      "      АПВ4      ",
      " Пуск від МСЗ1  ",
      " Пуск від МСЗ2  ",
      " Пуск від МСЗ3  ",
      " Пуск від МСЗ4  "
    },
    {
      "       AR       ",
      "      AR2       ",
      "      AR3       ",
      "      AR4       ",
      " Start from OCP1",
      " Start from OCP2",
      " Start from OCP3",
      " Start from OCP4"
    },
    {
      "      АПВ       ",
      "      АПВ2      ",
      "      АПВ3      ",
      "      АПВ4      ",
      "  Пуск от МТЗ1  ",
      "  Пуск от МТЗ2  ",
      "  Пуск от МТЗ3  ",
      "  Пуск от МТЗ4  "
    }
  };
  unsigned char name_string_tmp[MAX_ROW_FOR_CONTROL_APV][MAX_COL_LCD];

  int index_language = index_language_in_array(current_settings.language);
  for(int index_1 = 0; index_1 < MAX_ROW_FOR_CONTROL_APV; index_1++)
  {
    for(int index_2 = 0; index_2 < MAX_COL_LCD; index_2++)
      name_string_tmp[index_1][index_2] = name_string[index_language][index_1][index_2];
  }
  
  /******************************************/
  //Виключаємо поля, які не треба відображати
  /******************************************/
  int additional_current_mtz = 0;
  int position_temp = current_ekran.index_position;
  int index_of_ekran;
  
  if ((current_settings.configuration & (1<<MTZ_BIT_CONFIGURATION)) == 0)
  {
    for (int current_index = INDEX_ML_CTRAPV_STARTED_FROM_MTZ1; current_index <= INDEX_ML_CTRAPV_STARTED_FROM_MTZ4; current_index++ )
    {
      int i = current_index - additional_current_mtz;
    
      if ((i+1) <= position_temp) position_temp--;
      do
      {
        for(unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((i+1) < (MAX_ROW_FOR_CONTROL_APV - additional_current_mtz)) name_string_tmp[i][j] = name_string_tmp[i + 1][j];
          else name_string_tmp[i][j] = ' ';
        }
        i++;
      }
      while (i < (MAX_ROW_FOR_CONTROL_APV - additional_current_mtz));
      additional_current_mtz++;
    }
  }
  /******************************************/

  //Множення на два величини position_temp потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;
  
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    if (index_of_ekran < ((MAX_ROW_FOR_CONTROL_APV - additional_current_mtz)<<1))//Множення на два константи MAX_ROW_FOR_CONTROL_APV потрібне для того, бо на одну позицію ми використовуємо два рядки (назва + значення)
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

        unsigned int temp_data;
          
        if(current_ekran.edition == 0) temp_data = current_settings.control_apv;
        else temp_data = edition_settings.control_apv;
          
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
