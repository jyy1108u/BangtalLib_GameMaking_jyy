
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <bangtal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

SceneID gamemain;

SoundID bgmMain, bgm, right, wrong, door, mes, done, cilck0;

ObjectID HTPWIN, HTPButton;
bool cilck1, cilck2 = false;

ObjectID ballonC, feel_Doll, OrderDoll;
ObjectID phone, OrderDollp, feel_Dollp, ballonP;
ObjectID customer;
ObjectID Metarial, xbutton;
ObjectID recipe, recipewin, xbutton2;
ObjectID Wokers[6];
ObjectID Selected[10];
ObjectID Selected_pro[6];

ObjectID Fabrics[10];
ObjectID Special1[10];
ObjectID Special2[10];

ObjectID gameover, replayg, maing, exitg;

TimerID gamemainTimer;
TimerID cusCome, cusMove1, cusMove2, cusMove3, cusMove4, feelTimer; //손님
TimerID OrdertimeP, feeltimerP; //전화
TimerID Dowork[6], Dowork2[6]; //직원
TimerID Worktimer[6]; //직원 애니메이팅 타임

bool maingamerun = true;
bool customerIN, CustomerWalking = false;
bool phoneON = false;
bool selectedF, selectedS1, selectedS2 = false;
bool selected_Pro = false;

bool WhoisWork[6] = { false, false, false, false, false, false };
bool DoAnimate[6] = { false, false, false, false, false, false }; //노동 애니메이팅 타임

int customX = 30;
int RandomDoll = 0; //랜덤인형 번호 저장_오더판별에 사용_손님용
int RandomDollp = 0; //랜덤인형 번호 저장_오더판별에 사용_전화용
int initWorkerX[6] = {1083, 1015, 1084, 856, 796, 865};
int initWorkerY[6] = { 525, 312, 141, 525, 312, 141 }; //직원 초기좌표
int Wx[6] = { 1062, 1015, 1084, 836, 799, 865 };
int Wy[6] = { 510, 312, 124, 511, 313, 124 }; //직원 애니메이팅 좌표

int money = 0; //수익
int buf[6]; //수익창 숫자 저장

char cusName1[20]; char cusName2[20]; char cusName3[20]; char cusName4[20]; char cusName5[20]; //손님 애니메이팅에 사용
char DollRandflie[20]; char DollRandflie2[20]; //인형 랜덤생성저장

// ---------- 로비

ObjectID Lobbyscreen;
ObjectID HowToPlay;
ObjectID Startbutton;

// 인형 생성용-------------------------------

int MakingLine1[5]; int MakingLine2[5]; //인형 값 저장할 배열 2개
bool DonePreTask1[3] = { false, false, false }; //란국라인 작업상태
bool DonePreTask2[3] = { false, false, false }; //블린라인 작업상태
bool WhentoSave[2] = { false, false }; //라인 구별용..

ObjectID Febline[4];
ObjectID Spc1line[4];
ObjectID Spc2line[2]; //작업표시줄 표시될 것 저장

ObjectID ReadyMade[4]; //작업표시줄 인형
int ReadyMadeID[2] = { 0,0 }; //아이디 중간저장용 슬롯
bool WhereisDoll[2] = { false, false }; //어디에 인형이 표시될지..

ObjectID product[6]; //인형 슬롯 저장
int productid[6]; //저장된 인형 고유번호...
bool IsitSelected[6] = { false, false, false, false, false, false };
bool SlotFilled[6] = { false, false, false, false, false, false }; //슬롯 빈칸 배열
char Dollimage1[20]; char Dollimage2[20]; //파일명 저장용

struct DollList {
    int p1; int p2; int p3; int p4; int p5;
};

DollList TD[11] = { //TheDoll

    { 3, 10, 3, 9, 2 }, //란국인형_1번
    { 9, 10, 4, 9, 1}, //블린인형_2번
    { 4, 10, 2, 10, 3}, //만두인형_3번
    { 5, 10, 1, 3, 3 }, //앵무인형_4번
    { 1, 6, 4, 5, 3}, //커비인형_5번
    { 2, 3, 1, 3, 1 }, //오리인형_6번
    { 1, 5, 2, 9, 4 }, //스페셜 7번_준
    { 7, 8, 4, 6, 1 }, //스페셜 8번_너구리
    { 3, 9, 6, 8, 3 }, //스페셜 9번_영철
    { 3, 6, 7, 10, 1 }, //스페셜 10번_하울
    { 3, 7, 7, 8, 4 } //스페셜 11번_라푼젤
};

//------------------


//오브젝트 생성
ObjectID CreateObject1(const char* Obimage, SceneID Scene, int x, int y) {
    ObjectID object = createObject(Obimage);
    locateObject(object, Scene, x, y);
    return object;
}


//money에 저장된 숫자를 하나씩 분리해 저장
void countnum() {
    int moneySP = money;
    int moneyleft;

    for (int i = 0; i < 6; i++) {
        moneyleft = moneySP % 10;
        moneySP = moneySP / 10;

        buf[5 - i] = moneyleft;
    }
}


//수익 카운터 화면에 표시
void ShowhowMoney() { 
    for (int i = 0; i < 6; i++) {
        char images[20];
        sprintf_s(images, "images/T%d.png", buf[i]);
        ObjectID obj = CreateObject1(images, gamemain, 775 + 50 * i, 646);
        showObject(obj);
    }
}


