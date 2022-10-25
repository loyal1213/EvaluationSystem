/***************************************************************
			�����������ݽṹ
****************************************************************/

#pragma pack(1)

typedef struct _net_way_point_node{
	short _num;						// ��·����
	char  _airSpace[20];
	float _longitude;				// ��·�㾭��
	float _latitude;				// ��·��γ��
	float _altitude;				// ��·��߶�
	float _vk;						// ��·���ٶ�
	float _traceLen;				// �ܵ�����
	float _tracePsi;				// �ܵ�����,��������̨�ͻ���
}NET_WayPointNode;

typedef struct _net_wp_nodes{
	short			_course5Num125;			// ���мƻ�5��·������, 31
	NET_WayPointNode _wpn[31];
}NET_WPNodes;

typedef struct _net_NavigationZone_Data //����
{
	BYTE	byNaviZoneNo;	    //���
	char	chNaviZoneName[20];	//����(10��������)
	BYTE	byNaviZoneType;		//ͼ������ 0������� 1��Բ��
	unsigned long	byNaviColor;		//��ɫ 0:�� 1:�� 2:�� 3:�� 4:�� 5:�� 6:�� 7:��
	BYTE    byCoordNum;         //�������Ŀ�����20��
	double	dLongitude[20];		//����㾭��
	double	dLatitude[20];		//�����γ��    
}NET_NAVIGATIONZONEDATA;

//���紫��ĺ���/��������
typedef struct _net_DLC_Data
{
	int							iMyselfExerciseIdentifier;	//������
	NET_WPNodes					sNavigationWPNodes;			//��·��
	BYTE						byNaviZoneNum;              //������Ŀ�����100������
	NET_NAVIGATIONZONEDATA		sNavigationZone[30];        //������Ϣ 
}NET_DLC_DATA;


#define pi 3.1415926535897932
#define MAXAIRCRAFT 100
#define BOLTZMANNS_CONSTANT	1.38E-23
struct	entityIDStruct
{
	BYTE byCamp;
	BYTE byType;
	BYTE byKind;
	BYTE byAttr;
	unsigned short wMainID;//ʵ���
	unsigned short wChildID;
};


struct	entityListStruct{
	struct	entityIDStruct	entityID[MAXAIRCRAFT];		//ʵ����
	unsigned long	disable_times[MAXAIRCRAFT];			//����ʱ�� 
	BYTE	number;										//�м���Ŀ��
};
struct PoseData
{
	float	psi;						//�����
	float	theta;						//������
	float	gamma;						//������
	float	alpha;						//ӭ��
	float	V;							//�����
	float	Vx;							//�ٶ�X����
	float	Vy;							//�ٶ�Y����
	float	Vz;							//�ٶ�Z����
	double	longitude;					//����
	double	latitude;					//γ��
	float	altitude;					//�߶�
};
struct TargetData
{
	float	EnterAngle;					//Ŀ������
	short	LockCount;					//��¼�������������ʱ��
	double	D;							//Ŀ����Ա�����б��
	float	M;							//Ŀ��M��

	float	targetPsi;					//Ŀ����Ա�����λ
	float	targetTheta;				//Ŀ����Ա�������
	float	targetPsiRPS;				//
	float	targetThetaRPS;				//
	float	acPsi;						//�������Ŀ�귽λ
	float	acTheta;					//�������Ŀ�긩��


	float	vxy_rel_sl;					//Ŀ���ڱ�����������ϵ�������xyƽ����ٶ�
	float	vx_rel_sl;					//Ŀ���ڱ�����������ϵ���������x���ٶ�
	float	vy_rel_sl;					//Ŀ���ڱ�����������ϵ���������y���ٶ�
	float	vz_rel_sl;					//Ŀ���ڱ�����������ϵ���������z���ٶ�

	float	vx_tb;					//Ŀ���ڱ�����������ϵx���ٶ�
	float	vy_tb;					//Ŀ���ڱ�����������ϵy���ٶ�
	float	vz_tb;					//Ŀ���ڱ�����������ϵz���ٶ�

	float	Vx;
	float	Vy;
	float	Vz;


