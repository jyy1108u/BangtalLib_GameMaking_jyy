
#include <stdio.h>
#include <bangtal.h>

SceneID stage1, stage2; //스테이지별 장면 변수선언

//객체 변수
ObjectID Cciga1, Cciga2, Ccake1, Ccake2, Ci1, Ci2, Cn1, Cn2, Cw1, Cw2; //카드 객체 변수선언
ObjectID Cf1, Cf2, Cf3, Cf4, Cf5, Cf6, Cf7, Cf8, Cf9, Cf10; //카드뒷면 객체*10 변수선언
ObjectID ReplayB, ReplayB2, ExitB, ExitB2, StartB, palse, palse2, Scake, Sciga, Si,  Sn, Sw; // 기타객체
ObjectID Xcheck1, Xcheck2, Xcheck3, Check1, Check2, Check3, Check4, Check5; //표시관련 객체
ObjectID Gnormal, Gwrong; //캐릭터

// bool 변수 선언
bool RunGame = false; 
bool RestartOK = true; //스타트버튼 이중눌림 방지
bool Card_First = true; //직전에 뒤집힌 카드 유무 판정용
bool Timeover = false; //이벤트 시간 지연을 위한 변수
bool CardFlipStop = false; //카드 2장이상 뒤집기 방지
bool C1, C2, C3, C4, C5, C6, C7, C8, C9, C10 = false; //카드의 뒤집힘 여부
bool BC1_8, BC2_10, BC3_4, BC5_6, BC7_9 = false; //카드 짝맞춤 여부
bool Su1, Su2, Su3, Su4, Su5 = false; //게임 승리 변수
bool Heart1, Heart2 = false; //3번의 기회를 만드는 변수
bool G_normalSet = false;

//타이머 변수
TimerID timer1, timer2;


/* ---------------------------------------- */


//물체 생성 함수(예제 참조)
ObjectID CreateObject1(const char* Obimage, SceneID Scene, int x, int y)
{
	ObjectID object = createObject(Obimage);
	locateObject(object, Scene, x, y);
	return object;
}


//사용자 정의 함수 1 - 게임 옵션 설정하기(인벤토리, 메시지 창, 장면 이름 기능 on/off)
GameOption Setoption1(GameOption option, int num) {
	GameOption optionname = option;
	setGameOption(optionname, num);
	return optionname;
}


//사용자 정의 함수 2 - 게임오버 
void GameOver() {
	stopTimer(timer2);
	palse = CreateObject1("images/gameover.png", stage1, 0,0);
	ExitB = CreateObject1("images/gameover1.png", stage1, 547, 282);
	ReplayB = CreateObject1("images/gameover2.png", stage1, 506, 331);
	showObject(palse); showObject(ReplayB); showObject(ExitB);

}


//사용자 정의 함수 3 - 게임 승리
void GameClear() {
	if (Su1 && Su2 && Su3 && Su4 && Su5) {
		stopTimer(timer2);
		palse2 = CreateObject1("images/GC1.png", stage1, 0, 0);
		ExitB = CreateObject1("images/gameover1.png", stage1, 547, 282);
		ReplayB = CreateObject1("images/gameover2.png", stage1, 506, 331);
		showObject(palse2); showObject(ReplayB); showObject(ExitB);
	}
	else {

	}
}


//사용자 정의 함수 4 - 짝을 맞추지 못했을 때 원래대로 뒤집기
void WrongCardreturn() {
	if (C1) {
		hideObject(Cw1);
		C1 = false;

	}if (C2) {
		hideObject(Cciga1);
		C2 = false;

	}if (C3) {
		hideObject(Ccake1);
		C3 = false;

	}if (C4) {
		hideObject(Ccake2);
		C4 = false;

	}if (C5) {
		hideObject(Ci1);
		C5 = false;

	}if (C6) {
		hideObject(Ci2);
		C6 = false;

	}if (C7) {
		hideObject(Cn1);
		C7 = false;

	}if (C8) {
		hideObject(Cw2);
		C8 = false;

	}if (C9) {
		hideObject(Cn2);
		C9 = false;

	}if (C10) {
		hideObject(Cciga2); 
		C10 = false;
	}
	CardFlipStop = false;
}


