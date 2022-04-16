#include<stdio.h>
#include<stdlib.h>
#include<bangtal.h> 
#include<time.h>
#pragma comment(lib,"bangtal.lib")

SceneID scene1;
ObjectID startbutton, restartbutton;
ObjectID card[53];// 스페이드 다이아 하트 클로버 순으로 숫자 2~10 K Q J A 조커
ObjectID ot_card[3];
ObjectID chip[6];
ObjectID hit, split, stand, betstop, upbutton, downbutton;
int chip_posi[6][2];  //x y 좌표
int dealer[10];
int player[10][2];
int dealer_posi[10][2];
int player_posi[10][2][2]; // x y 좌표 split여부
int deck[53];

int start_state = 0, dealer_st = 0, restart_st = 0;//배팅금액, 게임 현황, 딜러 카드 순서 재시작 여부
int split_st = 0, split_stud = 0, player_cardu = 0, player_cardd = 0, bet_end = 0; // 0,1 스플릿 유무, 0 이면 위에꺼 1이면 아래꺼,플레이어 위 아래 카드 순서 좋을듯 ,베팅 끝 여부
int player_dua = 0, player_dda = 0, dealer_a = 0;// 위 아래 딜러 a개수 처음에는 a로 계산 최후에는 하나씩 10으로 바꾼다
int scoreu = 0, scored = 0, scorede = 0, burstu = 0, burstd = 0, burstdealer = 0; // 위 아래 딜러 카드 점수 총합.. 버스트(게임진행) 상태 

double money = 100000, bet_moneyu = 0, bet_moneyd = 0;

void card_set();
int exam(int k);
void dealerturn();
int blackjack(int n);
int shuffle_f();
int last();
int split_ex();
void split_f();
void MIH();
//MIH 전체함수 초기화

int split_ex()
{
	int i;
	for (i = 0; i < 13; i++)
	{
		if (player[0][0] >= 0 + i * 4 && player[0][0] <= 3 + i * 4 && player[1][0] >= 0 + i * 4 && player[1][0] <= 3 + i * 4)
		{
			return 1;
		}
	}
	return 0;
}

void split_f()
{
	money -= bet_moneyu;
	bet_moneyd = bet_moneyu;
	player_cardu = 1;
	player_cardd = 1;
	player[0][1] = player[1][0];
	player[1][0] = 0;

	locateObject(card[player[0][1]], scene1, player_posi[0][0][1], player_posi[0][1][1]);
}

int blackjack(int n) // 블랙잭 여부 0이면 아님 1이면 블랙잭 n이 0이면 위 1이면 아래 2이면 딜러 조사 카드 수가 2개일때만 조사 시킴..
{
	if (n == 0)
	{
		if (player[0][0] >= 48 && player[0][0] <= 51 && player[1][0] >= 32 && player[1][0] <= 47)
		{
			return 1;
		}
		else if (player[1][0] >= 48 && player[1][0] <= 51 && player[0][0] >= 32 && player[0][0] <= 47)
		{
			return 1;
		}
	}
	else if (n == 1)
	{
		if (player[0][1] >= 48 && player[0][1] <= 51 && player[1][1] >= 32 && player[1][1] <= 47)
		{
			return 1;
		}
		else if (player[1][1] >= 48 && player[1][1] <= 51 && player[0][1] >= 32 && player[0][1] <= 47)
		{
			return 1;
		}
	}
	else if (n == 2)
	{
		if (dealer[0] >= 48 && dealer[0] <= 51 && dealer[1] >= 32 && dealer[1] <= 47)
		{
			return 1;
		}
		else if (dealer[1] >= 48 && dealer[1] <= 51 && dealer[0] >= 32 && dealer[0] <= 47)
		{
			return 1;
		}
	}
	return 0;
}

