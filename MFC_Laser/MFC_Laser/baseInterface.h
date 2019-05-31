#ifndef __HL_79D5442D_3EBB_46B7_89F6_2086B40DC5FC_H
#define __HL_79D5442D_3EBB_46B7_89F6_2086B40DC5FC_H

#include <afxtempl.h>
#include "baseHatch.h"

//������ṹ�嶨��
typedef struct LAYER_PARA
{
	int			nCount;				//������
	double	dbMarkV;			//ʸ������ٶ�(mm/s)
	double	dbJumpV;			//ʸ�������ٶ�(mm/s)
	double	dbQFreq;			//QƵ(KHz)
	double	dbQRls;				//Q�ͷ�(us)
	double	dbCurrent;			//����(A)
	int			nFpkTime;			//����������ʱ��	
	double	dbFPKInitRate;		//�׵����(0.0~1.0)	
	double	dbFPKLenRate;		//���Ƴ���ϵ��(0.0~0.999)
	double	dbFPKMaxPRate;	//��ֵ��������(0.0~1.0)
	double	dbFPKCurRate;		//���������ϵ��(0.0~1.0)
	int			nQuality;			//�������ϵ��
	int			nLayerDelay;		//����ʱ
	int			nLaserOnDelay;	//���⿪��ʱ(us)
	int			nLaserOffDelay;	//�������ʱ(us)
	int			nMoveLineDelay;	//�߱���ʱ(us)
	int			nJumpDelay;		//��ת��ʱ(us)
	int			nRoundDelay;		//������ʱ(us)
	double	dbCurrentR;			//����(%)
	double	dbSimmerCur;		//ά�ֵ���(%)
	int			nWaveForm;		//����
	double	dbPowerR;			//����(%)
	
	void Assign(const LAYER_PARA& src)
	{
		memcpy(this, &src, sizeof(LAYER_PARA));
	}
	
	BOOL Equal(const LAYER_PARA & src)
	{
		if( &src == this )	
		{
			return TRUE;
		}
		
		return (
			nCount == src.nCount   &&
			dbMarkV == src.dbMarkV &&
			dbJumpV == src.dbJumpV &&
			dbQFreq  == src.dbQFreq  &&
			dbQRls == src.dbQRls &&
			dbCurrent == src.dbCurrent &&
			nFpkTime == src.nFpkTime &&
			dbFPKInitRate == src.dbFPKInitRate &&
			dbFPKLenRate == src.dbFPKLenRate &&
			dbFPKMaxPRate == src.dbFPKMaxPRate &&
			dbFPKCurRate == src.dbFPKCurRate &&
			nQuality == src.nQuality &&
			nLayerDelay == src.nLayerDelay &&
			nLaserOnDelay == src.nLaserOnDelay &&
			nLaserOffDelay == src.nLaserOffDelay &&
			nMoveLineDelay == src.nMoveLineDelay &&
			nJumpDelay == src.nJumpDelay &&
			nRoundDelay == src.nRoundDelay &&
			dbCurrentR == src.dbCurrentR &&
			dbSimmerCur == src.dbSimmerCur &&
			nWaveForm == src.nWaveForm && 
			dbPowerR  == src.dbPowerR 
			);
	}
} LAYERPARA,*LP_LAYERPARA;

//����˶������ṹ�嶨��
typedef struct MOTOR_PARA
{
	WORD   nVer;			//�汾��,Ԥ��0
	//////////////////
	WORD   nAxis;			//���
	WORD   nMoveMode;		//�˶�ģʽ:0-�����˶�,1-����˶�
	LONG   nMovePos;		//�˶�����λ��
	UINT   nMoveSpeed;		//�˶��ٶ�(����/��)
	UINT   nMoveAcc;		//���ٶ�(��������/��)
	UINT   nDelay;			//�˶�����ʱ
}*LP_MOTOR_PARA;

//��������ṹ�嶨��
typedef struct  FONT_PARA
{
	int     nFontType;			//�������� 0-TTF��1-SHX��2-HANS��3-DOT
    BYTE    nCharSet;			//�ַ���,Unicode����ʱ��
	char   szFontName[100];		//TTF��HANS������
	char   szShxCN [100];		//SHX��DOT������������
	char   szShxEN[100];		//SHX��DOT��Ӣ��������
	char   szShxNum [100];
} FONTPARA,*LP_FONTPARA;

typedef LAYERPARA  EMCCPARA;

