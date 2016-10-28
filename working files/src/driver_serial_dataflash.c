#include "header.h"

/*****************************************************/
//Запустити передачу і прийом по SPI
/*****************************************************/
void start_exchange_via_spi(int index_chip, unsigned int number_bytes_transfer)
{
  //Фіксуємо скільки байт ми будем передавати (це потрібно на випадок винекнення помилки - щоб можна було повторно запустити цю операцію)
  number_bytes_transfer_spi_df = number_bytes_transfer;
  
  //Зупиняємо потік DMA на прийом даних якщо він запущений
  if ((DMA_StreamSPI_DF_Rx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamSPI_DF_Rx->CR &= ~(uint32_t)DMA_SxCR_EN;
  DMA_StreamSPI_DF_Rx->NDTR = number_bytes_transfer;
  //Зупиняємо потік DMA на передачу даних якщо він запущений
  if ((DMA_StreamSPI_DF_Tx->CR & (uint32_t)DMA_SxCR_EN) !=0) DMA_StreamSPI_DF_Tx->CR &= ~(uint32_t)DMA_SxCR_EN;
  DMA_StreamSPI_DF_Tx->NDTR = number_bytes_transfer;
  
  //Очищаємо прапореці, що сигналізує про завершення прийом/передачі даних для DMA1 по каналу SPI_DF_Rx і SPI_DF_Tx
  DMA_ClearFlag(DMA_StreamSPI_DF_Rx, DMA_FLAG_TCSPI_DF_Rx | DMA_FLAG_HTSPI_DF_Rx | DMA_FLAG_TEISPI_DF_Rx | DMA_FLAG_DMEISPI_DF_Rx | DMA_FLAG_FEISPI_DF_Rx);
  DMA_ClearFlag(DMA_StreamSPI_DF_Tx, DMA_FLAG_TCSPI_DF_Tx | DMA_FLAG_HTSPI_DF_Tx | DMA_FLAG_TEISPI_DF_Tx | DMA_FLAG_DMEISPI_DF_Tx | DMA_FLAG_FEISPI_DF_Tx);
  
  //Дозволяємо передачу через DMA
  if ((SPI_DF->CR2 & SPI_I2S_DMAReq_Tx) == 0) SPI_DF->CR2 |= SPI_I2S_DMAReq_Tx;
  //Дозволяємо прийом через DMA
  if ((SPI_DF->CR2 & SPI_I2S_DMAReq_Rx) == 0) SPI_DF->CR2 |= SPI_I2S_DMAReq_Rx;

  //Виставляємо chip_select  з встановленням у драйвері повідомлення, що іде обмін
  if (index_chip == INDEX_DATAFLASH_1) GPIO_SPI_DF_TOGGLE->BSRRH = GPIO_SPI_DF_TOGGLE_Pin;//DF_TOGGLE - пін переводимо у 0 
  else if (index_chip == INDEX_DATAFLASH_2)GPIO_SPI_DF_TOGGLE->BSRRL = GPIO_SPI_DF_TOGGLE_Pin;//DF_TOGGLE - пін переводимо у 1
  else
  {
    //Відбцлася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(6);
  }
  driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING;
  GPIO_SPI_DF->BSRRH = GPIO_NSSPin_DF; //Виставляємо Chip_select переводом NSS  у 0

  //Дозволяэмо генерацыю переривань від потоку DMA_StreamSPI_DF_Rx
  DMA_StreamSPI_DF_Rx->CR |= DMA_IT_TC;
  
  //Очищаємо можливі помилкит
  SPI_DF->DR;
  SPI_DF->SR;

  //Запускаємо прийом-передачу 
  DMA_StreamSPI_DF_Rx->CR |= (uint32_t)DMA_SxCR_EN;
  DMA_StreamSPI_DF_Tx->CR |= (uint32_t)DMA_SxCR_EN;
}
/*****************************************************/

/*****************************************************/
//Запуск зчитування статусу з мікросхеми dataFlash
/*****************************************************/
void dataflash_status_read(int index_chip)
{
  if ((index_chip == INDEX_DATAFLASH_1) || (index_chip == INDEX_DATAFLASH_2))
  {
    driver_spi_df[index_chip].code_operation = CODE_OPERATION_STATUS_READ;
    TxBuffer_SPI_DF[0] = 0xD7;
    start_exchange_via_spi(index_chip, 2);
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(7);
  }
  
}
/*****************************************************/

/*****************************************************/
//Подача команди на переконфігурацію DataFlash на розмір сторінки 256Б
/*****************************************************/
void dataflash_set_pagesize_256(int index_chip)
{
  if ((index_chip == INDEX_DATAFLASH_1) || (index_chip == INDEX_DATAFLASH_2))
  {
    driver_spi_df[index_chip].code_operation = CODE_OPERATION_PAGESIZE_256;
    TxBuffer_SPI_DF[0] = 0x3D;
    TxBuffer_SPI_DF[1] = 0x2A;
    TxBuffer_SPI_DF[2] = 0x80;
    TxBuffer_SPI_DF[3] = 0xA6;
    start_exchange_via_spi(index_chip, 4);
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(8);
  }
  
}
/*****************************************************/

/*****************************************************/
//Подача команди повного стирання мікросхеми
/*****************************************************/
void dataflash_erase(int index_chip)
{
  if ((index_chip == INDEX_DATAFLASH_1) || (index_chip == INDEX_DATAFLASH_2))
  {
    driver_spi_df[index_chip].code_operation = CODE_OPERATION_ERASE;
    TxBuffer_SPI_DF[0] = 0xC7;
    TxBuffer_SPI_DF[1] = 0x94;
    TxBuffer_SPI_DF[2] = 0x80;
    TxBuffer_SPI_DF[3] = 0x9A;
    start_exchange_via_spi(index_chip, 4);
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(9);
  }
  
}
/*****************************************************/

/*****************************************************/
//Запис даних у буфер з подальшим стиранням і перепрограмуванням
/*****************************************************/
void dataflash_mamory_page_program_through_buffer(int index_chip)
{
  unsigned int size_page;
  driver_spi_df[index_chip].code_operation = CODE_OPERATION_WRITE_PAGE_THROUGH_BUFFER;
  TxBuffer_SPI_DF[0] = 0x82;
  
  if (index_chip == INDEX_DATAFLASH_1)
  {
    size_page = SIZE_PAGE_DATAFLASH_1;

    //Запис даних дискретного реєстратора
      
    //Формуємо буфер для запису
    if(part_writing_dr_into_dataflash < NUMBER_PAGES_IN_ONE_DR_RECORD)
    {
      //Формуємо адресу для запису
      unsigned int address_for_program_dataflash = info_rejestrator_dr.next_address + part_writing_dr_into_dataflash*size_page;
        
      TxBuffer_SPI_DF[1] = (address_for_program_dataflash >> 16) & 0x0f; 
      TxBuffer_SPI_DF[2] = (address_for_program_dataflash >> 8 ) & 0xff; 
      TxBuffer_SPI_DF[3] = 0; 
        
      //Заповнюємо дальше буфер даними, які треба записати 
      unsigned int offset_from_start = part_writing_dr_into_dataflash*size_page;
      for (unsigned int i = 0; i < size_page; i++ )
        TxBuffer_SPI_DF[4 + i] = buffer_for_save_dr_record_level_2[offset_from_start + i];
    }
    else
    {
      //Відбулася невизначена помилка, тому треба піти на перезавантаження
      total_error_sw_fixed(10);
    }
  }
  else if (index_chip == INDEX_DATAFLASH_2)
  {
    size_page = SIZE_PAGE_DATAFLASH_2;

    //Запис даних аналогового реєстратора (цілої сторінки повністю)

    //Формуємо адресу для запису
    TxBuffer_SPI_DF[1] = (temporary_address_ar >> 16) & 0x1f; 
    TxBuffer_SPI_DF[2] = (temporary_address_ar >> 8 ) & 0xfe; 
    TxBuffer_SPI_DF[3] = 0; 
        
    //Заповнюємо дальше буфер даними, які треба записати 
    for (unsigned int i = 0; i < size_page; i++ )
    TxBuffer_SPI_DF[4 + i] = buffer_for_save_ar_record[i];
    
    /*
    Очищаємо кількість байт для запису у змінній count_to_save 
    Це означає, що всі дані для запису "забрані" з мкасиву buffer_for_save_ar_record
    і його можна зараз заповнювати новими даними, поки попередні записуються у DataFlash
    Тому виставляєм будь-яким ненульоаим значенням у змінній permit_copy_new_data дозвіл на підготовку нових даних
    
    Перед цим визначаємо наступну адресу для запису
    */
    temporary_address_ar += size_page;
    count_to_save = 0;
    permit_copy_new_data = 0xff;
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(11);
  }
  
  //Запускаємо процес запису
  start_exchange_via_spi(index_chip, (4 + size_page));
}
/*****************************************************/

/*****************************************************/
//Зчитування даних на швидкості до 66МГц
/*****************************************************/
void dataflash_mamory_read(int index_chip)
{
  unsigned int size_page;

  driver_spi_df[index_chip].code_operation = CODE_OPERATION_READ_HIGH_FREQ;
  TxBuffer_SPI_DF[0] = 0x0B;

  int temp_value_for_address;
    
  if (index_chip == INDEX_DATAFLASH_1)
  {
    size_page = SIZE_PAGE_DATAFLASH_1;

    if(
       (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_MENU ) ||
       (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_USB  ) ||
       (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_RS485)
      )
    {
      //Читання даних дискретного реєстратора
      unsigned int part_reading;
      unsigned int number_record;
      if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_MENU)
      {
        part_reading = part_reading_dr_from_dataflash_for_menu;
        number_record = number_record_of_dr_for_menu;
      }
      else if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_USB)
      {
        part_reading = part_reading_dr_from_dataflash_for_USB;
        number_record = number_record_of_dr_for_USB;
      }
      else
      {
        part_reading = part_reading_dr_from_dataflash_for_RS485;
        number_record = number_record_of_dr_for_RS485;
      }

      //Формуємо буфер для передавання у мікросхему DataFlash
      if(part_reading < NUMBER_PAGES_IN_ONE_DR_RECORD)
      {
        //Формуємо адресу для читання частини запису дисретного реєстратора
        temp_value_for_address = info_rejestrator_dr.next_address - (((number_record + 1)*NUMBER_PAGES_IN_ONE_DR_RECORD - part_reading)<<VAGA_SIZE_PAGE_DATAFLASH_1);
        while (temp_value_for_address < MIN_ADDRESS_DR_AREA) temp_value_for_address = temp_value_for_address + SIZE_DR_AREA; 
      
        TxBuffer_SPI_DF[1] = (temp_value_for_address >> 16) & 0x0f; 
        TxBuffer_SPI_DF[2] = (temp_value_for_address >> 8 ) & 0xff; 
        TxBuffer_SPI_DF[3] = (temp_value_for_address      ) & 0xff; 
        
        //Після адреси має іти один додатковй байт як буфер перед початком отримування реальних даних
        
        //Подальше вмістиме не має значення

        //Запускаємо процес запису
        start_exchange_via_spi(index_chip, (5 + size_page));
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(12);
      }
    }
    else if (
             (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU ) ||
             (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB  ) ||
             (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_RS485)
            )   
    {
      //Читання даних реєстратора програмних помилок
      unsigned int number_record;
      if (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU)
      {
        number_record = number_record_of_pr_err_into_menu;
      }
      else if (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB)
      {
        number_record = number_record_of_pr_err_into_USB;
      }
      else
      {
        number_record = number_record_of_pr_err_into_RS485;
      }

      //Визначаємо початкову адресу запису, яку треба зчитати
      temp_value_for_address = info_rejestrator_pr_err.next_address - ((number_record + 1)<<VAGA_SIZE_ONE_RECORD_PR_ERR);
      while (temp_value_for_address < MIN_ADDRESS_PR_ERR_AREA) temp_value_for_address = temp_value_for_address + SIZE_PR_ERR_AREA; 
        
      TxBuffer_SPI_DF[1] = (temp_value_for_address >> 16) & 0x0f; 
      TxBuffer_SPI_DF[2] = (temp_value_for_address >> 8 ) & 0xff; 
      TxBuffer_SPI_DF[3] = (temp_value_for_address      ) & 0xff; 

      //Після адреси має іти один додатковй байт як буфер перед початком отримування реальних даних
        
      //Подальше вмістиме не має значення

      //Запускаємо процес запису
      start_exchange_via_spi(index_chip, (5 + SIZE_ONE_RECORD_PR_ERR));
    }
    else
    {
      //Відбулася невизначена помилка, тому треба піти на перезавантаження
      total_error_sw_fixed(13);
    }
  }
  else if (index_chip == INDEX_DATAFLASH_2)
  {
    size_page = SIZE_PAGE_DATAFLASH_2;

    if(
       (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_MENU ) ||
       (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_USB  ) ||
       (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_RS485)
      )
    {
      //Читання даних аналогового реєстратора
      unsigned int number_bytes;
      unsigned int size_one_ar_record_tmp = size_one_ar_record, max_number_records_ar_tmp = max_number_records_ar;
      if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_MENU)
      {
        temp_value_for_address = info_rejestrator_ar.next_address - (number_record_of_ar_for_menu + 1)*size_one_ar_record_tmp;
        while (temp_value_for_address < MIN_ADDRESS_AR_AREA) temp_value_for_address = temp_value_for_address + size_one_ar_record_tmp*max_number_records_ar_tmp; 
        number_bytes = sizeof(__HEADER_AR);
      }
      else 
      {
        unsigned int number_record_of_ar;
        int first_number_time_sample, last_number_time_sample;
        
        if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_USB)
        {
          number_record_of_ar = number_record_of_ar_for_USB;
          first_number_time_sample = first_number_time_sample_for_USB;
          last_number_time_sample = last_number_time_sample_for_USB;
        }
        else
        {
          number_record_of_ar = number_record_of_ar_for_RS485;
          first_number_time_sample = first_number_time_sample_for_RS485;
          last_number_time_sample = last_number_time_sample_for_RS485;
        }

        //Визначаємо першу адресу вибраного запису
        /*
        Використовую принцип, що мая програма не передбачає, що запис може початися в кінці адресного опростору 
        DataFlash, а закічення - вже на початку мікросхеми. Такого я не передбачав, тому коли я вже визначив базову адресу, 
        то адреси різних частин вже визначаю без перевірки на завершення "адресного простору" для вибраних витримок (доаварійного
        і післяаварійного масивів)
        */
        temp_value_for_address = info_rejestrator_ar.next_address - (number_record_of_ar + 1)*size_one_ar_record_tmp;
        while (temp_value_for_address < MIN_ADDRESS_AR_AREA) temp_value_for_address = temp_value_for_address + size_one_ar_record_tmp*max_number_records_ar_tmp; 

        if (first_number_time_sample == -1)
        {
          //Читання часових зрізів разом з заголовком аналогового реєстратора
          number_bytes = sizeof(__HEADER_AR) + (last_number_time_sample + 1)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int);
          //Адреса читання вде визначена, бо вона співпадає з базовою адресою
        }
        else
        {
          //Читання часових зрізів разом без заголовка аналогового реєстратора
          number_bytes = (last_number_time_sample + 1 - first_number_time_sample)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int);
          temp_value_for_address += (sizeof(__HEADER_AR) + first_number_time_sample*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int));
        }
      }

      //Формуємо буфер для передавання у мікросхему DataFlash
      if(number_bytes <= size_page)
      {
        TxBuffer_SPI_DF[1] = (temp_value_for_address >> 16) & 0x1f; 
        TxBuffer_SPI_DF[2] = (temp_value_for_address >> 8 ) & 0xff; 
        TxBuffer_SPI_DF[3] = (temp_value_for_address      ) & 0xff; 
        
        //Після адреси має іти один додатковй байт як буфер перед початком отримування реальних даних
        
        //Подальше вмістиме не має значення

        //Запускаємо процес запису
        start_exchange_via_spi(index_chip, (5 + number_bytes));
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(31);
      }
    }
    else
    {
      //Відбулася невизначена помилка, тому треба піти на перезавантаження
      total_error_sw_fixed(32);
    }
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(14);
  }
}
/*****************************************************/

