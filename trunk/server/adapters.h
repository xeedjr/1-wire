#ifndef ADAPTERS_H
#define ADAPTERS_H

//#include "SerialPort.h"

#define ADAPTER_DS9097U 1

class TAdapters {
    char    adapter_type;
    char    port;
    //MSerial Com;
public:
    type_set(char type);
    set_port(char port_in);
    init();
    close();

};

#endif