//����ӿڵ����ݽṹ
class CInterDot
{
public:
	CInterDot()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	CInterDot(double x1,double y1)
	{
		x = x1;
		y = y1;
	}

	CInterDot(double x1,double y1,double z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}

	CInterDot & operator=(const CInterDot &dot)
	{
		if ( this == &dot )
			return *this;
		else
		{
			x = dot.x;
			y = dot.y;
			z = dot.z;
		}
		return *this;
	}

public:
	double  x;	//x
	double  y;	//y
	double  z;	//z (3DԤ��)
};

//����ӿ�����������
class CInterStroke: public CObject
{
public:
	CInterStroke()
	{
		m_nLayer = 0;
		m_dotArray.RemoveAll();
	}
	
	virtual ~CInterStroke (){
		m_dotArray.RemoveAll();
	}
	
	void AddDot(CInterDot& dot){
		m_dotArray.Add(dot);
	}

	void SetLayer( int nLayer ){
		m_nLayer = nLayer;
	}

	CInterStroke & operator=(const CInterStroke &stroke)
	{
		if ( this == &stroke )
			return *this;
		else
		{
			m_nLayer = stroke.m_nLayer;
			m_dotArray.RemoveAll();
			m_dotArray.Append(stroke.m_dotArray);
		}
		return *this;
	}
	
public:
	CArray<CInterDot,CInterDot> m_dotArray;
	int   m_nLayer;
};

//Text������չ����
typedef struct  TEXT_EXINFO
{
	BOOL		bVar;              //�Ƿ�Ϊ�ɱ����	
	TCHAR	szVarName[100];    //�ɱ��ı�����

	int		nRow;
	int		nCol;
	/////////////////
	TCHAR   szExtInfo[100];
	TCHAR	szInfo[10][100];
	int		    nValue[10];
	double	dbValue[10];
	DWORD	dwValue[10];

	TEXT_EXINFO()
	{
		InitData();
	}

	void InitData()
	{
		bVar = FALSE;
		memset( szVarName, 0, 100*sizeof(TCHAR) );		

		nRow = -1;
		nCol = -1;

		memset( szExtInfo, 0, 100*sizeof(TCHAR) );

		for(int i=0; i<10; i++)
		{
			memset( szInfo[i], 0, 100*sizeof(TCHAR) );
			nValue[i] = 0;
			dbValue[i] = 0.0;
			dwValue[i] = 0;
		}
	}

	void operator=(const TEXT_EXINFO &src)
	{
		if ( this == &src )
			return;

		bVar = src.bVar;
		_tcscpy(szVarName,src.szVarName);

		nRow = src.nRow;
		nCol = src.nCol;

		_tcscpy(szExtInfo, szExtInfo);

		for(int i=0; i<10; i++)
		{
			_tcscpy(szInfo[i], src.szInfo[i]);
			nValue[i] = src.nValue[i];
			dbValue[i] = src.dbValue[i];
			dwValue[i] = src.dwValue[i];
		}		
	}

} TEXTEXINFO,*LP_TEXTEXINFO;

//DOC��չ����
typedef struct  DOC_EXINFO
{
	TCHAR   szExtInfo[10][100];
	int		    nReserved[10];
	double	dbReserved[10];
	DWORD	dwReserved[10];

	DOC_EXINFO()
	{
		InitData();
	}

	void InitData()
	{
		memset(this,0,sizeof(DOC_EXINFO));
	}

	void operator=(const DOC_EXINFO &src)
	{
		if ( this == &src )
			return;
		
		memcpy(this,&src,sizeof(DOC_EXINFO));
	}
} DOCEXINFO,*LP_DOCEXINFO;

//����ӿ�������������
typedef CTypedPtrList<CObList, CInterStroke *> InterStrokeList;



typedef struct BUF_CAMERA_PARA
{
	WORD  nVer;				//�汾��,0
	///////
	UINT		nProgID;			//�����
	UINT		nNGRepNum;	//���Դ���
	UINT		nOverTime;		//��ʱʱ��(ms)
	UINT		nErrorMode;	//������
	UINT		nBaudRate;		//������
	BOOL		bScanJump;	//��ǿ����ת
	double   dbX;				//��Xλ��(mm)
	double   dbY;				//��Yλ��(mm)
	UINT		nDelay;			//��ת��ʱ(ms)
}*LP_CAMERA_PARA;



















#endif ///__HL_79D5442D_3EBB_46B7_89F6_2086B40DC5FC_H