//사용자 정의 함수 5 - 캐릭터 표시하는 함수 지정
void Character() {
	if (BC1_8) {
		setObjectImage(Gnormal, "images/G_Wine.png");
	}
	else if (BC2_10) {
		setObjectImage(Gnormal, "images/G_ciga.png");
	}
	else if (BC3_4) {
		setObjectImage(Gnormal, "images/G_cake.png");
	}
	else if (BC5_6) {
		setObjectImage(Gnormal, "images/G_phone.png");
	}
	else if (BC7_9) {
		setObjectImage(Gnormal, "images/G_note.png");
	}
	else if (G_normalSet) {
		hideObject(Gwrong);
		showObject(Gnormal);
		setObjectImage(Gnormal, "images/G_normal.png");
		G_normalSet = false;
	}
}


//사용자 정의 함수 6 - 짝 맞추기를 틀렸을 경우 하트를 차감하고, 3번 이후에는 게임오버
void HeartDown() {
	if (Heart1 && Heart2) {
		Xcheck3 = CreateObject1("images/X.png", stage1, 1130, 104);
		showObject(Xcheck3);
		GameOver();
	}
	else if (Heart1) {
		Xcheck2 = CreateObject1("images/X.png", stage1, 1080, 104);
		showObject(Xcheck2);
		Heart2 = true;
	}
	else {
		Xcheck1 = CreateObject1("images/X.png", stage1, 1038, 104);
		showObject(Xcheck1);
		Heart1 = true;
	}
}


// 사용자 정의 함수 7 - 카드가 틀렸을경우의 이벤트 실행
void WrongCardEvent() {
	if (Timeover) {
		Timeover = false;
		G_normalSet = true;
		Character();
		WrongCardreturn();
	}
	else {
		timer1 = createTimer(1.0f); //타이머 숨김처리
		startTimer(timer1);
		HeartDown();
		hideObject(Gnormal);
		showObject(Gwrong);

	}
}


//타이머 콜백 함수 (예제 참조)
void timerCallback(TimerID timer) {
	if (timer == timer1) {
		Timeover = true;
		WrongCardEvent();
	} //딜레이 시간 만드는 타이머
	
	if (timer == timer2) {
		GameOver();
	} //제한시간 만드는 타이머
}


//사용자 정의 함수 8 - 카드의 짝이 맞은경우 실루엣 제거 + 체크표시 + 캐릭터 변경 후 bool변수를 모두 초기화
void RightCardEvent() {
	
	if (BC1_8) {
		hideObject(Sw); 
		Check1 = CreateObject1("images/Check.png", stage1, 226, 103);
		showObject(Check1);
		Character();
		C1 = false;
		C8 = false;
		Card_First = true;
		CardFlipStop = false;
		BC1_8 = false;
		GameClear();
	}
	else if (BC2_10) {
		hideObject(Sciga);
		Check2 = CreateObject1("images/Check.png", stage1, 668, 103);
		showObject(Check2);
		Character();
		Cn2 = false;
		C10 = false;
		Card_First = true;
		CardFlipStop = false;
		BC2_10 = false;
		GameClear();
	}
	else if (BC3_4) {
		hideObject(Scake);
		Check3 = CreateObject1("images/Check.png", stage1, 342, 103);
		showObject(Check3);
		Character();
		C3 = false;
		C4 = false;
		Card_First = true;
		CardFlipStop = false;
		BC3_4 = false;
		GameClear();
	}
	else if (BC5_6) {
		hideObject(Si);
		Check4 = CreateObject1("images/Check.png", stage1, 816, 103);
		showObject(Check4);
		Character();
		C5 = false;
		C6 = false;
		Card_First = true;
		CardFlipStop = false;
		BC5_6 = false;
		GameClear();
	}
	else if (BC7_9) {
		hideObject(Sn);
		Check5 = CreateObject1("images/Check.png", stage1, 507, 103);
		showObject(Check5);
		Character();
		C7 = false;
		C9 = false;
		Card_First = true;
		CardFlipStop = false;
		BC7_9 = false;
		GameClear();
	}

}


// 사용자 정의 함수 9 - 짝패가 맞는지에 대한 확인
void GetCardcorrect() {
	if (C1 && C8) {
		BC1_8 = true;
		Su1 = true;
		RightCardEvent();
	}
	else if (C2 && C10) {
		BC2_10 = true;
		Su2 = true;
		RightCardEvent();
	}
	else if (C3 && C4) {
		BC3_4 = true;
		Su3 = true;
		RightCardEvent();
	}
	else if (C5 && C6) {
		BC5_6 = true;
		Su4 = true;
		RightCardEvent();
	}
	else if (C7 && C9) {
		BC7_9 = true;
		Su5 = true;
		RightCardEvent(); //맞은것을 처리하는 함수 실행
	}
	else {
		WrongCardEvent(); //틀린것을 처리하는 함수 실행
	}
}


