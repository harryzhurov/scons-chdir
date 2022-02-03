
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>

#include "snmp.h"

using namespace std;

//------------------------------------------------------------------------------
int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(sockfd < 0)
    {
        cout << "sock> E: cannot create socket" << sockfd << endl;
        cout << strerror(errno) << endl;
        return -1;
    }
    
    cout << "sock> sockfd: " << static_cast<int>(sockfd) << endl;
    struct sockaddr_in saddr;
    struct sockaddr_in caddr;
    
    saddr.sin_family      = AF_INET;
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port        = htons(161);
    
    
    cout << "sock> try to bind socket... ";
    if( !bind(sockfd, reinterpret_cast<const struct sockaddr*>(&saddr), sizeof(saddr)) < 0 )
    {
        cout << "sock> E: cannot bind socket" << endl;
        return -2;
    }
    
    cout << "success" << endl;

    
    socklen_t addrlen = sizeof(caddr);
    
    for(;;)
    {
        int size;
        uint8_t buf[BUF_SIZE];
        memset(buf, 0, BUF_SIZE);
        cout << "----------------------" << endl << "sock> begin receiving" << endl;
        do
        {

            size = recvfrom(sockfd, buf+SNMP_OFFSET, BUF_SIZE, 0, reinterpret_cast<struct sockaddr*>(&caddr), &addrlen);
            cout << "sock> size: " << size << endl;
        }
        while(size > 0 && size < MIN_ETH_PACKET_SIZE);

        cout << "sock> " << caddr.sin_addr.s_addr << " " << caddr.sin_port << endl;

        hls::stream<data_t> din;
        hls::stream<data_t> dout;
        
        size_t fullsize = size + SNMP_OFFSET;
        for(size_t i = 0; i < fullsize; ++i)
        {
            data_t item;

            item.data = buf[i];
            item.user = fullsize;
            item.keep = 1;
            item.last = (i == fullsize-1) ? 1 : 0;
            din.write(item);
        }

        snmp(din, dout);
        
        data_t item;
        size_t i = 0;
        do
        {
            item = dout.read();
            buf[i++] = item.data;
        }
        while(!item.last);
        
        size_t osize = item.user;

        cout << "sock> send packet out" << endl;
        sendto(sockfd, buf+SNMP_OFFSET, osize-SNMP_OFFSET, 0, reinterpret_cast<struct sockaddr *>(&caddr), addrlen);
    }
    
    return 0;
}
//------------------------------------------------------------------------------

