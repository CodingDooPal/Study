# 개발 관련 터미널 명령어 정리

## 목차 및 개요
- **목차**  
  - [파일 관리](#파일-관리)  
  - [SSH SCP](#ssh--scp)  
  - [개발 작업](#개발-작업)
  - [기타](#기타)  

---
- **개요**  
1\. 윈도우 및 리눅스에서 사용하는 명령어 정리 문서이다.  
2\. 사용 가능한 환경은 아래와 같이 표시한다.
```bash
# ex)
(linux|win|all)> [명령어]
```  

### 파일 관리
---
#### **탐색/이동**  

**1\. 상위 폴더로 이동**  
```bash 
all> cd ..
```
<br>

**2\. 두 단계 위로 이동**  
```bash
all> cd ../..
```
<br>

**3\. 홈 디렉터리로 이동**  
```bash
all> cd ~
```
<br>

**4\. 숨김 파일 포함 목록**  
```bash
linux> ls -la
```
<br>

**5\. 파일 목록 표시**  
```bash
win> dir
```
<br>

**6\. 현재 경로 출력**  
```bash
linux> pwd
```
<br>

---
#### **복사/이동/삭제**  

src/ => 원본 파일 또는 디렉터리 경로  
dst/ => 결과 파일 또는 디렉터리 경로
<br>

**1\. 폴더 통째로 복사**  
```bash
linux> cp -r src/ dst/
```
<br>

**2\. 폴더 하위까지 복사**  
```bash
win> xcopy src dst /E /I
```  
/E: 비어 있는 경우를 포함하여 디렉터리와 하위 디렉터리를 복사한다.  
/I: 대상을 찾을 수 없고 두 파일 이상 복사하면 대상을 디렉터리로 지정한다.
<br>

**3\. 파일 이동 또는 이름 변경**  
```bash
all> mv\|move src/ dst/
```
<br>

**4\. 폴더 강제 삭제(주의)**  
```bash
linux> rm -rf src/
```
<br>

**5\. 폴더 강제 삭제**
```bash
win> rmdir /S /Q src/
```  
`/S` : 지정된 디렉터리와 그 안의 모든 디렉터리 및 파일을 지운다.  
`/Q` : 지우는데 문제가 없으면 묻지 않는다.
<br>

---
#### **검색/확인**  

**1\. 현재 위치서 파일 검색**  
```bash
linux> find . -name "src/"
```
<br>

**2\. 파일 내용에서 문자열 검색**  
```bash
linux> grep -r "[내용]" src/
```
<br>

**3\. 파일 내용 출력**  
```bash
linux> cat src/
```
<br>

**4\. 파일 실시간 모니터링**  
```bash
linux> tail -f src/
```
<br>

**5\. 파일 내용 출력**  
```bash
win> type src/
```
<br>

---
#### **권한/소유자**  

**1\. 실행 권한 부여**  
```bash
linux> chmod 755 src/
```  
`사용자(User)`, `그룹(Group)`, `다른사용자(Other)`의 권한 설정  
`읽기(Read)` : 4   
`쓰기(Write)` : 2  
`실행(Execute)` : 1  
숫자를 더하는 형식으로 권한을 설정할 수 있다.
<br>

**2\. 소유자 변경**  
```bash
linux> chown user:group src/
```  
왼쪽에서 오른쪽으로 파일의 소유자를 변경한다.
<br>

**3\. 권한 확인**  
```bash
linux> ls -l
```
<br>

### **SSH & SCP**
---
#### **기본 접속**  

**1\. SSH 접속**  
```bash
all> ssh user@IP_addr
```
<br>

**2\. config 별칭으로 접속**   
```bash
# ex) MyServer라고 설정했다면 
all> ssh MyServer
```
<br>

**3\. 포트 지정 접속**  
ex) 2222번 포트로 접속 시  
```bash
all> ssh -p 2222 user@IP_addr
```
<br>

**4\. SSH 세션 종료**  
```bash
all> exit
```
<br>

---
#### **키 관리**  

**1\. SSH 키 생성**  
```bash
all> ssh-keygen -t ed25519
```  
- 주요 SSH 키 알고리즘 종류  
1. RSA(-t rsa -b 4096)  
2. Ed25519(-t ed25519) -> 웬만하면 얘 사용  
3. ECDSA(-t ecdsa)
<br>

**2\. 공개키 서버에 등록**  
```bash
linux> ssh-copy-id user@IP_addr
```
<br>

**3\. 공개키 내용 확인(ed25519 기준)**  
```bash
all> cat ~/.ssh/id_ed25519.pub
```
<br>

---
#### **SCP 파일 전송**  

**1\. Local -> Server 파일 전송**  
```bash
all> scp src/ user@IP_addr:dst/
```
<br>

**2\. Server -> Local 다운로드**  
```bash
all> scp user@IP_addr:src/ dst/
```
<br>

**3\. 폴더 통째로 전송**  
```bash
all> scp -r src/ user@IP_addr:dst/
```
<br>

---
#### **CONFIG 설정(~/.SSH/CONFIG)**  

**1\. 서버 별칭 지정(MyServer로 지정 시)**  
```bash
Host MyServer
```
<br>

**2\. 실제 접속 IP**  
```bash
HostName IP_addr
```
<br>

**3\. 접속 유저명**  
```bash
User username
```
<br>

**4\. 키 전달(점프 서버용)**  
```bash
ForwardAgent yes
```
<br>

### **개발 작업**
---
#### **프로세스/모니터링**  

**1\. 프로세스 검색**  
```bash
linux> ps aux | grep \[name\]
```
<br>

**2\. 프로세스 강제 종료**  
```bash
linux> kill -9 \[PID\]
```
<br>

**3\. 실시간 리소스 모니터링**  
```bash
linux> top|htop
```
<br>

**4\. 프로세스 목록/종료**  
```bash
win> tasklist|taskkill
```
<br>

---
#### 네트워크  

**1\. 열린 포트 확인**  
```bash
linux> ss -tuln
```
<br>

**2\. 열린 포트 + PID 확인**  
```bash
win> netstat -ano
```
<br>

**3\. 연결 확인**  
```bash
all> ping IP
```
<br>

**4\. HTTP 응답 확인**  
```bash
curl http://IP_addr:PORT
```
<br>

---
#### **빌드/컴파일(C++)**

**1\. C++ 컴파일**  
```bash
linux> g++ main.cpp -0 main
```
<br>

**2\. 디버그 심볼 포함 빌드**  
```bash
linux> g++ main.cpp -o main -g
```
<br>

**3\. Makefile 기반 빌드**  
```bash
linux> make
```
<br>

**4\. 빌드된 바이너리 실행**  
```bash
linux> ./main
```
<br>

---
#### **GIT**  

**1\. 변경 사항 확인**  
```bash
all> git status
```
<br>

**2\. 전체 스테이징 후 커밋**  
```bash
all> git add . && git commit
```  
add 뒤에 원하는 디렉터리나 파일을 지정할 수 있다.  
commit에 -m 옵션을 붙여 커밋 메시지를 추가할 수 있다.
<br>

**3\. 커밋 히스토리 간략 확인**  
```bash
all> git log --oneline
```
<br>

**4\. 변경된 내용 diff 확인**  
```bash
all> git diff
```
<br>

### **기타**
---
