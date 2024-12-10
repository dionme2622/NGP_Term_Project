#include "Player.h"
#include "Map.h"
#include "Packet.h"


CPlayer::CPlayer(HINSTANCE _hInst, SC_PlayersInfoPacket receivedPacket, int playerID)
{
	
	ID = playerID;

	// Player Initialize
	if (ID == 0)			// TODO : Player1�� �ʱ�ȭ
	{
		receivedPacket.playerData[0].ApplyToPlayer(this);
		bitmap_pos = {};
	}
	else				// TODO : Player2�� �ʱ�ȭ
	{
		// TODO : �ٸ� Player �ʱ�ȭ
		receivedPacket.playerData[1].ApplyToPlayer(this);
		bitmap_pos = {};
		bitmap_pos.live.y = 76, bitmap_pos.damage.y = 144, bitmap_pos.dead.y = 144, bitmap_pos.escape.y = 144;
	}
	

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
				bitmap_pos.live.fx += frameSpeed;  // �ε��Ҽ��� ��ǥ ������Ʈ
				while (bitmap_pos.live.fx >= 64.0f) {  // 64���� ũ�� �� ĭ�� �̵�
					bitmap_pos.live.fx = 0.0f;  // 64��ŭ ������ ����
					bitmap_pos.live.x += 64;       // xPos�� 64�� ����
					if (bitmap_pos.live.x >= 512)   // �� �ֱ⸦ �Ѿ�� �ʱ�ȭ
						bitmap_pos.live.x = 0;
				}
			}

			if (direction == DIR_LEFT || direction == DIR_RIGHT) {
				bitmap_pos.live.fx += frameSpeed;
				while (bitmap_pos.live.fx >= 64.0f) {  // 64���� ũ�� �� ĭ�� �̵�
					bitmap_pos.live.fx = 0.0f;
					bitmap_pos.live.x += 64;
					if (bitmap_pos.live.x >= 384)  // �� �ֱ⸦ �Ѿ�� �ʱ�ȭ
						bitmap_pos.live.x = 0;
				}
			}
		}
		else {
			bitmap_pos.live.x = 0, bitmap_pos.damage.x = 0, bitmap_pos.dead.x = 0, bitmap_pos.escape.x = 0;
		}
	}
	else if (state == DAMAGE)
	{
		bitmap_pos.damage.fx += 88.0f * 5.0f * fTimeElapsed;
		while (bitmap_pos.damage.fx >= 88.0f) {  // 88���� ũ�� �� ĭ�� �̵�
			if (bitmap_pos.damage.x >= 1144) {
				break;  // xPos�� 1144 �̻��̸� �̵��� ���߰� while ������ ����
			}

			bitmap_pos.damage.fx = 0.0f;  // 88��ŭ ������ ����
			bitmap_pos.damage.x += 88;       // xPos�� 88�� ����

			if (bitmap_pos.damage.x >= 1144) {
				bitmap_pos.damage.x = 1144;  // xPos�� 1144 �̻��� �Ǹ� 1144�� ����
				break; // xPos�� 1144 �̻��̸� �� �̻� �̵����� ����
			}
		}

	}
	else if (state == DEAD)
	{
		bitmap_pos.dead.fx += 88.0f * 5.0f * fTimeElapsed;
		while (bitmap_pos.dead.fx >= 88.0f) {  //88���� ũ�� �� ĭ�� �̵�
			bitmap_pos.dead.fx = 0.0f;  // 64��ŭ ������ ����
			bitmap_pos.dead.x += 88;       // xPos�� 64�� ����
			if (bitmap_pos.dead.x >= 968)   // �� �ֱ⸦ �Ѿ�� �ʱ�ȭ
				return;
		}
	}
	else if (state == ESCAPE)
	{
		printf("���� ESCAPE �����Դϴ�\n");
		bitmap_pos.escape.fx += 88.0f * 10.0f * fTimeElapsed;
		while (bitmap_pos.escape.fx >= 88.0f) {  //88���� ũ�� �� ĭ�� �̵�
			bitmap_pos.escape.fx = 0.0f;  // 64��ŭ ������ ����
			bitmap_pos.escape.x += 88;       // xPos�� 64�� ����
			if (bitmap_pos.escape.x >= 616) bitmap_pos.escape.x = 0;
		}
	}
	receivedPacket.playerData[ID].ApplyToPlayer(this);		// Player Update

}

void CPlayer::Render(HDC MemDC, HDC MemDCImage, CMap* Map)
{
	if (state == LIVE)
	{
		if (direction == DIR_DOWN)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[0]); //--- �Ʒ�
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, bitmap_pos.live.x, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_LEFT)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[1]); //--- ����
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, bitmap_pos.live.x, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_RIGHT)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[2]); //--- ������
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, bitmap_pos.live.x, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
		}
		else if (direction == DIR_UP)
		{
			(HBITMAP)SelectObject(MemDCImage, MainBitmap[3]); //--- ��
			if (!stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, bitmap_pos.live.x, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
			if (stop)
				TransparentBlt(MemDC, x, y, 60, 60, MemDCImage, 0, bitmap_pos.live.y, 64, 76, RGB(255, 0, 255));
		}
	}
	else if (state == DAMAGE)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[4]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, bitmap_pos.damage.x, bitmap_pos.damage.y, 88, 144, RGB(255, 0, 255));
	}
	else if (state == DEAD)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[5]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, bitmap_pos.dead.x, bitmap_pos.dead.y, 88, 144, RGB(255, 0, 255));
	}
	else if (state == ESCAPE)
	{
		(HBITMAP)SelectObject(MemDCImage, MainBitmap[6]);
		TransparentBlt(MemDC, x - 10, y - 10, 70, 70, MemDCImage, bitmap_pos.escape.x, bitmap_pos.escape.y, 88, 144, RGB(255, 0, 255));
	}

	(HBITMAP)SelectObject(MemDCImage, MainBitmap[8]); //--- playerData[0] ȭ��ǥ
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
	//for (int i = 0; i < ballon_num; i++)
	//{
	//	if (ballon[i]->GetState() == 0)
	//	{

	//		ballon[i]->x = (x + 30 - 30) / 60 * 60;
	//		ballon[i]->y = (y + 30 - 65) / 60 * 60;
	//		//printf("��ǳ�� ��ġ! x: %d, y: %d\n", ballon[i]->x / 60, ballon[i]->y / 60);		// DEBUG

	//		if (Map->GetBoard((ballon[i]->y / 60), (ballon[i]->x / 60)).GetState() == 1)
	//		{
	//			//printf("�� %d \n", Map->GetBoard((ballon[i]->y / 60) - 1, (ballon[i]->x / 60) - 1).GetState());		// DEBUG

	//			ballon[i]->SetState(1);
	//			Map->GetBoard(11, 13).SetState(4);
	//			//printf("�� %d \n", Map->GetBoard(11,13).GetState());		// DEBUG
	//			//if(Map->GetBoard(11, 13).GetState() == 4) printf("%d\n", ballon[0]->GetState());

	//		}
	//	}
	//}
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