/*****************************************************/
//Зчитування сторінки пам'яті DataFlash у буфер
/*****************************************************/
void dataflash_mamory_page_into_buffer(int index_chip)
{
  unsigned int address_into_dataflash;
  driver_spi_df[index_chip].code_operation = CODE_OPERATION_READ_PAGE_INTO_BUFFER;
  TxBuffer_SPI_DF[0] = 0x53;
  
  if (index_chip == INDEX_DATAFLASH_1)
  {
    //Формуємо адресу сторінки для зчитування у буфер DataFlash
    address_into_dataflash = info_rejestrator_pr_err.next_address;

    TxBuffer_SPI_DF[1] = (address_into_dataflash >> 16) & 0x0f; 
    TxBuffer_SPI_DF[2] = (address_into_dataflash >> 8 ) & 0xff; 
    TxBuffer_SPI_DF[3] = 0; 
  }
  else if (index_chip == INDEX_DATAFLASH_2)
  {
    //Формуємо адресу сторінки для зчитування у буфер DataFlash
    address_into_dataflash = temporary_address_ar;

    TxBuffer_SPI_DF[1] = (address_into_dataflash >> 16) & 0x1f; 
    TxBuffer_SPI_DF[2] = (address_into_dataflash >> 8 ) & 0xfe; 
    TxBuffer_SPI_DF[3] = 0; 
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(15);
  }

  //Запускаємо процес запису
  start_exchange_via_spi(index_chip, 4);
}
/*****************************************************/