	float	V_dpc;						//Ŀ���ٶ�dpc(�ڳ�memory״̬����D����ͬ)
	float	D_dpc;						//Ŀ��б��dpc(�ڳ�memory״̬����D����ͬ)
	float	altitude_dpc;				//Ŀ��߶�dpc(�ڳ�memory״̬����D����ͬ)
	float	psi_dpc;					//Ŀ���뱾��֮��ĺ���ǵĲ�ֵ
	float	Vz_dpc;						//Ŀ����Z���ϵ��ٶ�
	float	vx_rel_sl_dpc;				//Ŀ���ڡ�����������
	float	targetPsiRPS_dpc;			//
	float	targetThetaRPS_dpc;			//

	float	V_dpc_dot;					//Ŀ���ٶ�dpc_dot
	float	D_dpc_dot;					//Ŀ��б��dpc_dot
	float	altitude_dpc_dot;			//Ŀ��߶�dpc_dot
	float	psi_dpc_dot;				//Ŀ���뱾��֮��ĺ���ǵĲ�ֵ_dot
	float	Vz_dpc_dot;					//Ŀ����Z���ϵ��ٶ�_dot
	float	vx_rel_sl_dpc_dot;			//Ŀ���ڡ�����������_dot
	float	targetPsiRPS_dpc_dot;		//
	float	targetThetaRPS_dpc_dot;		//

	float	V_dpc_l;
	float	psi_dpc_l;
	float	targetPsiRPS_dpc_l;
	float	targetThetaRPS_dpc_l;

	float	V_self;						//Ŀ�������

	float	pdet;						//Ŀ��̽�����
	float	timeInMemory;				//����ʱ��
	short	RadStatus;					//�״����Ŀ��Ĺ���״̬JAMMING��MEMORY��SCAN��NOT_SEEN
	float	priority;					//Ŀ�����ȼ�
	short	priority_N;					//Ŀ�����ȼ������и���Ŀ���е�����
	short	AutoLockInhibit;			//��������ʶ
	short	t_AutoLockInhibit;			//������ʱ��
	short	selected;					//��ħ��ѡ��ı�ʶ

	double	longitude;					//����
	double	latitude;					//γ��
	float	altitude;					//�߶�
	float	psi;						//�����
	float	theta;						//������
	float	gamma;						//������
	unsigned char	jamming_power;				//���Ź��ʣ�ǿ�ȣ���
	char	jamming_mode;				//����ģʽ
};

struct RadarTargetList
{
	struct	entityListStruct	Name;
	struct  TargetData			TData[MAXAIRCRAFT];
};
struct  RadarData
{
	short	RadarName;					//�״�����

	float	SencorThetaSwitch;			//����������������ֵ��-1 ~ 1֮��(ģ��)
	float	SencorPsiSwitch;			//��������λ������ֵ��-1 ~ 1֮��(ģ��)
	char	RWS_SCAN_ANGLE_RANGE;		//RWS״̬�·�λɨ��Ƕȷ�Χ
	char	RWS_SCAN_ANGLE_RANGE_l;		//RWS״̬����һ�εķ�λɨ��Ƕȷ�Χ

	char	Other_Mode_l;				//�ϴν���ĳ��˸�����ģʽ
	char	ACM_Mode_l;					//�ϴν����ģʽ��
	char	AG_Mode_l;					//�ϴν����ģʽ��
	char	DogFight;					//�񶷵���
	char	DogFight_l;
	char	MFD_Status;
	char	TargetMangeFWD_l;
	char	TargetMangeAFT_l;
	char	TargetMangeRIGHT_l;
	char	TargetMangeLEFT_l;
	char	TargetMangeDOWN_l;
	char	DisplayMangeFWD_l;
	char	DisplayMangeAFT_l;
	char	DisplayMangeRIGHT_l;
	char	DisplayMangeLEFT_l;

	char	TargetMangeFWD;
	char	TargetMangeAFT;
	char	TargetMangeRIGHT;
	char	TargetMangeLEFT;
	char	TargetMangeDOWN;
	char	DisplayMangeFWD;
	char	DisplayMangeAFT;
	char	DisplayMangeRIGHT;
	char	DisplayMangeLEFT;

	char	ESSwitch;					//����ʶ�����
	short	RadarLockSwitch;			//�״���������0��ʾ����1��ʾ̧�𣨽���Ҳ�ǿ��������
	short	RadarLockSwitch_l;			//��һ�ε��״���������0��ʾ����1��ʾ̧�𣨽���Ҳ�ǿ��������
	short	RadarUNLockSwitch;			//�״�������
	short	RadarUNLockSwitch_l;		//��һ�ε��״�������
	short	RadarPowerSwitch;			//�״��Դ����0��ʾ�ض�1��ʾ��ͨ
	short	RadarRadiateSwitch;			//�״���俪��0��ʾ�ض�1��ʾ��ͨ