//수익카운터 실행함수
void RevenueCount() {
    countnum();
    ShowhowMoney();
}


// 이미지 애니메이팅_Animate 1, 2, timeCallback 연결
TimerID Animate(ObjectID obj, const char* flie, SceneID scene, int x, int y, int n, bool move, Second sec) {

    setObjectImage(obj, flie); //프레임 변경할 이미지
    if (move) {
        customX = customX + n;
        locateObject(obj, scene, x, y); //애니메이팅 이동 시
    }

    TimerID timer = createTimer(sec); //프레임 간격설정
    startTimer(timer);
    return timer;
}


TimerID Animate2(ObjectID obj, const char* flie, SceneID scene, int x, int y, int n, bool move, Second sec) {

    setObjectImage(obj, flie);
    if (move) {
        customX = customX + n;
        locateObject(obj, scene, x, y);
    }

    TimerID timer = createTimer(sec);
    startTimer(timer);
    return timer;
}


// 랜덤으로 손님을 생성 후 애니메이션 재생_입장
void MakeCustomerCome() {

    CustomerWalking = true;
    srand(time(NULL));

    int random = (rand() % 8) + 1;

    sprintf_s(cusName1, "images/c%d.png", random);
    sprintf_s(cusName2, "images/c%d_1.png", random);
    sprintf_s(cusName3, "images/c%d_2.png", random);
    sprintf_s(cusName4, "images/c%d_3.png", random);
    sprintf_s(cusName5, "images/c%d_4.png", random);

    door = createSound("audios/door.mp3");
    playSound(door, false);

    setObjectImage(customer, cusName1);
    cusCome = createTimer(0.5f);
    startTimer(cusCome);

}


// 핸드폰 주문생성_대기
void MakephoneOrder() {

    setObjectImage(phone, "images/phone_off.png"); //휴대폰 끄기
    OrdertimeP = createTimer(4.0f);
    startTimer(OrdertimeP);
}


//핸드폰 주문생성_끄기
void Makephonefeelwait() {

    feeltimerP = createTimer(1.0f);
    startTimer(feeltimerP);
}


// 랜덤으로 인형 오더 생성1
void RandOrderDoll() {

    srand(time(NULL));
    int random = (rand() % 11) + 1;
    RandomDoll = random;
    sprintf_s(DollRandflie, "images/d%d.png", random);
}

void RandOrderDoll2() {

    srand(time(NULL));
    int random2 = (rand() % 11) + 1;
    RandomDollp = random2;
    sprintf_s(DollRandflie2, "images/d%d.png", random2);
}


//작업 완료된 인형 구별하고 표시하기
void WhatwasMade() {

    playSound(done, false); //인형 완성 효과음
    
    if (WhereisDoll[0]) { //1라인 인형 저장

        ReadyMade[2] = CreateObject1("images/d12.png", gamemain, 698, 98);
        showObject(ReadyMade[2]); hideObject(Spc2line[0]); //틀린 레시피 입력 시

        for (int i = 0; i < 11; i++) {
            
            if (TD[i].p1 == MakingLine1[0] && TD[i].p2 == MakingLine1[1] && TD[i].p3 == MakingLine1[2]
                && TD[i].p4 == MakingLine1[3] && TD[i].p5 == MakingLine1[4]) {
                hideObject(ReadyMade[2]);
                sprintf(Dollimage1, "images/d%d.png", i+1);
                ReadyMade[0] = CreateObject1(Dollimage1, gamemain, 698, 98);
                showObject(ReadyMade[0]); hideObject(Spc2line[0]); //레시피에 맞는 인형 제작
                ReadyMadeID[0] = i + 1;
                break;
            }
        }

        WhereisDoll[0] = false;
    }
    else if (WhereisDoll[1]) { //2라인 인형 저장

        ReadyMade[3] = CreateObject1("images/d12.png", gamemain, 624, 98);
        showObject(ReadyMade[3]); hideObject(Spc2line[1]); //틀린 레시피 입력 시

        for (int i = 0; i < 11; i++) {

            if (TD[i].p1 == MakingLine2[0] && TD[i].p2 == MakingLine2[1] && TD[i].p3 == MakingLine2[2]
                && TD[i].p4 == MakingLine2[3] && TD[i].p5 == MakingLine2[4]) {
                hideObject(ReadyMade[3]);
                sprintf(Dollimage2, "images/d%d.png", i+1);
                ReadyMade[1] = CreateObject1(Dollimage2, gamemain, 624, 98);
                showObject(ReadyMade[1]); hideObject(Spc2line[1]); //레시피에 맞는 인형 제작
                ReadyMadeID[1] = i + 1;
                break;
            }
        }

        WhereisDoll[1] = false;
    }
}


// 생산된 인형 저장하는 슬롯
void SaveDolls(int n, int doll_ID) {

    for (int i = 0; i < 6; i++) {
        if (!SlotFilled[i]) {
            char image[20];
            
            if (n == 0) strcpy(image, Dollimage1);
            else strcpy(image, Dollimage2);
            
            product[i] = CreateObject1(image, gamemain, 57 + (84 * i), 55);
            showObject(product[i]);
            productid[i] = doll_ID; //인형 아이디 저장.
            SlotFilled[i] = true;
            break;
        }
    }
}