/*****************************************************/
//Запис нових даних у буфер мікрсхеми DataFlash
/*****************************************************/
void dataflash_mamory_write_buffer(int index_chip)
{
  unsigned int size_page;

  driver_spi_df[index_chip].code_operation = CODE_OPERATION_WRITE_BUFFER;
  TxBuffer_SPI_DF[0] = 0x84;

  if (index_chip == INDEX_DATAFLASH_1)
  {
    size_page = SIZE_PAGE_DATAFLASH_1;

    //Запис даних реєстратора програмних подій з останнього свобідного місця і або всі дані, або стільки скільки може вміститися до кінця сторінки
      
    //Формуємо адресу для запису
    unsigned int next_address_into_buffer = info_rejestrator_pr_err.next_address & 0xff;
        
    TxBuffer_SPI_DF[1] = 0; 
    TxBuffer_SPI_DF[2] = 0; 
    TxBuffer_SPI_DF[3] = next_address_into_buffer; 
    
    //Починаємо заповнювати буфер для передачі у буфер мікросхеми DataFlash даними для запису
    number_recods_writing_into_dataflash_now = 0;
    unsigned int head = head_fifo_buffer_pr_err_records, tail = tail_fifo_buffer_pr_err_records;
    while(
          ((next_address_into_buffer + SIZE_ONE_RECORD_PR_ERR - 1) < size_page) &&
          (tail != head)
         )
    {
      //Заповнюємо дальше буфер даними, які треба записати 
      for (unsigned int i = 0; i < SIZE_ONE_RECORD_PR_ERR; i++ )
        TxBuffer_SPI_DF[4 + number_recods_writing_into_dataflash_now*SIZE_ONE_RECORD_PR_ERR + i] =
          buffer_pr_err_records[tail*SIZE_ONE_RECORD_PR_ERR + i];
      
      //Змінюємо контролюючі змінні
      number_recods_writing_into_dataflash_now++;
      next_address_into_buffer += SIZE_ONE_RECORD_PR_ERR;
      tail++;
      if(tail >= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER) tail = 0;
    }

    //Запускаємо процес запису
    start_exchange_via_spi(index_chip, (4 + number_recods_writing_into_dataflash_now*SIZE_ONE_RECORD_PR_ERR));
  }
  else if (index_chip == INDEX_DATAFLASH_2)
  {
    size_page = SIZE_PAGE_DATAFLASH_2;

    //Запис частини сторінки для аналогового реєстратора

    //Формуємо адресу для запису
    TxBuffer_SPI_DF[1] = 0; 
    TxBuffer_SPI_DF[2] = (temporary_address_ar >>  8) & 0x01; 
    TxBuffer_SPI_DF[3] =  temporary_address_ar        & 0xff; 
    
    //Заповнюємо дальше буфер даними, які треба записати 
    for (unsigned int i = 0; i < count_to_save; i++ )
      TxBuffer_SPI_DF[4 + i] = buffer_for_save_ar_record[i];

    //Запускаємо процес запису
    start_exchange_via_spi(index_chip, (4 + count_to_save));

    /*
    Не очищаємо кількість байт для запису у змінній count_to_save тут, бо програма
    має ророблений захист на випадок, якщо по незрозумілій ситуації (яка теоретичноніколи не має відбутися)
    порушився порядок "Заргустка сторіни у внутрішній буфер_1 DataFlash" - "Модифікація цього внутрішнього буферу" -
    "Запис цього буферу у DataFlash"
    Тому виконаємо цю очистку після того, коли будемо впевнені, що повторно ці дані точно вже не знагодяться.
    
    Хоч, якщо все працювати мало б правильно, то вже з цього місця теперішнє вмістиме буферу buffer_for_save_ar_record не потрібне 
    */
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(16);
  }
}
/*****************************************************/

