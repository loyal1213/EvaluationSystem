// MFC_OSG.cpp : implementation of the cOSG class
//
#include "stdafx.h"
#include "OSG.h"
#include "ModelPositionCallback.h"
#include "Compass.h"
#include "NavigationZoneAndLineStruct.h"
#include "CreateTrackCallbcak.h"
#include "TrailerCallback.h"
/******************************************************************/
#include <osgEarth/MapNode>

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>

#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/ImageOverlayEditor>

#include <osgEarthSymbology/GeometryFactory>

#include <osgViewer/Viewer>

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Features;
using namespace osgEarth::Util;


#include<Windows.h>
#include<osgViewer/Viewer>
#include<osgDB/ReadFile>
#include<osg/MatrixTransform>
#include<osgEarth/MapNode>
#include<osg/AnimationPath>
#include<osg/CoordinateSystemNode>
#include <osg/PositionAttitudeTransform>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osg/Notify>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/MouseCoordsTool>
#include <osgEarthUtil/MGRSFormatter>
#include <osgEarthUtil/LatLongFormatter>

#include <osgEarthUtil/GeodeticGraticule>
#include <osgEarthUtil/MGRSGraticule>
#include <osgEarthUtil/UTMGraticule>
#include <osgEarthUtil/GARSGraticule>
#include "PickHandler.h"

using namespace osgEarth::Util;
using namespace osgEarth::Annotation;
/******************************************************************/

#include<Windows.h>
#include <osgEarth/MapNode>

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>

#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/ImageOverlayEditor>

#include <osgEarthSymbology/GeometryFactory>

#include <osgViewer/Viewer>
#include<string>

//键盘事件
#include<osgViewer/ViewerEventHandlers>
//osgGA相关的库
#include<osgGA/StateSetManipulator>

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Features;
using namespace osgEarth::Util;


/******************************************************************/
osg::ref_ptr<osgViewer::Viewer> g_viewer;
osg::MatrixTransform* g_mt_compass = new osg::MatrixTransform();

cOSG::cOSG(HWND hWnd) :
	m_hWnd(hWnd),
	label_event_(nullptr)
{
}

cOSG::~cOSG()
{
	mViewer->setDone(true);
	Sleep(1000);
	mViewer->stopThreading();

	delete mViewer;
}

void cOSG::InitOSG(std::string modelname)
{
	// Store the name of the model to load
	m_ModelName = modelname;

	// Init different parts of OSG
	InitManipulators();
	InitSceneGraph();
	InitCameraConfig();
	g_viewer = getViewer();

	// 创建星空
	CreateStarrySky();

	// 显示经纬度高度
	AddViewPointLable();

	// 添加飞机模型
	AddAirPlane();
	// DoPreLineNow();

	// 添加指南针
	AddCompass();

	// 创建飞机历史航迹
	AddBuildHistoryRoute(mt_airfly_, 1.0f);

	// 创建飞机飞行彩带
	// AddBuildRibbon(512, mt_airfly_,20.0f);

	// 设置飞机视角跟随 接收 udp 数据，实时仿真
	// RealTimeSimulation();

	// 在左上角添加经度度标签，鼠标移动时候显示  
	// AddLonLatLabel();

	AddAnnotation();

	//定义视点类对象
	/*osgEarth::Viewpoint vp;
	//设置视点参数，focalPoint()函数返回的是一个点_point，然后调用set（）函数进行设置，第一个参数是空间参考参数，第二个是经度，第三个是纬度，第四个是Z值
	//最后一个参数用来描述Z是怎么解释的，有两种，ALTMODE_ABSOLUTE是绝对高程，从椭球体（？）开始算的，ALTMODE_RELATIVE是相对高程，从地形的标高以上开始算的，就是基准点在那个经纬度对应的点的最高处
	vp.focalPoint()->set(earth_map_node_->getMapSRS()->getGeographicSRS(), 112.36502062, 32.57574867, 0, osgEarth::ALTMODE_ABSOLUTE);
	//设置相机相对于焦点的俯仰
	vp.pitch() = -90.0;
	//相机到focalPoint点的距离，也就是高度
	vp.range() = 250000;*/
	// earth_manipulator_->setViewpoint(osgEarth::Viewpoint("", 112.36502062, 32.57574867, -2.5f, -90.0, 0.0, 1000), 5);
	// earth_manipulator_->setHomeViewpoint(vp, 5);

}

