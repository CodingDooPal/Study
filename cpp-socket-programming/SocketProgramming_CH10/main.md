# CH 09. 소켓의 다양한 옵션

## 09-1 소켓의 옵션과 입출력 버퍼의 크기

- 소켓의 다양한 옵션<br>

| Protocol Level | Option Name | Get | Set |
| :---: | --- | :---: | :---: |
| SOL_SOCKET | SO_SNDBUF<br>SO_RCVBUF<br>SO_REUSEADDR<br>SO_KEEPALIVE<br>SO_BROADCAST<br>SO_DONTROUTE<br>SO_OOBINLINE<br>SO_ERROR<br>SO_TYPE | O<br>O<br>O<br>O<br>O<br>O<br>O<br>O<br>O<br> | O<br>O<br>O<br>O<br>O<br>O<br>O<br>X<br>X |
| IPPROTO_IP | IP_TOS<br>IP_TTL<br>IP_MULTICAST_TTL<br>IP_MULTICAST_LOOP<br>IP_MULTICAST_IF | O<br>O<br>O<br>O<br>O | O<br>O<br>O<br>O<br>O |
| IPPROTO_TCP | TCP_KEEPALIVE<br>TCP_NODELAY<br>TCP_MAXSEG | O<br>O<br>O | O<br>O<br>O |

소켓의 옵션은 계층별로 분류된다. 
| 옵션 | 분류 |
| :---: | --- |
| SOL_SOCKET | 소켓에 대한 가장 일반적인 옵션 |
| IPPROTO_IP | IP 프로토콜에 관련된 옵션 |
| IPPROTO_TCP | TCP 프로토콜에 관련된 옵션 |

- getsockopt & setsockopt<br>
대부분의 옵션은 참조(Get) 및 변경(Set)이 가능하다. 참조 및 변경을 위해서 다음 두 함수를 사용한다.

1. 옵션 참조에 사용되는 함수
```cpp
#include <sys/socket.h>

int getsockopt(int sock, int level, int optname, void* optval, socklen_t *optlen);
// 성공 시 0, 실패 시 -1 반환
```

2. 옵션 변경에 사용되는 함수
```cpp
#include <sys/socket.h>

int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
// 성공 시 0, 실패 시 -1 반환
```

- SO_SNDBUF & SO_RCVBUF<br>
CH 05에서 소켓이 생성되면 기본적으로 입출력 버퍼가 생성된다고 했다. 두 옵션은 각각 출력, 입력 버퍼의 크기와 관련된 옵션이다. 이 두 옵션을 이용해서 입출력 버퍼의 크기를 참조할 수 있을 뿐만 아니라, 변경도 가능하다.<br>
두 옵션으로 입출력 버퍼를 변경해도 크기가 정확히 맞춰지지 않는다. 예를 들어 출력 버퍼를 0으로 변경하면 여러 부분에서 문제가 발생한다. 이러한 경우를 방지하기 위해 우리는 버퍼의 크기를 요구할 뿐이며, 그것이 정확하게 반영되는 것은 아니다.

## 09-2 SO_REUSEADDR
이 옵션에 대해 설명하기 전에 Time-wait라는 개념을 먼저 알아야 한다.<br>
다음과 같은 상황을 생각해보자. 서로 연결된 클라이언트와 서버가 있다. 이때 클라이언트가 먼저 연결 종료 요청을 보내는 경우를 생각해보자. 그러면 클라이언트가 먼저 서버에게 FIN 패킷을 보내고, 최종적으로 Four-Way Handshaking 과정을 거쳐 정상적으로 연결이 종료된다.<br>
반대로 서버가 먼저 연결 종료 요청을 보내는 경우를 생각해보자. 그러면 서버가 먼저 클라이언트에게 FIN 패킷을 보낸다. 이렇게 종료를 하게 되면 클라이언트가 먼저 종료 요청을 보낸 경우와 달리, 동일한 PORT 번호로 서버를 재실행 하면 "bind() error"가 발생하여 서버가 실행되지 않는다. 약 3분 정도 기다리면 정상적으로 실행이 된다.<br><br>

- Time-wait 상태<br>
서버가 먼저 연결 종료를 요청하면 서버는 Four-Way Handshaking 이후에 소켓이 바로 소멸되지 않고 Time-wait 상태라는 것을 일정시간 거친다. 이것 때문에 바로 재실행이 안 되는 것이다.<br>
서버가 먼저 연결 종료 요청을 한 Four-Way Handshaking 과정 마지막에는 서버가 클라이언트에게 ACK 패킷을 보내게 된다. 만약 이 패킷이 클라이언트에게 전송되지 못 한 상태에서 연결이 종료되고, 패킷이 소멸된다고 생각해보자. 그러면 클라이언트는 ACK 패킷을 재전송 하는 것을 요구할텐데 서버는 이미 연결을 종료했기 때문에 영원히 ACK 패킷을 받을 수 없다. 따라서 Time-wait 상태에 진입하여 패킷을 전송할 수 있도록 하는 것이다.

- 주소의 재할당<br>
위의 상황을 보면 Time-wait의 역할이 중요한 것으로 보인다. 그러나 문제가 되는 상황도 있다. 시스템에 문제가 생겨 서버가 다운된 경우를 생각해보자. 서버를 재가동시켜야 하는데 Time-wait 상태에 돌입했기 때문에 서버를 실행하는 것이 불가능하다. 추가적으로 Time-wait 상태는 경우에 따라 길어질 수 있다. 이런 상황을 우회하기 위해 SO_REUSEADDR의 상태를 변경하여, Time-wait 상태에 있는 소켓에 할당되어 있는 PORT 번호를 새로 시작하는 소켓에 할당되게 할 수 있다.
```cpp
optlen = sizeof(option);
option = TRUE;
setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
```

## 09-3 TCP_NODELAY
- Nagle 알고리즘<br>
Nagle 알고리즘은 네트워크상에서 돌아다니는 패킷들의 흘러 넘침을 막기 위해서 1984년에 제안된 알고리즘이다. Nagle 알고리즘은 앞서 전송한 데이터에 대한 ACK 메시지를 받아야만, 다음 데이터를 전송하는 알고리즘이다.<br>
그러나 Nagle 알고리즘이 항상 좋은 것은 아니다. `용량이 큰 파일 데이터의 전송`과 같이 Nagle 알고리즘을 적용하지 않아도 출력버퍼를 거의 꽉 채운 상태에서 패킷을 전송할 수 있는 경우가 있다.

- Nagle 알고리즘의 중단<br>
Nagle 알고리즘의 적용 여부에 따른 트래픽의 차이가 크지 않으면서도 Nagle 알고리즘을 적용하는 것보다 데이터의 전송이 빠른 경우에는 Nagle 알고리즘을 중단하는 것이 좋다. 방법은 다음과 같다.
```cpp
int opt_val = 1;
setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));
```
그리고 Nagle 알고리즘의 설정상태를 확인하려면 다음과 같이 TCP_NODELAY에 설정된 값을 확인하면 된다.
```cpp
int opt_val;
socklen_t opt_len;
getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len);
```
Nalge 알고리즘이 설정된 상태라면 함수호출의 결과로 opt_val에는 0이 저장되며, 반대로 설정되지 않은 상태라면 1이 저장된다.