/*****************************************************/
//Запис буфер DataFlash у пам'ять мікросхеми DataFlash
/*****************************************************/
void dataflash_mamory_buffer_into_memory(int index_chip)
{
  //Запис внутрішнбього буферу (буферу 1) пам'ять DataFlash
  driver_spi_df[index_chip].code_operation = CODE_OPERATION_WRITE_BUFFER_INTO_MEMORY_WITH_ERASE;
  TxBuffer_SPI_DF[0] = 0x83;

  if (index_chip == INDEX_DATAFLASH_1)
  {
    //У структурі по інформації стану реєстраторів виставляємо повідомлення, що почався запис і ще не закінчився
    _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
    info_rejestrator_pr_err.saving_execution = 1;
    
    //Формуємо адресу для запису
    unsigned int address_into_dataflash = info_rejestrator_pr_err.next_address;
        
    TxBuffer_SPI_DF[1] = (address_into_dataflash >> 16) & 0x0f; 
    TxBuffer_SPI_DF[2] = (address_into_dataflash >> 8 ) & 0xff; 
    TxBuffer_SPI_DF[3] = 0; 
  }
  else if (index_chip == INDEX_DATAFLASH_2)
  {
    //Формуємо адресу для запису
    TxBuffer_SPI_DF[1] = (temporary_address_ar >> 16) & 0x1f; 
    TxBuffer_SPI_DF[2] = (temporary_address_ar >> 8 ) & 0xfe; 
    TxBuffer_SPI_DF[3] = 0; 
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(17);
  }

  //Запускаємо процес запису
  start_exchange_via_spi(index_chip, 4);
}
/*****************************************************/


