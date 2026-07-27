# CH 05. TCP 기반 서버/클라이언트 2

## 05-1 에코 클라이언트의 완벽 구현
CH04에서 만든 에코 클라이언트에는 문제점이 존재한다. 먼저 에코 서버의 입출력 문장이다.
```cpp
while ((str_len = read(clnt_sock, message.data(), BUF_SIZE)) != 0) {
            write(clnt_sock, message.c_str(), str_len);
        }
```
다음으로 에코 클라이언트의 입출력 문장이다.
```cpp
write(sock, message.data(), message.size());
        str_len = read(sock, message.data(), BUF_SIZE - 1);
```
여기서 문제점은 수신하는 단위이다. 클라이언트의 코드를 조금 더 보자.
```cpp
while (true) {
    cout << "Input message(Q to quit): ";
    cin >> message;
    // . . . . 
    write(sock, message.data(), message.size());
    str_len = read(sock, message.data(), BUF_SIZE - 1);
    message[str_len] = 0;
    cout << "Massage from server: " << message << endl;
    }
```
에코 클라이언트는 read 함수호출을 통해서 자신이 전송한 문자열 데이터를 한 번에 수신하기를 원하고 있다. 바로 이 부분이 문제다. 예제의 상황에서는 데이터의 크기가 작기 때문에 큰 문제가 되지 않지만 만약 서버가 데이터를 분할해서 보내야 될만큼 크다면 문제가 생길 것이다.

해결법은 간단하다. 20바이트인 문자열을 전송했다면, 20바이트를 전부 수신할 때까지 반복해서 read 함수를 호출하면 된다.
```cpp
while (true) {
        cout << "Input message(Q to quit): ";
        string input{};
        getline(cin, input);
        // . . . .
        str_len = write(sock, input.c_str(), input.size());

        recv_len = 0;
        while (recv_len < str_len) {
            recv_cnt = read(sock, message.data(), BUF_SIZE - 1);
            if (recv_cnt == -1) {
                error_handling("read() error!");
            }
            recv_len += recv_cnt;
        }

        string received(message.data(), str_len);
        cout << "Message from server: " << received << endl;
    }
```
## 05-2 TCP의 이론적인 이야기
TCP 소켓에는 입출력 버퍼가 존재한다. write 함수를 호출하면 데이터가 출력버퍼로, read 함수를 호출하면 데이터가 입력버퍼로 이동한다. 따라서 write와 read 함수를 호출하는 순간이 데이터가 송수신 되는 순간이 아니다.

이때 한 가지 의문이 생길 수 있다.
> 클라이언트 입력버퍼의 크기가 50바이트인데, 서버에서 100바이트를 전송하면 어떻게 될까?

TCP 소켓의 통신에서는 입력버퍼의 크기를 초과하는 분량의 데이터 전송은 발생하지 않는다. TCP에 존재하는 `슬라이딩 윈도우(Sliding Window)`라는 프로토콜 때문이다. 그 과정을 대략적으로 표현하면 아래와 같다.

· `소켓 A`: "50바이트까지는 괜찮아."<br>
· `소켓 B`: "ㅇㅋㅇㅋ"<br>
· `소켓 A`: "내가 20바이트 비웠으니까 70바이트까지 괜찮아."<br>
· `소켓 B`: "ㅇㅋㅇㅋ"

이 과정에서 TCP는 소켓이 비워질 때마다 해당 내용을 주고받는 것이 아니라 `SWS(Silly Window Syndrome) 회피`에 따라 송신 측에서는 `Nagle's Algorithm`, 수신 측에서는 `Clark's Solution`을 사용해 데이터를 효율적으로 보낸다.

- **Nagle's Algorithm**<br>
네이글 알고리즘은 송신 측에서 데이터를 효율적으로 전송하기 위해 사용한다. 만약 상대방이 한 번에 최대 100바이트를 받을 수 있는데 1바이트씩 나누어서 전송하면 비효율적일 것이다.<br>
따라서 데이터를 가능한 모아서 더 큰 패킷으로 만들어 효율적으로 보낸다. 

- **Clark's Solution**<br>
클락의 해결책은 수신 측에서 데이터를 효율적으로 전송받기 위해 사용한다. 만약 사용할 수 있는 입력버퍼의 크기를 지속적으로 전송하면 비효율적일 것이다.<br>
따라서 MSS(Maximum Segment Size)와 입력버퍼 크기의 절반 중 작은 값을 기준으로 사용한다. 사용가능한 입력버퍼의 크기가 해당 값보다 크다면 입력버퍼가 받을 수 있는 데이터의 양을 전송한다.

TCP의 내부 동작 원리는 크게 세 가지로 구분할 수 있다.

- 상대 소켓과의 연결<br>
TCP에서 연결 동작은 Three-Way Handshaking으로 이루어진다.
1. 호스트 A가 호스트 B에게 SYN 패킷을 전송한다.
2. 호스트 B가 호스트 A에게 SYN+ACK 패킷을 전송한다.
3. 호스트 A가 호스트 B에게 ACK 패킷을 전송한다.<br><br>
이 과정에서 두 호스트는 `SEQ`와 `ACK`라는 메시지를 주고 받는다. `SEQ`는 `패킷의 번호`, `ACK`는 `다음 번에 전송해야 할 패킷의 번호`를 명시하고 있다.<br>

- 상대 소켓과의 데이터 송수신<br>
TCP에서는 소멸되는 데이터 없이 모든 데이터가 정상적으로 전송되어야 한다.
1. 호스트 A가 호스트 B에게 SEQ가 1200이고 100 바이트의 데이터를 보낸다.
2. 호스트 B는 호스트 A에게 ACK가 1301인 패킷을 보낸다.
3. 호스트 A가 호스트 B에게 SEQ가 1301이고 100 바이트의 데이터를 보낸다.
4. 호스트 B는 호스트 A에게 ACK가 1402인 패킷을 보낸다.<br>
이 과정에서 ACK는 SEQ 번호 + 전송된 바이트 크기 + 1이라는 값으로 설정되는 것을 알 수 있다. 이것은 데이터가 정상적으로 전송되지 않았을 때 해당 패킷을 재전송하기 위함이다. 만약 3번 과정에서 보낸 패킷을 호스트 B가 전송받지 못한다고 가정해보자. 이러면 호스트 A는 호스트 B에게 SEQ 1301에 대한 ACK 패킷을 받지 못하기 때문에 재전송을 진행한다. 이런 방식으로 TCP는 데이터 손실을 방지한다.<br><br>

- 상대 소켓과의 연결종료<br>
TCP에서 연결종료 동작은 Four-Way Handshaking으로 이루어진다.
1. 호스트 A가 FIN 패킷을 보낸다.
2. 호스트 B가 ACK 패킷을 보낸다.
3. 호스트 B가 FIN 패킷을 보낸다.
4. 호스트 A가 ACK 패킷을 보낸다.<br>
위 과정에서 보이는 것처럼 두 호스트가 서로 FIN, ACK 패킷을 주고 받으며 연결이 종료된다.