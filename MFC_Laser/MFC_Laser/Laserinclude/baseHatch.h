
#pragma once

#define FILLMINDIS  0.00001

typedef struct baseHatch
{
	baseHatch()
	{
		::memset(this, 0, sizeof(baseHatch));
		dbSpace = 0.01;
		nLoopNum = 1;
		dbLoopDist = 0.01;
		nFillNum = 1;
	}
	
public:
	BOOL	bEnable;		//���ʹ��
	BOOL	bEquaDist;		//�ȼ��ʹ��
	BOOL	bKeepAngle;		//�������������ת
	int		nFillStyle;		//�������
	double	dbSpace;		//�����
	double	dbAngle;		//���Ƕ�
	double	dbBeamComp;		//��դ��С
	double	dbLineReduce;   //���׵�������
	double	dbStartOffset;  //��ʼ������
	double	dbEndOffset;    //ĩβ������
	double	dbMinJump;      //��С��ת���
	
	BOOL	bLoop;			//����ʹ��
	int		nLoopNum;		//����Ȧ��
	double	dbLoopDist;		//�������
	BOOL	bRemoveLine;	//������ȥ�������
	BOOL	bRemoveOrg;		//������ȥ��ԭʼ����
	
	int    nFillNum;		//������
	double ProAngle;		//�۽��Ƕ�
	double AddDis;			//�۽����
	BOOL   bSaveCountour;	//���������
	double dbLineReduceEnd;	// ��β��������
}BaseHatch;

typedef struct HATCH_PARA 
{
	HATCH_PARA()
	{
		::memset(this, 0, sizeof(HATCH_PARA));
		
		newFileData1.dbSpace = 0.01;
		newFileData1.dbLoopDist = 0.01;
		newFileData1.nLoopNum = 1;
		newFileData1.dbAngle  = 0;
		newFileData1.nFillNum = 1;
		
		newFileData2.dbSpace = 0.01;
		newFileData2.dbLoopDist = 0.01;
		newFileData2.nLoopNum = 1;
		newFileData2.dbAngle = 90;
		newFileData2.nFillNum = 1;
		
		bFillFirst = TRUE;
	}
	
public:
	BaseHatch  newFileData1;
	BaseHatch  newFileData2;
	BOOL 	   bFillFirst;          //�ȴ����
	BOOL       bFillAlign;          //���ʱ����߶���

}HATCHPARA,*LP_HATCHPARA;

typedef BaseHatch  NEW_FILL_DATA;
typedef HATCHPARA  FILLDATA;