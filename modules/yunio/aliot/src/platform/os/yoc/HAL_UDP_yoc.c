/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iot_import.h>
#include "iot_import_coap.h"
#include <aos/log.h>

#define TAG "IOT_HAL"

intptr_t HAL_UDP_create_without_connect(_IN_ const char *host, _IN_ unsigned short port)
{
    struct sockaddr_in addr;
    long sockfd;
    //int opt_val = 1;
    struct hostent *hp;
    struct in_addr in;
    uint32_t ip;
    char ip_addr[NETWORK_ADDR_LEN];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        LOGE(TAG, "socket");
        return -1;
    }
    if (0 == port) {
        return (intptr_t)sockfd;
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));

    //if (0 != setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_BROADCAST, &opt_val, sizeof(opt_val)))// {
    //    LOGE(TAG, "setsockopt");
    //    close(sockfd);
    //    return -1;
    //}

    if (NULL == host) {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        if (inet_aton(host, &in)) {
            ip = *(uint32_t *)&in;
        } else {
            hp = gethostbyname(host);
            if (!hp) {
                LOGE(TAG, "can't resolute the host address \n");
                close(sockfd);
                return -1;
            }
            ip = *(uint32_t *)(hp->h_addr);
        }
        addr.sin_addr.s_addr = ip;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    inet_ntop(AF_INET, &addr.sin_addr, ip_addr, NETWORK_ADDR_LEN);

    LOGD(TAG, "The host IP %s, port is %d\r\n", ip_addr, port);

    if (-1 == connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) {
        close(sockfd);
        return -1;
    }

    LOGI(TAG, "success to establish udp, fd=%d", sockfd);

    return (intptr_t)sockfd;
}

int HAL_UDP_close_without_connect(_IN_ intptr_t sockfd)
{
    return close((int)sockfd);
}

intptr_t  HAL_UDP_create(_IN_ char *host, _IN_ unsigned short port)
{
    int rc = -1;
    long socket_id = -1;
    char port_ptr[6] = {0};
    struct addrinfo hints;
    struct addrinfo *res, *ainfo;
    struct sockaddr_in *sa = NULL;
    char addr[NETWORK_ADDR_LEN] = {0};

    if(NULL == host){
        return (intptr_t)-1;
    }

    sprintf(port_ptr, "%u", port);
    memset ((char *)&hints, 0x00, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    rc = getaddrinfo(host, port_ptr, &hints, &res);
    if(0 != rc){
        perror("getaddrinfo error");
        return (intptr_t)-1;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next)
    {
        if (AF_INET == ainfo->ai_family)
        {
            sa = (struct sockaddr_in *)ainfo->ai_addr;
            inet_ntop(AF_INET, &sa->sin_addr, addr, NETWORK_ADDR_LEN);
            fprintf(stderr, "The host IP %s, port is %d\r\n", addr, ntohs(sa->sin_port));

            socket_id = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);
            if (socket_id < 0) {
                perror("create socket error");
                continue;
            }
            if(0 == connect(socket_id, ainfo->ai_addr, ainfo->ai_addrlen)){
                  break;
            }

            close(socket_id);
        }
    }
    freeaddrinfo(res);

    return (intptr_t)socket_id;
}

 void HAL_UDP_close(_IN_ intptr_t p_socket)
{
    long socket_id = -1;

    socket_id = (long)p_socket;
    close(socket_id);
}

int HAL_UDP_write(_IN_  intptr_t p_socket,
                  _OU_  const unsigned char    *p_data,
                  _OU_  unsigned int            datalen)
{
    int rc = -1;
    long socket_id = -1;

    socket_id = (long)p_socket;
    rc = send(socket_id, (char*)p_data, (int)datalen, 0);
    if(-1 == rc)
    {
        return -1;
    }
    return rc;
}


int HAL_UDP_read(_IN_ intptr_t       p_socket,
                 _OU_ unsigned char *p_data,
                 _OU_ unsigned int   datalen)
{
    long socket_id = -1;
    int count = -1;

    if(NULL == p_data || 0 == p_socket){
        return -1;
    }

    socket_id = (long)p_socket;
    count = (int)recv(socket_id, p_data, datalen, 0);

    return count;
}



int HAL_UDP_readTimeout(_IN_ intptr_t p_socket, _OU_ unsigned char  *p_data,
                        _OU_ unsigned int datalen, _IN_ unsigned int timeout)
{
    int ret;
    struct timeval tv;
    fd_set read_fds;
    long socket_id = -1;

    if(0 == p_socket || NULL == p_data){
        return -1;
    }
    socket_id = (long)p_socket;

    if( socket_id < 0 )
      return -1;

    FD_ZERO( &read_fds );
    FD_SET( socket_id, &read_fds );

    tv.tv_sec  = timeout / 1000;
    tv.tv_usec = ( timeout % 1000 ) * 1000;

    ret = select( socket_id + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv );

    /* Zero fds ready means we timed out */
    if( ret == 0 )
      return -2; /* receive timeout */

    if( ret < 0 )
    {
        if( errno == EINTR )
          return -3; /* want read */

        return -4; /* receive failed */
    }

    /* This call will not block */
    return HAL_UDP_read(p_socket, p_data, datalen);
}


int HAL_UDP_resolveAddress(const char *p_host,  char addr[NETWORK_ADDR_LEN])
{
    struct addrinfo *res, *ainfo;
    struct addrinfo hints;
    int error = -1;
    struct sockaddr_in *sa = NULL;

    memset ((char *)&hints, 0x00, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    error = getaddrinfo(p_host, NULL, &hints, &res);

    if (error != 0)
    {
        fprintf(stderr, "getaddrinfo error\n");
        return error;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next)
    {
        switch (ainfo->ai_family)
        {
            case AF_INET:
                sa = (struct sockaddr_in *)ainfo->ai_addr;
                inet_ntop(AF_INET, &sa->sin_addr, addr, NETWORK_ADDR_LEN);
                break;
            default:
                ;
        }
    }

    freeaddrinfo(res);
    return 0;
}
int HAL_UDP_connect(_IN_ intptr_t sockfd,
                    _IN_ const char *host,
                    _IN_ unsigned short port)
{
    int                     rc = -1;
    char                    port_ptr[6] = {0};
    struct addrinfo         hints;
    struct addrinfo        *res, *ainfo;

    if (NULL == host) {
        return -1;
    }

    LOGI(TAG, "HAL_UDP_connect, host=%s, port=%d", host, port);
    sprintf(port_ptr, "%u", port);
    memset((char *)&hints, 0x00, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    rc = getaddrinfo(host, port_ptr, &hints, &res);
    if (0 != rc) {
        LOGE(TAG, "getaddrinfo error");
        return -1;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        if (AF_INET == ainfo->ai_family) {
            if (0 == connect(sockfd, ainfo->ai_addr, ainfo->ai_addrlen)) {
                freeaddrinfo(res);
                return 0;
            }
        }
    }
    freeaddrinfo(res);

    return -1;
}
