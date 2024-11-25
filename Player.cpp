#include "player.h"
#include "Map.h"
#include "Packet.h"

CPlayer::CPlayer(HINSTANCE _hInst, SC_PlayersInfoPacket receivedPacket, int playerID)
{
	
	//printf("�׽�Ʈ �ʱ�ȭ : %d\n", �׽�Ʈ);
	ID = playerID;
	// Player Initialize
	if (ID == 0)
	{
		SetPosition(receivedPacket.player1.x, receivedPacket.player1.y);
		direction = receivedPacket.player1.direction;
		state = receivedPacket.player1.state;
		speed = receivedPacket.player1.speed;
		ballon_num = receivedPacket.player1.ballon_num;
		ballon_length = receivedPacket.player1.ballon_length;
		stop = receivedPacket.player1.stop;
		xPos = 0, yPos = 0;
		xPosF = 0.0f, yPosF = 0.0f;
		//direction = DIR_DOWN;
		state = LIVE;
	}
	else 
	{
		// TODO : �ٸ� Player �ʱ�ȭ
		SetPosition(400, 400);
		xPos = 0, yPos = 76;
		xPosF = 0.0f, yPosF = 0.0f;
		//direction = DIR_DOWN;
		state = LIVE;
	}
	/* Bitmap Animation�� ���� �ؽ��� ��ǥ ��*/

	/*--------------------------------------*/
	for (int i = 0; i < 6; i++) ballon[i] = new CBallon(_hInst);
	printf("�ʱ�ȭ! %d %d\n", x, y);  // DEBUG

	// Resource
	MainBitmap[0] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_DOWN));			// ĳ���� �Ʒ� ���
	MainBitmap[1] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_LEFT));			// ĳ���� ���� ���
	MainBitmap[2] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_RIGHT));         // ĳ���� ������ ���
	MainBitmap[3] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_UP));            // ĳ���� �� ���
	MainBitmap[4] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_DAMAGE));        // ĳ���� �ǰ�
	MainBitmap[5] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_DEAD));          // ĳ���� ���
	MainBitmap[6] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_ESCAPE));        // ĳ���� Ż��
	MainBitmap[7] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_WIN));           // ĳ���� �¸�
	MainBitmap[8] = LoadBitmap(_hInst, MAKEINTRESOURCE(IDB_Arrow));         // ĳ���� �Ӹ� �� ȭ��ǥ
}

CPlayer::~CPlayer()
{
	// TODO : ��Ʈ�� ����
}

void CPlayer::Update(SC_PlayersInfoPacket receivedPacket, float fTimeElapsed)
{


	if (state == LIVE)
	{
		float frameSpeed = 64.0f * 15 * fTimeElapsed;  // �ε巯�� �̵��� ���� �ӵ� ���
		if (!GetStop()) {
			if (direction == DIR_DOWN || direction == DIR_UP) {
				xPosF += frameSpeed;  // �ε��Ҽ��� ��ǥ ������Ʈ
				while (xPosF >= 64.0f) {  // 64���� ũ�� �� ĭ�� �̵�
					xPosF = 0.0f;  // 64��ŭ ������ ����
					xPos += 64;       // xPos�� 64�� ����
					if (xPos >= 512)   // �� �ֱ⸦ �Ѿ�� �ʱ�ȭ
						xPos = 0;
				}
			}

			if (direction == DIR_LEFT || direction == DIR_RIGHT) {
				xPosF += frameSpeed;
				while (xPosF >= 64.0f) {  // 64���� ũ�� �� ĭ�� �̵�
					xPosF = 0.0f;
					xPos += 64;
					if (xPos >= 384)  // �� �ֱ⸦ �Ѿ�� �ʱ�ȭ
						xPos = 0;
				}
			}
		}
		else {
			xPosF = 0.0f;
			xPos = 0;
		}
	}
	else if (state == DAMAGE)
	{
		xPosF += 88.0f * 5.0f * fTimeElapsed;
		while (xPosF >= 88.0f) {  // 88���� ũ�� �� ĭ�� �̵�
			if (xPos >= 1144) {
				break;  // xPos�� 1232 �̻��̸� �̵��� ���߰� while ������ ����
			}

			xPosF = 0.0f;  // 88��ŭ ������ ����
			xPos += 88;       // xPos�� 88�� ����

			if (xPos >= 1144) {
				xPos = 1144;  // xPos�� 1232 �̻��� �Ǹ� 1232�� ����
				break; // xPos�� 1232 �̻��̸� �� �̻� �̵����� ����
			}
		}

		if (xPos == 1144) {
			// xPos�� 1232�� �����ϸ� 3�� Ÿ�̸� ����
			static float stateChangeTime = 0.0f;  // Ÿ�̸� ���� (static���� �����Ͽ� �� ����)
			static bool bDeadStateTriggered = false; // ���� ���� Ʈ���� ����

			// Ÿ�̸� �ʱ�ȭ�� �ѹ��� �ϰ� ���Ŀ��� ��� ������
			if (!bDeadStateTriggered) {
				bDeadStateTriggered = true;  // ���� ���� Ʈ����
			}

			// ����� �ð��� Ȯ���Ͽ� 3�ʰ� ������ DEAD ���·� ��ȯ
			stateChangeTime += fTimeElapsed;  // ��� �ð� �߰�

			if (stateChangeTime >= 2.0f) {  // 3�ʰ� ������
				xPosF = 0.0f;
				xPos = 0;
				state = DEAD;  // ���¸� DEAD�� ����
				bDeadStateTriggered = false; // ���� ���� Ʈ���� �ʱ�ȭ
			}
		}
	}
	else if (state == DEAD)
	{
		xPosF += 88.0f * 5.0f * fTimeElapsed;
		while (xPosF >= 88.0f) {  //88���� ũ�� �� ĭ�� �̵�
			xPosF = 0.0f;  // 64��ŭ ������ ����
			xPos += 88;       // xPos�� 64�� ����
			if (xPos >= 968)   // �� �ֱ⸦ �Ѿ�� �ʱ�ȭ
				xPos = 0;
		}
	}
	else if (state == ESCAPE)
	{
		xPosF += 88.0f * 10.0f * fTimeElapsed;
		while (xPosF >= 88.0f) {  //88���� ũ�� �� ĭ�� �̵�
			xPosF = 0.0f;  // 64��ŭ ������ ����
			xPos += 88;       // xPos�� 64�� ����
			if (xPos >= 616) xPos = 0;
		}
	}
	// TODO : �ӽ÷� �س��� ��

	/*if (ID == 0)
	{
		SetPosition(receivedPacket.player1.x, receivedPacket.player1.y);
		direction = receivedPacket.player1.direction;
		printf("dircetion : %d\n", direction);
		state = receivedPacket.player1.state;
		speed = receivedPacket.player1.speed;
		ballon_num = receivedPacket.player1.ballon_num;
		ballon_length = receivedPacket.player1.ballon_length;
		stop = receivedPacket.player1.stop;
		
	}
	direction = DIR_DOWN;*/

	SetPosition(receivedPacket.player1.x, receivedPacket.player1.y);
	SetDirection(receivedPacket.player1.direction);
	printf("������Ʈ player x : %d, y : %d, direction : %d\r", x, y, direction);	// DEBUG
}