int last() //우선 버스트 확인 후
{
	restart_st = 0;
	if (burstd == 2)
	{
		money += bet_moneyd * 3;
		bet_moneyd = 0;
		restart_st = 0;
		printf("\n보유 금액: %lf\n", money);
		showObject(restartbutton);
		return 0;
	}
	else if (burstu == 2)
	{
		money += bet_moneyu * 3;
		bet_moneyu = 0;
		restart_st = 0;
		printf("\n보유 금액: %lf\n", money);
		showObject(restartbutton);
		return 0;
	}
	else if (burstdealer == 2)
	{
		bet_moneyu = 0;
		bet_moneyd = 0;
		restart_st = 0;
		printf("\n보유 금액: %lf\n", money);
		showObject(restartbutton);
		return 0;
	}

	if (split_st == 0)
	{
		if (burstu == 1)
		{
			bet_moneyu = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
		}
		else if (burstu == 4)
		{
			if (burstdealer == 4)
			{
				money += bet_moneyu;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
			}
			else
			{
				money += bet_moneyu * 2.5;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 블랙잭 승
			}
		}
		else if (burstu == 3 && burstdealer == 4)
		{
			bet_moneyu = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어 일반 딜러 블랙잭  플레이어 패
		}
		else if (burstu == 3 && burstdealer == 1)
		{
			money += bet_moneyu * 2;
			bet_moneyu = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어일반 딜러 버스트 플레이어 승
		}
		else
		{
			if (scoreu > scorede)
			{
				money += bet_moneyu * 2;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 위 일반 승
			}
			else if (scoreu < scorede)
			{
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 패
			}
			else
			{
				money += bet_moneyu;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//무승부
			}
		}
	}
	else if (split_st == 1)
	{
		if (burstu == 1)
		{
			bet_moneyu = 0;
			restart_st = 0; 
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어 버스트로 패배 위
		}
		else if (burstu == 4)
		{
			if (burstdealer == 4)
			{
				money += bet_moneyu;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//둘다 블랙잭 무승부
			}
			else
			{
				money += bet_moneyu * 2.5;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 블랙잭 승 위
			}
		}
		else if (burstu == 3 && burstdealer == 4)
		{
			bet_moneyu = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어 일반 딜러 블랙잭  플레이어 패
		}
		else if (burstu == 3 && burstdealer == 1)
		{
			money += bet_moneyu * 2;
			bet_moneyu = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어일반 딜러 버스트 플레이어 승
		}
		else
		{
			if (scoreu > scorede)
			{
				money += bet_moneyu * 2;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 승
			}
			else if (scoreu < scorede)
			{
				bet_moneyu = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 패
			}
			else
			{
				money += bet_moneyu;
				bet_moneyu = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//무승부
			}
		}

		if (burstd == 1)
		{
			bet_moneyd = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어 버스트로 패배 아래
		}
		else if (burstd == 4)
		{
			if (burstdealer == 4)
			{
				money += bet_moneyd;
				bet_moneyd = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//둘다 블랙잭 무승부
			}
			else
			{
				money += bet_moneyd * 2.5;
				bet_moneyd = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 블랙잭 승 아래
			}
		}
		else if (burstd == 3 && burstdealer == 4)
		{
			bet_moneyd = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어 일반 딜러 블랙잭  플레이어 패
		}
		else if (burstd == 3 && burstdealer == 1)
		{
			money += bet_moneyd * 2;
			bet_moneyd = 0;
			restart_st = 0;
			printf("\n보유 금액: %lf\n", money);
			showObject(restartbutton);
			return 0;
			//플레이어일반 딜러 버스트 플레이어 승
		}
		else
		{
			if (scored > scorede)
			{
				money += bet_moneyd * 2;
				bet_moneyd = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 승
			}
			else if (scored < scorede)
			{
				bet_moneyd = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//플레이어 패
			}
			else
			{
				money += bet_moneyd;
				bet_moneyd = 0;
				restart_st = 0;
				printf("\n보유 금액: %lf\n", money);
				showObject(restartbutton);
				return 0;
				//무승부
			}
		}
	}
}

void dealerturn()  // 딜러가 카드 다 뽑고 난 후에 버스트 여부 및 점수로 승패 가른다 동점일 시 패배 둘다 블랙잭이면 무승부
{
	int i;
	player_dua = 0;
	player_dda = 0;
	dealer_a = 0;
	for (i = 0; i < player_cardu; i++)
	{
		if (player[i][0] >= 48 && player[i][0] <= 51)
		{
			player_dua++;
		}
	}
	if (split_st == 1)
	{
		for (i = 0; i < player_cardd; i++)
		{
			if (player[i][1] >= 48 && player[i][1] <= 51)
			{
				player_dda++;
			}
		}
		scored = exam(4);
	}
	for (i = 0; i < dealer_st; i++)
	{
		if (dealer[i] >= 48 && dealer[i] < 51)
		{
			dealer_a++;
		}
	}

	burstdealer = 0;
	scoreu = exam(3);
	scorede = exam(5);
	if (scorede > 16)
		burstdealer = 3;
	else if (scorede > 21)
		burstdealer = 1;

	while(burstdealer==0)
	{
		if (scorede > 16&&scorede<22)
		{
			burstdealer = 3;
			break;
		}else if (scorede > 21)
		{
			burstdealer = 1;
			break;
		}
		
		dealer[dealer_st] = shuffle_f();
		locateObject(card[dealer[dealer_st]], scene1, dealer_posi[dealer_st][0], dealer_posi[dealer_st][1]);
		showObject(card[dealer[dealer_st]]);
		
		dealer_st++;
		scorede = exam(5);
	}
	last();
}