void cOSG::AddLonLatLabel()
{
	osgEarth::Util::Formatter* formatter = 0L;
	osgEarth::Util::GeodeticGraticule* gr = new osgEarth::Util::GeodeticGraticule();
	earth_map_node_->getMap()->addLayer(gr);
	formatter = new osgEarth::Util::LatLongFormatter();


	// mouse coordinate readout:
	osgEarth::Util::ControlCanvas* canvas = new osgEarth::Util::ControlCanvas();
	mRoot->addChild( canvas );
	osgEarth::Util::VBox* vbox = new osgEarth::Util::VBox();
	canvas->addControl( vbox );

	osgEarth::Util::LabelControl* readout = new osgEarth::Util::LabelControl();
	vbox->addControl( readout );

	osgEarth::Util::MouseCoordsTool* tool = new osgEarth::Util::MouseCoordsTool( earth_map_node_ );
	tool->addCallback(new osgEarth::Util::MouseCoordsLabelCallback(readout, formatter));
	mViewer->addEventHandler( tool );


	// disable the small-feature culling
	mViewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

	// set a near/far ratio that is smaller than the default. This allows us to get
	// closer to the ground without near clipping. If you need more, use --logdepth
	mViewer->getCamera()->setNearFarRatio(0.0001);
}

// 实时跟踪
void cOSG::RealTimeSimulation(bool is_track)
{
	osgEarth::Viewpoint vp = earth_manipulator_->getViewpoint();
	if (is_track){
		vp.setNode(mt_airfly_); //node_airfly_);mt_airfly_
		vp.heading()->set(0/*24.261*/, osgEarth::Units::DEGREES);
		vp.range()->set(3000.95, osgEarth::Units::METERS); // 350000.0
		vp.pitch()->set(-9.701, osgEarth::Units::DEGREES);
		earth_manipulator_->setViewpoint(vp, 5.0);
	}else{
		vp.setNode(0);
	}	
}

void cOSG::AddCompass()
{
	// 添加指南针
	osg::ref_ptr<Compass> compass = new Compass;
	compass->setProjectionMatrix(osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0));
	compass->setPlate(createCompassPart("c:/track_data/images/panzi.png", 1.5f, -1.0f)); //圆盘图片
	compass->setNeedle(createCompassPart("c:/track_data/images/noddle.png", 1.5f, 0.0f));//指针图片
	compass->setWidthHeight(10,10,100,100); //起始点、宽高
	compass->setMainCamera(mViewer->getCamera());

	compass->setRenderOrder(osg::Camera::POST_RENDER);
	compass->setClearMask(GL_DEPTH_BUFFER_BIT);
	compass->setAllowEventFocus(false);
	compass->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	compass->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	compass->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	mRoot->addChild(compass); //加入跟节点

}

void cOSG::InitManipulators(void)
{
	// Create a trackball manipulator
	trackball = new osgGA::TrackballManipulator();

	// Create a Manipulator Switcher
	keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	// Add our trackball manipulator to the switcher
	keyswitchManipulator->addMatrixManipulator( '1', "Trackball", trackball.get());

	// Init the switcher to the first manipulator (in this case the only manipulator)
	keyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value

	// 初始化 earth 操作器
	earth_manipulator_ = new osgEarth::Util::EarthManipulator;

}


void cOSG::InitSceneGraph(void)
{
	// Init the main Root Node/Group
	mRoot  = new osg::Group;

	// Load the Model from the model name
	mModel = osgDB::readNodeFile(m_ModelName);
	if (!mModel) return;

	//查询地图节点
	earth_map_node_ = osgEarth::MapNode::findMapNode(mModel);
	if (!earth_map_node_) return;

	earth_manipulator_->setNode(earth_map_node_);
	earth_manipulator_->getSettings()->setArcViewpointTransitions(true);

	// Optimize the model
	osgUtil::Optimizer optimizer;
	optimizer.optimize(mModel.get());
	optimizer.reset();

	// Add the model to the scene
	mRoot->addChild(mModel.get());

}

void cOSG::InitCameraConfig(void)
{
	// Local Variable to hold window size data
	RECT rect;

	// Create the viewer for this window
	mViewer = new osgViewer::Viewer();

	// Add a Stats Handler to the viewer
	mViewer->addEventHandler(new osgViewer::StatsHandler);

	// Get the current window size
	::GetWindowRect(m_hWnd, &rect);

	// 设置并创建图形环境
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

	// Init the Windata Variable that holds the handle for the Window to display OSG in.
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);

	// Setup the traits parameters
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;

	//创建图形环境特性
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc->valid()){
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 1.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}else{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	// Init Master Camera for this View
	osg::ref_ptr<osg::Camera> camera = mViewer->getCamera();


	//根据分辨率确定合适的投影来保证显示的图形不变形
	double fovy, aspectRatio, zNear, zFar;
	camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0){
		camera->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//设置相机视口及图形环境
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setGraphicsContext(gc);

	//根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	camera->setDrawBuffer(bufferMaster);
	camera->setReadBuffer(bufferMaster);

	// Set projection matrix and camera attribtues
	camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0, 1000.0);

	// Add the Camera to the Viewer
	//mViewer->addSlave(camera.get());
	mViewer->setCamera(camera.get());

	// Add the Camera Manipulator to the Viewer
	//mViewer->setCameraManipulator(keyswitchManipulator.get());
	mViewer->setCameraManipulator(earth_manipulator_);

	// Set the Scene Data
	mViewer->setSceneData(mRoot.get());
	osg::ref_ptr<osgText::Text> updateText = new osgText::Text;
	mViewer->addEventHandler(new CPickHandler(updateText));
	mViewer->addEventHandler(new osgViewer::WindowSizeHandler());//F键控制全/半屏

	mViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);

	// Realize the Viewer
	mViewer->realize();

	// Correct aspect ratio
	/*double fovy,aspectRatio,z1,z2;
	mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
	aspectRatio=double(traits->width)/double(traits->height);
	mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);*/
}