//손님 애니메이션 재생_퇴장
void MakeCustomerOUT() {
    cusMove3 = Animate(customer, cusName2, gamemain, customX, 338, -10, true, 0.1f);
    CustomerWalking = true;
}


void timerCallback1(TimerID timer) {


    //전화 주문받기
    if (timer == OrdertimeP) {

        RandOrderDoll2(); //랜덤오더 생성기
        
        setObjectImage(phone, "images/phone_on.png"); //휴대폰 켜기
        setObjectImage(ballonP, "images/ballp.png");
        setObjectImage(OrderDollp, DollRandflie2);

        mes = createSound("audios/message.mp3");
        playSound(mes, false);

        phoneON = true;
    }

    if (timer == feeltimerP) {

        setObjectImage(phone, "images/phone_off.png"); //휴대폰 끄기
        setObjectImage(ballonP, "images/ball_N.png");
        setObjectImage(feel_Dollp, "images/ball_N.png"); //감정 제거
        MakephoneOrder(); //휴대폰 오더 대기시키기
    }


    // 손님 애니메이팅 _ 시작
    if (timer == cusCome) {
       cusMove1 = Animate(customer, cusName5, gamemain, 30, 338, 3, true, 0.2f);
    }


    //손님 애니메이팅2_좌표이동_들어오기
    if (timer == cusMove1) {
        if (customX > 160) {

            RandOrderDoll(); //랜덤오더 생성기

            setObjectImage(customer, cusName1);
            setObjectImage(ballonC, "images/ballc.png");
            setObjectImage(OrderDoll, DollRandflie);
            CustomerWalking = false;
            customerIN = true;
        }
        else cusMove2 = Animate2(customer, cusName4, gamemain, customX, 338, 10, true, 0.1f);
    }
    else if (timer == cusMove2) {
        cusMove1 = Animate(customer, cusName5, gamemain, customX, 338, 10, true, 0.1f);
    }

    else if (timer == feelTimer) { //퇴장시 감정표현 삭제
        setObjectImage(feel_Doll, "images/ball_N.png");
        setObjectImage(ballonC, "images/ball_N.png");
        MakeCustomerOUT();
    }

    else if (timer == cusMove3) { //퇴장 애니메이션 재생
        if (customX < 30) {
            MakeCustomerCome(); //나가자마자 새로운 손님 들여오기.

        }
        else cusMove4 = Animate2(customer, cusName3, gamemain, customX, 338, -10, true, 0.1f);
    }
    else if (timer == cusMove4) {
        cusMove3 = Animate(customer, cusName2, gamemain, customX, 338, -10, true, 0.1f);
    }


    // 직원 애니메이팅_작업
    for (int i = 0; i < 6; i++) {


        if (timer == Worktimer[i]) { //직원 애니메이팅 종료
            if (i == 2 or i == 5) {
                WhatwasMade();
            }
            DoAnimate[i] = false;
            
        }

        if (timer == Dowork[i]) {
            if (DoAnimate[i]) {
                if (i < 3) {
                    char fliename[30];
                    sprintf_s(fliename, "images/eggwork%d_2.png", i + 1);
                    Dowork2[i] = Animate2(Wokers[i], fliename, gamemain, Wx[i], Wy[i], 0, false, 0.2f);
                }
                else {
                    char fliename[30];
                    sprintf_s(fliename, "images/binwork%d_2.png", i - 2);
                    Dowork2[i] = Animate2(Wokers[i], fliename, gamemain, Wx[i], Wy[i], 0, false, 0.2f);
                }
            }
            else {
                locateObject(Wokers[i], gamemain, initWorkerX[i], initWorkerY[i]);
                if (i < 3) {
                    char fliename[30];
                    sprintf_s(fliename, "images/eggwork%d.png", i + 1);
                    setObjectImage(Wokers[i], fliename);
                }
                else {
                    char fliename[30];
                    sprintf_s(fliename, "images/binwork%d.png", i - 2);
                    setObjectImage(Wokers[i], fliename);
                }
            }
        }

        else if (timer == Dowork2[i]) {
            if (DoAnimate[i]) {
                if (i < 3) {
                    char fliename[30];
                    sprintf_s(fliename, "images/eggwork%d_1.png", i + 1);
                    Dowork[i] = Animate(Wokers[i], fliename, gamemain, Wx[i], Wy[i], 0, false, 0.2f);
                }
                else {
                    char fliename[30];
                    sprintf_s(fliename, "images/binwork%d_1.png", i - 2);
                    Dowork[i] = Animate(Wokers[i], fliename, gamemain, Wx[i], Wy[i], 0, false, 0.2f);
                }
            }
            else {
                locateObject(Wokers[i], gamemain, initWorkerX[i], initWorkerY[i]);
                if (i < 3) {
                    char fliename[30];
                    sprintf_s(fliename, "images/eggwork%d.png", i + 1);
                    setObjectImage(Wokers[i], fliename);
                }
                else {
                    char fliename[30];
                    sprintf_s(fliename, "images/binwork%d.png", i - 2);
                    setObjectImage(Wokers[i], fliename);
                }
            }
        }
    }

    if (timer == gamemainTimer) { //게임 오버

        maingamerun = false;

        stopSound(bgm);
        SoundID endbgm = createSound("audios/gameend.mp3");
        SoundID money = createSound("audios/money.mp3");
        playSound(endbgm, false); playSound(money, false);

        gameover = CreateObject1("images/gameover.png", gamemain, 0, 0);
        //maing = CreateObject1("images/gameoverMa.png", gamemain, 400, 264);
        exitg = CreateObject1("images/gameoverEx.png", gamemain, 800, 264);
        showObject(gameover); showObject(exitg);

        for (int i = 0; i < 6; i++) { //최종 카운터 표시
            char images[20];
            sprintf_s(images, "images/o%d.png", buf[i]);
            ObjectID obj = CreateObject1(images, gamemain, 642 + 21 * i, 384);
            showObject(obj);
        }

    }
}