	short	RadarScanSequencingSwitch;	//�״�����ɨ���Ⱥ�˳�򿪹�
	short	RadarScanPatternSwitch;		//�״�����ɨ��ģʽѡ�񿪹�
	short	RadarScanState;				//�״�����ɨ��״̬
	short	RadarScanRate;				//�״�����ɨ���ٶ�
	short	RadarGimbalSlewThetaSwitch;	//�״�����ڸ�����ת����0��ʾ�ض�1��ʾ����-1��ʾ����
	short	RadarGimbalSlewPsiSwitch;	//�״�����ڷ�λ��ת����0��ʾ�ض�1��ʾ����-1��ʾ������
	short	RadarGimbalSlewRate;		//�״�����ڻ�ת�ٶ�
	float	RadarGimbalSlewTime;		//�״�����ڻ�ת����ʱ��ʵ���Ƿ��沽����λΪ(��)

	float	BindHigh;					//װ��Ŀ��߶�
	char	LockOnModeSwitch;			//�ػ�ʽ����0:�ֶ�1:�Զ�
	char	CenterModeSwitch;			//̽������ģʽ����0:�ֶ�1:�Զ�
	char	HPONModeSwitch;				//ͷ���ͨ����0���Ͽ� 1���ٸ��� 2������
	char	LaserSwitch;				//���⿪��0���Ͽ� 1������

	float	RadarScanThetaDem;
	float	RadarScanPsiDem;

	float	RadarScanRWSThetaMax;		//�״�ɨ��RWS�������ֵ
	float	RadarScanRWSThetaMin;		//�״�ɨ��RWS������Сֵ
	float	RadarScanRWSPsiMax;			//�״�ɨ��RWS��λ���ֵ
	float	RadarScanRWSPsiMin;			//�״�ɨ��RWS��λ��Сֵ

	float	RadarScanThetaMax;			//�״����߸���ɨ�����ֵ
	float	RadarScanThetaMin;			//�״����߸���ɨ����Сֵ
	float	RadarScanTheta;				//�״�����ڸ���ɨ������
	float	RadarScanTheta_l;			//��һ�����״�����ڸ���ɨ������
	float	RadarScanPsiMax;			//�״����߷�λɨ�����ֵ
	float	RadarScanPsiMin;			//�״����߷�λɨ����Сֵ
	float	RadarScanPsi;				//�״�����ڷ�λɨ������
	float	RadarScanPsi_l;				//��һ�����״�����ڷ�λɨ������
	short	RadarScanVertical;			//�״�ɨ�费ֱ
	short	RadarScanHorizontal;		//�״�ɨ��ˮƽ

	float	RadarCursorY;			//�״�ָ�븩��
	//	float	RadarCursorY_l;			//�״�ָ�븩��(last)
	float	RadarCursorX;				//�״�ָ�뷽λ
	//	float	RadarCursorX_l;			//�״�ָ�뷽λ(last)
	float	RadarCursorRange;			//�״�ָ�뷶Χ���۲����80/40/20/10/5��
	short	RadarDisplayRange;			//�״���ʾ��Χ��ֵΪ0 ~ 4
	short	RadarScanRowNum;			//�״�����ɨ������
	short	RadarScanRowNum_l;			//��һ�ε��״�����ɨ������


	short   RadarMode;					//�״����Ŀ��Ĺ���״̬JAMMING��MEMORY��SCAN��NOT_SEEN
	short	RadarStatus;				//�״�TWS RWS
	short	RadarStatus_l;				//
	short	RadarLock;					//�״�������ʶ
	short	TrackNumber;				//�״����Ŀ�����Ŀ


	short	AntennaScanDirection;		//����ɨ�跽��1��-1
	float	AntennaScanTheta;			//����ɨ�踩��
	float	AntennaScanPsi;				//����ɨ�跽λ
	double	Antenna_alt_max;			//�������߶�
	double	Antenna_alt_min;			//������С�߶�

