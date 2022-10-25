/***************************************************************
			航区航线数据结构
****************************************************************/

#pragma pack(1)

typedef struct _net_way_point_node{
	short _num;						// 航路点编号
	char  _airSpace[20];
	float _longitude;				// 航路点经度
	float _latitude;				// 航路点纬度
	float _altitude;				// 航路点高度
	float _vk;						// 航路点速度
	float _traceLen;				// 跑道长度
	float _tracePsi;				// 跑道方向,用于塔康台和机场
}NET_WayPointNode;

typedef struct _net_wp_nodes{
	short			_course5Num125;			// 飞行计划5航路点数量, 31
	NET_WayPointNode _wpn[31];
}NET_WPNodes;

typedef struct _net_NavigationZone_Data //航区
{
	BYTE	byNaviZoneNo;	    //编号
	char	chNaviZoneName[20];	//名称(10个汉字内)
	BYTE	byNaviZoneType;		//图形类型 0：多边形 1：圆形
	unsigned long	byNaviColor;		//颜色 0:白 1:红 2:橙 3:黄 4:绿 5:青 6:蓝 7:紫
	BYTE    byCoordNum;         //坐标点数目，最多20个
	double	dLongitude[20];		//坐标点经度
	double	dLatitude[20];		//坐标点纬度    
}NET_NAVIGATIONZONEDATA;

//网络传输的航线/航区数据
typedef struct _net_DLC_Data
{
	int							iMyselfExerciseIdentifier;	//演练号
	NET_WPNodes					sNavigationWPNodes;			//航路点
	BYTE						byNaviZoneNum;              //航区数目，最多100个航区
	NET_NAVIGATIONZONEDATA		sNavigationZone[30];        //航区信息 
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
	unsigned short wMainID;//实体号
	unsigned short wChildID;
};


struct	entityListStruct{
	struct	entityIDStruct	entityID[MAXAIRCRAFT];		//实体编号
	unsigned long	disable_times[MAXAIRCRAFT];			//存在时间 
	BYTE	number;										//有几个目标
};
struct PoseData
{
	float	psi;						//航向角
	float	theta;						//俯仰角
	float	gamma;						//滚动角
	float	alpha;						//迎角
	float	V;							//真空速
	float	Vx;							//速度X分量
	float	Vy;							//速度Y分量
	float	Vz;							//速度Z分量
	double	longitude;					//经度
	double	latitude;					//纬度
	float	altitude;					//高度
};
struct TargetData
{
	float	EnterAngle;					//目标进入角
	short	LockCount;					//记录按下锁定键后的时间
	double	D;							//目标相对本机的斜距
	float	M;							//目标M数

	float	targetPsi;					//目标相对本机方位
	float	targetTheta;				//目标相对本机俯仰
	float	targetPsiRPS;				//
	float	targetThetaRPS;				//
	float	acPsi;						//本机相对目标方位
	float	acTheta;					//本机相对目标俯仰


	float	vxy_rel_sl;					//目标在本机机体坐标系相对视线xy平面的速度
	float	vx_rel_sl;					//目标在本机机体坐标系相对视线沿x的速度
	float	vy_rel_sl;					//目标在本机机体坐标系相对视线沿y的速度
	float	vz_rel_sl;					//目标在本机机体坐标系相对视线沿z的速度

	float	vx_tb;					//目标在本机机体坐标系x的速度
	float	vy_tb;					//目标在本机机体坐标系y的速度
	float	vz_tb;					//目标在本机机体坐标系z的速度

	float	Vx;
	float	Vy;
	float	Vz;


	float	V_dpc;						//目标速度dpc(在除memory状态外与D都相同)
	float	D_dpc;						//目标斜距dpc(在除memory状态外与D都相同)
	float	altitude_dpc;				//目标高度dpc(在除memory状态外与D都相同)
	float	psi_dpc;					//目标与本机之间的航向角的差值
	float	Vz_dpc;						//目标在Z轴上的速度
	float	vx_rel_sl_dpc;				//目标在。。。。。。
	float	targetPsiRPS_dpc;			//
	float	targetThetaRPS_dpc;			//

	float	V_dpc_dot;					//目标速度dpc_dot
	float	D_dpc_dot;					//目标斜距dpc_dot
	float	altitude_dpc_dot;			//目标高度dpc_dot
	float	psi_dpc_dot;				//目标与本机之间的航向角的差值_dot
	float	Vz_dpc_dot;					//目标在Z轴上的速度_dot
	float	vx_rel_sl_dpc_dot;			//目标在。。。。。。_dot
	float	targetPsiRPS_dpc_dot;		//
	float	targetThetaRPS_dpc_dot;		//

	float	V_dpc_l;
	float	psi_dpc_l;
	float	targetPsiRPS_dpc_l;
	float	targetThetaRPS_dpc_l;

