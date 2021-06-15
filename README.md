# fn_project
멀티스레드 기반의 c언어 채팅 프로그램<br/>
201944052 B반 조은지<br/><br/><br/>


> 서버 코드 설명.
#### 1. chat_serv 변수선언 
<img width="310" alt="서버분석1" src="https://user-images.githubusercontent.com/80368992/122088213-7fa9df00-ce40-11eb-8cfc-5e9663cc0ab1.PNG">

#### 2. chat_serv 서버 제공 멀티스레드 명령어
<img width="469" alt="서버분석2" src="https://user-images.githubusercontent.com/80368992/122089512-cb10bd00-ce41-11eb-9876-571d5f69699d.PNG">

#### 3. chat_serv 서버 메인 
<img width="477" alt="서버분석3" src="https://user-images.githubusercontent.com/80368992/122089807-1aef8400-ce42-11eb-8ec4-a65160917c3c.PNG">

#### 4. chat_serv 참여자 추가 및 참여자 탈퇴 처리 
<img width="404" alt="서버분석4" src="https://user-images.githubusercontent.com/80368992/122089816-1d51de00-ce42-11eb-8e71-df7a5f70a5d1.PNG">

#### 5. chat_serv 소켓 생성 및 읽기 
<img width="416" alt="서버분석5" src="https://user-images.githubusercontent.com/80368992/122089826-1fb43800-ce42-11eb-9750-f5fa6a34d9f4.PNG">

<br/><br/>
> 클라이언트 코드 설명.
#### 1. chat_clnt 클라이언트 메인 
<img width="626" alt="클라이언트분석1" src="https://user-images.githubusercontent.com/80368992/122090541-d3b5c300-ce42-11eb-98c8-82ed4b10675e.PNG">

#### 2. chat_clnt 소켓 연결 
<img width="391" alt="클라이언트분석2" src="https://user-images.githubusercontent.com/80368992/122090556-d57f8680-ce42-11eb-85e7-88214720f6ea.PNG">

<br/><br/>
> 실행결과

#### 1. 클라이언트와 서버 통신 성공 (클라이언트)
<img width="278" alt="클라이언트사용방법" src="https://user-images.githubusercontent.com/80368992/122092199-9d794300-ce44-11eb-8ddd-cf996f5c0648.PNG">

#### 2. 클라이언트와 서버 통신 성공 (서버)
<img width="367" alt="클라이언트연결완료시서버측" src="https://user-images.githubusercontent.com/80368992/122092204-9eaa7000-ce44-11eb-8c90-bb5d015267ed.PNG">

#### 3. 클라이언트 측 채팅결과 
<img width="315" alt="참여자모두에게채팅방송" src="https://user-images.githubusercontent.com/80368992/122092676-2a240100-ce45-11eb-9737-610a8fbfb5b0.PNG">

#### 4. 서버 측 채팅결과
<img width="306" alt="서버측참여자들의대화" src="https://user-images.githubusercontent.com/80368992/122092768-458f0c00-ce45-11eb-8e0c-f6a6caa28305.PNG">