void MakeThemwork() { //직원들 애니메이팅 호출하는 함수
    for (int i = 0; i < 6; i++) {

        if (WhoisWork[i]) { // 작업공 애니메이팅 작동

            WhoisWork[i] = false;
            DoAnimate[i] = true;

            Worktimer[i] = createTimer(2.0f);
            startTimer(Worktimer[i]);

            char fliename[30];
            if (i < 3) { //란국
                sprintf_s(fliename, "images/eggwork%d_1.png", i + 1);
                locateObject(Wokers[i], gamemain, Wx[i], Wy[i]);
                Dowork[i] = Animate(Wokers[i], fliename, gamemain, Wx[i], Wy[i], 0, false, 0.2f);
            }
            else { //블린
                sprintf_s(fliename, "images/binwork%d_1.png", i - 2);
                locateObject(Wokers[i], gamemain, Wx[i], Wy[i]);
                Dowork[i] = Animate(Wokers[i], fliename, gamemain, Wx[i], Wy[i], 0, false, 0.2f);
            }

        }
    }
}


// 천 띄우는 함수
void ShowFabrics() {
    char nameFabrics[20];
    for (int i = 1; i < 6; i++) {
        sprintf_s(nameFabrics, "images/f%d.png", i);
        Fabrics[i-1] = CreateObject1(nameFabrics, gamemain, 108 + (119 * (i - 1)), 479);
        Selected[i - 1] = CreateObject1("images/select.png", gamemain, 83 + (118 * (i - 1)), 462);
        showObject(Fabrics[i-1]);
    }
    for (int i = 6; i < 11; i++) {
        sprintf_s(nameFabrics, "images/f%d.png", i);
        Fabrics[i-1] = CreateObject1(nameFabrics, gamemain, 108 + (119 * (i - 6)), 382);
        Selected[i - 1] = CreateObject1("images/select.png", gamemain, 83 + (118 * (i - 6)), 361);
        showObject(Fabrics[i-1]);
    }
}


//2번째 부자재
void ShowSpc1() {
    char nameSpc1[20];
    for (int i = 1; i < 6; i++) {
        sprintf_s(nameSpc1, "images/m%d.png", i);
        Special1[i - 1] = CreateObject1(nameSpc1, gamemain, 99 + (118 * (i - 1)), 330);
        Selected[i - 1] = CreateObject1("images/select.png", gamemain, 83 + (118 * (i - 1)), 317);
        showObject(Special1[i - 1]);
    }
    for (int i = 6; i < 11; i++) {
        sprintf_s(nameSpc1, "images/m%d.png", i);
        Special1[i - 1] = CreateObject1(nameSpc1, gamemain, 99 + (119 * (i - 6)), 233);
        Selected[i - 1] = CreateObject1("images/select.png", gamemain, 83 + (118 * (i - 6)), 216);
        showObject(Special1[i - 1]);
    }
}


//3번째 부자재
void ShowSpc2() {
    char nameSpc2[20];
    for (int i = 1; i < 5; i++) {
        sprintf_s(nameSpc2, "images/n%d.png", i);
        Special2[i - 1] = CreateObject1(nameSpc2, gamemain, 99 + (118 * (i - 1)), 150);
        Selected[i - 1] = CreateObject1("images/select.png", gamemain, 83 + (118 * (i - 1)), 137);
        showObject(Special2[i - 1]);
    }
}


//인형 제작 라인 관리함수_작업줄 표시

void MakingDoll_Line2() { //블린라인전용

    if (!DonePreTask2[0]) { //2라인 천 선택이 끝나면...

        char image[20]; char image2[20];
        sprintf(image, "images/A%d.png", MakingLine2[0]);
        sprintf(image2, "images/A%d.png", MakingLine2[1]);
        Febline[2] = CreateObject1(image, gamemain, 627, 439);
        Febline[3] = CreateObject1(image2, gamemain, 655, 439);
        showObject(Febline[2]); showObject(Febline[3]);
        DonePreTask2[0] = true;
    }
    else if (!DonePreTask2[1]) { //2라인 부자재1 선택이 끝나면...

        char image[20]; char image2[20];
        sprintf(image, "images/B%d.png", MakingLine2[2]);
        sprintf(image2, "images/B%d.png", MakingLine2[3]);
        Spc1line[2] = CreateObject1(image, gamemain, 625, 285);
        Spc1line[3] = CreateObject1(image2, gamemain, 655, 285);
        showObject(Spc1line[2]); showObject(Spc1line[3]);
        DonePreTask2[1] = true;
    }
    else if (!DonePreTask2[2]) { //2라인 부자재2 선택이 끝나면...

        char image[20];
        sprintf(image, "images/CK%d.png", MakingLine2[4]);
        Spc2line[1] = CreateObject1(image, gamemain, 626, 111);
        showObject(Spc2line[1]);
        DonePreTask2[2] = true;
        WhereisDoll[1] = true;
    }
    else; //셋 모두 참일 경우 초기화 전까지 작업 x

}