int exam(int k) //0이면 위조사, 1이면 아래 버스트 여부 판단이기에 a는 1로  @ 딜러턴@  3은 위 4는 아래 5는 딜러 체크 이때는 a값을 조정  
{
	int i, sum = 0, t;                                      //  전부 블랙잭 검사 조건 넣어줘야함 잊지말자 하면 OX 체크 표시 ( X )
	if (k == 0)
	{
		if (player_cardu == 2)
		{
			if (blackjack(0) == 1)
			{
				burstu = 4;
				return sum;
			}
		}
		for (i = 0; i < player_cardu; i++)
		{
			if (48 <= player[i][0] && player[i][0] < 52)
				sum += 1;
			else if (player[i][0] < 36)
				sum += player[i][0] / 4 + 2;
			else if (player[i][0] > 35 && player[i][0] < 48)
				sum += 10;
			else if (player[i][0] == 52) //조커 등장 이러면 다음에 히트가 불가 ㅇㅇ 
			{
				burstu = 2;
				break;
			}
		}
	}
	else if (k == 1)
	{
		if (player_cardd == 2)
		{
			if (blackjack(1) == 1)
			{
				burstd = 4;
				return sum;
			}
		}
		for (i = 0; i < player_cardd; i++)
		{
			if (48 <= player[i][1] && player[i][1] < 52)
				sum += 1;
			else if (player[i][1] < 36)
				sum += player[i][1] / 4 + 2;
			else if (player[i][1] > 35 && player[i][1] < 48)
				sum += 10;
			else if (player[i][1] == 52)
			{
				burstd = 2;
				break;
			}
		}
	}
	else if (k == 5)
	{
		if (dealer_st == 2)
		{
			if (blackjack(2) == 1)
			{
				burstdealer = 4;
			}
		}

		for (i = 0; i < dealer_st; i++)
		{
			if (48 <= dealer[i] && dealer[i] < 52)
				sum += 1;
			else if (dealer[i] < 36)
				sum += dealer[i] / 4 + 2;
			else if (dealer[i] > 35 && dealer[i] < 48)
				sum += 10;
			else if (dealer[i] == 52)
			{
				burstdealer = 2;
				break;
			}
			
		}
	}
	else if (k == 3)
	{
		for (i = 0; i < player_cardu; i++)
		{
			if (48 <= player[i][0] && player[i][0] < 52)
				sum += 11;
			else if (player[i][0] < 36)
				sum += player[i][0] / 4 + 2;
			else if (player[i][0] > 35 && player[i][0] < 48)
				sum += 10;
			else if (player[i][0] == 52) //조커 등장 이러면 다음에 히트가 불가 ㅇㅇ 
			{
				burstu = 2;
				break;
			}
		}
		for (i = player_dua; i >= 0; i--)
		{
			if (sum + i * 10 <= 21)
			{
				sum += i * 10;
				break;
			}
		}
	}
	else if (k == 4)
	{
		for (i = 0; i < player_cardd; i++)
		{
			if (48 <= player[i][1] && player[i][1] < 52)
				sum += 11;
			else if (player[i][1] < 36)
				sum += player[i][1] / 4 + 2;
			else if (player[i][1] > 35 && player[i][1] < 48)
				sum += 10;
			else if (player[i][1] == 52)
			{
				burstd = 2;
				break;
			}
		}
		for (i = player_dda; i >= 0; i--)
		{
			if (sum + i * 10 <= 21)
			{
				sum += i * 10;
				break;
			}
		}
	}
	return sum;
}