void CPlayer::Render(HDC MemDC, HDC MemDCImage, CMap* Map)
{
	if (state == LIVE)
	{
		if (direction == DIR_DOWN)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[0]); //--- �Ʒ�
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_LEFT)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[1]); //--- ����
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_RIGHT)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[2]); //--- ������
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_UP)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[3]); //--- ��
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, xPos, yPos, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, yPos, 64, 76, RGB(255, 0, 255));
		}
	}
	else if (state == DAMAGE)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[4]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, xPos, yPos, 88, 144, RGB(255, 0, 255));
	}
	else if (state == DEAD)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[5]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, xPos, yPos, 88, 144, RGB(255, 0, 255));
	}
	else if (state == ESCAPE)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[6]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, xPos, yPos, 88, 144, RGB(255, 0, 255));
	}

	(HBITMAP)SelectObject(MemDCImage, MainBitmap[8]); //--- player1 ȭ��ǥ
	TransparentBlt(MemDC, x + 20, y - 30, 24, 20, MemDCImage, 0, 0, 24, 28, RGB(255, 0, 255));


}

void CPlayer::SetDirection(int _direction)
{
	direction = _direction;
}


void CPlayer::SetPosition(float _fx, float _fy) {
	fx = _fx;
	fy = _fy;
	x = static_cast<int>(fx);  // �ε��Ҽ��� ��ǥ�� ������ ��ȯ
	y = static_cast<int>(fy);
}

void CPlayer::SetBallon(CMap* Map)
{
	for (int i = 0; i < ballon_num; i++)
	{
		if (ballon[i]->GetState() == 0)
		{

			ballon[i]->x = (x + 30 - 30) / 60 * 60;
			ballon[i]->y = (y + 30 - 65) / 60 * 60;
			//printf("��ǳ�� ��ġ! x: %d, y: %d\n", ballon[i]->x / 60, ballon[i]->y / 60);		// DEBUG

			if (Map->GetBoard((ballon[i]->y / 60), (ballon[i]->x / 60)).GetState() == 1)
			{
				//printf("�� %d \n", Map->GetBoard((ballon[i]->y / 60) - 1, (ballon[i]->x / 60) - 1).GetState());		// DEBUG

				ballon[i]->SetState(1);
				Map->GetBoard(11, 13).SetState(4);
				//printf("�� %d \n", Map->GetBoard(11,13).GetState());		// DEBUG
				//if(Map->GetBoard(11, 13).GetState() == 4) printf("%d\n", ballon[0]->GetState());

			}
		}
	}
}

void CPlayer::Move(float fTimeElapsed)
{
	/*if (!GetStop())
	{
		if (direction == DIR_DOWN) fy += speed * fTimeElapsed;
		else if (direction == DIR_LEFT) fx -= speed * fTimeElapsed;
		else if (direction == DIR_UP) fy -= speed * fTimeElapsed;
		else if (direction == DIR_RIGHT) fx += speed * fTimeElapsed;

		SetPosition(fx, fy);
	}*/

}
