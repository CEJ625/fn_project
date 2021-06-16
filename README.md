# fn_project
멀티스레드 기반의 c언어 채팅 프로그램<br/>
201944052 B반 조은지<br/><br/><br/>

## 개요<br/><br/><br/>

## 목적<br/><br/><br/>

> 서버 코드 설명
#### 1. chat_serv 멀티스레드 명령어
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
main(int argc, char *argv[]) -> argc는 사용자가 입력한 인자값의 개수, *argv[]는 사용자가 입력한 문자열을 말한다 <br/><br/>이제 클라이언트와 연결하기 위해 소켓을 생성하고 읽어들이는 메서드인 tcp_listen(host, port, backlog)함수를 호출한다. [tct_listen함수](https://user-images.githubusercontent.com/80368992/122089826-1fb43800-ce42-11eb-9750-f5fa6a34d9f4.PNG)<br/>호출하고 반환된 소켓 값을 listen_Sock에 저장한다. <br/><br/>
pthread_create -> 스레드를 생성한다 <br/>
* pthread_create(&a_thread, null, thread_function,(void *)null) <br/>
pthread_create(생성되는 스레드의 번호를 받을 변수, 스레드 속성, 스레드가 생성되면서 실행할 변수, 실행할 함수에 들어갈 변수)<br/><br/>
* FD_ZERO, FD_SET 함수는 둘다 자신이 체크하고 싶은 소켓을 그룹에 포함시킨 후에 select 함수를 사용해야 한다.<br/> 
* fd_set은 파일 디스크립터 번호를 배열로 가지는 '비트 필드'구조체이다. <br/>
* select()는 입출력 다중화 구현이 가능하며 지정한 범위의 fd중 FD_SET으로 1로 set되어 있는 파일을 순회하면서 변화가 있는 fd를 반환한다. <br/>

FD_ZERO()함수로 read_fds의 메모리를 모두 초기화한다<br/>
FD_SET() 함수로 읽어들인 파일 디스크립터 중 listen_sock에 해당하는 비트를 1로 하고<br/>

참여자 수 만큼 for문을 통해 FD_SET()함수로 읽어온 파일 스크립터 주소를 채팅에 참가한 참여자를 소켓 목록(clisock_list)에 추가한다<br/>
관리하는 파일의 최대 개수(max_fdp)는 채팅 하는 동안 계속 재 계산된다. 파일의 개수는 최대 파일 지정 번호 + 1로 지정함 <br/> 
!!!!!!!!이부분 좀 설명 미흡함 <br/> 
select()함수로 변화가 있는 파일 디스크립터는 반환한다. <br/>만약 최대 파일 디스크립터 범위내에 검색한 fd가 0이하(변화 없음)라면 select에 실패한것이다 <br/>
만약 FD_ISSET(listen_sock, &read_fds) -> 읽어온 파일 디스크립터 중 소켓 listen_sock에 해당하는 비트가 set되어 있으면 해당 listen_sock을 반환한다 <br/>

#### 3. chat_serv 소켓 생성 및 읽기 
<img width="416" alt="서버분석5" src="https://user-images.githubusercontent.com/80368992/122089826-1fb43800-ce42-11eb-9750-f5fa6a34d9f4.PNG"><br/>

#### 4. chat_serv 참여자 추가 및 참여자 탈퇴 처리 
<img width="404" alt="서버분석4" src="https://user-images.githubusercontent.com/80368992/122089816-1d51de00-ce42-11eb-8e71-df7a5f70a5d1.PNG"><br/>



<br/><br/>
> 클라이언트 코드 설명
#### 1. chat_clnt 클라이언트 메인 
<img width="626" alt="클라이언트분석1" src="https://user-images.githubusercontent.com/80368992/122090541-d3b5c300-ce42-11eb-98c8-82ed4b10675e.PNG">

#### 2. chat_clnt 소켓 연결 
<img width="391" alt="클라이언트분석2" src="https://user-images.githubusercontent.com/80368992/122090556-d57f8680-ce42-11eb-85e7-88214720f6ea.PNG">

<br/><br/>
> 실행 

#### 1. 클라이언트와 서버 통신 성공 (클라이언트)
<img width="278" alt="클라이언트사용방법" src="https://user-images.githubusercontent.com/80368992/122092199-9d794300-ce44-11eb-8ddd-cf996f5c0648.PNG">

#### 2. 클라이언트와 서버 통신 성공 (서버)
<img width="367" alt="클라이언트연결완료시서버측" src="https://user-images.githubusercontent.com/80368992/122092204-9eaa7000-ce44-11eb-8c90-bb5d015267ed.PNG">

#### 3. 클라이언트 측 채팅결과 
<img width="315" alt="참여자모두에게채팅방송" src="https://user-images.githubusercontent.com/80368992/122092676-2a240100-ce45-11eb-9737-610a8fbfb5b0.PNG">

#### 4. 서버 측 채팅결과
<img width="306" alt="서버측참여자들의대화" src="https://user-images.githubusercontent.com/80368992/122092768-458f0c00-ce45-11eb-8e0c-f6a6caa28305.PNG">


<br/><br/><br/>## 아쉬운 