int shuffle_f()
{
	int k;
	while (true)
	{
		srand(time(NULL));
		k = rand()%53 ;
		if (deck[k] == 1)
			continue;
		else if (deck[k] == 0)
		{
			deck[k] = 1;
			break;
		}
	}
	
	return k;
}

void MIH()
{
	int i;
	for (i = 0; i < player_cardu; i++)
	{
		hideObject(card[player[i][0]]);
		player[i][0] = 0;
	}
	for (i = 0; i < player_cardd; i++)
	{
		hideObject(card[player[i][1]]);
		player[i][1] = 0;
	}
	for (i = 0; i < dealer_st; i++)
	{
		hideObject(card[dealer[i]]);
		dealer[i] = 0;
	}


	dealer_st = 2;
	split_st = 0;
	split_stud = 0;
	player_cardu = 2;
	player_cardd = 0;
	bet_end = 0;

	burstd = 0;
	burstu = 0;
	burstdealer = 0;
	
	scored = 0;
	scorede = 0;
	scoreu = 0;

	bet_moneyu = 0;
	bet_moneyd = 0;
	
	dealer[0] = shuffle_f();
	dealer[1] = shuffle_f();
	player[0][0] = shuffle_f();
	player[1][0] = shuffle_f();

	locateObject(card[dealer[0]], scene1, dealer_posi[0][0], dealer_posi[0][1]);
	locateObject(card[dealer[1]], scene1, dealer_posi[1][0], dealer_posi[1][1]);
	locateObject(card[player[0][0]], scene1, player_posi[0][0][0], player_posi[0][1][0]);
	locateObject(card[player[1][0]], scene1, player_posi[1][0][0], player_posi[1][1][0]);

}

