#include "FPSManager.h"


CFPSManager::CFPSManager()
{
	//FPS�Œ�******************************************************************************************************************************
	// ���C�����[�v�ɓ���O�ɐ��x���擾���Ă���
	QueryPerformanceFrequency(&timeFreq);
	// 1�x�擾���Ă���(����v�Z�p)
	QueryPerformanceCounter(&timeStart);

	fps = 0;
	//**************************************************************************************************************************************
}


CFPSManager::~CFPSManager()
{
}
VOID CFPSManager::SetFPS60()
{
	// ���̎��Ԃ��擾
	QueryPerformanceCounter(&timeEnd);
	// (���̎��� - �O�t���[���̎���) / ���g�� = �o�ߎ���(�b�P��)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) { // ���Ԃɗ]�T������
									  // �~���b�ɕϊ�
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1); // ����\���グ��(�������Ȃ���Sleep�̐��x�̓K�^�K�^)
		Sleep(sleepTime);   // �Q��
		timeEndPeriod(1);   // �߂�

							// ���T�Ɏ����z��(�������Ȃ���fps���ςɂȂ�?)
		return;
	}

	if (frameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		fps = (fps*0.99f) + (0.01f / frameTime); // ����fps���v�Z


		timeStart = timeEnd; // ����ւ�

	}
}