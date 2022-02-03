

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "snmp.h"

using namespace std;

//------------------------------------------------------------------------------

void dump(uint8_t *p, size_t size)
{
    cout << endl << endl;
    for(size_t i = 0; i < size; ++i)
    {
        cout << hex << setfill('0') << setw(2) << static_cast<int>(p[i]) << " ";
        if( (i+1)%8 == 0)
        {
            cout << " ";
        }
        if( (i+1)%16 == 0)
        {
            cout << endl;
        }
    }
    cout << endl << endl;
}

//------------------------------------------------------------------------------
size_t invalid_response(uint8_t *pkt, size_t offset, size_t size)
{
    pkt[offset]     = 0x80;
    pkt[offset+1]   = 0x00;
    pkt[PDU_TYPE_OFFSET] = pduGET_RESPONSE;

    return size;
}
//------------------------------------------------------------------------------
void output_message(hls::stream<data_t> &dout, uint8_t *buf, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        data_t item;

        item.data = buf[i];
        item.user =  size;
        item.keep = 1;
        item.last = (i == size-1) ? 1 : 0;
        dout.write(item);
    }
}
//------------------------------------------------------------------------------

