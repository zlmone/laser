#ifndef		__INNERMSG_H_98520F42_6FD0_424F_B982_87F79749E32F
#define		__INNERMSG_H_98520F42_6FD0_424F_B982_87F79749E32F

#include "baseHatch.h"

#define EPSILON  (0.000001)
#define SAMEVALUE(a,b)	( fabs(a-b) < EPSILON )
#define DIFFERVALUE(a,b)	( fabs(a-b) > EPSILON )

//�����Ǳ��������ĺ궨��
#define PAI				3.141592653589793		//Բ����
#define CR				((char)0x0D+(char)0x0A)	//�س�
#define CENTER		0				//ʵ���������ĵ�

//���������괿����������任
#define LCENTER			16384			//�߼����ĵ�
#define DPMM				50				//�߼������е�ÿ������ʾ�ĵ���

#define PTY_INIT			0	//��ʼ��
#define PTY_LAYER		1	//����ɫ�ı�
#define PTY_PARA		2	//��������ı�
#define PTY_OUTLINE	3	//�ƶ��߿�
#define PTY_DEL			4	//ɾ����
#define PTY_FILL			5	//�ƶ������
#define PTY_ADD			6	//�����²�
#define PTY_CHANGE	7	//�ı䵱ǰ��
#define PTY_FILL_COUNTOUR		8	//�ƶ������

//������ɫ
#define  BLACK 		RGB(0,0,0)
#define  RED   		RGB(255,0,0)
#define	GREEN 		RGB(0,255,0)
#define	BLUE  		RGB(0,0,255)
#define	YELLOW		RGB(255,255,0)
#define	MAGENTA	RGB(255,0,255)
#define	YANGREEN	RGB(0,255,255)
#define	GRAY	 	    RGB(127,127,127)

//���������λ��
#define BASE_TL			0
#define BASE_TM			1
#define BASE_TR			2
#define BASE_ML			3
#define BASE_MM			4
#define BASE_MR			5
#define BASE_BL			6
#define BASE_BM			7
#define BASE_BR			8

//ʰȡ���λ��
#define	PT					0
#define	TRACKLEFT		5	
#define	TRACKRIGHT	7
#define	UPPER			4
#define	LOWER			6
#define	UPPER_LEFT	0
#define	UPPER_RIGHT	1
#define	LOWER_LEFT	3
#define	LOWER_RIGHT	2
#define TRACKCENTER	9	//���ĵ�

// ����ڵ�����
enum ENU_OBJTYPE
{	
	NODE_BASE			= 0,		// δ֪�ڵ�
	NODE_LINE			= 1,		// ���� 
	NODE_RECT			= 2,		// ����
	NODE_HLTEXT		= 3,		// �ı�
	NODE_SERIAL		= 4,		// ����
	NODE_BMP			= 5,		// BMP
	NODE_ELLIPSE		= 6,		// Բ
	NODE_PLT			= 7,		// PLT
	NODE_GRID			= 8,		// ����
	NODE_POLYGON	= 9,		// �����
	NODE_DXF			= 10,		// DXF
	NODE_CODEBAR	= 11,		// ��ά����
	NODE_COMBIN     = 12,		// ��϶���
	/*������*/
	NODE_DELAY		= 13,		//��ʱ
	NODE_INPUT		= 14,		//����
	NODE_OUTPUT		= 15,		//���
	//
	NODE_SPLINE      = 16,		//��������
	NODE_ARC			= 17,		//���㷽ʽԲ��
	NODE_CIRCLE      = 18,		//Բ
	NODE_DOT			= 19,		//��
	NODE_UNION       = 20,		//Ⱥ�����
	NODE_MOTORMOVE	= 21, //�˶����ƶ���
	NODE_MOTORPOS		= 22, //�˶���λ���趨
	NODE_CONDITIONS	= 23, //�����ж϶���
	NODE_CONDITIONE	= 24, //�����ж϶���
	NODE_NC				= 25,
	NODE_BUFFERLASER = 26, //������������ƶ���
	NODE_CCD				= 27, //CCD����
	NODE_POSCALI		= 28,	//λ��У������
	NODE_AI					= 29,
	NODE_3DMODEL		= 30,
	NODE_3DFRAME		= 31,
	NODE_END
};

const USHORT OBJ_MAX_MARKCOUNT = 6;

typedef struct tagWToC
{	
	double   dbW;
	double   dbC;
	//////////
	tagWToC()
	{
		::memset(this,0,sizeof(tagWToC));
	};
	void operator=( const tagWToC & src )
	{
		if( &src == this )	
			return;
		//////////
		dbW   = src.dbW;
		dbC   = src.dbC;
	}
} WTOC;

//б��������
struct GPara
{
	double DisLoop;
};

//����������
struct CPara
{
	double Dis;	
	int iLoopNum;
};

//����������
struct RPara
{
	//����������
	double	Dis;          //�����
	double	OriAngle;     //���Ƕ�
	BOOL	bFillToAndFro;//�������
	