void cOSG::PreFrameUpdate()
{
	// Due any preframe updates in this routine
}

void cOSG::PostFrameUpdate()
{
	// Due any postframe updates in this routine
}


// 创建星空
int cOSG::CreateStarrySky(void)
{
	// 设置时间
	osgEarth::DateTime dateTime(2022, 7, 17, 20);		//格林尼治时间
	osgEarth::Util::SkyOptions skyOptions;
	skyOptions.ambient() = 0.4;//控制黑夜部分明暗程度，数值越小，越黑暗

	osgEarth::Util::Ephemeris* ephemeris = new osgEarth::Util::Ephemeris;

	osgEarth::Util::SkyNode* m_pSkyNode = osgEarth::Util::SkyNode::create(skyOptions, earth_map_node_);
	m_pSkyNode->setName("SkyNode");
	m_pSkyNode->setEphemeris(ephemeris);
	m_pSkyNode->setDateTime(dateTime);
	m_pSkyNode->attach(mViewer, 0);
	m_pSkyNode->setLighting(true);
	m_pSkyNode->addChild(earth_map_node_);
	mRoot->addChild(m_pSkyNode);

	return 0;
}

CRenderingThread::CRenderingThread( cOSG* ptr )
	:   OpenThreads::Thread(), _ptr(ptr), _done(false)
{
}

CRenderingThread::~CRenderingThread()
{
	_done = true;
	if (isRunning())
	{
		cancel();
		join();
	}
}

void CRenderingThread::run()
{
	if ( !_ptr )
	{
		_done = true;
		return;
	}

	osgViewer::Viewer* viewer = _ptr->getViewer();
	do
	{
		_ptr->PreFrameUpdate();
		viewer->frame();
		_ptr->PostFrameUpdate();
	} while ( !testCancel() && !viewer->done() && !_done );
}

void UpdateCompass::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	osg::Matrix vm = g_viewer->getCamera()->getViewMatrix();
	vm.setTrans(osg::Vec3());
	//只考虚vm的旋转

	//观察矩阵是单位矩阵的情况下是朝向Z轴负方向的，假如我们认为Z轴的负方向是朝北，则经过了vm之后，现在朝chaoXiang
	//X轴正方向是朝东

	//接着vm改变了，就看其绕y轴转了多少角度，其它的旋转都可以忽略
	osg::Vec3 chaoXiang = (-osg::Z_AXIS)*vm;
	chaoXiang.y() = 0.0;
	chaoXiang.normalize();

	//接下来看(-osg::Z_AXIS)和chaoXiang的夹角就是指针应该转多少度
	float angle = acos((-osg::Z_AXIS)*chaoXiang);

	//因为acos的范围是[0, PI]，但是东西南北的旋转是360度的，因此要判断夹角的正负，使用叉乘来判断
	osg::Vec3f axis = (-osg::Z_AXIS) ^ chaoXiang;
	if (axis*osg::Y_AXIS < 0)
	{
		angle = 2 * osg::PI - angle;
	}

	g_mt_compass->setMatrix(osg::Matrix::translate(osg::Vec3(-64, -64, 0))*osg::Matrix::rotate(angle, osg::Z_AXIS)*osg::Matrix::translate(osg::Vec3(64, 64, 0)));

	traverse(node, nv);
}