void mousetCallback(ObjectID object, int x, int y, MouseAction)
{
	int i, j, k;
	if (object == startbutton)
	{
		start_state = 1; //게임 시작 여부
		restart_st = 1; // 재시작 여부 0이면 버튼 눌러야함 1이어야 다른 버튼도 눌림...
		dealer_st = 2; // 딜러상태
		split_st = 0; // 스플릿 유무
		split_stud = 0; // 히트시 받을 카드 위치 스플릿 관련
		player_cardu = 2;
		player_cardd = 0; // 플레이어 카드 총합
		bet_end = 0; // 베팅 종료 여부
		burstd = 0;
		burstu = 0; 
		burstdealer = 0;  //버스트 여부
		scoreu = 0; // 스코어 합 스탠드 이후 사용될 값임... 이거 계산할때
		scored = 0;
		scorede = 0;
		bet_moneyu = 0;
		bet_moneyd = 0;

		dealer[0] = shuffle_f();
		dealer[1] = shuffle_f();
		player[0][0] = shuffle_f();
		player[1][0] = shuffle_f();

		locateObject(card[dealer[0]], scene1, dealer_posi[0][0], dealer_posi[0][1]);
		locateObject(card[dealer[1]], scene1, dealer_posi[1][0], dealer_posi[1][1]);
		locateObject(card[player[0][0]], scene1, player_posi[0][0][0], player_posi[0][1][0]);
		locateObject(card[player[1][0]], scene1, player_posi[1][0][0], player_posi[1][1][0]);


		hideObject(startbutton);
		for (i = 0; i < 6; i++)
		{
			showObject(chip[i]);
		}
		locateObject(hit, scene1, 30, 500);
		locateObject(split, scene1, 30, 600);
		locateObject(stand, scene1, 30, 700);
		showObject(hit);
		showObject(split);
		showObject(stand);

		locateObject(ot_card[0], scene1, dealer_posi[0][0], dealer_posi[0][1]);
		showObject(ot_card[0]);
		start_state = 1;
	}

	//이하는 히트(), 스플릿(), 스탠드, 버스트 유무 ..

	if (object == restartbutton && restart_st == 0)  //////////////////////////////////////////////////// REstart
	{
		restart_st = 1;
		hideObject(restartbutton);
		MIH();
	}

	if (object == betstop&&bet_end==0&&restart_st==1)
	{
		bet_end = 1;
		showObject(ot_card[0]);
		showObject(card[dealer[1]]);
		showObject(card[player[0][0]]);
		showObject(card[player[1][0]]);
		exam(0);
	}
	if (object == split && split_st == 0 && bet_end==1 && restart_st == 1&& money>bet_moneyu) //이 기능은 최종에서 마무리 처음 같은 카드만 적용되게끔.. 전용 함수 만들면 됨..
	{
		if (split_ex() == 1)
		{
			split_f();
		}
		split_st = 1;
	}

	if (bet_end == 0 && restart_st == 1) //베팅 추가 수정은 필요없음... 내 지갑 기능 추가하고....
	{
		if (object == chip[0])
		{
			if (money < 10)
			{
				endGame();
			}
			bet_moneyu += 10;
			money -= 10;
		}
		else if (object == chip[1])
		{
			if (money < 50)
			{
				endGame();
			}
			bet_moneyu += 50;
			money -= 50;
		}
		else if (object == chip[2])
		{
			if (money < 100)
			{
				endGame();
			}
			bet_moneyu += 100;
			money -= 100;
		}
		else if (object == chip[3])
		{
			if (money < 500)
			{
				endGame();
			}
			bet_moneyu += 500;
			money -= 500;
		}
		else if (object == chip[4])
		{
			if (money < 1000)
			{
				endGame();
			}
			bet_moneyu += 1000;
			money -= 1000;
		}
		else if (object == chip[5])
		{
			if (money < 5000)
			{
				endGame();
			}
			bet_moneyu +=5000;
			money -= 5000;
		}
		printf("보유 금액: %lf   베팅금:%lf\n", money, bet_moneyu);
	}

	if (split_st == 1 && bet_end == 1 && restart_st == 1)
	{
		if (object == upbutton)
			split_stud = 0;
		else if (object == downbutton)
			split_stud = 1;
	}

	if (start_state == 1&&bet_end==1 && restart_st == 1)
	{
		if (object == hit && split_stud == 0 &&burstu==0)
		{
			player[player_cardu][split_stud] = shuffle_f();
			locateObject(card[player[player_cardu][split_stud]], scene1, player_posi[player_cardu][0][split_stud], player_posi[player_cardu][1][split_stud]);
			showObject(card[player[player_cardu][split_stud]]);
			player_cardu++;
		}else if (object == hit && split_stud == 1&& burstd==0)
		{
			player[player_cardd][split_stud] = shuffle_f();
			locateObject(card[player[player_cardd][split_stud]], scene1, player_posi[player_cardd][0][split_stud], player_posi[player_cardd][1][split_stud]);
			showObject(card[player[player_cardd][split_stud]]);
			player_cardd++;
		}
	}
	
	// 여기서부터는 검사 위주로 구성할것 

	if (split_st == 1&& bet_end==1 && restart_st == 1) //자동으로 뒤진경우 1로 수정 
	{
		if (exam(0) > 21)
			burstu = 1;
		if (exam(1) > 21)
			burstd = 1;
	}
	else if(split_st==0 && bet_end == 1 && restart_st == 1)
	{
		if (exam(0) > 21)
			burstu = 1;
	}

	if (split_st == 1 && bet_end == 1 && object==stand && restart_st == 1)// 스탠드 누른 여부인데 exam 뒤에 있으니 버스트가 0이면 실행 가능하게 하면 될듯
	{
		if (split_stud == 1&& burstd==0)
		{
			burstd = 3;
		}
		else if (split_stud == 0 && burstu == 0)
		{
			burstu = 3;
		}
	}
	else if (split_st == 0 && bet_end == 1 && object == stand && restart_st == 1)
	{
		if (split_stud == 0 && burstu == 0)
		{
			burstu = 3;
		}
	}

	if (split_st == 1 && bet_end == 1 && restart_st == 1) // 버스트 상태가 0이 아닌 경우 딜러 턴으로 넘김 1,2,3 상태 모두 반영한다라 볼 수 있다.
	{
		if (burstd != 0 && burstu != 0)
		{
			hideObject(ot_card[0]);
			showObject(card[dealer[0]]);
			dealerturn();
		}
	}
	else if (split_st == 0 && bet_end == 1 && restart_st == 1)
	{
		if (burstu != 0)
		{
			hideObject(ot_card[0]);
			showObject(card[dealer[0]]);
			dealerturn();
		}
	}
	
}

