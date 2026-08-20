#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(string_view message);

int main(int argc, char *argv[]) {
    int serv_sock{}, clnt_sock{};
    struct sockaddr_in serv_adr{}, clnt_adr{};
    socklen_t adr_sz{};
    int str_len{}, i{};
    char buf[BUF_SIZE]{};

    struct epoll_event *ep_events{nullptr};
    struct epoll_event event{};
    int epfd{}, event_cnt{};

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }

    epfd = epoll_create(EPOLL_SIZE);
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while (true) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1) {
            cout << "epoll_wait() error" << endl;
            break;
        }

        for (i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == serv_sock) {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                cout << "connected client: " << clnt_sock << endl;
            }
            else {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if(str_len == 0) { // close request!
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    cout << "close client: " << ep_events[i].data.fd << endl;
                }
                else {
                    write(ep_events[i].data.fd, buf, str_len); // echo!
                }
            }
        }
    }

    close(serv_sock);
    close(epfd);
    return 0;
}

void error_handling(string_view message) {
    cout << message << endl;
    exit(1);
}