// 添加标签控件，显示经纬度高度
void cOSG::AddViewPointLable()
{
	if (mViewer==nullptr){
		TRACE(TEXT("mViewer is null, at addViewPointLable!"));
		return ;
	}

	canvas_ = osgEarth::Util::Controls::ControlCanvas::getOrCreate(mViewer);

	// 添加控件，用来显示视点信息
	osgEarth::Util::Controls::LabelControl* mouse_coords = new osgEarth::Util::Controls::LabelControl(TEXT("mouse_point"), Color::White);
	mouse_coords->setBackColor(osgEarth::Util::Controls::Color(osgEarth::Util::Controls::Color::Green, 0.5));
	mouse_coords->setFont(osgText::readFontFile("c:/track_data/fonts/msyh.ttf"));
	mouse_coords->setEncoding(osgText::String::ENCODING_UTF8);
	mouse_coords->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 1));
	mouse_coords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouse_coords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);

	// mRoot->addChild(osgEarth::Util::Controls::ControlCanvas::get(mViewer));
	// osgEarth::Util::Controls::ControlCanvas* canvas = osgEarth::Util::Controls::ControlCanvas::get(mViewer);

	// 添加控件用来显示视点信息
	osgEarth::Util::Controls::LabelControl* view_coods = new osgEarth::Util::Controls::LabelControl("view_point",osg::Vec4(1.0,0.0,0.0,1.0));
	view_coods->setFont(osgText::readFontFile("c:/track_data/fonts/msyh.ttf"));
	view_coods->setEncoding(osgText::String::ENCODING_UTF8);
	// view_coods->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 1));
	view_coods->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	view_coods->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	view_coods->setBackColor(osg::Vec4(0,0,0,0.5));
	//view_coods->setSize(500,50);
	view_coods->setMargin(10);


	// 添加控件，用来显示飞机信息
	osgEarth::Util::Controls::LabelControl* fly_coords = new osgEarth::Util::Controls::LabelControl(TEXT("fly_point"), Color::White);
	fly_coords->setBackColor(osgEarth::Util::Controls::Color(osgEarth::Util::Controls::Color::Green, 0.5));
	fly_coords->setFont(osgText::readFontFile("c:/track_data/fonts/msyh.ttf"));
	fly_coords->setEncoding(osgText::String::ENCODING_UTF8);
	fly_coords->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 1));
	fly_coords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	fly_coords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);


	canvas_->addControl(mouse_coords);
	canvas_->addControl(view_coods);
	canvas_->addControl(fly_coords);

	if (label_event_ == 0){
		label_event_ = new CLabelControlEventHandler(earth_map_node_,mouse_coords,view_coods,fly_coords);
	}

	mViewer->addEventHandler(label_event_);

	// 添加控件用来显示鼠标交点信息
	/*osgEarth::Util::Controls::LabelControl* mouse_coods = new osgEarth::Util::Controls::LabelControl("TestViewpoint2",osg::Vec4(1.0,0.0,0.0,1.0));
	mouse_coods->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouse_coods->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	mouse_coods->setBackColor(osg::Vec4(0,0,0,0.5));
	mouse_coods->setSize(400,50);
	mouse_coods->setMargin(10);
	canvas->addChild(mouse_coods);*/

}


//void cOSG::OnRecv(const char* buf, USHORT len, const char* fromIp, USHORT fromPort){
/*aircraftStatusData *dlc_data = (aircraftStatusData *)buf;
TRACE("经度:%0.8f,维度:%0.8f,高度:%0.8f,\
航向角:%0.8f,俯仰角:%0.8f,翻滚角:%0.8f,\
迎角:%0.8f,侧滑角:%0.8f,空速:%0.8f\t,ip:%s,port:%d\n",\
dlc_data->dbLong,dlc_data->dbLati,dlc_data->fHeight,\
dlc_data->fPsi,dlc_data->fTheta,dlc_data->fGama,\
dlc_data->fAlpha,dlc_data->fTheta,dlc_data->fVel,fromIp, fromPort);
osg::Matrix matrix;
coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(dlc_data->dbLati), osg::DegreesToRadians(dlc_data->dbLong), 5e5, matrix);
matrix.preMult(osg::Matrix::scale(3e4, 3e4, 3e4)*osg::Matrix::rotate(osg::inDegrees(180.0), osg::Vec3(0, 0, 1)));
xform_->setMatrix(matrix);
//mViewer->frame();
Sleep(20);*/
//log_cur += 0.0001;
//lat_cur += 0.00001;

//earthManipulator->setViewpoint(osgEarth::Viewpoint("模拟无人机", log_cur, lat_cur, 0.0, 0.0, -90.0, 5e2));//const char* name, double lon, double lat, double z, double heading(水平角度), double pitch（垂直角度）, double range（视点距离）
//}

osg::MatrixTransform* cOSG::createCompassPart(const std::string &image, float radius, float height)
{
	osg::Vec3 center(-radius, -radius, height);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(
		createTexturedQuadGeometry(center, osg::Vec3(radius*2.0f, 0.0f, 0.0f), osg::Vec3(0.0f, radius*2.0f, 0.0f)));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(image));

	osg::ref_ptr<osg::MatrixTransform> part = new osg::MatrixTransform;
	part->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	part->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	part->addChild(geode.get());
	return part.release();
}