//사용자 정의 함수 10 - 카드가 처음 뒤집혔는지/아닌지 판별.
void GetCardFirst() {
	if (Card_First) {
		Card_First = false;
	}
	else {
		Card_First = true;
		CardFlipStop = true;
		GetCardcorrect();
	}
}


//마우스콜백 함수(예제 참조)
void mouseCallback1(ObjectID object, int x, int y, MouseAction action) {

	//시작 버튼 누를시
	if (object == StartB) {
		if (RestartOK) {
			startTimer(timer2);
			RunGame = true;
			RestartOK = false;
		}
		else {

		}
	}

	//게임오버 - 리플레이 버튼
	if (object == ReplayB) {
		RunGame = false;
		setTimer(timer2, 60.0f);
		stopTimer(timer2);
		hideObject(palse); hideObject(palse2); hideObject(ReplayB); hideObject(ExitB);

		// 게임 초기화
		RestartOK = true;
		Card_First = true;
		Timeover = false; 
		CardFlipStop = false; 
		C1, C2, C3, C4, C5, C6, C7, C8, C9, C10 = false;
		BC1_8, BC2_10, BC3_4, BC5_6, BC7_9 = false;
		Su1 = false;
		Su2 = false;
		Su3 = false;
		Su4 = false;
		Su5 = false;
		Heart1 = false;
		Heart2 = false; 
		G_normalSet = true;

		Character();

		hideObject(Cciga1); 
		hideObject(Cciga2); 
		hideObject(Ccake1); 
		hideObject(Ccake2); 
		hideObject(Ci1);
		hideObject(Ci2); 
		hideObject(Cw1); 
		hideObject(Cw2); 
		hideObject(Cn1); 
		hideObject(Cn2);
		hideObject(Check1); hideObject(Check2); hideObject(Check3); hideObject(Check4); hideObject(Check5); 
		hideObject(Xcheck1); hideObject(Xcheck2); hideObject(Xcheck3);

		showObject(Scake); showObject(Sciga); showObject(Si); showObject(Sn); showObject(Sw);
		
	}

	//게임오버 - 게임 종료
	if (object == ExitB || ExitB2) {
		endGame();

	}


	if (RunGame) {
		//RunGame = true일 시에만 게임 플레이 가능

		if (CardFlipStop) {
			//stop될 시(딜레이시간 동안) 다른 카드 누르는 것 불가능.
		}
		else { // 10종의 카드 뒷면에 대해 각각 반응 지정
			if (object == Cf1) {
				Cw1 = CreateObject1("images/CW.png", stage1, 200, 424);
				showObject(Cw1);
				C1 = true;
				GetCardFirst();
			}
			else if (object == Cf2) {
				Cciga1 = CreateObject1("images/C_ciga.png", stage1, 355, 424);
				showObject(Cciga1);
				C2 = true;
				GetCardFirst();
			}
			else if (object == Cf3) {
				Ccake1 = CreateObject1("images/CC.png", stage1, 510, 424);
				showObject(Ccake1);
				C3 = true;
				GetCardFirst();
			}
			else if (object == Cf4) {
				Ccake2 = CreateObject1("images/CC.png", stage1, 665, 424);
				showObject(Ccake2);
				C4 = true;
				GetCardFirst();
			}
			else if (object == Cf5) {
				Ci1 = CreateObject1("images/CI.png", stage1, 820, 424);
				showObject(Ci1);
				C5 = true;
				GetCardFirst();
			}
			else if (object == Cf6) {
				Ci2 = CreateObject1("images/CI.png", stage1, 200, 186);
				showObject(Ci2);
				C6 = true;
				GetCardFirst();
			}
			else if (object == Cf7) {
				Cn1 = CreateObject1("images/CN.png", stage1, 355, 186);
				showObject(Cn1);
				C7 = true;
				GetCardFirst();
			}
			else if (object == Cf8) {
				Cw2 = CreateObject1("images/CW.png", stage1, 510, 186);
				showObject(Cw2);
				C8 = true;
				GetCardFirst();
			}
			else if (object == Cf9) {
				Cn2 = CreateObject1("images/CN.png", stage1, 665, 186);
				showObject(Cn2);
				C9 = true;
				GetCardFirst();
			}
			else if (object == Cf10) {
				Cciga2 = CreateObject1("images/C_ciga.png", stage1, 820, 186);
				showObject(Cciga2);
				C10 = true;
				GetCardFirst();
			}
		}

	}
	else {

	}
}