int main(void)
{
	int i, j, k;
	scene1 = createScene("GAME", "background.png");
	startbutton = createObject("start.png");
	restartbutton = createObject("restart.png");
	betstop = createObject("stop.png");
	upbutton = createObject("redbutton_up.png");
	downbutton = createObject("redbutton_down.png");
	setMouseCallback(mousetCallback);
	
	card_set();

	for (i = 0; i < 3; i++) // 칩 위치 정하기
	{
		chip_posi[i * 2][0] = 30;
		chip_posi[i * 2][1] = 370 - i * 100;
		chip_posi[i * 2 + 1][0] = 130;
		chip_posi[i * 2 + 1][1] = 370 - i * 100;
		locateObject(chip[i * 2], scene1, chip_posi[i * 2][0], chip_posi[i * 2][1]);
		locateObject(chip[i * 2 + 1], scene1, chip_posi[i * 2 + 1][0], chip_posi[i * 2 + 1][1]);
	}

	for (i = 0; i < 8; i++) // 플레이어 카드 위치, 딜러 카드 위치 설정
	{
		player_posi[i][0][0] = 340 + i * 100;
		player_posi[i][1][0] = 300;
		player_posi[i][0][1] = 340 + i * 100;
		player_posi[i][1][1] = 150;
		dealer_posi[i][0] = 340 + i * 100;
		dealer_posi[i][1] = 650;
	}

	locateObject(restartbutton, scene1, 220, 700);
	locateObject(downbutton, scene1, 250, 150);
	showObject(downbutton);
	locateObject(upbutton, scene1, 250, 350);
	showObject(upbutton);
	locateObject(betstop, scene1, 220, 500);
	showObject(betstop);

	locateObject(startbutton, scene1, 100, 500);
	showObject(startbutton);
	locateObject(ot_card[1], scene1, 1100, 480);
	showObject(ot_card[1]);
	startGame(scene1);

	return 0;
}

void card_set()
{
	ot_card[0] = createObject("dealer_back.png");
	ot_card[1] = createObject("deck_back.png");
	ot_card[2] = createObject("back.png");

	chip[0] = createObject("10.png");
	chip[1] = createObject("50.png");
	chip[2] = createObject("100.png");
	chip[3] = createObject("500.png");
	chip[4] = createObject("1000.png");
	chip[5] = createObject("5000.png");

	hit = createObject("hit.png");
	split = createObject("split.png");
	stand = createObject("stand.png");

	card[0] = createObject("2spade.png");
	card[1] = createObject("2diamond.png");
	card[2] = createObject("2heart.png");
	card[3] = createObject("2club.png");
	card[4] = createObject("3spade.png");
	card[5] = createObject("3diamond.png");
	card[6] = createObject("3heart.png");
	card[7] = createObject("3club.png");
	card[8] = createObject("4spade.png");
	card[9] = createObject("4diamond.png");
	card[10] = createObject("4heart.png");
	card[11] = createObject("4club.png");
	card[12] = createObject("5spade.png");
	card[13] = createObject("5diamond.png");
	card[14] = createObject("5heart.png");
	card[15] = createObject("5club.png");
	card[16] = createObject("6spade.png");
	card[17] = createObject("6diamond.png");
	card[18] = createObject("6heart.png");
	card[19] = createObject("6club.png");
	card[20] = createObject("7spade.png");
	card[21] = createObject("7diamond.png");
	card[22] = createObject("7heart.png");
	card[23] = createObject("7club.png");
	card[24] = createObject("8spade.png");
	card[25] = createObject("8diamond.png");
	card[26] = createObject("8heart.png");
	card[27] = createObject("8club.png");
	card[28] = createObject("9spade.png");
	card[29] = createObject("9diamond.png");
	card[30] = createObject("9heart.png");
	card[31] = createObject("9club.png");
	card[32] = createObject("10spade.png");
	card[33] = createObject("10diamond.png");
	card[34] = createObject("10heart.png");
	card[35] = createObject("10club.png");
	card[36] = createObject("Kspade.png");
	card[37] = createObject("Kdiamond.png");
	card[38] = createObject("Kheart.png");
	card[39] = createObject("Kclub.png");
	card[40] = createObject("Qspade.png");
	card[41] = createObject("Qdiamond.png");
	card[42] = createObject("Qheart.png");
	card[43] = createObject("Qclub.png");
	card[44] = createObject("Jspade.png");
	card[45] = createObject("Jdiamond.png");
	card[46] = createObject("Jheart.png");
	card[47] = createObject("Jclub.png");
	card[48] = createObject("Aspade.png");
	card[49] = createObject("Adiamond.png");
	card[50] = createObject("Aheart.png");
	card[51] = createObject("Aclub.png");
	card[52] = createObject("Jocker.png");

}