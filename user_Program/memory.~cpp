#include <string.h>

#include "memory.h"
#include "adapters.h"

TMem memory;

TMem::add_adapter(char type, char *name, int comport, int port, char *ip)
{
    switch(type)
    {
    case ADAPTER_DS9097U:
        mem.adapters[mem.adapters_num].type = ADAPTER_DS9097U;
        strcpy(mem.adapters[mem.adapters_num].name, name);
        mem.adapters[mem.adapters_num].comport = comport;
        mem.adapters_num++;
        break;
    }
}
 