void MakingDoll_Line1(){ //란국라인전용

    if (!DonePreTask1[0]) { //1라인 천 선택이 끝나면...

        char image[20]; char image2[20];
        sprintf(image, "images/A%d.png", MakingLine1[0]);
        sprintf(image2, "images/A%d.png", MakingLine1[1]);
        Febline[0] = CreateObject1(image, gamemain, 701, 439);
        Febline[1] = CreateObject1(image2, gamemain, 729, 439);
        showObject(Febline[0]); showObject(Febline[1]);
        DonePreTask1[0] = true;
    }
    else if (!DonePreTask1[1]) { //1라인 부자재1 선택이 끝나면...

        char image[20]; char image2[20];
        sprintf(image, "images/B%d.png", MakingLine1[2]);
        sprintf(image2, "images/B%d.png", MakingLine1[3]);
        Spc1line[0] = CreateObject1(image, gamemain, 700, 285);
        Spc1line[1] = CreateObject1(image2, gamemain, 730, 285);
        showObject(Spc1line[0]); showObject(Spc1line[1]);
        DonePreTask1[1] = true;
    }
    else if (!DonePreTask1[2]) { //1라인 부자재2 선택이 끝나면...

        char image[20];
        sprintf(image, "images/CK%d.png", MakingLine1[4]);
        Spc2line[0] = CreateObject1(image, gamemain, 701, 111);
        showObject(Spc2line[0]);
        DonePreTask1[2] = true;
        WhereisDoll[0] = true;
    }
    else; //거짓일 경우 아무것도 x


}


