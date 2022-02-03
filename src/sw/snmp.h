
#ifndef SNMP_H
#define SNMP_H

#include <ap_axi_sdata.h>
#include <hls_stream.h>


const size_t MIN_ETH_PACKET_SIZE = 40;
const size_t BUF_SIZE            = 2048;

const size_t SNMP_OFFSET       = 0x2a;
const size_t MSG_LEN_OFFSET    = 1;
const size_t PDU_TYPE_OFFSET   = 13;
const size_t PDU_LEN_OFFSET    = 14;
const size_t RID_TYPE_OFFSET   = 15;
const size_t RID_LEN_OFFSET    = 16;
const size_t VBLIST_LEN_OFFSET = 25;
const size_t VBIND_LEN_OFFSET  = 27;
const size_t OID_OFFSET        = 28;
const size_t ADDR_PAR_OFFSET   = 38;
const size_t DATA_OFFSET       = 42;

enum PDU_TYPE
{
    pduGET_REQUEST  = 0xa0,
    pduGET_RESPONSE = 0xa2,
    pduSET_REQUEST  = 0xa3
};

typedef hls::axis<uint8_t, 8, 0, 0> data_t;

void   dump(uint8_t *p, size_t size);
void   snmp(hls::stream<data_t>& dinp, hls::stream<data_t>& dout);
size_t invalid_response(uint8_t *pkt, size_t offset, size_t size);
void   output_message(hls::stream<data_t>& dout, uint8_t *buf, size_t size);

#endif // SNMP_H