	//������
	int		nFillNum;     //������
	double	ProAngle;     //�۽��Ƕ�
	double	AddDis;       //�۽����	
	
	//�Ż�����	
	double	dbStartOffset;//��ʼƫ��
	double	dbEndOffset;  //ĩβƫ��
	double	dbLineReduce; //��������
	BOOL	bEqualDis;    //�ȼ��

	//������������
	double	Reduce;       //�����߾�
	int		iLoopNum;     //����Ȧ��
	BOOL	bInner;       //����
	BOOL	bOuter;       //����
	
	//
	BOOL	bSaveCon;     //��������
	BOOL	bSaveFLine;   //���������
	BOOL	bConFirst;    //����������

	double dbLineReduceEnd;	// ��β��������
};


//�����괦��
typedef struct tagSubObjMarkSet
{	
	tagSubObjMarkSet()
	{
		bIsMarkFram   = FALSE;
		bIsMarkFill   = FALSE;
        bIsFillFirst  = FALSE;
		nLayer        = 0;
		nSubCnt       = 1;
		bIsRevMark    = FALSE;
	};
	
	void operator = ( const tagSubObjMarkSet & src )
	{
		if ( &src == this )
			return;
		
		bIsMarkFram= src.bIsMarkFram;
		bIsMarkFill	= src.bIsMarkFill;
        bIsFillFirst	= src.bIsFillFirst;
		nLayer       = src.nLayer;
		nSubCnt		= src.nSubCnt;
		bIsRevMark = src.bIsRevMark;
	}
	
public:
	int 		nSubCnt;
	int			nLayer;
	BOOL		bIsMarkFram;
	BOOL 	bIsMarkFill;
	BOOL 	bIsFillFirst;
	BOOL 	bIsRevMark;
} SUBOBJMARKSET;



typedef struct tagObjMarkSet
{	
	tagObjMarkSet()
	{
		nMarkCount = 0;
	};
	
	void operator = ( const tagObjMarkSet & src )
	{
		if( &src == this )	
			return;
		//////////////
		nMarkCount   = src.nMarkCount;
		memcpy(stSub, src.stSub, sizeof(SUBOBJMARKSET) * OBJ_MAX_MARKCOUNT);
		if (nMarkCount > OBJ_MAX_MARKCOUNT) 
		{
			nMarkCount = OBJ_MAX_MARKCOUNT; // ���ֻ�ܴ�����
		}
	}
	
	int   nMarkCount;
	SUBOBJMARKSET stSub[OBJ_MAX_MARKCOUNT];
}OBJMARKSET;


//
#define USER_RIGHT_ADMIN			0x0001  //0:����Ա     1
#define USER_RIGHT_NORMALMARK 0x0002  //1:ͨ�ô��   2
#define USER_RIGHT_MULMARK		0x0004  //2:���ĵ���� 4
#define USER_RIGHT_CREATE			0x0008  //3:��������   8
#define USER_RIGHT_EDIT				0x0010  //4:����༭   16
#define USER_RIGHT_SYSTEMSET	0x0020  //5:ϵͳ����   32
#define USER_RIGHT_CALI				0x0040  //6:У������   64
#define USER_RIGHT_SAVE			0x0080  //7:�ļ�����   128
#define USER_RIGHT_OPEN			0x0100  //8:���ĵ�   256
#define USER_RIGHT_LAYERSET		0x0200  //9:��������� 512
#define USER_RIGHT_ADVANCE		0x0400  //10:�߼�����  1024

typedef struct tagUserInfo
{
	tagUserInfo()
	{
		memset(this,0,sizeof(tagUserInfo));
	};

	void operator = ( const tagUserInfo & src )
	{
		if( &src == this )	
			return;
		
		dwRight1   = src.dwRight1;
		dwRight2   = src.dwRight2;
		nFlag = src.nFlag;
		bIsDefaultUser = src.bIsDefaultUser;
		memcpy(&szUserName, &(src.szUserName), 64*sizeof(TCHAR));
		memcpy(&szPassWord, &(src.szPassWord), 64*sizeof(TCHAR));
	}
	
	TCHAR    szUserName[64];
	TCHAR    szPassWord[64];
	BOOL     bIsDefaultUser;
	int			nFlag;
	
	// Bit 0:����Ա;   1:ͨ�ô��; 2:���ĵ����; 3:��������; 4:����༭
	//     5:ϵͳ����; 6:У������; 7:�ļ�����;   8:���ĵ�; 9:���������
	//    10:�߼�����
	DWORD   dwRight1;
	DWORD   dwRight2;
} USERINFO;


typedef struct tagRightInfo
{
	tagRightInfo()
	{
		memset(this,0,sizeof(tagRightInfo));
	};
	
public:
	int		nVersion;
	int     nUserCount;		
}RIGHTINFO;


#endif //__INNERMSG_H_98520F42_6FD0_424F_B982_87F79749E32F