//마우스콜백
void mouseCallack1(ObjectID obj, int x, int y, MouseAction act) {

    playSound(cilck0, false); //클릭 시마다 효과음

    if (maingamerun) { //게임 메인요소 동작함수
        
        if (!SlotFilled[0] || !SlotFilled[1] || !SlotFilled[2] || !SlotFilled[3] || !SlotFilled[4] || !SlotFilled[5]) { 
            //최소한 한칸이 비어있어야 내릴 수 있음..

            if (obj == ReadyMade[0] || obj == ReadyMade[2]) {
                for (int i = 0; i < 5; i++) {
                    if (i < 3) {
                        DonePreTask1[i] = false;
                    }
                    if (i < 2) {
                        hideObject(Febline[i]);
                        hideObject(Spc1line[i]);
                    }
                    MakingLine1[i] = 0;
                }

                if (obj == ReadyMade[0]) {
                    SaveDolls(0, ReadyMadeID[0]);
                }

                hideObject(Spc2line[0]);
                hideObject(ReadyMade[0]);
                hideObject(ReadyMade[2]);

            }

            if (obj == ReadyMade[1] || obj == ReadyMade[3]) {
                for (int i = 0; i < 5; i++) {
                    if (i < 3) {
                        DonePreTask2[i] = false;
                    }
                    if (i < 2) {
                        hideObject(Febline[i + 2]);
                        hideObject(Spc1line[i + 2]);
                    }
                    MakingLine2[i] = 0;
                }

                if (obj == ReadyMade[1]) {
                    SaveDolls(1, ReadyMadeID[1]);
                }

                hideObject(Spc2line[1]);
                hideObject(ReadyMade[1]);
                hideObject(ReadyMade[3]);
            }
        }

          
        if (obj == Wokers[0] && !DoAnimate[0]) { //도면공 1,2

            if (!DonePreTask1[0]) { //1라인 아무것도 작업하지 않았을 때에만 가능

                Metarial = CreateObject1("images/meta.png", gamemain, 53, 345);
                xbutton = CreateObject1("images/xb.png", gamemain, 652, 568);
                showObject(Metarial); showObject(xbutton);
                ShowFabrics();
                maingamerun = false;
                WhoisWork[0] = true; WhentoSave[0] = true; //란국
                
            }
            
            
        }
        else if (obj == Wokers[3] && !DoAnimate[3]) {
            if (!DonePreTask2[0]) { //2라인 아무것도 작업하지 않았을 때에만 가능

                Metarial = CreateObject1("images/meta.png", gamemain, 53, 345);
                xbutton = CreateObject1("images/xb.png", gamemain, 652, 568);
                showObject(Metarial); showObject(xbutton);
                ShowFabrics();
                maingamerun = false;
                WhoisWork[3] = true; WhentoSave[1] = true; //블린

            }

        }
        else if (obj == Wokers[1] && !DoAnimate[1]) { //재봉공 1,2
            
            if (DonePreTask1[0] && !DonePreTask1[1]) { //천 작업이 완료되어서 차 있을 때에만 가능.

                Metarial = CreateObject1("images/meta.png", gamemain, 53, 200);
                xbutton = CreateObject1("images/xb.png", gamemain, 652, 423);
                showObject(Metarial); showObject(xbutton);
                ShowSpc1();
                maingamerun = false;
                WhoisWork[1] = true; WhentoSave[0] = true;

            }
            
        }
        else if (obj == Wokers[4] && !DoAnimate[4]) {

            if (DonePreTask2[0] && !DonePreTask2[1]) { //천 작업이 완료되어서 차 있을 때에만 가능.

                Metarial = CreateObject1("images/meta.png", gamemain, 53, 200);
                xbutton = CreateObject1("images/xb.png", gamemain, 652, 423);
                showObject(Metarial); showObject(xbutton);
                ShowSpc1();
                maingamerun = false;
                WhoisWork[4] = true; WhentoSave[1] = true;

            }
        }
        else if (obj == Wokers[2] && !DoAnimate[2]) { //마무리공
            
            if (DonePreTask1[0] && DonePreTask1[1] && !DonePreTask1[2]) { //천이랑 부자재 1 작업이 둘다 되어있을떄만 가능
                Metarial = CreateObject1("images/meta.png", gamemain, 53, 20);
                xbutton = CreateObject1("images/xb.png", gamemain, 652, 243);
                showObject(Metarial); showObject(xbutton);
                ShowSpc2();
                maingamerun = false;
                WhoisWork[2] = true; WhentoSave[0] = true;
            }
            
        }
        else if (obj == Wokers[5] && !DoAnimate[5]) {
            
            if (DonePreTask2[0] && DonePreTask2[1] && !DonePreTask2[2]) { //천이랑 부자재 1 작업이 둘다 되어있을떄만 가능
                Metarial = CreateObject1("images/meta.png", gamemain, 53, 20);
                xbutton = CreateObject1("images/xb.png", gamemain, 652, 243);
                showObject(Metarial); showObject(xbutton);
                ShowSpc2();
                maingamerun = false;
                WhoisWork[5] = true; WhentoSave[1] = true;
            }
        }


        for (int i = 0; i < 6; i++) { //product 인형 처리
            if (obj == product[i]) {
                if (!selected_Pro) {
                    Selected_pro[i] = CreateObject1("images/select_p.png", gamemain, 46 + (84 * i), 57);
                    showObject(Selected_pro[i]);
                    IsitSelected[i] = true;
                    selected_Pro = true; //하나가 선택됨
                }
            }

            if (obj == Selected_pro[i]) {
                hideObject(Selected_pro[i]);
                IsitSelected[i] = false;
                selected_Pro = false;
            }

        }


        if (obj == OrderDoll) { //인형 오더 완료하기.
            if (selected_Pro && customerIN) { //선택된상태이고, 손님이 카운터에 대기중인 상태에서만 말풍선 클릭가능.
                for (int i = 0; i < 6; i++) {
                    if (IsitSelected[i]) {
                        if (RandomDoll == productid[i]) { //같으면
                            if (productid[i] == 1 || productid[i] == 2) money += 700; //란국,블린인형 700원
                            else if (productid[i] == 3 || productid[i] == 4 || productid[i] == 5 || productid[i] == 6) money += 1000; //나머지 일반인형 1000원
                            else money += 1500; //스페셜 인형 1500원

                            //수익금 업데이트
                            RevenueCount();
                            
                            // 좋은기분 표시.
                            playSound(right, false); //효과음
                            setObjectImage(OrderDoll, "images/ball_N.png");
                            setObjectImage(feel_Doll, "images/H.png");
                            feelTimer = createTimer(1.0f); //감정표현 후 퇴장시키기
                            startTimer(feelTimer);
                        }
                        else {
                            //나쁜기분 표시.
                            playSound(wrong, false); //효과음
                            setObjectImage(OrderDoll, "images/ball_N.png");
                            setObjectImage(feel_Doll, "images/H1.png");
                            feelTimer = createTimer(1.0f); //감정표현 후 퇴장시키기
                            startTimer(feelTimer);
                        }

                        hideObject(product[i]); hideObject(Selected_pro[i]); //인형, 선택을 제거
                        selected_Pro = false; //선택해제
                        IsitSelected[i] = false; //해당 인형이 선택되었다는것을 해제
                        SlotFilled[i] = false; //슬롯을 비워줌.
                        customerIN = false;

                        break;
                    }
                }
                
            }
        }


        if (obj == OrderDollp) { //인형 오더 완료하기_전화
            if (selected_Pro && phoneON) { //선택된상태이고, 휴대폰 켜진상태에서만 말풍선 클릭가능.
                for (int i = 0; i < 6; i++) {
                    if (IsitSelected[i]) {
                        if (RandomDollp == productid[i]) { //같으면
                            if (productid[i] == 1 || productid[i] == 2) money += 800; //란국,블린인형 800원 (현장판매 +100\)
                            else if (productid[i] == 3 || productid[i] == 4 || productid[i] == 5 || productid[i] == 6) money += 1100; //나머지 일반인형 1100원
                            else money += 1600; //스페셜 인형 1600원

                            //수익금 업데이트
                            RevenueCount();

                            // 좋은기분 표시.
                            playSound(right, false); //효과음
                            setObjectImage(OrderDollp, "images/ball_N.png");
                            setObjectImage(feel_Dollp, "images/H.png");
                            feeltimerP = createTimer(1.0f); //감정표현 후 퇴장시키기
                            startTimer(feeltimerP);
                        }
                        else {
                            //나쁜기분 표시.
                            playSound(wrong, false); //효과음
                            setObjectImage(OrderDollp, "images/ball_N.png");
                            setObjectImage(feel_Dollp, "images/H1.png");
                            feeltimerP = createTimer(1.0f); //감정표현 후 퇴장시키기
                            startTimer(feeltimerP);
                        }

                        hideObject(product[i]); hideObject(Selected_pro[i]); //인형, 선택을 제거
                        selected_Pro = false; //선택해제
                        IsitSelected[i] = false; //해당 인형이 선택되었다는것을 해제
                        SlotFilled[i] = false; //슬롯을 비워줌.
                        phoneON = false;

                        break;
                    }
                }

            }
        }


        if (obj == recipe) {
            
            maingamerun = false;

            recipewin = CreateObject1("images/doll_rep.png", gamemain, 137, 61);
            xbutton2 = CreateObject1("images/xb2.png", gamemain, 1005, 595);
            showObject(recipewin); showObject(xbutton2);

        }
    }


    else { //게임 러닝 멈췄을때_인벤토리 상호작용

        if (obj == xbutton) { 

            hideObject(Metarial); hideObject(xbutton);

            for (int i = 0; i < 10; i++) {

                if (i < 5 && WhoisWork[i]) {
                    if (i < 3) MakingLine1[2*i] = 0;
                    else MakingLine2[(i - 3)*2] = 0; //슬롯을 비워준다.
                    
                }

                if (i < 6) WhoisWork[i] = false;
                hideObject(Fabrics[i]);
                hideObject(Special1[i]);
                hideObject(Special2[i]);
                hideObject(Selected[i]);
            }

            WhentoSave[0] = false;
            WhentoSave[1] = false;
            selectedF = false;
            selectedS1 = false;
            selectedS2 = false;
            maingamerun = true;
        }

        if (obj == xbutton2) {

            hideObject(recipewin); hideObject(xbutton2);
            maingamerun = true;
        }

        //반복작업이 필요한 오브젝트 모음
        for (int i = 0; i < 10; i++) {
             
            //선택한 것 다시누름
            if (obj == Selected[i]) {
                selectedF = false;
                selectedS1 = false;
                hideObject(Selected[i]);
            }


            //선택지_천
            if (obj == Fabrics[i]) { //천 중 하나를 선택했을 때

                if (selectedF) { //하나를 선택한 상태에서
                    hideObject(Metarial); hideObject(xbutton);
                    selectedF = false;
                    maingamerun = true;
                    for (int j = 0; j < 10; j++) {
                        hideObject(Fabrics[j]);
                        hideObject(Selected[j]);
                    }

                    if (WhentoSave[0]) { //란국라인
                        MakingLine1[1] = i+1;
                        if (MakingLine1[0] > MakingLine1[1]) { //리스트를 정렬해주는 기능
                            int a = MakingLine1[0];
                            MakingLine1[0] = MakingLine1[1];
                            MakingLine1[1] = a;
                        }
                        
                        MakingDoll_Line1();
                    }
                    else if (WhentoSave[1]) { //블린라인
                        MakingLine2[1] = i+1;
                        if (MakingLine2[0] > MakingLine2[1]) { //리스트를 정렬해주는 기능
                            int a = MakingLine2[0];
                            MakingLine2[0] = MakingLine2[1];
                            MakingLine2[1] = a;
                        }
                        MakingDoll_Line2();
                    }

                    WhentoSave[0] = false;
                    WhentoSave[1] = false; //즉시 둘 모두를 false로 돌림

                    MakeThemwork(); //작업 애니메이션 재생

                }
                else { //하나도 누르지 않은 상태에서 선택
                    selectedF = true;
                    showObject(Selected[i]);
                    if (WhentoSave[0]) {
                        MakingLine1[0] = i+1;
                    }
                    else if (WhentoSave[1]) {
                        MakingLine2[0] = i+1;
                    }
                }
            }

            //선택지_부자재1
            if (obj == Special1[i]) { //부자재1 중 하나를 선택했을 때

                if (selectedS1) { //하나를 선택한 상태에서
                    hideObject(Metarial); hideObject(xbutton);
                    selectedS1 = false;
                    maingamerun = true;
                    for (int j = 0; j < 10; j++) {
                        hideObject(Special1[j]);
                        hideObject(Selected[j]);
                    }

                    if (WhentoSave[0]) { //란국라인
                        MakingLine1[3] = i + 1;
                        if (MakingLine1[2] > MakingLine1[3]) { //리스트를 정렬해주는 기능
                            int a = MakingLine1[2];
                            MakingLine1[2] = MakingLine1[3];
                            MakingLine1[3] = a;
                        }
                        
                        MakingDoll_Line1();
                    }
                    else if (WhentoSave[1]) { //블린라인
                        MakingLine2[3] = i + 1;
                        if (MakingLine2[2] > MakingLine2[3]) { //리스트를 정렬해주는 기능
                            int a = MakingLine2[2];
                            MakingLine2[2] = MakingLine2[3];
                            MakingLine2[3] = a;
                        }
                        MakingDoll_Line2();
                    }

                    WhentoSave[0] = false;
                    WhentoSave[1] = false; //즉시 둘 모두를 false로 돌림
                    MakeThemwork();

                }
                else { //하나도 누르지 않은 상태에서
                    selectedS1 = true;
                    showObject(Selected[i]);
                    if (WhentoSave[0]) {
                        MakingLine1[2] = i + 1;
                    }
                    else if (WhentoSave[1]) {
                        MakingLine2[2] = i + 1;
                    }
                }
            }

            //선택지_부자재2
            if (obj == Special2[i]) { //부자재2 중 하나를 선택했을 때

                hideObject(Metarial); hideObject(xbutton);
                selectedF = false;
                maingamerun = true;
                for (int j = 0; j < 10; j++) {
                    hideObject(Special2[j]);
                    hideObject(Selected[j]);
                }

                if (WhentoSave[0]) { //란국라인
                    MakingLine1[4] = i + 1;
                    MakingDoll_Line1();
                }
                else if (WhentoSave[1]) { //블린라인
                    MakingLine2[4] = i + 1;
                    MakingDoll_Line2();
                }

                WhentoSave[0] = false;
                WhentoSave[1] = false;
                
                MakeThemwork();
            }

        }

    }

    if (obj == Startbutton) {

        //사운드 변경
        stopSound(bgmMain);
        bgm = createSound("audios/background2.mp3");
        playSound(bgm, true);

        //로비 화면 제거
        hideObject(Startbutton); hideObject(HowToPlay); hideObject(Lobbyscreen);
        
        //게임 레디
        showObject(Wokers[0]); showObject(Wokers[1]); showObject(Wokers[2]); showObject(Wokers[3]);
        showObject(Wokers[4]); showObject(Wokers[5]); showObject(recipe);
        showObject(ballonC); showObject(feel_Doll); showObject(OrderDoll);
        showObject(ballonP); showObject(OrderDollp); showObject(feel_Dollp); showObject(phone);

        //게임 타이머 생성
        gamemainTimer = createTimer(5.0f);
        showTimer(gamemainTimer);
        startTimer(gamemainTimer);

        // 손님 생성
        customer = CreateObject1("images/c0.png", gamemain, 58, 338);
        showObject(customer);

        MakeCustomerCome(); //바로 손님 등장시키기
        CustomerWalking = true; //걷는도중 상호작용 금지

        //전화주문 생성
        MakephoneOrder();

        //수익 계산기 표시
        RevenueCount();

    }

    if (obj == HowToPlay) {

        if (!cilck1) {
            cilck1 = true;
            HTPWIN = CreateObject1("images/htp1.png", gamemain, 124, 46);
            HTPButton = CreateObject1("images/htpbutton.png", gamemain, 1071, 600);
            showObject(HTPWIN); showObject(HTPButton);//도움말 창 띄우기
        }
    }

    if (obj == HTPButton) {

        if (!cilck2) {
            setObjectImage(HTPWIN, "images/htp2.png");
            cilck2 = true;
        }
        else {
            hideObject(HTPWIN); hideObject(HTPButton);
            cilck2 = false;
            cilck1 = false;
        }
    }

    //게임오버시 버튼 3가지... 일 예정이었으나 결국 나가기 버튼만ㅠ 만들었는
    //if (obj == maing) {

    //    money = 0; //돈 초기화
    //    customerIN, CustomerWalking = false;
    //    phoneON = false;
    //    selectedF, selectedS1, selectedS2 = false;
    //    selected_Pro = false;

    //    for (int i = 0; i < 6; i++) {
    //        WhoisWork[i] = false;
    //        DoAnimate[i] = false;
    //    }

    //    RandomDoll = 0;
    //    RandomDollp = 0; 

    //}

    if (obj == exitg) endGame(); //게임 종료
}