/*****************************************************/
//аналіз отриманих даних після запиту
/*****************************************************/
inline void analize_received_data_dataflash(int index_chip)
{
  switch (driver_spi_df[index_chip].code_operation)
  {
  case CODE_OPERATION_STATUS_READ:
    {
      if ((index_chip == INDEX_DATAFLASH_1) || (index_chip == INDEX_DATAFLASH_2))
      {
        //Виконувалася операція зчитування статусу мікросхеми DataFlash
        if ((RxBuffer_SPI_DF[1] & (1<< 7)) != 0) dataflash_not_busy |= (1 << index_chip);
        else dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
        
        driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
        driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(26);
      }
      break;
    }
  case CODE_OPERATION_ERASE:
    {
      //Подана команда повного стирання мікросхеми
     
      //Знімаємо з черги запуск повного стирання
      if (index_chip == INDEX_DATAFLASH_1) control_tasks_dataflash &= (unsigned int)(~TASK_ERASE_DATAFLASH_1);
      else if (index_chip == INDEX_DATAFLASH_2) control_tasks_dataflash &= (unsigned int)(~TASK_ERASE_DATAFLASH_2);
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(18);
      }

      //Зараз іде стирання, яке займає певний проміжок часу, тому помічаємо, що мікросхема є зайнятою і регістром статусу визначаємо, коли ця операція повністю закінчиться
      dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
      
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  case CODE_OPERATION_WRITE_PAGE_THROUGH_BUFFER:
    {
      if (index_chip == INDEX_DATAFLASH_1)
      {
        if(part_writing_dr_into_dataflash < (NUMBER_PAGES_IN_ONE_DR_RECORD - 1)) part_writing_dr_into_dataflash++;
        else
        {
          //Визначаємо нову адресу наступного запису, нову кількість записів і знімаємо сигналізацію, що зараз іде запис
          unsigned int temp_value_for_address = (info_rejestrator_dr.next_address + SIZE_BUFFER_FOR_DR_RECORD);
          while (temp_value_for_address > MAX_ADDRESS_DR_AREA) temp_value_for_address = temp_value_for_address - SIZE_DR_AREA; 

          //Виставляємо команду запису цієї структури у EEPROM
          /*
          Команду виставляємо скоріше, а потім робимо зміни у полях, які треба змінити,
          бо по вимозі проконтролювати достовірність даних інформації по дискреному
          реєстратору відбувається копіювання з системи захистів структури
          info_rejestrator_adr у резервну копію. Це копіювання блокується у випадку 
          "читання з"/"запису в" EEPROM цієї інформації. Тому виставлення спочатку команди
          запису заблокує копіювання.
          З другої сторони не можливо, щоб почався запис до модифікації, 
          бо запис ініціюється функцією main_routines_for_i2c - яка виконується нижчому
          рівні пріоритетності, що і функція analize_received_data_dataflash.
          Тобто спочатку треба дійти до виклику функції - вийти з цієї функції і дійти 
          до виклику функції main_routines_for_i2c, і аж тоді можливе виконання команди,
          яку ми виставили перед зміною даних, яку ми зараз гарантовано зробимо 
          (до виклику функції main_routines_for_i2c)
          */
          _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_DR_EEPROM_BIT);
          
          info_rejestrator_dr.next_address = temp_value_for_address;
          info_rejestrator_dr.saving_execution = 0;
          if (info_rejestrator_dr.number_records < MAX_NUMBER_RECORDS_INTO_DR) info_rejestrator_dr.number_records += 1;

          //Знімаємо з черги запуск запису дискретного реєстратора
          /*
          Оскільки почати новий запис система захистів, а саме функція routine_for_queue_dr 
          (більш пріоритетна за дану функцію analize_received_data_dataflash,
          яка викликається з системи обслуговування DataFlash) , тільки тоді, коли
          біт за маскою TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR скинуитий
          у командному слові control_tasks_dataflash. 
          Подібна ситуація з очисткою реєстратора програмних подій - команду очистки
          можна тільки подати, коли маска TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR
          скинута, тоді з систекми захистів дискретний реєстратор скинеться.
          Тому цей біт скидаємо після того, як ми зробили модифікації у інформаційній структурі дискреного реєстратора
          */
          control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR);
        }
      }
      else if (index_chip == INDEX_DATAFLASH_2)
      {
        /*
        Завершився запис цілої сторінки для аналогового реєстратора
        Знімаємо з черги запуск запису цідлї сторінки у DataFlash для аналогового реєстратора
        */
        control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR);
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(27);
      }
       
      //Зараз іде процес запису, який займає певний проміжок часу, тому помічаємо, що мікросхема є зайнятою і регістром статусу визначаємо, коли ця операція повністю закінчиться
      dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
      
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  case CODE_OPERATION_READ_HIGH_FREQ:
    {
      //Копіюємо прочитані дані у буфер
      unsigned char *point_buffer;
      unsigned int number_byte_copy_into_target_buffer;
      unsigned int size_page;

      if (index_chip == INDEX_DATAFLASH_1)
      {
        size_page = SIZE_PAGE_DATAFLASH_1;
        unsigned int *point_part_reading;

        if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_MENU)
        {
          point_buffer = (unsigned char *)(buffer_for_manu_read_record + part_reading_dr_from_dataflash_for_menu*size_page);
          number_byte_copy_into_target_buffer = size_page;
          point_part_reading = &part_reading_dr_from_dataflash_for_menu;
        }
        else if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_USB)
        {
          point_buffer = (unsigned char *)(buffer_for_USB_read_record_dr + part_reading_dr_from_dataflash_for_USB*size_page);
          number_byte_copy_into_target_buffer = size_page;
          point_part_reading = &part_reading_dr_from_dataflash_for_USB;
        }
        else if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_RS485)
        {
          point_buffer = (unsigned char *)(buffer_for_RS485_read_record_dr + part_reading_dr_from_dataflash_for_RS485*size_page);
          number_byte_copy_into_target_buffer = size_page;
          point_part_reading = &part_reading_dr_from_dataflash_for_RS485;
        }
        else if (
                 (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU )||
                 (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB  )||
                 (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_RS485)
                )   
        {
          if (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU)
            point_buffer = (unsigned char *)(buffer_for_manu_read_record);
          else if (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB)
            point_buffer = (unsigned char *)(buffer_for_USB_read_record_pr_err);
          else
            point_buffer = (unsigned char *)(buffer_for_RS485_read_record_pr_err);
          
          number_byte_copy_into_target_buffer = SIZE_ONE_RECORD_PR_ERR;
        }
        
        if (
            (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_MENU     ) ||
            (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_USB      ) ||  
            (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_RS485    ) ||  
            (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU ) ||
            (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB  ) ||  
            (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_RS485)  
           )
        {
          //По ідеї ця умова завжди має виконуватися
          for (unsigned int i = 0; i < number_byte_copy_into_target_buffer; i++ )
            *(point_buffer + i) = RxBuffer_SPI_DF[5 + i];
        }
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed(46);
        }

        if (
            (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_MENU ) ||
            (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_USB  ) ||  
            (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_RS485)  
           )
        {
          //Відбувалося зчитування дискретного реєстратора
          if((*point_part_reading) < (NUMBER_PAGES_IN_ONE_DR_RECORD - 1)) (*point_part_reading)++;
          else
          {
            //Знімаємо з черги запуск зчитування запису дискретного реєстратора
            if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_MENU)
            {
              control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU);
              
              /*Подаємо команду на обновлення екрану на LCD, хоч він десь за 
              час <= 1c обновиться автоматично, бо система меню чекає, поки
              буде зчитано запис
              */
              new_state_keyboard |= (1<<BIT_REWRITE);
            }
            else if (what_we_are_reading_from_dataflash_1 == READING_DR_FOR_USB)
              control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB);
            else
              control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485);
          
            *point_part_reading = 0;
          }
        }
        else if (
                 (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU ) ||
                 (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB  ) ||  
                 (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_RS485)  
                )
        {
          //Відбувалося зчитування реєстратора програмних подій
          //Знімаємо з черги запуск зчитування запису реєстратора програмних подій
          if (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_MENU)
          {
            control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU);

            /*Подаємо команду на обновлення екрану на LCD, хоч він десь за 
            час <= 1c обновиться автоматично, бо система меню чекає, поки
            буде зчитано запис
            */
            new_state_keyboard |= (1<<BIT_REWRITE);
          }
          else if (what_we_are_reading_from_dataflash_1 == READING_PR_ERR_FOR_USB)
            control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB);
          else
            control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485);
        }
      }
      else if (index_chip == INDEX_DATAFLASH_2)
      {
        size_page = SIZE_PAGE_DATAFLASH_2;
        
        if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_MENU)
        {
          point_buffer = (unsigned char *)(buffer_for_manu_read_record);
          number_byte_copy_into_target_buffer = sizeof(__HEADER_AR);
        }
        else if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_USB)
        {
          point_buffer = (unsigned char *)(buffer_for_USB_read_record_ar);
          number_byte_copy_into_target_buffer = size_page; /*Фактично може бути і менше, але точно не більше, але щоб не входити у розрахунки копіюю повністю цілий буфер прийому. Тим більше що йде намагання читати стільки часовх зрізів, щоб вони максимально заповники цей об'єм*/
        }
        else if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_RS485)
        {
          point_buffer = (unsigned char *)(buffer_for_RS485_read_record_ar);
          number_byte_copy_into_target_buffer = size_page; /*Фактично може бути і менше, але точно не більше, але щоб не входити у розрахунки копіюю повністю цілий буфер прийому. Тим більше що йде намагання читати стільки часовх зрізів, щоб вони максимально заповники цей об'єм*/
        }

        if (
            (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_MENU ) ||
            (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_USB  ) ||  
            (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_RS485)  
           )
        {
          for (unsigned int i = 0; i < number_byte_copy_into_target_buffer; i++ )
            *(point_buffer + i) = RxBuffer_SPI_DF[5 + i];
        }
        else
        {
          //Теоретично цього ніколи не мало б бути
          total_error_sw_fixed(47);
        }

        //Знімаємо з черги запуск зчитування запису аналогового реєстратора
        if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_MENU)
        {
          control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU);
              
          /*Подаємо команду на обновлення екрану на LCD, хоч він десь за 
          час <= 1c обновиться автоматично, бо система меню чекає, поки
          буде зчитано запис
          */
          new_state_keyboard |= (1<<BIT_REWRITE);
        }
        else if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_USB)
          control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB);
        else if (what_we_are_reading_from_dataflash_2 == READING_AR_FOR_RS485)
          control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485);
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(33);
      }
          
       
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  case CODE_OPERATION_READ_PAGE_INTO_BUFFER:
    {
      //Завершилося подача команди зчитування пам'яті програм у буфер мікросхеми DataFlash
      unsigned int *label_to_etap_writing;
      if (index_chip == INDEX_DATAFLASH_1) label_to_etap_writing = &etap_writing_pr_err_into_dataflash;
      else if (index_chip == INDEX_DATAFLASH_2) label_to_etap_writing = &etap_writing_part_page_ar_into_dataflash;
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(28);
      }
      
      //Переводимо режим запису відповідного реєстратора у запис нових значень
      if (*label_to_etap_writing == ETAP_READ_MEMORY_INTO_BUFFER)
        *label_to_etap_writing = ETAP_HAVE_READ_MEMORY_INTO_BUFFER;
      else 
        *label_to_etap_writing = ETAP_ERROR_BEFALLEN;
      
      //Зараз іде процес зчитування у буфер, який займає певний проміжок часу, тому помічаємо, що мікросхема є зайнятою і регістром статусу визначаємо, коли ця операція повністю закінчиться
      dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
      
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  case CODE_OPERATION_WRITE_BUFFER:
    {
      //Завершилося передача даних у буфер мікросхеми DataFlash
      unsigned int *label_to_etap_writing;
      if (index_chip == INDEX_DATAFLASH_1) label_to_etap_writing = &etap_writing_pr_err_into_dataflash;
      else if (index_chip == INDEX_DATAFLASH_2) label_to_etap_writing = &etap_writing_part_page_ar_into_dataflash;
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(29);
      }
      
      //Переводимо режим запису у запершення модифікації фуферу DataFlah
      if (*label_to_etap_writing == ETAP_MODIFY_AND_WRITE_BUFFER)
        *label_to_etap_writing = ETAP_MODIFIED_AND_WRITTEN_BUFFER;
      else 
        *label_to_etap_writing = ETAP_ERROR_BEFALLEN;
      
      //Помічаємо, що мікросхема є зайнятою і регістром статусу визначаємо, коли ця операція повністю закінчиться
      dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
      
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  case CODE_OPERATION_WRITE_BUFFER_INTO_MEMORY_WITH_ERASE:
    {
      if (index_chip == INDEX_DATAFLASH_1)
      {
        if (etap_writing_pr_err_into_dataflash == ETAP_WRITE_BUFFER_INTO_MEMORY)
        {
          //Виставляємо команду запису структури реєстратора програмних подій у EEPROM
          _SET_BIT(control_i2c_taskes, TASK_START_WRITE_INFO_REJESTRATOR_PR_ERR_EEPROM_BIT);
          //Визначаємо нову адресу наступного запису, нову кількість записів і знімаємо сигналізацію, що зараз іде запис
          unsigned int temp_value_for_address = (info_rejestrator_pr_err.next_address + number_recods_writing_into_dataflash_now*SIZE_ONE_RECORD_PR_ERR);
          while (temp_value_for_address > MAX_ADDRESS_PR_ERR_AREA) temp_value_for_address = temp_value_for_address - SIZE_PR_ERR_AREA; 
          info_rejestrator_pr_err.next_address = temp_value_for_address;
          info_rejestrator_pr_err.saving_execution = 0;
          if ((info_rejestrator_pr_err.number_records + number_recods_writing_into_dataflash_now) <= MAX_NUMBER_RECORDS_INTO_PR_ERR) info_rejestrator_pr_err.number_records += number_recods_writing_into_dataflash_now;
          else info_rejestrator_pr_err.number_records = MAX_NUMBER_RECORDS_INTO_PR_ERR;
        
          //Змінюємо індекси буферу типу FIFO 
          tail_fifo_buffer_pr_err_records += number_recods_writing_into_dataflash_now;
          while(tail_fifo_buffer_pr_err_records >= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER) tail_fifo_buffer_pr_err_records -= MAX_NUMBER_RECORDS_PR_ERR_INTO_BUFFER;
          number_recods_writing_into_dataflash_now = 0;
        
          if (_CHECK_SET_BIT(diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT) != 0)
          {
            //Треба спробувати зняти повідомлення про переповнення буферу FIFO програмних подій
            _SET_BIT(clear_diagnostyka, ERROR_PR_ERR_OVERLOAD_BIT);
          }

          //Знімаємо з черги запуск запису у реєстратор програмних подій
          etap_writing_pr_err_into_dataflash = ETAP_NONE;
          control_tasks_dataflash &= (unsigned int)(~TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH);
        }
        else 
          etap_writing_pr_err_into_dataflash = ETAP_ERROR_BEFALLEN;
      }
      else if (index_chip == INDEX_DATAFLASH_2)
      {
        if (etap_writing_part_page_ar_into_dataflash == ETAP_WRITE_BUFFER_INTO_MEMORY)
        {
          /*
          Очищаємо кількість байт для запису у змінній count_to_save
          Коли ми знаходимося у цьому місці, то дані вже точно записані у память DataFlash
          і терерішнє вмістиме масиву buffer_for_save_ar_record вже точно не потрібне.
          Хоч фактично воно вже не було потрібне з моменту, коли ми це вмістиме записали у внутрішній
          буфер мікросхеми DataFlash.
          Але через мехенізм стаховки від програмної помилки ми дозволяємо запонювати масив
          buffer_for_save_ar_record новими значеннями тільки тут.

          Тому виставляєм будь-яким ненульоаим значенням у змінній permit_copy_new_data дозвіл на підготовку нових даних
          Перед цим визначаємо наступну адресу для запису
          */
          temporary_address_ar += count_to_save;
          count_to_save = 0;
          permit_copy_new_data = 0xff;
           
          //Знімаємо з черги запуск запису неповної сторінки для аналогового реєстратора
          etap_writing_part_page_ar_into_dataflash = ETAP_NONE;
          control_tasks_dataflash &= (unsigned int)(~TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR);
        }
        else 
          etap_writing_part_page_ar_into_dataflash = ETAP_ERROR_BEFALLEN;
      }
      else
      {
        //Відбулася невизначена помилка, тому треба піти на перезавантаження
        total_error_sw_fixed(30);
      }
       
      //Зараз іде процес запису, який займає певний проміжок часу, тому помічаємо, що мікросхема є зайнятою і регістром статусу визначаємо, коли ця операція повністю закінчиться
      dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
      
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  default:
    {
      //По ідеї, сюди програма ніколи не мала б попадати. 
      //Якщо ми сюди попали, то треба виставити повідомлення, що не знаєм чи мікросхема є вільною і скидаємо всі опреації
      dataflash_not_busy &= (unsigned int)(~(1 << index_chip));
      driver_spi_df[index_chip].state_execution = TRANSACTION_EXECUTING_NONE;
      driver_spi_df[index_chip].code_operation = CODE_OPERATION_NONE;
      break;
    }
  }
}
/*****************************************************/

