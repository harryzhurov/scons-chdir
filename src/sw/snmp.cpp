
//------------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <ap_int.h>

#include "snmp.h"

using namespace std;

//------------------------------------------------------------------------------
void snmp(hls::stream<data_t> &din, hls::stream<data_t> &dout)
{
    const uint8_t oid_prefix[] = { 0x06, 0x0c, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82, 0xe5, 0x2d };
    
    const uint32_t mmr[] =
    {
        0x1234,
        0x2345,
        0x3456,
        0x4567
    };
    
    uint8_t buf[BUF_SIZE];
    
    data_t item;
    size_t i = 0;
    do
    {
        item = din.read();
        buf[i++] = item.data;
    }
    while(!item.last);
    
    size_t size = item.user;
    
    uint8_t *pkt = buf + SNMP_OFFSET;
    int rid_len  = pkt[RID_LEN_OFFSET] - 1;
    cout << "snmp> begin parsing" << endl;
    // check PDU Type
    if(pkt[PDU_TYPE_OFFSET] != pduGET_REQUEST && pkt[PDU_TYPE_OFFSET] != pduSET_REQUEST)
    {
        cout << "snmp> E: invalid PDU type '" << static_cast<int>(pkt[PDU_TYPE_OFFSET]) << "'" << endl;
        dump(pkt, size);
        output_message(dout, buf, invalid_response(pkt, DATA_OFFSET+rid_len, size));
        return;
        
    }
    
    // check Request ID
    if(pkt[RID_TYPE_OFFSET] != 0x02)
    {
        cout << "snmp> pkt[RID_TYPE_OFFSET]: " << static_cast<int>(pkt[RID_TYPE_OFFSET]) << endl;
        output_message(dout, buf, invalid_response(pkt, DATA_OFFSET+rid_len, size));
        return;
    }
    
    // check OID prefix
    cout << "snmp> rid_len: " << static_cast<int>(rid_len) << endl;
    cout << "snmp> check oid" << endl;
    for(size_t i = 0; i < sizeof(oid_prefix); ++i)
    {
        size_t idx = OID_OFFSET + rid_len + i;
        
        if(pkt[OID_OFFSET+rid_len+i] != oid_prefix[i])
        {
            cout << "----------------------------------------------" << endl;
            for(size_t j = 0; j < 10; ++j)
            {
                size_t idx = OID_OFFSET + rid_len + j;
                cout << dec << setfill('0') << setw(2) << idx << " " << hex << setfill('0') << setw(2) << static_cast<int>(pkt[idx]) << endl;
            }

            cout << "snmp> pkt[OID_OFFSET+rid_len+i]: " << static_cast<int>(pkt[OID_OFFSET+rid_len+i]) << endl;
            output_message(dout, buf, invalid_response(pkt, DATA_OFFSET+rid_len, size));
            return;
        }
    }
    
    // MMR access
    cout << "snmp> retrieve address" << endl;
    uint8_t p0    = pkt[ADDR_PAR_OFFSET + rid_len + 0];
    uint8_t p1    = pkt[ADDR_PAR_OFFSET + rid_len + 1];
    uint8_t p2    = pkt[ADDR_PAR_OFFSET + rid_len + 2];
    uint8_t p3    = pkt[ADDR_PAR_OFFSET + rid_len + 3];
    uint32_t addr = (p0 << 22) + (p1 << 16) + (p2 << 8) + p3;
    if(addr > sizeof(mmr)/sizeof(mmr[0])-1)
    {
        cout << "snmp> E: address exceeds MMR space" << endl;
        output_message(dout, buf, invalid_response(pkt, DATA_OFFSET+rid_len, size));
        return;
    }
    uint32_t data = mmr[addr];
    
    // write data to SNMP message
    cout << "snmp> put data" << endl;
    pkt[DATA_OFFSET + rid_len + 0] = 0x02; 
    pkt[DATA_OFFSET + rid_len + 1] = 0x04; 
    pkt[DATA_OFFSET + rid_len + 2] = data >> 24;
    pkt[DATA_OFFSET + rid_len + 3] = data >> 16; 
    pkt[DATA_OFFSET + rid_len + 4] = data >> 8; 
    pkt[DATA_OFFSET + rid_len + 5] = data; 

    // update outcoming message
    cout << "snmp> fix len fields" << endl;
    pkt[MSG_LEN_OFFSET]            += 4;
    pkt[PDU_TYPE_OFFSET]            = pduGET_RESPONSE;
    pkt[PDU_LEN_OFFSET]            += 4;
    pkt[VBLIST_LEN_OFFSET+rid_len] += 4;
    pkt[VBIND_LEN_OFFSET+rid_len]  += 4;
    
    // move data out
    size += 4;
        
    output_message(dout, buf, size);
}
//------------------------------------------------------------------------------