	float	V_self;						//目标真空速

	float	pdet;						//目标探测概率
	float	timeInMemory;				//存在时间
	short	RadStatus;					//雷达对于目标的工作状态JAMMING，MEMORY，SCAN，NOT_SEEN
	float	priority;					//目标优先级
	short	priority_N;					//目标优先级在所有跟踪目标中的排序
	short	AutoLockInhibit;			//自锁定标识
	short	t_AutoLockInhibit;			//自锁定时间
	short	selected;					//被魔球选择的标识

	double	longitude;					//经度
	double	latitude;					//纬度
	float	altitude;					//高度
	float	psi;						//航向角
	float	theta;						//俯仰角
	float	gamma;						//滚动角
	unsigned char	jamming_power;				//干扰功率（强度）；
	char	jamming_mode;				//干扰模式
};

struct RadarTargetList
{
	struct	entityListStruct	Name;
	struct  TargetData			TData[MAXAIRCRAFT];
};
struct  RadarData
{
	short	RadarName;					//雷达名称

	float	SencorThetaSwitch;			//传感器俯仰开关数值是-1 ~ 1之间(模入)
	float	SencorPsiSwitch;			//传感器方位开关数值是-1 ~ 1之间(模入)
	char	RWS_SCAN_ANGLE_RANGE;		//RWS状态下方位扫描角度范围
	char	RWS_SCAN_ANGLE_RANGE_l;		//RWS状态下上一次的方位扫描角度范围

	char	Other_Mode_l;				//上次进入的除了格斗以外模式
	char	ACM_Mode_l;					//上次进入格斗模式的
	char	AG_Mode_l;					//上次进入格斗模式的
	char	DogFight;					//格斗电门
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

	char	ESSwitch;					//敌我识别电门
	short	RadarLockSwitch;			//雷达锁定开关0表示按下1表示抬起（解锁也是靠这个键）
	short	RadarLockSwitch_l;			//上一次的雷达锁定开关0表示按下1表示抬起（解锁也是靠这个键）
	short	RadarUNLockSwitch;			//雷达解除锁定
	short	RadarUNLockSwitch_l;		//上一次的雷达解除锁定
	short	RadarPowerSwitch;			//雷达电源开关0表示关断1表示接通
	short	RadarRadiateSwitch;			//雷达辐射开关0表示关断1表示接通

	short	RadarScanSequencingSwitch;	//雷达天线扫描先后顺序开关
	short	RadarScanPatternSwitch;		//雷达天线扫描模式选择开关
	short	RadarScanState;				//雷达天线扫描状态
	short	RadarScanRate;				//雷达天线扫描速度
	short	RadarGimbalSlewThetaSwitch;	//雷达万向节俯仰回转开关0表示关断1表示向上-1表示向下
	short	RadarGimbalSlewPsiSwitch;	//雷达万向节方位回转开关0表示关断1表示向左-1表示向有右
	short	RadarGimbalSlewRate;		//雷达万向节回转速度
	float	RadarGimbalSlewTime;		//雷达万向节回转步长时间实际是仿真步长单位为(秒)

	float	BindHigh;					//装订目标高度
	char	LockOnModeSwitch;			//截获方式开关0:手动1:自动
	char	CenterModeSwitch;			//探测中心模式开关0:手动1:自动
	char	HPONModeSwitch;				//头瞄接通开关0：断开 1：假负载 2：辐射
	char	LaserSwitch;				//激光开关0：断开 1：辐射

	float	RadarScanThetaDem;
	float	RadarScanPsiDem;

	float	RadarScanRWSThetaMax;		//雷达扫描RWS俯仰最大值
	float	RadarScanRWSThetaMin;		//雷达扫描RWS俯仰最小值
	float	RadarScanRWSPsiMax;			//雷达扫描RWS方位最大值
	float	RadarScanRWSPsiMin;			//雷达扫描RWS方位最小值

	float	RadarScanThetaMax;			//雷达天线俯仰扫描最大值
	float	RadarScanThetaMin;			//雷达天线俯仰扫描最小值
	float	RadarScanTheta;				//雷达万向节俯仰扫描中心
	float	RadarScanTheta_l;			//上一周期雷达万向节俯仰扫描中心
	float	RadarScanPsiMax;			//雷达天线方位扫描最大值
	float	RadarScanPsiMin;			//雷达天线方位扫描最小值
	float	RadarScanPsi;				//雷达万向节方位扫描中心
	float	RadarScanPsi_l;				//上一周期雷达万向节方位扫描中心
	short	RadarScanVertical;			//雷达扫描垂直
	short	RadarScanHorizontal;		//雷达扫描水平