/*****************************************************/
//Основна функція обробки даних по управлінню DataFlash (обробка прийнятих даних)
/*****************************************************/
void main_function_for_dataflash_resp(int index_chip)
{
  if ((index_chip >= INDEX_DATAFLASH_1) && (index_chip <= INDEX_DATAFLASH_2))
  {
    if(driver_spi_df[index_chip].state_execution == TRANSACTION_EXECUTED_WAIT_ANALIZE)
    {
      //Виконуємо функцію обробки даних по завершення трансакції
      analize_received_data_dataflash(index_chip);
    }
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(19);
  }
}
/*****************************************************/

/*****************************************************/
//Основна функція обробки даних по управлінню DataFlash (подача нової команди даних)
/*****************************************************/
void main_function_for_dataflash_req(int index_chip)
{
  if ((index_chip >= INDEX_DATAFLASH_1) && (index_chip <= INDEX_DATAFLASH_2))
  {
    if (driver_spi_df[index_chip].state_execution == TRANSACTION_EXECUTING_NONE)
    {
      //Якщо зараз не запущено ніякрої операції, то можна при потребі нову трансакцію
      if((dataflash_not_busy & (1 << index_chip)) == 0)
      {
        //Попередній раз прийшло повідомлення про те, що мікросхема є занятою, тому треба повторно зчитувати статус поки вона не зтане незанятою
        dataflash_status_read(index_chip);
      }
      else
      {
        //мікросхема зараз є вільною. тому можна запускати при необхідності нові трансакції
        unsigned int tasks_register;
        if (index_chip == INDEX_DATAFLASH_1) tasks_register = control_tasks_dataflash & 0xffff;
        else tasks_register = control_tasks_dataflash & 0xffff0000;
        
        if (tasks_register !=0)
        {
          //Зараз стоять у черзці операції, які треба виконати
          if (
              ((tasks_register & TASK_ERASE_DATAFLASH_1) !=0) ||
              ((tasks_register & TASK_ERASE_DATAFLASH_2) !=0)
             )
          {
            //Треба виконати команду повного стирання мікросхеми
            dataflash_erase(index_chip);
          }
          else if ((tasks_register & TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH) !=0 )
          {
            //Треба виконати команди: зчитування у буфер, зміна буферу, запис буферу зпопереднім його стиранням для реєстратора програмних подій
            if (etap_writing_pr_err_into_dataflash > ETAP_ERROR_BEFALLEN)
              etap_writing_pr_err_into_dataflash = ETAP_ERROR_BEFALLEN;
            
            switch (etap_writing_pr_err_into_dataflash)
            {
            case ETAP_NONE:
              {
                etap_writing_pr_err_into_dataflash = ETAP_READ_MEMORY_INTO_BUFFER;
                dataflash_mamory_page_into_buffer(index_chip);
                break;
              }
            case ETAP_HAVE_READ_MEMORY_INTO_BUFFER:
              {
                //Переходимо у режим запису даних у буфер
                etap_writing_pr_err_into_dataflash = ETAP_MODIFY_AND_WRITE_BUFFER;
                //Треба виконати команду запису даних реєстратора програмних подій у буфер мыкросхеми
                dataflash_mamory_write_buffer(index_chip);
                break;
              }
            case ETAP_MODIFIED_AND_WRITTEN_BUFFER:
              {
                //Переходимо у режим запису даних з буферу у пам'ять мікросхеми DataFlash
                etap_writing_pr_err_into_dataflash = ETAP_WRITE_BUFFER_INTO_MEMORY;
                //Треба виконати команду запису даних реєстратора програмних подій у буфер мыкросхеми
                dataflash_mamory_buffer_into_memory(index_chip);
                break;
              }
            default:
              {
                /*
                Сюди може зайти програма, якщо якась виникла помилка, 
                тоді знімаємо режим запису реєстратора програмних помилок і переводимо все у висхідний стан
                запис має початися з наступного разу  
                */
                etap_writing_pr_err_into_dataflash = ETAP_NONE;
                //Знімаємо з черги запуск запису у реєстратор програмних подій
                control_tasks_dataflash &= (unsigned int)(~TASK_WRITE_PR_ERR_RECORDS_INTO_DATAFLASH);
                
                break;
              }
            }   
          }   
          else if ((tasks_register & TASK_MAMORY_PART_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR) !=0 )
          {
            //Треба виконати команди: зчитування у буфер, зміна буферу, запис буферу зпопереднім його стиранням для аналогового реєстратора
            if (etap_writing_part_page_ar_into_dataflash > ETAP_ERROR_BEFALLEN)
              etap_writing_part_page_ar_into_dataflash = ETAP_ERROR_BEFALLEN;
            
            switch (etap_writing_part_page_ar_into_dataflash)
            {
            case ETAP_NONE:
              {
                etap_writing_part_page_ar_into_dataflash = ETAP_READ_MEMORY_INTO_BUFFER;
                dataflash_mamory_page_into_buffer(index_chip);
                break;
              }
            case ETAP_HAVE_READ_MEMORY_INTO_BUFFER:
              {
                //Переходимо у режим запису даних у буфер
                etap_writing_part_page_ar_into_dataflash = ETAP_MODIFY_AND_WRITE_BUFFER;
                //Треба виконати команду запису даних реєстратора програмних подій у буфер мыкросхеми
                dataflash_mamory_write_buffer(index_chip);
                break;
              }
            case ETAP_MODIFIED_AND_WRITTEN_BUFFER:
              {
                //Переходимо у режим запису даних з буферу у пам'ять мікросхеми DataFlash
                etap_writing_part_page_ar_into_dataflash = ETAP_WRITE_BUFFER_INTO_MEMORY;
                //Треба виконати команду запису даних реєстратора програмних подій у буфер мыкросхеми
                dataflash_mamory_buffer_into_memory(index_chip);
                break;
              }
            default:
              {
                /*
                Сюди може зайти програма, якщо якась виникла помилка, 
                тоді перезапускаємо цю операцію з початку  
                */
                etap_writing_part_page_ar_into_dataflash = ETAP_NONE;
                break;
              }
            }   
          }   
          else if (
                   ((tasks_register & TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_DR) !=0) ||
                   ((tasks_register & TASK_MAMORY_PAGE_PROGRAM_THROUGH_BUFFER_DATAFLASH_FOR_AR) !=0)
                  )
          {
            //Треба виконати команду запису даних у сторінку через буфер з попереднім стиранням
            dataflash_mamory_page_program_through_buffer(index_chip);
          }
          else if (
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU     ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB      ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485    ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU     ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB      ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485    ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB  ) !=0 ) ||
                   ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) !=0 )
                  )   
          {
            
            //Може виконуватися тільки одна операція для одної мікросхеми DataFlash(причому більi пріоритетна може переривати менш пріоритетну, якщо задача виконується у декілька етапів)
            //DataFlash1
            if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_DR_USB) !=0 )
              what_we_are_reading_from_dataflash_1 = READING_DR_FOR_USB; //Читання для USB завжди має більший пріоритет порівняно з читанням для RS-485 і меню
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_USB) !=0 )
              what_we_are_reading_from_dataflash_1 = READING_PR_ERR_FOR_USB;//Читання для USB завжди має більший пріоритет порівняно з читанням для RS-485 і меню
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_DR_RS485) !=0 )
              what_we_are_reading_from_dataflash_1 = READING_DR_FOR_RS485; //Читання для RS-485 завжди має більший пріоритет порівняно з читанням для меню
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_RS485) !=0 )
              what_we_are_reading_from_dataflash_1 = READING_PR_ERR_FOR_RS485;//Читання для RS-485 завжди має більший пріоритет порівняно з читанням для меню
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_DR_MENU) !=0 )
              what_we_are_reading_from_dataflash_1 = READING_DR_FOR_MENU;
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_PR_ERR_MENU) !=0 )
              what_we_are_reading_from_dataflash_1 = READING_PR_ERR_FOR_MENU;
            
            //DataFlash2
            if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_AR_USB) !=0 )
              what_we_are_reading_from_dataflash_2 = READING_AR_FOR_USB; //Читання для USB завжди має більший пріоритет порівняно з читанням для RS-485 і меню
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_AR_RS485) !=0 )
              what_we_are_reading_from_dataflash_2 = READING_AR_FOR_RS485; //Читання для RS485 завжди має більший пріоритет порівняно з читанням для меню
            else if ((tasks_register & TASK_MAMORY_READ_DATAFLASH_FOR_AR_MENU) !=0 )
              what_we_are_reading_from_dataflash_2 = READING_AR_FOR_MENU;
            //Треба виконати команду читання даних
            dataflash_mamory_read(index_chip);
          }
        }
      }
    }
  }
  else
  {
    //Відбулася невизначена помилка, тому треба піти на перезавантаження
    total_error_sw_fixed(24);
  }
}
/*****************************************************/

