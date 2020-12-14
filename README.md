# SystemPrograming Team5
#### 팀원: 이기섭, 조윤하, 권도윤
#### 주제: 경찰과 도둑 추격 게임

<br>
# 게임 실행법
1. LaunchGame폴더로 이동한다.
2. make -f makeserver.mk<br>make -f makepolice.mk<br>makethief.mk 를 입력하여 서버와 경찰과 도둑 클라이언트를 생성한다.

3. ./serv <포트번호> 를 입력하여 서버를 실행
4. ./thiefclient <서버 IP> <포트번호> 를 입력하여 도둑 입장
5. ./policeclient <서버 IP> <프트번호> 를 입력하여 경찰 입장 후 게임 시작 <br>(입장 순서는 달라도 됩니다)
6. 게임 play