int cOSG::AddAirPlane(void)
{
	//if (0){
	//	coordinate_system_node_ = new osg::CoordinateSystemNode; // 创建坐标系节点
	//	coordinate_system_node_->setEllipsoidModel(new osg::EllipsoidModel()); // 设置椭圆体模型

	//	// 加载机场
	//	airport_ = osgDB::readNodeFile("C:/track_data/airport/heinei_airport.ive"); // 读取机场文件
	//	mtAirport_ = new osg::MatrixTransform; // 矩阵变换
	//	mtAirport_->addChild(airport_);
	//	mRoot->addChild(mtAirport_);

	//	// 设置机场矩阵
	//	osg::Matrixd mtTemp;   // 机场位置  109.13 34.38 高度：8434.96  海拔：390
	//	coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 460, mtTemp);
	//	mtAirport_->setMatrix(mtTemp);

	//	// 加载飞机
	//	osg::Matrixd::value_type plane_angle = osg::PI_4f*1.6554;  //正值： 逆时针  

	//	node_airfly_ = osgDB::readNodeFile("c:/track_data/aircrafts/F-16.ive"); // 读取飞机文件
	//	node_airfly_->setName(TEXT("F16"));
	//	airfly_mt_ = new osg::MatrixTransform();
	//	airfly_mt_->setDataVariance(osg::Object::STATIC);
	//	airfly_mt_->setMatrix(osg::Matrix::scale(10,10,10)
	//		* osg::Matrixd::rotate(osg::DegreesToRadians(75.0f), osg::Vec3(0,0,1))
	//		// * osg::Matrix::translate(osg::Vec3f(0, 0, 0))
	//		); // -(osg::PI_2/2*10)

	//	airfly_mt_->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL,osg::StateAttribute::ON);// 设置属性，光照法线
	//	airfly_mt_->addChild(node_airfly_);

	//	// airfly_positioned_->addChild(m_pBuildRader.BuildRader(50,30).get());


	//	// mtrix_fly_self->addChild(m_pBuildRader->BuildRader(500,300).get());
	//	xform_ = new osg::MatrixTransform();
	//	xform_->addChild(airfly_mt_);

	//	mRoot->addChild(xform_);


	//	// 设置飞机矩阵
	//	coordinate_system_node_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(34.376128), osg::DegreesToRadians(109.125682), 537, mtTemp);
	//	xform_->setMatrix(mtTemp);


	//	// xform_->addUpdateCallback(new ModelPositionCallback(100.0f));
	//}


	//模型1
	node_airfly_ = osgDB::readNodeFile("c:/track_data/aircrafts/F-16.ive");
	
	//没有这句代码，飞机是黑色的
	node_airfly_->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON); // 设置属性，光照法线

	mt_airfly_ = new osg::MatrixTransform;
	mt_airfly_->addChild(node_airfly_);
	mt_airfly_->setName(_T("歼16"));

	mt_airfly_->addChild(m_pBuildRader.BuildRader(500,300).get());

	mt_airfly_->setDataVariance(osg::Object::STATIC);
	mt_airfly_->setMatrix(osg::Matrix::scale(1,1,1)*osg::Matrixd::rotate(osg::DegreesToRadians(0.0f), osg::Vec3(0,0,1)));
	mRoot->addChild(mt_airfly_);

	return 0;
}


// 飞机起飞
void cOSG::DoPreLineNow()
{

	/*ModelPositionCallback* nc = new ModelPositionCallback(100.0f);
	xform_->setUpdateCallback(nc);*/


	//下面是区别
	//获取当前操作器的视点，将模型放进这个视角中，然后设置这个视角的一些参数，比如从哪个角度和距离观察模型。然后将这个视点设置为操作器的视点。
	osgEarth::Viewpoint vp = earth_manipulator_->getViewpoint();
	vp.setNode(xform_);
	// vp.name()._set("view_point5");
	vp.range()->set(3000.0, osgEarth::Units::METERS);//观察的距离
	vp.pitch()->set(-30.0f, osgEarth::Units::DEGREES);//观察的角度


	// 加载尾迹
	// BuildTail(osg::Vec3(0,0,0),mtrix_fly_self);
	// BuildRibbon(512, mt_fly_.get());
	// BuildHistoryRoute(xform_, 2.0f);
	// osg::DegreesToRadians(34.3762), osg::DegreesToRadians(109.1263), 460, mtTemp);
	// 文件名 经度  纬度 高度 水平方位角 垂直俯仰角 可视范围
	// earth_manipulator_->setViewpoint(osgEarth::Viewpoint("view_point5", 109.126324, 34.376233, 4000, -60, -90, 1000),1);

	// todo
	/*CUdpClient *p_udp_client = new CUdpClient(this);
	p_udp_client->Start(3000);*/

	// earth_manipulator_->setViewpoint(vp, 1.0);
}



