#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 0xFFFF

using namespace std;

int main() {
    int socket_fd;
    struct sockaddr_in server_addr;
    int len;
    char send_buf[MAXSIZE];
    char get_buff[MAXSIZE];
    int recv_num;
    int fun_res;
    string str;

    fd_set rfds;
    struct timeval tv;
    int max_fd;

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(server_addr);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        cout << "socket error!" << endl;
        exit(1);
    }

    fun_res = connect(socket_fd, (struct sockaddr *) &server_addr, len);
    if (fun_res < 0) {
        cout << "connect error!" << endl;
        exit(1);
    }

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);

        max_fd = 0;
        FD_SET(socket_fd,&rfds);
        if (max_fd < socket_fd) max_fd = socket_fd;

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        fun_res = select(max_fd+1, &rfds, NULL, NULL, &tv);
        if (fun_res < 0) {
            cout << "select error!" << endl;
            exit(1);
        } else if(fun_res == 0) {
            //cout << "no msg!waiting..." << endl;
            continue;
        } else {
            if (FD_ISSET(socket_fd,&rfds)) {
                recv_num = recv(socket_fd, get_buff, sizeof(get_buff), 0);
                if (recv_num < 0) {
                    cout << "recv error!" << endl;
                    exit(1);
                } else {
                    get_buff[recv_num] = 0;
                    cout << "server msg: " << get_buff << endl;
                }
            }

            if (FD_ISSET(0,&rfds)) {
                cin >> send_buf;
                str = send_buf;

                fun_res = send(socket_fd,send_buf,strlen(send_buf),0);
                if (fun_res < 0) {
                    cout << "send error!" << endl;
                    exit(1);
                }
                if (str == "exit") exit(1);
            }
        }
    }

    close(socket_fd);
    return 0;
}