/*****************************************************/
//Дії при зміні витримок для налалогового реєстратора
/*****************************************************/
void actions_after_changing_tiomouts_ar(void)
{
  //Переводимо структуру інформації про записи аналогового реєстратора у початковий (обнулений стан)
  
  //Підраховуємо розмір одного запису і максимальну кількість записів у аналоговому реєстраторі для даних витримок
  calc_size_and_max_number_records_ar(current_settings.prefault_number_periods, current_settings.postfault_number_periods);
  
  //Помічаємо, що треба очистити аналоговий реєстратор
  clean_rejestrators |= CLEAN_AR;

  //Помічаємо, що номер запису не вибраний
  number_record_of_ar_for_menu = 0xffff;
  number_record_of_ar_for_USB = 0xffff;
  number_record_of_ar_for_RS485 = 0xffff;
}
/*****************************************************/

/*****************************************************/
//Визначення розміру одного запису і максимальної кількісті записів у аналоговому реєстраторі для даних витримок
/*****************************************************/
void calc_size_and_max_number_records_ar(unsigned int prefault_number_periods, unsigned int postfault_number_periods)
{
  size_one_ar_record = sizeof(__HEADER_AR) + ((prefault_number_periods + postfault_number_periods) << VAGA_NUMBER_POINT_AR)*(NUMBER_ANALOG_CANALES + number_word_digital_part_ar)*sizeof(short int);
  max_number_records_ar = (NUMBER_PAGES_INTO_DATAFLASH_2 << VAGA_SIZE_PAGE_DATAFLASH_2) / size_one_ar_record;
}
/*****************************************************/

