# fn_project
멀티스레드 기반의 c언어 채팅 프로그램<br/>
201944052 B반 조은지<br/><br/>
소스코드 파일은 master 브랜치에 있습니다.<br/><br/><br/>

## 개요<br/>
강의 때 멀티스레드를 이용한 채팅 프로그램 예제를 따라해보고서 실행 결과를 보고 생각했다.<br/>
클라이언트 간에 구분이 잘 안가고 클라이언트 각자가 자신의 고유한 이름을 갖고 대화에 참여하면 대화에
혼동이 생기지 않고 대화하는데 더 좋을것 이라고 <br/> 그리고 다수의 클라이언트들이 서버에 접속했을 때 서버는 그만큼 관리가 어려워질 수 있는데<br/> 오로지 서버에게 제공되는 명령어를 만들어 보다 관리가 쉽도록 구현하고자 했다.<br/>
비록 다양한 기능은 구현해내지 못해도 결함이 없으며 실행이 잘되는 그런 프로그램을 만들고자 한다<br/><br/>

## 목적<br/>
멀티스레드 기반의 소켓 프로그래밍 채팅 프로그램 구현<br/>
멀티스레드 사용방법 명령어 목록 서버에게 제공<br/>
클라이언트 각자의 고유한 이름을 갖고 대화에 참가<br/>
각자 클라이언트의 대화는 서버 뿐만아니라 모든 클라이언트에게도 전송 가능<br/><br/>

## 설계<br/>
<img width="310" alt="서버분석1" src="https://user-images.githubusercontent.com/80368992/122233395-fd302680-cef6-11eb-97f3-d789f3880cfa.jpg"><br/><br/><br/>

> 서버 코드 설명

#### 0. chat_serv 변수
<img width="310" alt="서버분석1" src="https://user-images.githubusercontent.com/80368992/122088213-7fa9df00-ce40-11eb-8cfc-5e9663cc0ab1.PNG">

#### 1. chat_serv 도우미 명령어
<img width="469" alt="서버분석2" src="https://user-images.githubusercontent.com/80368992/122089512-cb10bd00-ce41-11eb-9876-571d5f69699d.PNG"><br/>
서버에게 제공되는 명령어 (도움말, 참가자 수, 채팅 수, 참가자 ip목록, 나가기) 목록이 있음<br/>
fgets()함수를 통해 스트링을 읽는다 <br/>
strcmp() 함수를 사용해  읽어들인 문자열과 두 문자열이 같은지 비교할 수 있다.<br/><br/>