	float	RadarCursorY;			//雷达指针俯仰
	//	float	RadarCursorY_l;			//雷达指针俯仰(last)
	float	RadarCursorX;				//雷达指针方位
	//	float	RadarCursorX_l;			//雷达指针方位(last)
	float	RadarCursorRange;			//雷达指针范围（观测距离80/40/20/10/5）
	short	RadarDisplayRange;			//雷达显示范围数值为0 ~ 4
	short	RadarScanRowNum;			//雷达天线扫描行数
	short	RadarScanRowNum_l;			//上一次的雷达天线扫描行数


	short   RadarMode;					//雷达对于目标的工作状态JAMMING，MEMORY，SCAN，NOT_SEEN
	short	RadarStatus;				//雷达TWS RWS
	short	RadarStatus_l;				//
	short	RadarLock;					//雷达锁定标识
	short	TrackNumber;				//雷达跟踪目标的数目


	short	AntennaScanDirection;		//天线扫描方向1，-1
	float	AntennaScanTheta;			//天线扫描俯仰
	float	AntennaScanPsi;				//天线扫描方位
	double	Antenna_alt_max;			//天线最大高度
	double	Antenna_alt_min;			//天线最小高度

	char	beamwidth_CTL;				//带宽控制
	char	gain_CTL;					//增益控制量
	char	repeatFrequency_CTL;
	char	carrierFrequency_Point;		//频点号
	char	channl;						//通道号
	char	ECCM_level;					//抗干扰等级
	float	main_clutter_gain;			//主瓣增益
	float	carrierFrequency;			//载频
	float	repeatFrequency;			//重复频率
	float	HOJ_timmer;					//HOJ状态的定时器

	struct	RadarTargetList		RTL;	//雷达目标列表
	struct	PoseData			PD;		//由主控给出的姿态数据
	float   rdrDis;						//雷达距离
	float   laserDis;					//激光距离
	struct	entityIDStruct	  LockFired;//由主控给出的游标要锁定的目标
};

struct FrameHead {//zhangwx,数据帧头
	WORD wType; //数据类型
	WORD wVer; //版本编号
	struct entityIDStruct eidSrc; //实体标识
	struct entityIDStruct actSrc; //实体扮演标识
	DWORD dwNo; //数据流水号
	WORD wFlag; //标志字
	WORD wLength;  //数据长度,不含帧头
};
/******************************2 实体状态数据***************************/
struct	aircraftStatusData{//模拟器发送的实体状态数据
	struct FrameHead fhFrmHead;
	long frameCounter;//帧数
	long lSec;//秒数
	long lMicroSec;//微秒数
	double dbLong; //飞机经度
	double dbLati;//飞机纬度
	float fHeight;//飞机高度
	float fPsi;//航向角(度)
	float fTheta;//俯仰角(度)
	float fGama;//滚动角(度)
	float fAlpha;//迎角(度)
	float fBeta;//侧滑角(度)
	float fVx;//速度x分量,坐标北东下
	float fVy;//速度y分量
	float fVz;//速度Z分量
	float fVcx;//地心系速度x分量（米/秒）
	float fVcy;//地心系速度y分量（米/秒）
	float fVcz;//地心系速度y分量（米/秒）
	float fAcx;//地心系加速度x分量（米/秒秒）
	float fAcy;//地心系加速度y分量（米/秒秒）
	float fAcz;//地心系加速度y分量（米/秒秒）
	float M;//马赫数
	float fRp;//滚转角速度机体轴
	float fRq;//滚转角速度侧轴
	float fRr;//滚转角速度立轴
	float fARp;//滚转角加速度机体轴（米/秒秒）
	float fARq;//滚转角加速度侧轴（米/秒秒）
	float fARr;//滚转角加速度立轴（米/秒秒）
	float fRevL;//发动机左发转速（单发）
	float fRevR;//发动机右发转速
	float fRev3;//发动机三发转速（单发）
	float fRev4;//发动机四发转速
	float fPowerL;//左发加力（单发）
	float fPowerR;//右发加力
	float fPower3;//三发加力（单发）
	float fPower4;//四发加力
	long dwChainStatus;//铰链设备状态
	BYTE byItemStatus;//实体状态
	BYTE byRBFlag;//敌我标识
	BYTE byGroup;//批次
	short sTKband;//塔康波道号
	long lPolitNo;//飞行员代号
	float fVel; //空速(公里/小时)
	BYTE byHitchScen; //装挂方案
	BYTE byFireStatus; //1:发射
	BYTE byReLoad; //2:重装弹
	BYTE byBatch; //批号：从0起，飞机被击毁后加1
	BYTE abyReserved[100]; //用于发送模拟器雷达设备状态数据
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
	int	zhanhao;		//站号
	int miyu;			//密语
	int	pihao1;			//批号1
	int	pihao2;			//批号2
	double	longitude;	//经度
	double  latitude;	//纬度
	int 	high;		//高度
	char	num;		//数量
	int		time;		//时间
	float	psi;		//航向
	float	v;			//速度
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