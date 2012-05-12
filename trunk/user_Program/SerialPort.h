// класс дл€ работы с компортом по RS-232 в асинхронном режиме

// используемые выводы компорта
//      5 - GND - общий
//		3 - TxD - передача
//		2 - RxD - прием


#include <windows.h>

// констатнты скоростей приема-передачи данных
#define BaudRate_110  				CBR_110
#define BaudRate_300 				CBR_300
#define BaudRate_600  				CBR_600
#define BaudRate_1200 	          	CBR_1200
#define BaudRate_2400           	CBR_2400
#define BaudRate_4800           	CBR_4800
#define BaudRate_9600           	CBR_9600
#define BaudRate_14400          	CBR_14400
#define BaudRate_19200          	CBR_19200
#define BaudRate_38400          	CBR_38400
#define BaudRate_56000          	CBR_56000
#define BaudRate_57600          	CBR_57600
#define BaudRate_115200 			CBR_115200
#define BaudRate_128000         	CBR_128000
#define BaudRate_256000 	        CBR_256000

// число информационных бит в передаваемом - принимаемом символе
#define    Data5Bit                     5
#define    Data6Bit                     6
#define    Data7Bit                     7
#define    Data8Bit                     8

// константы четности
#define    NonParity                    0
#define    OddParity                    1
#define    EvenParity                   2
#define    MarkParity                   3
#define    SpaceParity                  4

// количество стоповых битов
#define    OneStopBit                   0
#define    One5StopBit                  1
#define    TwoStopBit                   2

// размер буферов приема - передачи
#define    SizeBuffer 			204800

//  онстанты тайм-аутов в миллисекундах
#define    TimeoutInterval    10
              // максимальный интервал между двум€ символами
#define    TimeoutChar        1
             // максимальное врем€ на прин€тие одного символа
#define    TimeOutWait        100
             // максимальное врем€ ожидани€ посылки



class MSerial
{
   public:

      boolean Open(char port[], DWORD BaudRate, BYTE ByteSize,
                                                BYTE Parity, BYTE StopBits);
      // возвращает TRUE если порт открылс€ нормально

      void Close(void);

      void ResetRB(void); // очистка буфера приема
      void ResetWB(void); // очистка буфера передачи

      unsigned short Read(BYTE *buffer, unsigned short numbytes);
      // возвращает количество считанных байт
      unsigned short Write(BYTE *buffer, unsigned short numbytes);
      // возвращает количество переданных байт

      unsigned short MSerial::GetRBSize(void);
      // возвращает число прин€тых символов в буфере приема
      // (еще не считанных функцией Read)
      unsigned short GetWBSize(void);
      // возвращает число еще не переданных символов в буфере передачи


   private:

      HANDLE           handle;
      COMMTIMEOUTS     CommTimeOuts;
      DCB              dcb;
      COMSTAT          ComState;
      OVERLAPPED       Overlap;


};

