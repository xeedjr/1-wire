#include <stdio.h>

#include "adapters.h"

TAdapters::type_set(char type)
{
    adapter_type = type;
}

TAdapters::set_port(char port_in)
{
    port = port_in;
}

TAdapters::init()
{
     switch(adapter_type)
     {
     case ADAPTER_DS9097U:
        char port_s[10];

        sprintf(port_s, "%d", port);
        if (Com.Open(port_s, BaudRate_9600, Data8Bit, NonParity, OneStopBit) != true)
        {
            //error

        };
        break;
     default:
        break;   
     }
}

TAdapters::close()
{
     switch(adapter_type)
     {
     case ADAPTER_DS9097U:
        Com.Close();
        break;
     default:
        break;   
     }
}