/*****************************************************/
//Формування буферу (масиву) для запису у у сторінку мікросхеми DataFlash частини аналогового реєстратора
/*****************************************************/
unsigned int making_buffer_for_save_ar_record(unsigned int *point_unsaved_bytes_of_header_ar)
{
  unsigned int error = 0; //По замовчуванні ставимо, що помилок не зафіксовано .бо якщо у мене все правильно працює, то взагалі б помилок ця функція не малаб фіксувати

  //Визначаємо скільки можна з текучої адреси ще записати у текучу сторінку DataFlash
  unsigned int free_space_in_page_df = SIZE_PAGE_DATAFLASH_2 - (temporary_address_ar & 0x1ff);
  
  if (*point_unsaved_bytes_of_header_ar > 0)
  {
    //Спочатку треба перекопіювати заголовок аналогового реєстратора, якщо він ще не перекопійоманий
    if (*point_unsaved_bytes_of_header_ar <= sizeof(__HEADER_AR))
    {
      unsigned char  *point = (unsigned char*)(&header_ar);
      point += (sizeof(__HEADER_AR) - *point_unsaved_bytes_of_header_ar);
      
      while (
             ((count_to_save + 1) <= free_space_in_page_df) &&
             (*point_unsaved_bytes_of_header_ar > 0)  
            )
      {
        //Додаємо по одному байту, поки є вільне місце для заповнення
        buffer_for_save_ar_record[count_to_save++] = *point;
        point++;
        *point_unsaved_bytes_of_header_ar -= 1;
      }
    }
    else
    {
      //Теоретично сюди програма ніколи не не мала б заходити
      error = 1;
    }
  }
  
  if (
      (state_ar_record == STATE_AR_SAVE_SRAM_AND_SAVE_FLASH) ||
      (state_ar_record == STATE_AR_ONLY_SAVE_FLASH         )
     )  
  {
    //Копіювєм миттєві виборки, якщо є такі і якщо є вільне місце для копіювання
    unsigned int index_array_ar_heat_tmp = index_array_ar_heat;/*це робим для того, бо компілятор видає завуваження при порівнянні змінних з префіксом volatile*/
    unsigned int index_array_ar_tail_tmp = index_array_ar_tail;/*це робим для того, бо компілятор видає завуваження при порівнянні змінних з префіксом volatile*/
    while (
           ((count_to_save + 2) <= free_space_in_page_df)           /*Є вільне місце у записуваній сторінці на два байти миттєвого значення*/
           &&  
           (
            (index_array_ar_tail_tmp != index_array_ar_heat_tmp) || /* "хвіст" буферу FIFO не "догнав" ще "голову"*/
            (state_ar_record == STATE_AR_ONLY_SAVE_FLASH)           /*іде завершення формування всіх записів*/ 
           )
           &&  
           (copied_number_samples < total_number_samples)           /*ще не було скопійовано вся кількість миттєвих виборок, яка має бути у одному записі аналогового реєстратора при вибраній ширині доаварійного і післяаварійного масиву*/
          )
    {
    
      //Додаємо по два байти (миттєва виборка складається з двох байт), поки є вільне місце для заповнення
      unsigned int sample;
      if (index_array_ar_tail_tmp != index_array_ar_heat_tmp)
      {
        /*прицюємо з справжньою змінною index_array_ar_tail, а не з її комією index_array_ar_tail_tmp, бо тут компілятор не видає зауваження*/
        sample = array_ar[index_array_ar_tail++];
        if (index_array_ar_tail >= SIZE_BUFFER_FOR_AR) index_array_ar_tail = 0; /*Умова мал аб бути ==, але щоб перестахуватися на невизначену помилку я поставив >=*/
        index_array_ar_heat_tmp = index_array_ar_heat;/*це робим для того, бо компілятор видає завуваження при порівнянні змінних з префіксом volatile*/
        index_array_ar_tail_tmp = index_array_ar_tail;/*це робим для того, бо компілятор видає завуваження при порівнянні змінних з префіксом volatile*/
      }
      else
      {
        /*
        Сюди програма може зайти тільки у кінці процесу формування даних для запису, 
        (коли змінна state_ar_record рівна STATE_AR_ONLY_SAVE_FLASH) і коли попередньо
        відбулося переповнення, тобто частина даних є втраченою, тому недостаючі миттєві
        виборки заповнюємо нулями у кінці запису
        */
        sample = 0;
      }

      //Копіюємо у Little Endian mode (молодшим байтом вперед)
      buffer_for_save_ar_record[count_to_save++] = sample        & 0xff;
      buffer_for_save_ar_record[count_to_save++] = (sample >> 8) & 0xff;
      copied_number_samples++; //Збільшуємо кількість "забраних" миттєвих значень
    }
  
    if (
        (count_to_save > free_space_in_page_df) ||
        (copied_number_samples > total_number_samples)  
       )
    {
      //Теоретично сюди програма ніколи не не мала б заходити
      error = 2;
    }
    else if (
             (
              (*point_unsaved_bytes_of_header_ar > 0      ) &&
              ((count_to_save + 1) > free_space_in_page_df)  
             )
             ||
             (
              (*point_unsaved_bytes_of_header_ar == 0     ) &&
              ((count_to_save + 2) > free_space_in_page_df)  
             )
             ||
             (copied_number_samples == total_number_samples)
            )   
    {
      //Умова зупинуи подальшого копіювання нових даних і умова подати сигнал на запис у DataFlash
      permit_copy_new_data = 0;
    }
  }
  
  return error;
}
/*****************************************************/

/*****************************************************/
//
/*****************************************************/
/*****************************************************/
