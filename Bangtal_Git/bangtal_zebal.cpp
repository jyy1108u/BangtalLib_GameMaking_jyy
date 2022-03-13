#include <stdio.h>
#include <bangtal.h>

//변수설정
SceneID room1, room2, room3;
ObjectID Shoe, Mouse, Trap, Hole, Win, Pic, DoorOp, DoopOping, Sp, Lan, Wire, Ch, Wood, bag;
ObjectID Clock, win2, win3, win4, car, closet, closet2, memo, safe, axe, li1, li2;
bool sp_Ex = false; //스패너 중복획득 방지
bool axe_Ex = false; // 도끼 중복획득 방지
bool safeopen_2 = false; //금고분기
// bool Door1_closed = true; //필요없게 되었으나 흔적을 남겨 둠

//획득가능 아이템 변수설정



void mouseCallback1(ObjectID object, int x, int y, MouseAction action)
{
	//씬1 기본텍스트 출력
	if (object == Pic) {
		showMessage("멋진 액자에 걸린 그림이다."); //그림_방1
	} 
	else if (object == Win) {
		if (Sp == getHandObject()) {
			showMessage("못이 깊게 박혀있어 스패너로 빼내기는 어려워 보인다.");
		}
		else {
			showMessage("널빤지로 막혀있어 바깥이 보이지 않는다.");  //창문_방1
		}
	}
	else if (object == Hole) { //열쇠구멍
		if (Wire == getHandObject()) {
			showMessage("철사를 대충 끼워넣고 돌리니 벽이 열렸다!");
			DoorOp = createObject("images/DoorOpened.png");
			locateObject(DoorOp, room1, 686, 211);
			showObject(DoorOp);
		}
		else {
				showMessage("벽에 작은 구멍이 뚫려 있다."); // 철사 이외로 구멍을 클릭 시
		}
	} 
	else if (object == DoorOp) {
		showMessage("방 탈출 성공! \n . . . 인 줄 알았는데. . . \n [과제 : 바깥으로 탈출하라!]");
		enterScene(room2);
	}
	else if (object == Shoe) {
		showMessage("평범한 슬리퍼 한 짝. \n [슬리퍼를 획득하였습니다.]");
		pickObject(Shoe); //슬리퍼
	}
	else if (object == Lan) {
		if (sp_Ex) {
			showMessage("더 뒤져봐도 특별한 것은 나오지 않는다.");
		}
		else {
			showMessage("옷 더미를 뒤지자 무언가가 떨어졌다.");
			Sp = createObject("images/Spennar.png");
			locateObject(Sp, room1, 801, 9);
			showObject(Sp); //스패너 찾기
		}
	}
	else if (object == Mouse) {
		if (Shoe == getHandObject()) {
			showMessage("철썩! 슬리퍼로 때리자 쥐가 구멍 속으로 도망쳤다. \n 안쪽에 있던 덫을 밀치고 간 모양이다.");
			hideObject(Mouse);
			Trap = createObject("images/Trap.png");
			locateObject(Trap, room1, 323, 293);
			showObject(Trap); //슬리퍼 사용하고 쥐를 클릭 시 덫 등장

		}
		else {
			showMessage("...꽤 징그러운 쥐 한 마리가 있다."); //쥐를 단순클릭 시
		}

	}
	else if (object == Sp) {
		pickObject(Sp); //스패너 획득
		showMessage("[스패너를 획득하였습니다.]");
		hideObject(Sp);
		sp_Ex = true;
	}
	else if (object == Trap) {
		if (Sp == getHandObject()) {
			showMessage("쥐덫을 스패너로 분해했다. \n [철사를 획득하였습니다.] \n [나무판을 획득하였습니다.]\n [치즈를 획득하였습니다.]");
			hideObject(Trap);
			pickObject(Wire);
			pickObject(Wood);
			pickObject(Ch);
			defineCombination(Wire, Wood, Trap);
				//스패너로 쥐덫을 분해
		}
		else
		{
			showMessage("쥐에 의해 밀려나온 쥐덫이다. \n 꽤 영리한 쥐였던 모양이다."); //이외 아이템으로는 덫 분해 불가능.
		}
	}

	// 철사, 치즈, 나무판 Drop 시 다시 줍기 가능하도록...
	if (object == Wire) {
		pickObject(Wire);
		hideObject(Wire);
	}

	if (object == Wood) {
		pickObject(Wood);
		hideObject(Wood);
	}

	if (object == Ch) {
		pickObject(Ch);
		hideObject(Ch);
		showMessage("쥐덫에 걸려있던 치즈다.\n찝찝하니 다시 버리는 편이 나을지도 모르겠다.");
	}


	//방2 텍스트 출력

	if (object == Clock) {
		showMessage("평범한 벽시계이지만 바늘이 가지 않고 있다.");
	}
	else if (object == win2) {
		showMessage("이 창문 또한 널빤지로 막혀있다. \n 틈으로 바깥 불빛이 새어들어온다.");
	}
	else if (object == car) {
		showMessage("푹신한 양털 카펫이지만 들춰보아도 아무것도 없다. 이 방에 문이 있기는 한 걸까?");
	}
	else if (object == closet) {
		showMessage("옷장 문이 열렸다. 거대한 금고가 들어있다.");
		closet2 = createObject("images/closet2.png");
		memo = createObject("images/memo.png");
		locateObject(closet2, room2, 651, 189);
		locateObject(memo, room2, 737, 227);
		showObject(memo);
		showObject(closet2);
	}
	else if (object == memo) {
		showMessage("1009");
	}
	else if (object == closet2) {
		if (axe_Ex) {
			showMessage("안이 텅 비어있다.");
		}
		else if (safeopen_2) {
			if (Sp == getHandObject()) {
				showMessage("스패너로 금고 잠금장치를 부숴 열었다.");
				safe = createObject("images/safe.png");
				axe = createObject("images/axe.png");
				locateObject(safe, room2, 694, 317);
				locateObject(axe, room2, 715, 323);
				showObject(safe);
				showObject(axe);
			}
			else {
				showMessage("잠금장치를 부수기에 충분치 않은 것 같다.");
			}
		}
		else {
			showMessage("비밀번호를 입력했으나 잠금장치가 뒤틀려 열리지 않는다. \n 잠금장치를 부술 만한 무언가가 필요할 것 같다.");
			safeopen_2 = true;
			
		}
	}
	else if (object == axe) {
		showMessage("[도끼를 획득하였습니다.]");
		pickObject(axe);
		hideObject(axe);
		hideObject(win2);
		win3 = createObject("images/win3.png");
		locateObject(win3, room2, 233, 357);
		showObject(win3);
		axe_Ex = true;
	}
	else if (object == win3) {
		if (axe == getHandObject()) {
			showMessage("도끼로 널빤지를 부수자 환한 노을 풍경이 비친다.");
			li1 = createObject("images/lightroom.png");
			win4 = createObject("images/win4.png");
			locateObject(li1, room2, 0, 0);
			locateObject(Clock, room2, 13, 518);
			locateObject(win2, room2, 233, 357);
			locateObject(car, room2, 226, 34);
			locateObject(win4, room2, 233,357);
			showObject(li1);
			showObject(Clock);
			showObject(win2);
			showObject(car);
			showObject(win4);

		}
		else {
			showMessage("널빤지로 막힌 창문이다. 사이로 빛이 새어들어오고 있다.");
		}
	}
	else if (object == win4) {
		showMessage("창문이 손쉽게 열렸다.");
		li2 = createObject("images/lightroom2.png");
		locateObject(li2, room2, 0, 0);
		showObject(li2);
	}
	else if (object == li2) {
		showMessage("탈출 성공! 축하합니다! \n [x버튼을 눌러 게임을 종료하세요]");
		enterScene(room3);
	}
}



