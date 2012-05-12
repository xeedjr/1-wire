// ����� ��� ������ � ��������� �� RS-232 � ����������� ������

// ������������ ������ ��������
//      5 - GND - �����
//		3 - TxD - ��������
//		2 - RxD - �����


#include <windows.h>

// ���������� ��������� ������-�������� ������
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

// ����� �������������� ��� � ������������ - ����������� �������
#define    Data5Bit                     5
#define    Data6Bit                     6
#define    Data7Bit                     7
#define    Data8Bit                     8

// ��������� ��������
#define    NonParity                    0
#define    OddParity                    1
#define    EvenParity                   2
#define    MarkParity                   3
#define    SpaceParity                  4

// ���������� �������� �����
#define    OneStopBit                   0
#define    One5StopBit                  1
#define    TwoStopBit                   2

// ������ ������� ������ - ��������
#define    SizeBuffer 			204800

// ��������� ����-����� � �������������
#define    TimeoutInterval    10
              // ������������ �������� ����� ����� ���������
#define    TimeoutChar        1
             // ������������ ����� �� �������� ������ �������
#define    TimeOutWait        100
             // ������������ ����� �������� �������



class MSerial
{
   public:

      boolean Open(char port[], DWORD BaudRate, BYTE ByteSize,
                                                BYTE Parity, BYTE StopBits);
      // ���������� TRUE ���� ���� �������� ���������

      void Close(void);

      void ResetRB(void); // ������� ������ ������
      void ResetWB(void); // ������� ������ ��������

      unsigned short Read(BYTE *buffer, unsigned short numbytes);
      // ���������� ���������� ��������� ����
      unsigned short Write(BYTE *buffer, unsigned short numbytes);
      // ���������� ���������� ���������� ����

      unsigned short MSerial::GetRBSize(void);
      // ���������� ����� �������� �������� � ������ ������
      // (��� �� ��������� �������� Read)
      unsigned short GetWBSize(void);
      // ���������� ����� ��� �� ���������� �������� � ������ ��������


   private:

      HANDLE           handle;
      COMMTIMEOUTS     CommTimeOuts;
      DCB              dcb;
      COMSTAT          ComState;
      OVERLAPPED       Overlap;


};