int main()
{
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    
    gamemain = createScene("인형공방","images/gamemain.png");

    Lobbyscreen = CreateObject1("images/gamelobby.png", gamemain, 0, 0);
    Startbutton = CreateObject1("images/startbutton.png", gamemain, 1012, 182);
    HowToPlay = CreateObject1("images/HTP.png", gamemain, 1012, 101);

    Wokers[0] = CreateObject1("images/eggwork1.png", gamemain, 1083, 525);
    Wokers[1] = CreateObject1("images/eggwork2.png", gamemain, 1015, 312);
    Wokers[2] = CreateObject1("images/eggwork3.png", gamemain, 1084, 141);
    Wokers[3] = CreateObject1("images/binwork1.png", gamemain, 856, 525);
    Wokers[4] = CreateObject1("images/binwork2.png", gamemain, 796, 312);
    Wokers[5] = CreateObject1("images/binwork3.png", gamemain, 865, 141);

    ballonC = CreateObject1("images/ball_N.png", gamemain, 43, 351);
    feel_Doll = CreateObject1("images/ball_N.png", gamemain, 58, 382);
    OrderDoll = CreateObject1("images/ball_N.png", gamemain, 58, 371);

    ballonP = CreateObject1("images/ball_N.png", gamemain, 480, 422);
    phone = CreateObject1("images/phone_off.png", gamemain, 448, 388);
    feel_Dollp = CreateObject1("images/ball_N.png", gamemain, 512, 462);
    OrderDollp = CreateObject1("images/ball_N.png", gamemain, 511, 456);

    recipe = CreateObject1("images/rep.png", gamemain, 500, 200);

    showObject(Lobbyscreen); showObject(Startbutton); showObject(HowToPlay);

    bgmMain = createSound("audios/background.mp3");
    cilck0 = createSound("audios/pop.mp3");
    right = createSound("audios/right.mp3");
    wrong = createSound("audios/wrong.mp3");
    done = createSound("audios/done.mp3");

    playSound(bgmMain, true);

    setMouseCallback(mouseCallack1);
    setTimerCallback(timerCallback1);

    startGame(gamemain);
}