int cOSG::AddAnnotation(void)
{ 
	osg::Group* annoGroup = new osg::Group();
	mRoot->addChild( annoGroup );

	// Make a group for labels
	osg::Group* labelGroup = new osg::Group();
	annoGroup->addChild( labelGroup );

	osg::Group* editGroup = new osg::Group();
	mRoot->addChild( editGroup );

	// Style our labels:
	Style labelStyle;
	labelStyle.getOrCreate<TextSymbol>()->alignment() = TextSymbol::ALIGN_CENTER_CENTER;
	labelStyle.getOrCreate<TextSymbol>()->fill()->color() = Color::Yellow;
	labelStyle.getOrCreate<TextSymbol>()->font() = "simhei.ttf";
	labelStyle.getOrCreate<TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;

	// A lat/long SRS for specifying points.
	const SpatialReference* geoSRS = earth_map_node_->getMapSRS()->getGeographicSRS();

	//--------------------------------------------------------------------

	//label
	{
		Style pm;
		pm.getOrCreate<IconSymbol>()->url()->setLiteral( "../data/placemark32.png" );
		pm.getOrCreate<IconSymbol>()->declutter() = true;
		pm.getOrCreate<TextSymbol>()->halo() = Color("#5f5f5f");
		pm.getOrCreate<TextSymbol>()->font() = "simhei.ttf";
		pm.getOrCreate<TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;

		// bunch of pins:
		std::string beijing, shanghai, xian, wuhan, shenzhen, chengdu,shangluo,lasa, kunming, nanjing,
			wulumuqi, huhehaote;
		unicodeToUTF8(L"北京",beijing);
		unicodeToUTF8(L"上海", shanghai);
		unicodeToUTF8(L"西安", xian);
		unicodeToUTF8(L"武汉", wuhan);
		unicodeToUTF8(L"深圳", shenzhen);
		unicodeToUTF8(L"成都", chengdu);
		unicodeToUTF8(L"商洛", shangluo);
		unicodeToUTF8(L"拉萨", lasa);
		unicodeToUTF8(L"昆明", kunming);
		unicodeToUTF8(L"南京", nanjing);
		unicodeToUTF8(L"乌鲁木齐", wulumuqi);
		unicodeToUTF8(L"呼和浩特", huhehaote);

		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 116.46, 39.92), beijing, pm));
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 121.4726444, 31.231706), shanghai, pm));
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 108.95, 34.27), xian , pm));
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 114.31, 30.52), wuhan  , pm));
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 114.06, 22.62), shenzhen  , pm));
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 104.07, 30.67), chengdu  , pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 102.72, 25.05), kunming, pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 118.78, 32.07), nanjing, pm));
		labelGroup->addChild(new PlaceNode(GeoPoint(geoSRS, 87.68, 43.77), wulumuqi, pm));
		labelGroup->addChild(new LabelNode(GeoPoint(geoSRS, 111.73, 40.83), huhehaote, labelStyle));
		// test with an LOD:
		osg::LOD* lod = new osg::LOD();
		lod->addChild( new PlaceNode(GeoPoint(geoSRS, 109.93, 33.87), shangluo, pm), 0.0, 2e6);
		labelGroup->addChild( lod );

		// absolute altitude:
		labelGroup->addChild( new PlaceNode(GeoPoint(geoSRS, 91.13, 29.65, 1000, ALTMODE_ABSOLUTE), lasa, pm));
	}

	//--------------------------------------------------------------------

	// 多边形
	{
		Geometry* geom = new osgEarth::Symbology::Polygon();
		geom->push_back(osg::Vec3d(95.0, 30.0, 0));
		geom->push_back(osg::Vec3d(100.0, 30.0, 0));
		geom->push_back(osg::Vec3d(100.0, 40.0, 0));
		geom->push_back(osg::Vec3d(95.0, 40.0, 0));

		osgEarth::Features::Feature* feature = new osgEarth::Features::Feature(geom, geoSRS);
		//插值类型
		feature->geoInterp() = GEOINTERP_RHUMB_LINE;

		Style geomStyle;
		geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Cyan;
		geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 5.0f;
		geomStyle.getOrCreate<LineSymbol>()->tessellationSize() = 75000;
		geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;

		FeatureNode* fnode = new FeatureNode(feature, geomStyle);

		annoGroup->addChild( fnode );
	}

	//--------------------------------------------------------------------

	// 多边形
	{
		Geometry* geom = new osgEarth::Symbology::Polygon();
		geom->push_back( -160., -30. );
		geom->push_back(  150., -20. );
		geom->push_back(  160., -45. );
		geom->push_back( -150., -40. );
		Style geomStyle;

		Feature* feature = new Feature(geom, geoSRS);
		feature->geoInterp() = GEOINTERP_RHUMB_LINE;

		geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Lime;
		geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 3.0f;
		geomStyle.getOrCreate<LineSymbol>()->tessellationSize() = 75000;
		geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;

		FeatureNode* gnode = new FeatureNode(feature, geomStyle);
		annoGroup->addChild( gnode );

		labelGroup->addChild( new LabelNode(GeoPoint(geoSRS, 105, 35), "Antimeridian polygon", labelStyle) );
	}

	//--------------------------------------------------------------------



	//线
	FeatureNode* pathNode = 0;
	{
		Geometry* path = new LineString();
		path->push_back( osg::Vec3d(108.95, 34.27, 0) );//西安
		path->push_back( osg::Vec3d(116.46, 39.92, 0) );//北京

		Feature* pathFeature = new Feature(path, geoSRS);
		pathFeature->geoInterp() = GEOINTERP_GREAT_CIRCLE;

		Style pathStyle;
		pathStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::White;
		pathStyle.getOrCreate<LineSymbol>()->stroke()->width() = 1.0f;
		//对线几何进行曲面细分，使任何线段的长度都不会超过此值
		pathStyle.getOrCreate<LineSymbol>()->tessellationSize() = 75000;
		//pathStyle.getOrCreate<PointSymbol>()->size() = 5;
		//pathStyle.getOrCreate<PointSymbol>()->fill()->color() = Color::Red;
		pathStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		pathStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;

		//OE_INFO << "Path extent = " << pathFeature->getExtent().toString() << std::endl;

		pathNode = new FeatureNode(pathFeature, pathStyle);
		annoGroup->addChild( pathNode );

		//labelGroup->addChild( new LabelNode(mapNode, GeoPoint(geoSRS,112.705, 37.095), "Great circle path", labelStyle) );
	}

	//--------------------------------------------------------------------

	// 圆
	{
		Style circleStyle;
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Cyan, 0.5);//青色
		circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

		CircleNode* circle = new CircleNode();
			circle->set(
			GeoPoint(geoSRS, 118.78, 32.07, 1000., ALTMODE_RELATIVE),//南京
			Distance(300, Units::KILOMETERS), //半径
			circleStyle, Angle(-45.0, Units::DEGREES), Angle(45.0, Units::DEGREES), true);
		annoGroup->addChild( circle );

		editGroup->addChild( new CircleNodeEditor(circle) );
	}

	{
		Style circleStyle;
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Red, 0.5);
		circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;

		CircleNode* circle = new CircleNode();
		circle->set(
			GeoPoint(geoSRS, 118.78, 32.07, 1000., ALTMODE_RELATIVE),//南京
			Distance(300, Units::KILOMETERS),
			circleStyle, Angle(45.0, Units::DEGREES), Angle(360.0 - 45.0, Units::DEGREES), true);
		annoGroup->addChild( circle );

		editGroup->addChild( new CircleNodeEditor(circle) );
	}

	//--------------------------------------------------------------------

	// 立体椭圆
	{
		Style ellipseStyle;
		ellipseStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Orange, 0.75);
		ellipseStyle.getOrCreate<ExtrusionSymbol>()->height() = 25000.0; // meters MSL
		osgEarth::Annotation::EllipseNode* ellipse = new osgEarth::Annotation::EllipseNode();
			ellipse->set(
			GeoPoint(geoSRS, 102.72, 25.05, 0.0, ALTMODE_RELATIVE),//云南
			Distance(250, Units::MILES),
			Distance(100, Units::MILES),
			Angle   (0, Units::DEGREES),
			ellipseStyle,
			Angle(45.0, Units::DEGREES),
			Angle(360.0 - 45.0, Units::DEGREES), 
			true);
		annoGroup->addChild( ellipse );
		editGroup->addChild( new EllipseNodeEditor(ellipse) );

		//CircleNode
		Style circleStyle;
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Orange, 0.75);
		circleStyle.getOrCreate<ExtrusionSymbol>()->height() = 25000.0; // meters MSL
		osgEarth::Annotation::CircleNode* circle = new osgEarth::Annotation::CircleNode();
		ellipse->set(
			GeoPoint(geoSRS, 102.72, 25.05, 0.0, ALTMODE_RELATIVE),
			Distance(250, Units::MILES),
			Distance(100, Units::MILES),
			Angle   (0, Units::DEGREES),
			circleStyle,
			Angle(45.0, Units::DEGREES),
			Angle(360.0 - 45.0, Units::DEGREES)
			);
		//annoGroup->addChild(circle);
		//editGroup->addChild(new CircleNodeEditor(circle));
	}
	{
		Style ellipseStyle;
		ellipseStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Blue, 0.75);
		ellipseStyle.getOrCreate<ExtrusionSymbol>()->height() = 25000.0; // meters MSL
		EllipseNode* ellipse = new EllipseNode();
		ellipse->set(
			GeoPoint(geoSRS, 102.72, 25.05, 0.0, ALTMODE_RELATIVE),
			Distance(250, Units::MILES),
			Distance(100, Units::MILES),
			Angle   (0, Units::DEGREES),
			ellipseStyle, 
			Angle(-40.0, Units::DEGREES), 
			Angle(40.0, Units::DEGREES), 
			true);
		annoGroup->addChild( ellipse );

		editGroup->addChild( new EllipseNodeEditor(ellipse) );
	}

	//--------------------------------------------------------------------

	{
		// 矩形
		Style rectStyle;
		rectStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Green, 0.5);
		rectStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		rectStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;
		RectangleNode* rect = new RectangleNode(
			GeoPoint(geoSRS, 87.68, 43.77),//乌鲁木齐
			Distance(300, Units::KILOMETERS ),
			Distance(600, Units::KILOMETERS ),
			rectStyle);
		annoGroup->addChild( rect );

		editGroup->addChild( new RectangleNodeEditor(rect) );
	}    

	//--------------------------------------------------------------------

	//立体拉伸多边形
	{
		Geometry* utah = new osgEarth::Symbology::Polygon();
		utah->push_back( 114.052, 37.0   );
		utah->push_back( 109.054, 37.0   );
		utah->push_back( 109.054, 41.0   );
		utah->push_back( 111.040, 41.0   );
		utah->push_back( 111.080, 42.059 );
		utah->push_back( 114.080, 42.024 );

		Style utahStyle;
		utahStyle.getOrCreate<ExtrusionSymbol>()->height() = 250000.0; // meters MSL
		utahStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::White, 0.8);

		Feature*     utahFeature = new Feature(utah, geoSRS);
		FeatureNode* featureNode = new FeatureNode(utahFeature, utahStyle);
		annoGroup->addChild( featureNode );
	}

	//--------------------------------------------------------------------

	// 贴图
	{
		ImageOverlay* imageOverlay = 0L;
		osg::Image* image = osgDB::readImageFile( "../data/placemark32.png" );
		if ( image )
		{
			imageOverlay = new ImageOverlay(earth_map_node_,image);
			imageOverlay->setBounds( Bounds( 115, 39, 116, 40.0) );// 左下 右上
			annoGroup->addChild( imageOverlay );
			editGroup->addChild( new ImageOverlayEditor(imageOverlay) );
		}
	}

	//--------------------------------------------------------------------
	mViewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

	return 0;
}