#### 2. chat_serv 서버 메인 
<img width="477" alt="서버분석3" src="https://user-images.githubusercontent.com/80368992/122089807-1aef8400-ce42-11eb-8ec4-a65160917c3c.PNG"><br/>
sockeraddr_in 소켓 주소 구조체의 cliaddr -> 클라이언트 주소 구조체 정의<br/>
buf[MAXLINE + 1] -> 클라이언트에서 받은 메시지<br/><br/>
fd_set read_fds -> 읽기를 감지할 fd_set 구조체 정의, fd_set read_fds ; 와 같이 선언만 하고 안에 배열값을 찍어보면 쓰레기값이 있을 수 있다. 따라서 구조체를 초기화 해야 하며 밑에서 해줄 것이다. <br/> 
pthread_t a_thread -> 클라이언트 스레드용 구조체 정의<br/><br/>
argc 인자값이 2개가 아닌경우 서버 사용법을 알려준다 <br/>
main(int argc, char *argv[]) -> argc는 사용자가 입력한 인자값의 개수, *argv[]는 사용자가 입력한 문자열을 말한다 <br/><br/>이제 클라이언트와 연결하기 위해 소켓을 생성하고 읽어들이는 메서드인 tcp_listen(host, port, backlog)함수를 호출한다. [tct_listen 함수](https://user-images.githubusercontent.com/80368992/122089826-1fb43800-ce42-11eb-9750-f5fa6a34d9f4.PNG)<br/>호출하고 반환된 소켓 값을 listen_Sock에 저장한다. <br/><br/>
pthread_create -> 스레드를 생성한다 <br/>
* pthread_create(&a_thread, null, thread_function,(void *)null) <br/>
pthread_create(생성되는 스레드의 번호를 받을 변수, 스레드 속성, 스레드가 생성되면서 실행할 변수, 실행할 함수에 들어갈 변수)<br/><br/>
* FD_ZERO, FD_SET 함수는 둘다 자신이 체크하고 싶은 소켓을 그룹에 포함시킨 후에 select 함수를 사용해야 한다.<br/> 
* fd_set은 파일 디스크립터 번호를 배열로 가지는 '비트 필드'구조체이다. <br/>
* select()는 입출력 다중화 구현이 가능하며 지정한 범위의 fd중 FD_SET으로 1로 set되어 있는 파일을 순회하면서 변화가 있는 fd를 반환한다. <br/>

FD_ZERO()함수로 read_fds의 메모리를 모두 초기화한다<br/>
FD_SET() 함수로 읽어들인 파일 디스크립터 중 listen_sock에 해당하는 비트를 1로 하고<br/>

참여자 수 만큼 for문을 통해 FD_SET()함수로 읽어온 파일 스크립터 주소를 채팅에 참가한 참여자를 소켓 목록(clisock_list)에 추가한다<br/>
관리하는 파일의 최대 개수(max_fdp)는 채팅 하는 동안 계속 재 계산된다 [소켓 번호 최대 찾기 함수](https://user-images.githubusercontent.com/80368992/122089826-1fb43800-ce42-11eb-9750-f5fa6a34d9f4.PNG). 파일의 개수는 최대 파일 지정 번호 + 1로 지정함 <br/> 
select()함수로 변화가 있는 파일 디스크립터는 반환한다. <br/>만약 최대 파일 디스크립터 범위내에 검색한 fd가 0이하(변화 없음)라면 select에 실패한것이다 <br/>
만약 FD_ISSET(listen_sock, &read_fds) -> fd_set으로 선언된 read_fds에 listen_sock 연결된 소켓 값이 설정되어 있는지 확인 한다.<br/>
* accpet(소켓, 주소정보 구조체 주소, 주소 정보 구조체 크기를 담을 변수 구조) => 아직 처리되지 않은 연결들이 대기하고 있는 큐에서 제일 처음 연결된 연결을 가져와서 새로운 연결된 소켓을 만든다. 그리고 소켓을 가르키는 파일 지정자를 할당하고 이것을 리턴<br/>
accpet() 함수를 통해 소켓으로 부터 연결을 연결을 받아들여 받아들인 소켓 디스크립터를 accp_sock에 저장한다.<br/>
accp_sock이 -1이면 연결실패 아니라면 연결 성공 
연결이 성공되면 add_Client() 함수(연결이 성공한 참가자를 처리함)를 [add_Client 함수](https://user-images.githubusercontent.com/80368992/122089816-1d51de00-ce42-11eb-8e71-df7a5f70a5d1.PNG)호출한다 <br/>
send()함수를 통해 연결된 소켓 디스크립터(참여자)에게 전송할 데이터(START 문구)를 전송한다.<br/><br/>

* time() 이라는 함수의 반환형으로써 time() 함수가 어떠한 기준에 의한 시간데이터를 반환하고 그 시간을 다루기 위해서 사용하는 데이터 타입 즉 1970년 1월 1일 00시 00분(UTC) 부터 지금까지 초단위의 시간을 정수값으로 표현하는 데이터 <br/>
* tm구조체는 시간을 우리가 알아볼 수 있게 세세하게 변수로 나누어서 만들어져 있는 구조체. <br/>
tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year,tm_day등 9개의 변수가 각각 표현하는 값이 다르다.<br/>

사용자에게 환영 메시지를 보낸 후 현재 시각과 참가자 수를 출력한다.<br/>

클라이언트가 보낸 메시지를 모든 클라이언트에게 방송할 때 <br/>
참여자 수 만큼 for문을 통해 fd_set으로 선언된 read_fds에 clisock_list[i] (말한 참가자의 연결된 소켓 값)이 있는지 검사하고 총 대화수 chat_num을 증가시키고.<br/>
읽은 데이터를 저장할 버퍼를 nbyte변수에 저장한다.<br/>
만약 데이터(nbyte)가 없거나 참여자가 "exit"를 입력할 경우 클라이언트의 종료로 받아들이고 remove_Client()함수를 [remove_Client 함수](https://user-images.githubusercontent.com/80368992/122089816-1d51de00-ce42-11eb-8e71-df7a5f70a5d1.PNG) 호출하여 참여자를 탈퇴처리 한다.아니라면 모든 채팅 참여자에게 메시지를 방송한다.<br/><br/>


#### 3. chat_serv 최대 소켓 번호 찾기 & 소켓 생성 및 읽기 
<img width="416" alt="서버분석5" src="https://user-images.githubusercontent.com/80368992/122089826-1fb43800-ce42-11eb-9750-f5fa6a34d9f4.PNG"><br/>
* 최대 소켓 번호 찾기
int max = listen_sock -> 서버가 가장 먼저 읽어온 소켓을 max에 저장 <br/>
참여자 수 만큼 for문을 돌리며 만약 참여자 소켓 목록이 max보다 크다면 max에 참여자 소켓 목록을 저장한 후 max값을 반환하여 max(새롭게 참여자가 추가된 소켓 목록이 저장된) 값에 +1 한 값을 max_fdp(최대 소켓 번호)에 저장한다.<br/>

* 소켓 생성 및 읽기
sockaddr_in 주소 구조체의 servaddr -> 서버 주소 구조체 정의<br/>
socket() 함수를 통해 소켓 생성 생성된 소켓을 sd에 저장 <br/>
만약 소켓 생성에 실패한다면 -1을 반환한다 아니면 seraddr 구조체의 내용을 세팅한다. ( 클라이언트와 연결하기 위한 준비)<br/>
1. bzero -> 구조체를 0으로 초기화 한다 ( ! memset과 마찬가지로 메모리를 초기화하기위한 목적으로 주로 사용 )<br/>
2. 서버 주소 체계 (sin_family) = AF_INET -> IPv4 <br/>
3. 서버 주소 (sin_addr.s_addr = htonl(host))-> Host의 Byte 순서를 Network-Byte-Order(Big Endian)으로 변환<br/>
4. 서버 포트 (sin_port = htons(port)) -> 포트 번호 데이터를 네트워크 바이트 순서로 변경<br/>
5. bind() 함수를 이용해 서버와 클라이언트를 바인딩한다. 즉 연결되지 않은 소켓 sockaddr(소켓)에 클라이언트와 연결하기 위해 필요한 정보를 할당한다. <br/>
6. listen() 함수를 이용해 소켓을 통해 들어오는 클라이언트 연결 요청을 기다리고 연결 성공시 0을, 실패시 -1을 반환한다.<br/><br/>


#### 4. chat_serv 새로운 참가자 처리 & 참가자 탈퇴 처리 
<img width="404" alt="서버분석4" src="https://user-images.githubusercontent.com/80368992/122089816-1d51de00-ce42-11eb-8e71-df7a5f70a5d1.PNG"><br/>

* 새로운 참가자 처리
Inet_ntop -> IP주소 변환, newcliaddr주소를 sin_addr로 바꿔서 buf에 저장 <br/>
채팅 클라이언트 목록에 연결된 소켓 (참가자) 추가 <br/>
strcpy() -> 문자열을 복사하는 함수 방금 전 newcliaddr주소(새로운 참여자 주소)가 저장된 buf를 복사하여 ip_list에 추가한다.<br/>
user_num증가 됨<br/>

* 참가자 탈퇴 처리 
저장된 리스트를 재배열하여 현재 참가자 목록에서 유저수 -1를 하여 다시 참가자 목록에 변경된 유저 목록을 저장함 <br/>
변경된 유저 목록을 다시 strcpy()함수를 사용하여 복사한 후 ip_list에 추가한다. <br/><br/><br/>


<br/><br/>
> 클라이언트 코드 설명
#### 1. chat_clnt 클라이언트 메인 
<img width="626" alt="클라이언트분석1" src="https://user-images.githubusercontent.com/80368992/122258737-59517580-cf0c-11eb-92f5-f8bfc8313f0b.PNG"><br/>
main의 인자값이 4개가 아닌경우 사용법을 알려줌 -> ./파일명 IP주소 Port번호 닉네임 <br/>
사용자의 닉네임을 저장할 변수 : bufname<br/>
닉네임 길이 : namelen <br/><br/>

tcp_connect()함수를 호출하여 [tcp_connect 함수](https://user-images.githubusercontent.com/80368992/122090556-d57f8680-ce42-11eb-85e7-88214720f6ea.PNG) 서버에 접속한다.<br/>
tcp_connect(어떤 통신 체계를 사용할지 결정하는 값, 서버 ip, 포트번호)<br/>
함수 호출 후 반환 값이 0이라면 서버 접속에 성공한 것이다. <br/>
!! AP_INET과 PF_INET의 차이점 : 둘다 인터넷 프로토콜이지만 AF는 주소 체계를 표현할 때 사용하는 IPv4인터넷 프로토콜, 
PF는 프로토콜 체계를 표현할 때 사용하는 IPv4인터넷 프로토콜이다. <br/>
max_fdp = s+1 ->  최대 소켓 파일 디스크립터는 서버에 접속된 tcp 개수에 +1된 개수로 정한다. <br/>
FD_ZERO로 read_fds 메모리를 초기화하고<br/>
FD_SET으로 읽어온 파일 디스크립터(read_fds)중 생성된 소켓(s)에 해당하는 비트값을 set해준다.<br/><br/>

* select(검사할 파일 디스크립터 최대 번호, 읽기를 검사할 fd_set, 쓰기를 검사할 fd_set, 예외를 검사할 fd_set, 검사하는 시간 제한 ( NULL인경우 무한정 대기임))<br/>
select() 함수를 통해 fd중 fd_set으로 1로 set되어 있는 파일을 순회하며 변화가 있는 fd를 반환 즉 검사를 할 최대 파일 디스크립터의 범위 내에서 read_fds를 검사한다.<br/><br/>

읽어온 파일 디스크립터 (read_fds) 중 s(참가자 소켓)에 해당하는 비트가 1로 set되어 있는지 검사하고
만약 메세지를 읽어올 데이터가 있다면 메시지를 출력하고 참가자의 닉네임을 출력한다.<br/>

읽어온 파일 디스크립터 (read_fds) 중 비트가 0으로 set되어 있는것을 검사한다
이는 참가자가 아무 데이터도 입력하지 않고 대기하는 상태이다. <br/>
사용자가 "exit"를 입력하면 서버에서 나가게 되며 소켓이 소멸된다.<br/><br/>

#### 2. chat_clnt 소켓 연결 
<img width="391" alt="클라이언트분석2" src="https://user-images.githubusercontent.com/80368992/122090556-d57f8680-ce42-11eb-85e7-88214720f6ea.PNG"><br/>
!! 서버와 비슷한 연결 과정 <br/>
1. 소켓 구조체를 결정하고 -> sockaddr_in servaddr<br/>
2. 소켓을 생성한다. -> socket()<br/>
3. bzero() 함수로 채팅 서버의 소켓주소 구조체 servaddr 초기화하고.<br/>
4. 서버 주소 체계 -> sin_family = af<br/>
5. inet_pton() 함수를 통해 IP주소를 이진 데이터로 변경 <br/>
6. 서버 포트 (sin_port = htons(port)) -> 포트 번호 데이터를 네트워크 바이트 순서로 변경<br/>
7. connect() 함수를 통해 서버에 연결 요청<br/>
<br/><br/><br/>


> 실행 

#### 1. 클라이언트와 서버 통신 성공 (클라이언트)
<img width="278" alt="클라이언트사용방법" src="https://user-images.githubusercontent.com/80368992/122092199-9d794300-ce44-11eb-8ddd-cf996f5c0648.PNG"><br/>
클라이언트 1.meow 2.bullet 3.jeff 서버와 연결 성공 -> 클라이언트 측에 환영 메시지 출력<br/>

#### 2. 클라이언트와 서버 통신 성공 (서버)
<img width="367" alt="클라이언트연결완료시서버측" src="https://user-images.githubusercontent.com/80368992/122092204-9eaa7000-ce44-11eb-8c90-bb5d015267ed.PNG"><br/>
클라이언트 1.meow 2.bullet 3.jeff 서버와 연결 성공 -> 서버 지원 명령어 목록, 현재 참가 수를 출력<br/>

#### 3. 클라이언트 측 채팅결과 
<img width="315" alt="참여자모두에게채팅방송" src="https://user-images.githubusercontent.com/80368992/122092676-2a240100-ce45-11eb-9737-610a8fbfb5b0.PNG"><br/>
자신의 메시지, 다른 클라이언트의 메시지를 모두 읽을 수 있음<br/>

#### 4. 서버 측 채팅결과
<img width="306" alt="서버측참여자들의대화" src="https://user-images.githubusercontent.com/80368992/122092768-458f0c00-ce45-11eb-8e0c-f6a6caa28305.PNG"><br/><br/>


## 아쉬운 점 <br/>
현재 구현한 채팅 프로그램에 대해서 기간적인 면에서나 본인의 코딩 능력으로썬 최선을 다해 여러 자료를 참고하고 이해하면서 프로그램을 구현했기에 아쉬운 것은 그닥 없었던 것 같지만.. <br/>
그래도 제일 아쉬운 점은 아직 리눅스에 대한 공부나 코딩이 부족하기에 다음번엔 충분한 기간을 가지고서 여러 기능을 갖춘 재밌는 프로그램을 만들어보고싶다.<br/>