	char	beamwidth_CTL;				//�������
	char	gain_CTL;					//���������
	char	repeatFrequency_CTL;
	char	carrierFrequency_Point;		//Ƶ���
	char	channl;						//ͨ����
	char	ECCM_level;					//�����ŵȼ�
	float	main_clutter_gain;			//��������
	float	carrierFrequency;			//��Ƶ
	float	repeatFrequency;			//�ظ�Ƶ��
	float	HOJ_timmer;					//HOJ״̬�Ķ�ʱ��

	struct	RadarTargetList		RTL;	//�״�Ŀ���б�
	struct	PoseData			PD;		//�����ظ�������̬����
	float   rdrDis;						//�״����
	float   laserDis;					//�������
	struct	entityIDStruct	  LockFired;//�����ظ������α�Ҫ������Ŀ��
};

struct FrameHead {//zhangwx,����֡ͷ
	WORD wType; //��������
	WORD wVer; //�汾���
	struct entityIDStruct eidSrc; //ʵ���ʶ
	struct entityIDStruct actSrc; //ʵ����ݱ�ʶ
	DWORD dwNo; //������ˮ��
	WORD wFlag; //��־��
	WORD wLength;  //���ݳ���,����֡ͷ
};
/******************************2 ʵ��״̬����***************************/
struct	aircraftStatusData{//ģ�������͵�ʵ��״̬����
	struct FrameHead fhFrmHead;
	long frameCounter;//֡��
	long lSec;//����
	long lMicroSec;//΢����
	double dbLong; //�ɻ�����
	double dbLati;//�ɻ�γ��
	float fHeight;//�ɻ��߶�
	float fPsi;//�����(��)
	float fTheta;//������(��)
	float fGama;//������(��)
	float fAlpha;//ӭ��(��)
	float fBeta;//�໬��(��)
	float fVx;//�ٶ�x����,���걱����
	float fVy;//�ٶ�y����
	float fVz;//�ٶ�Z����
	float fVcx;//����ϵ�ٶ�x��������/�룩
	float fVcy;//����ϵ�ٶ�y��������/�룩
	float fVcz;//����ϵ�ٶ�y��������/�룩
	float fAcx;//����ϵ���ٶ�x��������/���룩
	float fAcy;//����ϵ���ٶ�y��������/���룩
	float fAcz;//����ϵ���ٶ�y��������/���룩
	float M;//�����
	float fRp;//��ת���ٶȻ�����
	float fRq;//��ת���ٶȲ���
	float fRr;//��ת���ٶ�����
	float fARp;//��ת�Ǽ��ٶȻ����ᣨ��/���룩
	float fARq;//��ת�Ǽ��ٶȲ��ᣨ��/���룩
	float fARr;//��ת�Ǽ��ٶ����ᣨ��/���룩
	float fRevL;//��������ת�٣�������
	float fRevR;//�������ҷ�ת��
	float fRev3;//����������ת�٣�������
	float fRev4;//�������ķ�ת��
	float fPowerL;//�󷢼�����������
	float fPowerR;//�ҷ�����
	float fPower3;//����������������
	float fPower4;//�ķ�����
	long dwChainStatus;//�����豸״̬
	BYTE byItemStatus;//ʵ��״̬
	BYTE byRBFlag;//���ұ�ʶ
	BYTE byGroup;//����
	short sTKband;//����������
	long lPolitNo;//����Ա����
	float fVel; //����(����/Сʱ)
	BYTE byHitchScen; //װ�ҷ���
	BYTE byFireStatus; //1:����
	BYTE byReLoad; //2:��װ��
	BYTE byBatch; //���ţ���0�𣬷ɻ������ٺ��1
	BYTE abyReserved[100]; //���ڷ���ģ�����״��豸״̬����
} ;
struct	aircraftStatusData_entity
{
	short	disableTime;
	struct	aircraftStatusData a_s_d;
};
struct	aircraftStatusDataList
{
	short num;
	struct	aircraftStatusData_entity a_s_d_e[100];
};
struct	report40
{
	int	zhanhao;		//վ��
	int miyu;			//����
	int	pihao1;			//����1
	int	pihao2;			//����2
	double	longitude;	//����
	double  latitude;	//γ��
	int 	high;		//�߶�
	char	num;		//����
	int		time;		//ʱ��
	float	psi;		//����
	float	v;			//�ٶ�
};
struct	report40_entity
{
	short	disableTime;
	struct	report40	r_40;
};
struct	reportList40
{
	short num;
	struct	report40_entity		e_40[100];
};





#pragma pack()