/*
<알고리즘 구현계획 메모>

카드 1개 뒤집은 뒤, 2번째로 뒤집을 시 bool을 트리거로 한 함수 활성되는 것 필요함. 
그러려면....카드를 뒤집는 콜백 이벤트 발생때마다 해당 함수를 호출한다. / 여기까지 마우스콜백에서....
만일 해당 카드가 처음으로 뒤집는 것이라면 판정 bool이 false인 상태에서 작동, bool을 true로 변경.
다음에 뒤집는 경우는 bool이 true가 되므로 / Get함수를 실행시켜 해당 카드가 짝이 맞는것인지에 대해 판정한다.
Get함수를 불러와서 조건문을 따라 내려가면서 패가 맞는지 확인하고, 
만일 맞는 것이 있다면 짝패의 카드 상태를 true로 만든다. / 겟함수 기능
그 다음에 카드 오브젝트 아이디를 받아오고!!!! true 상태가 된 짝패를 유지시켜주고, 
하단의 아이템 실루엣을 제거하고, 해당되는 이미지를 출력하는 함수를 받아온다!!

맞는 카드가 아닐 시........ 짝패의 카드 상태/첫두번째 판정 함수의 bool 연산자가 false가 되고 
이 부분은 마우스콜백의 이후 조건문으로 해결가능한것으로 생각됨.
하트에 대한 각각의 bool 연산자도 필요할것으로 생각된다.
이후 조건문의 반복되는 코드도 사용자 정의 함수를 이용해 간략화하는 것이 가능할것다.

게임을 끝내기 위해서는 모든 짝패의 bool = true가 되면 승리 모션을 띄워준다.

*/

// 카드정답 : 1-8, 2-10, 3-4, 5-6, 7-9


//Main 함수
int main()
{

	//옵션 설정 - 기능 비활성화
	GameOption OptionI = Setoption1(GameOption::GAME_OPTION_INVENTORY_BUTTON, 0);
	GameOption OptionM = Setoption1(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, 0);
	GameOption OptionT = Setoption1(GameOption::GAME_OPTION_ROOM_TITLE, 0);


	//스테이지 시작장면
	stage1 = createScene("Stage1", "images/배경-1.png");

	//리소스파일 로드 - 초기화면
	StartB = CreateObject1("images/StartButton.png", stage1, 2, 300);
	ReplayB = CreateObject1("images/ExitButton.png", stage1, 2, 0);
	Gnormal = CreateObject1("images/G_normal.png", stage1, 1049, 259);
	Gwrong = CreateObject1("images/G_wrong.png", stage1, 1005, 259);//기본버튼 및 캐릭터 표시
	
	Scake = CreateObject1("images/S_c.png", stage1, 395, 41);
	Sciga = CreateObject1("images/S_ciga.png", stage1, 717, 48);
	Si = CreateObject1("images/S_i.png", stage1, 857, 47);
	Sn = CreateObject1("images/S_n.png", stage1, 553, 45);
	Sw = CreateObject1("images/S_w.png", stage1, 273, 39); //실루엣 배치

	Cf1 = CreateObject1("images/C_flip.png", stage1, 200, 424);
	Cf2 = CreateObject1("images/C_flip.png", stage1, 355, 424);
	Cf3 = CreateObject1("images/C_flip.png", stage1, 510, 424);
	Cf4 = CreateObject1("images/C_flip.png", stage1, 665, 424);
	Cf5 = CreateObject1("images/C_flip.png", stage1, 820, 424);
	Cf6 = CreateObject1("images/C_flip.png", stage1, 200, 186);
	Cf7 = CreateObject1("images/C_flip.png", stage1, 355, 186);
	Cf8 = CreateObject1("images/C_flip.png", stage1, 510, 186);
	Cf9 = CreateObject1("images/C_flip.png", stage1, 665, 186);
	Cf10 = CreateObject1("images/C_flip.png", stage1, 820, 186); // 카드 뒷면 각자 배치


	showObject(StartB); showObject(Gnormal);
	showObject(Scake); showObject(Sciga); showObject(Si); showObject(Sn); showObject(Sw);
	showObject(Cf1); showObject(Cf2); showObject(Cf3); showObject(Cf4); showObject(Cf5);
	showObject(Cf6); showObject(Cf7); showObject(Cf8); showObject(Cf9); showObject(Cf10); //showobject 함수를 빼냄

	setMouseCallback(mouseCallback1);
	setTimerCallback(timerCallback);

	//타이머 제작
	timer2 = createTimer(60.0f);
	showTimer(timer2);

	//메시지 표시
	showMessage("시작하려면 start버튼을 클릭하세요.");

	startGame(stage1);

}