int main()
{
	room1 = createScene("첫 번째 방", "images/배경-1.png");
	room2 = createScene("두 번째 방", "images/배경-2.png");
	room3 = createScene("탈출!", "images/배경-3.png");

	//맵1 리소스파일
	Shoe = createObject("images/shoe.png");
	Mouse = createObject("images/Mouse.png");
	Hole = createObject("images/Hole.png");
	Win = createObject("images/Win.png");
	Pic = createObject("images/pic.png");
	Lan = createObject("images/Lan.png");
	Wire = createObject("images/Wire_Iron.png");
	Ch = createObject("images/Cheesse.png");
	Wood = createObject("images/WoodPlate.png");
	bag = createObject("images/InventoryButton.png");

	locateObject(Shoe, room1, 0, 0);
	locateObject(Mouse, room1, 310, 285);
	locateObject(Hole, room1, 855, 453);
	locateObject(Win, room1, 535, 380);
	locateObject(Pic, room1, 102, 438);
	locateObject(Lan, room1, 891, 90);

	showObject(Shoe);
	showObject(Mouse);
	showObject(Hole);
	showObject(Win);
	showObject(Pic);
	showObject(Lan);



	//맵2 리소스파일
	Clock = createObject("images/Clock.png");
	win2 = createObject("images/win2.png");
	car = createObject("images/car.png");
	closet = createObject("images/closet.png");
	closet2 = createObject("closet2.png");
	memo = createObject("images/memo.png");
	safe = createObject("images/safe.png");
	axe = createObject("images/axe.png");
	li1 = createObject("images/lightroom.png");
	li2 = createObject("images/lightroom2.png");

	locateObject(Clock, room2, 13, 518);
	locateObject(win2, room2, 233, 357);
	locateObject(car, room2, 226, 34);
	locateObject(closet, room2, 658, 219);

	showObject(Clock);
	showObject(win2);
	showObject(car);
	showObject(closet);




	setMouseCallback(mouseCallback1);


	//게임시작 멘트
	showMessage("어째서인지 이상한 방 안에 들어왔다. . . . . .\n[과제: 수상한 방에서 탈출하자!]");
	pickObject(bag);

	startGame(room1);
}