// 创建飞机历史航迹
void cOSG::AddBuildHistoryRoute(osg::MatrixTransform* scaler, float lineWidth)
{
	osg::ref_ptr<osg::Group> rpgroup = new osg::Group;
	scaler->addUpdateCallback(new CreateTrackCallback(rpgroup,scaler,lineWidth));
	mRoot->addChild(rpgroup);

}


// 创建飞机飞行彩带
void cOSG::AddBuildRibbon(int size, osg::MatrixTransform* scaler, int ribbonWidth)
{
	osg::ref_ptr<osg::Geometry> rpgeom = new osg::Geometry;
	//设置顶点
	osg::ref_ptr<osg::Vec3Array> rpvec3Vertex = new osg::Vec3Array(size);
	//设置颜色
	osg::ref_ptr<osg::Vec4Array> rpvec4Color = new osg::Vec4Array(size);

	for(unsigned int i = 0;i <size-1;i+=2)
	{
		(*rpvec3Vertex)[i] = osg::Vec3(0,0,0);
		(*rpvec3Vertex)[i+1] = osg::Vec3(0,0,0);

		float falpha = sinf(osg::PI * (float)i / (float)size);

		(*rpvec4Color)[i] = osg::Vec4(m_vec3RibbonColor_,falpha);
		(*rpvec4Color)[i+1] = osg::Vec4(m_vec3RibbonColor_,falpha);
	}
	//场景数据动态改变
	rpgeom->setDataVariance(osg::Object::DYNAMIC);
	//禁用显示列表，动态更新不安全
	rpgeom->setUseDisplayList(false);
	//使用VBO模式
	rpgeom->setUseVertexBufferObjects(true);

	rpgeom->setVertexArray(rpvec3Vertex);

	rpgeom->setColorArray(rpvec4Color);
	rpgeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	rpgeom->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP,0,size));

	osg::ref_ptr<osg::Geode> rpgeode = new osg::Geode;
	rpgeode->addDrawable(rpgeom);
	//灯光、透明度
	rpgeom->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	rpgeom->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
	rpgeom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	scaler->addUpdateCallback(new CTrailerCallback(rpgeom,size,ribbonWidth));

	mRoot->addChild(rpgeode);

}


// 添加显示视点信息的控件
//void cOSG::AddViewPointLabel(void)
//{
//	
//}


int cOSG::createControls(void)
{

	return 0;
}


int cOSG::FlyTo(double longitude, double latitude, double altitude, double height, double heading, double pitch, double range)
{
	earth_manipulator_->setViewpoint(osgEarth::Viewpoint(_T(""), longitude, latitude, altitude, -60, -45, 1000), 2);
	return 0;
}


int cOSG::StartFly(void)
{	
	mt_airfly_->setUpdateCallback(new ModelPositionCallback());

	return 0;
}
