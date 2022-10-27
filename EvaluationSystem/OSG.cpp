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
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>

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

/******************************************************************/

#include <osg/Notify>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/MapNode>
#include <osgEarth/XmlUtils>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/LinearLineOfSight>
#include <osgEarthUtil/RadialLineOfSight>
#include <osg/io_utils>
#include <osg/MatrixTransform>
#include <osg/Depth>
#include<osg/LineWidth>
#include<osg/ShapeDrawable>
#include<osgEarthAnnotation/PlaceNode>

/**************************************************************/
using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Features;
using namespace osgEarth::Util;

osg::ref_ptr<osgViewer::Viewer> g_viewer;
osg::MatrixTransform* g_mt_compass = new osg::MatrixTransform();

COSG::COSG(HWND hWnd) :
	m_hWnd(hWnd),
	label_event_(nullptr)
{
	m_vec3RibbonColor_.set(1.0f, 0.0f, 0.0f); // 设置彩带颜色为红色
}

COSG::~COSG()
{
	mViewer->setDone(true);
	Sleep(1000);
	mViewer->stopThreading();

	delete mViewer;
}

void COSG::InitOSG(std::string modelname)
{
	// Store the name of the model to load
	m_ModelName = modelname;

	InitManipulators();

	// Init different parts of OSG
	if (!InitSceneGraph()){
		AfxMessageBox("读取earth文件失败, 请确认配置路径是否正确, 即将退出程序 !", MB_OK, MB_ICONWARNING);
		//ExitProcess(0);
	}

	InitCameraConfig();

	g_viewer = getViewer();

	// 创建星空
	CreateStarrySky();

	// 显示经纬度高度
	AddViewPointLable();

	// 添加帧率标签
	AddFrameRateLabel();

	// 添加飞机模型
	AddAirPlane();
	// DoPreLineNow();

	// 添加指南针
	AddCompass();

	// 创建飞机历史航迹
	// AddBuildHistoryRoute(mt_airfly_, 1.0f);

	// 创建飞机飞行彩带
	AddBuildRibbon(512.0f, mt_airfly_, 10.0f);

	// 设置飞机视角跟随 接收 udp 数据，实时仿真
	RealTimeSimulation();

	// 在左上角添加经度度标签，鼠标移动时候显示  
	// AddLonLatLabel();
	
	// 添加标注
	AddAnnotation();
}

void COSG::AddLonLatLabel()
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
void COSG::RealTimeSimulation(bool is_track)
{
	osgEarth::Viewpoint vp = earth_manipulator_->getViewpoint();
	//if (is_track){
	vp.setNode(mt_airfly_); //node_airfly_);mt_airfly_
	vp.heading()->set(-76.738, osgEarth::Units::DEGREES);
	vp.range()->set(4774.368, osgEarth::Units::METERS); // 350000.0
	vp.pitch()->set(-21.000, osgEarth::Units::DEGREES);
	earth_manipulator_->setViewpoint(vp, 5.0);
	//}else{
	//	vp.setNode(0);
	//}	
}

void COSG::AddCompass()
{
	// 添加指南针
	osg::ref_ptr<Compass> compass = new Compass;
	compass->setProjectionMatrix(osg::Matrixd::ortho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0));
	compass->setPlate(createCompassPart("../data/images/panzi.png", 1.5f, -1.0f)); //圆盘图片
	compass->setNeedle(createCompassPart("../data/images/noddle.png", 1.5f, 0.0f));//指针图片
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

void COSG::InitManipulators(void)
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


bool COSG::InitSceneGraph(void)
{
	// Init the main Root Node/Group
	mRoot  = new osg::Group;

	// Load the Model from the model name
	mModel = osgDB::readNodeFile(m_ModelName);
	if (!mModel) return false;

	//查询地图节点
	earth_map_node_ = osgEarth::MapNode::findMapNode(mModel);
	if (!earth_map_node_) return false;

	earth_manipulator_->setNode(earth_map_node_);
	earth_manipulator_->getSettings()->setArcViewpointTransitions(true);

	// Optimize the model
	osgUtil::Optimizer optimizer;
	optimizer.optimize(mModel.get());
	optimizer.reset();

	// Add the model to the scene
	mRoot->addChild(mModel.get());

	return true;
}

void COSG::InitCameraConfig(void)
{
	// Local Variable to hold window size data
	RECT rect;

	// Create the viewer for this window
	mViewer = new osgViewer::Viewer();

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

	//添加状态事件，可以相应键盘和鼠标事件，响应L T B W
	mViewer->addEventHandler(new osgGA::StateSetManipulator(camera->getOrCreateStateSet()));
	//窗口大小变化，响应F
	mViewer->addEventHandler(new osgViewer::WindowSizeHandler);
	//添加路径记录 Z
	mViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
	//帮助文档显示H
	mViewer->addEventHandler(new osgViewer::HelpHandler);
	//截屏 C
	mViewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
	//添加一些常用状态设置，响应S
	mViewer->addEventHandler(new osgViewer::StatsHandler);

	mViewer->addEventHandler(new osgViewer::ThreadingHandler());

	mViewer->addEventHandler(new osgViewer::LODScaleHandler());

	// osg::ref_ptr<osgText::Text> updateText = ;
	mViewer->addEventHandler(new CPickHandler(new osgText::Text));
	mViewer->setThreadingModel(osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext);

	//解决Lines or Annotations (FeatureNode, etc.) 不被渲染的问题
	osgEarth::GLUtils::setGlobalDefaults(mViewer->getCamera()->getOrCreateStateSet());

	// Realize the Viewer
	mViewer->realize();
}

void COSG::PreFrameUpdate()
{
	// Due any preframe updates in this routine
}

void COSG::PostFrameUpdate()
{
	// Due any postframe updates in this routine
}

// 创建星空
int COSG::CreateStarrySky(void)
{
	//CString str;
	CTime tm;
	tm = CTime::GetCurrentTime();
	//str=tm.Format("现在时间是%Y年%m月%d日 %X");
	//TRACE(str);

	// 设置时间
	osgEarth::DateTime dateTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour());		//格林尼治时间
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

CRenderingThread::CRenderingThread( COSG* ptr )
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
void COSG::AddViewPointLable()
{
	if (mViewer==nullptr){
		TRACE(TEXT("mViewer is null, at addViewPointLable!"));
		return ;
	}

	canvas_ = osgEarth::Util::Controls::ControlCanvas::getOrCreate(mViewer);
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("../data/fonts/msyh.ttf");

	// 添加控件，用来显示鼠标信息
	osgEarth::Util::Controls::LabelControl* mouse_coords = new osgEarth::Util::Controls::LabelControl("", Color::White);
	mouse_coords->setBackColor(osgEarth::Util::Controls::Color(osgEarth::Util::Controls::Color::Green, 0.5));
	mouse_coords->setFont(font.get());
	mouse_coords->setEncoding(osgText::String::ENCODING_UTF8);
	mouse_coords->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 0.5));
	mouse_coords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	mouse_coords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_BOTTOM);
	canvas_->addControl(mouse_coords);

	// mRoot->addChild(osgEarth::Util::Controls::ControlCanvas::get(mViewer));
	// osgEarth::Util::Controls::ControlCanvas* canvas = osgEarth::Util::Controls::ControlCanvas::get(mViewer);

	// 添加控件用来显示视点信息
	osgEarth::Util::Controls::LabelControl* view_coods = new osgEarth::Util::Controls::LabelControl("", Color::White);
	view_coods->setBackColor(osgEarth::Util::Controls::Color(osgEarth::Util::Controls::Color::Green, 0.5));
	view_coods->setFont(font.get());
	view_coods->setEncoding(osgText::String::ENCODING_UTF8);
	view_coods->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 0.5));
	view_coods->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_LEFT);
	view_coods->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
	//view_coods->setBackColor(osg::Vec4(0,0,0,0.5));
	//view_coods->setSize(500,50);
	view_coods->setMargin(10);
	canvas_->addControl(view_coods);

	// 添加控件，用来显示飞机信息
	osgEarth::Util::Controls::LabelControl* fly_coords = new osgEarth::Util::Controls::LabelControl(TEXT(""), Color::White);
	fly_coords->setBackColor(osgEarth::Util::Controls::Color(osgEarth::Util::Controls::Color::Green, 0.5));
	fly_coords->setFont(font.get());
	fly_coords->setEncoding(osgText::String::ENCODING_UTF8);
	fly_coords->setHaloColor(osg::Vec4(1.0, 0.5, 0.0, 0.5));
	fly_coords->setHorizAlign(osgEarth::Util::Controls::Control::ALIGN_RIGHT);
	fly_coords->setVertAlign(osgEarth::Util::Controls::Control::ALIGN_TOP);
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

osg::MatrixTransform* COSG::createCompassPart(const std::string &image, float radius, float height)
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

int COSG::AddAirPlane(void)
{
	//模型1
	node_airfly_ = osgDB::readNodeFile("../data/aircrafts/F-16.ive");
	
	//没有这句代码，飞机是黑色的
	node_airfly_->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON); // 设置属性，光照法线

	mt_airfly_ = new osg::MatrixTransform;
	mt_airfly_->addChild(node_airfly_);
	mt_airfly_->setName(_T("歼16"));

	mt_airfly_->addChild(m_pBuildRader.BuildRader(50,50).get());
	mt_airfly_->setUpdateCallback(new ModelPositionCallback());
	mt_airfly_->setDataVariance(osg::Object::STATIC);
	mt_airfly_->setMatrix(osg::Matrix::scale(1,1,1)*osg::Matrixd::rotate(osg::DegreesToRadians(0.0f), osg::Vec3(0,0,1)));
	mRoot->addChild(mt_airfly_);


	//在山顶放一个坦克
	osg::ref_ptr<osg::Node>tank = osgDB::readNodeFile("tank.flt.170,170,170.scale");
	osg::MatrixTransform* tankMT = new osg::MatrixTransform;
	osg::Matrix mt;
	earth_map_node_->getMapSRS()->getGeographicSRS()->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(46.2054), osg::DegreesToRadians(-121.488), 2030.11,mt);
	tankMT->setMatrix(mt);
	tankMT->addChild(tank);
	mRoot->addChild(tankMT);

	return 0;
}


// 飞机起飞
void COSG::DoPreLineNow()
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



int COSG::AddAnnotation(void)
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

		labelGroup->addChild( new LabelNode(GeoPoint(geoSRS,112.705, 37.095), "Great circle path", labelStyle) );
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
		circle->set(
			GeoPoint(geoSRS, 102.72, 25.05, 0.0, ALTMODE_RELATIVE),
			Distance(250, Units::MILES),
			circleStyle,
			Angle(45.0, Units::DEGREES),
			Angle(360.0 - 45.0, Units::DEGREES)
			);
		annoGroup->addChild(circle);
		editGroup->addChild(new CircleNodeEditor(circle));
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
void COSG::AddBuildHistoryRoute(osg::MatrixTransform* scaler, float lineWidth)
{
	osg::ref_ptr<osg::Group> rpgroup = new osg::Group;
	scaler->addUpdateCallback(new CreateTrackCallback(rpgroup,scaler,lineWidth));
	mRoot->addChild(rpgroup);

}


// 创建飞机飞行彩带
void COSG::AddBuildRibbon(int size, osg::MatrixTransform* scaler, int ribbonWidth)
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


int COSG::createControls(void)
{

	return 0;
}


int COSG::FlyTo(double longitude, double latitude, double altitude, double height, double heading, double pitch, double range)
{
	earth_manipulator_->setViewpoint(osgEarth::Viewpoint(_T(""), longitude, latitude, altitude, -60, -45, 1000), 2);
	return 0;
}


int COSG::StartFly(void)
{	
	
	return 0;
}

GetFrameRate::GetFrameRate(osgEarth::Util::LabelControl* fpsLabelContral):fpsLabel(fpsLabelContral)
{
	curFrameNum = 0;
	curTime = 0;
	nextFrameNum = 0;
	nextTime = 0;
	fps = 0;
}

bool GetFrameRate::handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter&aa)
{
	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	//获取帧率
	osg::FrameStamp* FS = viewer->getViewerFrameStamp();
	curFrameNum = FS->getFrameNumber();
	curTime = FS->getSimulationTime();
	if ((curTime - nextTime)<=0.005){
		// todo
	}else{ 
		fps = 1 / ((curTime - nextTime)); 
		char wsrc[512];
		sprintf(wsrc, "%.2f", fps);
		fpsLabel->setText(wsrc);
		std::cout << fps << std::endl;
	}

	nextFrameNum = FS->getFrameNumber();
	nextTime = FS->getSimulationTime();
	return false;
}


// 添加帧率标签
int COSG::AddFrameRateLabel(void)
{
	//contralLable显示信息
	osgEarth::Util::Grid* grid = new osgEarth::Util::Grid();
	//设置几个Label文字控件显示在场景中
	grid->setControl(0, 0, new osgEarth::Util::LabelControl("FPS:  "));
	osgEarth::Util::LabelControl* fpsLabelContral = grid->setControl(1, 0, new osgEarth::Util::LabelControl("60.0"));
	grid->setPosition(10, 30);

	//控件绘制容器
	osgEarth::Util::ControlCanvas* fpsCanvas = new osgEarth::Util::ControlCanvas();
	//将要显示的控件加入到root组节点中去
	mRoot->addChild(fpsCanvas);
	fpsCanvas->addControl(grid);
	fpsCanvas->setNodeMask(1);


	//获取帧率
	mViewer->addEventHandler(new GetFrameRate(fpsLabelContral));

	return 0;
}


// 显示多维态势
int COSG::DisplaySituation(void)
{

	osg::Group* losGroup = new osg::Group();
	losGroup->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	losGroup->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false));
	mRoot->addChild(losGroup);

	// so we can speak lat/long:
	//世界坐标和经纬坐标SRS
	const SpatialReference* mapSRS = earth_map_node_->getMapSRS();
	const SpatialReference* geoSRS = mapSRS->getGeographicSRS();

	//Create a point to point LineOfSightNode.
	//第一根线
	LinearLineOfSightNode* los = new LinearLineOfSightNode(
		earth_map_node_,
		GeoPoint(geoSRS, -121.665, 46.0878, 1258.00, ALTMODE_ABSOLUTE),
		GeoPoint(geoSRS, -121.488, 46.2054, 3620.11, ALTMODE_ABSOLUTE) );
	osg::ref_ptr<osg::LineWidth>Lw2 = new osg::LineWidth(3);
	los->getOrCreateStateSet()->setAttribute(Lw2, osg::StateAttribute::ON);
	los->setGoodColor(osg::Vec4(0,0,1,1));
	losGroup->addChild( los );
	//Create an editor for the point to point line of sight that allows you to drag the beginning and end points around.
	//This is just one way that you could manipulator the LineOfSightNode.
	//设置线顶点可编辑
	LinearLineOfSightEditor* p2peditor = new LinearLineOfSightEditor( los );
	mRoot->addChild( p2peditor );

	//Create a relative point to point LineOfSightNode.
	//第二根线  两点之间生成一条线
	LinearLineOfSightNode* relativeLOS = new LinearLineOfSightNode( 
		earth_map_node_,
		GeoPoint(geoSRS, -121.2, 46.1, 10, ALTMODE_RELATIVE),//最后一个参数相对高度为10
		GeoPoint(geoSRS, -121.488, 46.2054, 10, ALTMODE_RELATIVE) );
	osg::ref_ptr<osg::LineWidth>Lw3 = new osg::LineWidth(3);
	relativeLOS->getOrCreateStateSet()->setAttribute(Lw3, osg::StateAttribute::ON);
	relativeLOS->setGoodColor(osg::Vec4(0, 1, 1, 1));
	losGroup->addChild( relativeLOS );
	//设置两条线为可编辑状态
	LinearLineOfSightEditor* relEditor = new LinearLineOfSightEditor( relativeLOS );
	mRoot->addChild( relEditor );


	//在山顶放一个坦克
	osg::ref_ptr<osg::Node>tank = osgDB::readNodeFile("tank.flt.170,170,170.scale");
	osg::MatrixTransform* tankMT = new osg::MatrixTransform;
	osg::Matrix mt;
	geoSRS->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(46.2054), osg::DegreesToRadians(-121.488), 3430.11,mt);
	tankMT->setMatrix(mt);
	tankMT->addChild(tank);
	mRoot->addChild(tankMT);



	//Create a RadialLineOfSightNode that allows you to do a 360 degree line of sight analysis.
	//第一个扇形区域
	RadialLineOfSightNode* radial = new RadialLineOfSightNode( earth_map_node_ );
	radial->setCenter( GeoPoint(geoSRS, -121.515, 46.054, 847.604, ALTMODE_ABSOLUTE) );
	radial->setRadius( 2000 );
	radial->setNumSpokes( 100 );    
	losGroup->addChild( radial );
	//设置扇形可编辑
	RadialLineOfSightEditor* radialEditor = new RadialLineOfSightEditor( radial );
	losGroup->addChild( radialEditor );

	//第二个扇形区域
	//Create a relative RadialLineOfSightNode that allows you to do a 360 degree line of sight analysis.
	RadialLineOfSightNode* radialRelative = new RadialLineOfSightNode( earth_map_node_ );
	radialRelative->setCenter( GeoPoint(geoSRS, -121.2, 46.054, 10, ALTMODE_RELATIVE) );
	radialRelative->setRadius( 3000 );
	radialRelative->setNumSpokes(60);    
	losGroup->addChild( radialRelative );
	//设置扇形可编辑
	RadialLineOfSightEditor* radialRelEditor = new RadialLineOfSightEditor( radialRelative );
	losGroup->addChild( radialRelEditor );


	//Load a plane model.  
	osg::ref_ptr< osg::Node >  plane = osgDB::readNodeFile("cessna.osgb.50,50,50.scale");//name.x,y,z.scale   猜测是点乘比例   name.x,y,z.trans  平移
	//Create 2 moving planes
	osg::Node* plane1 = CreatePlane(plane, GeoPoint(geoSRS, -121.656, 46.0935, 9133.06, ALTMODE_ABSOLUTE), mapSRS, 50000, 5); //半径 时间
	osg::Node* plane2 = CreatePlane(plane, GeoPoint(geoSRS, -121.321, 46.2587, 6390.09, ALTMODE_ABSOLUTE), mapSRS, 30000, 5);
	mRoot->addChild( plane1 );
	mRoot->addChild( plane2 );

	//Create another plane and attach a RadialLineOfSightNode to it using the RadialLineOfSightTether
	osg::Node* plane3 = CreatePlane(plane, GeoPoint(geoSRS, -121.463, 46.3548, 3348.71, ALTMODE_ABSOLUTE), mapSRS, 10000, 5);
	plane3->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
	losGroup->addChild(plane3);


	{
		//Create a LineOfSightNode that will use a LineOfSightTether callback to monitor
		//the two plane's positions and recompute the LOS when they move
		//第一个和第二个飞机之间生成跟踪线
		LinearLineOfSightNode* tetheredLOS = new LinearLineOfSightNode(earth_map_node_);
		//设置跟踪线颜色
		tetheredLOS->setGoodColor(osg::Vec4(0, 1, 0, 1));
		//设置线宽
		osg::ref_ptr<osg::LineWidth>Lw = new osg::LineWidth(3.0);
		tetheredLOS->getOrCreateStateSet()->setAttribute(Lw, osg::StateAttribute::ON);

		losGroup->addChild(tetheredLOS);
		tetheredLOS->setUpdateCallback(new LineOfSightTether(plane1, plane2));


		//第2和3架飞机之间的连接线
		LinearLineOfSightNode* tetheredLOS2 = new LinearLineOfSightNode(earth_map_node_);
		//设置跟踪线颜色
		tetheredLOS2->setGoodColor(osg::Vec4(1, 0, 0, 1));  //红色
		//设置线宽
		osg::ref_ptr<osg::LineWidth>Lw4 = new osg::LineWidth(3.0);
		tetheredLOS2->getOrCreateStateSet()->setAttribute(Lw4, osg::StateAttribute::ON);
		//不知道实际作用
		//tetheredLOS2->setBadColor(osg::Vec4(1, 0, 0, 1));
		losGroup->addChild(tetheredLOS2);
		tetheredLOS2->setUpdateCallback(new LineOfSightTether(plane2, plane3));


		//第1架飞机和坦克之间的连接线
		LinearLineOfSightNode* tetheredLOS3 = new LinearLineOfSightNode(earth_map_node_);
		//设置跟踪线颜色
		tetheredLOS3->setGoodColor(osg::Vec4(1, 0, 0, 1));  //红色
		tetheredLOS3->getOrCreateStateSet()->setAttribute(Lw4, osg::StateAttribute::ON);
		losGroup->addChild(tetheredLOS3);
		tetheredLOS3->setUpdateCallback(new LineOfSightTether(plane1, tank));
	}


	{
		//坦克周围的包围扇形
		RadialLineOfSightNode* tetheredRadia = new RadialLineOfSightNode(earth_map_node_);
		tetheredRadia->setRadius(5000);
		tetheredRadia->setFill(true);
		//若只有GoodColor，则整个区域填充此颜色
		tetheredRadia->setGoodColor(osg::Vec4(1, 0, 0, 0.3));
		tetheredRadia->setNumSpokes(100);
		losGroup->addChild(tetheredRadia);
		tetheredRadia->setUpdateCallback(new RadialLineOfSightTether(tank));

		//飞机1周围的包围扇形
		RadialLineOfSightNode* tetheredRadia2 = new RadialLineOfSightNode(earth_map_node_);
		tetheredRadia2->setRadius(5000);
		tetheredRadia2->setFill(true);
		//若只有GoodColor，则整个区域填充此颜色
		tetheredRadia2->setGoodColor(osg::Vec4(0, 1, 0, 0.3));
		tetheredRadia2->setNumSpokes(100);
		losGroup->addChild(tetheredRadia2);
		tetheredRadia2->setUpdateCallback(new RadialLineOfSightTether(plane1));

		//飞机2周围的包围扇形
		RadialLineOfSightNode* tetheredRadia3 = new RadialLineOfSightNode(earth_map_node_);
		tetheredRadia3->setRadius(5000);
		tetheredRadia3->setFill(true);
		//若只有GoodColor，则整个区域填充此颜色
		tetheredRadia3->setGoodColor(osg::Vec4(0, 1, 0, 0.3));
		tetheredRadia3->setNumSpokes(100);
		losGroup->addChild(tetheredRadia3);
		tetheredRadia3->setUpdateCallback(new RadialLineOfSightTether(plane2));

		//飞机3周围的包围扇形
		RadialLineOfSightNode* tetheredRadia4 = new RadialLineOfSightNode(earth_map_node_);
		tetheredRadia4->setRadius(5000);
		//This RadialLineOfSightNode is going to be filled, so set some alpha values for the colors so it's partially transparent
		tetheredRadia4->setFill(true);
		//若只有GoodColor，则整个区域填充此颜色
		tetheredRadia4->setGoodColor(osg::Vec4(1, 0, 0, 0.3));
		//BadColor好像没啥影响
		//后面参数为多边形外接切线，参数越大越接近圆，参数越小则退化为正多边形
		tetheredRadia4->setNumSpokes(100);
		losGroup->addChild(tetheredRadia4);
		//设置包围圈跟踪飞机模型
		tetheredRadia4->setUpdateCallback(new RadialLineOfSightTether(plane3));

		//包围球
		osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
		osg::BoundingSphere bs = plane3->getBound();
		osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(new osg::Sphere(bs.center(),bs.radius()+300));
		sd->setColor(osg::Vec4(1,0,0,0.2));
		gnode->addChild(sd);
		osg::MatrixTransform* plane3MT = dynamic_cast<osg::MatrixTransform*>(plane3);
		plane3MT->addChild(gnode);
		gnode->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateSet::TRANSPARENT_BIN);
		gnode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		//添加标识
		osgEarth::Style style;
		osgEarth::Annotation::PlaceNode *pn = new osgEarth::Annotation::PlaceNode(osgEarth::GeoPoint(geoSRS, -121.488, 46.2054, 0, ALTMODE_RELATIVE), "Tank", style);

		mRoot->addChild(pn);

	}


	//设置初始视点
	osgEarth::Viewpoint vp;
	vp.name() = "Mt Ranier";
	vp.focalPoint()->set(geoSRS, -121.488, 46.2054, 0, ALTMODE_ABSOLUTE);
	vp.pitch() = -50.0;
	vp.range() = 100000;
	earth_manipulator_->setHomeViewpoint( vp );


	return 0;
}


osg::ref_ptr<osg::Node> COSG::CreatePlane(osg::Node* node, const GeoPoint& pos, const SpatialReference* mapSRS, double radius, double time)
{
	osg::MatrixTransform* positioner = new osg::MatrixTransform;
	positioner->addChild( node );
	osg::AnimationPath* animationPath = CreateAnimationPath(pos, mapSRS, radius, time);
	positioner->setUpdateCallback( new osg::AnimationPathCallback(animationPath, 0.0, 1.0));
	return positioner;
}


osg::ref_ptr<osg::AnimationPath> COSG::CreateAnimationPath(const GeoPoint& pos, const SpatialReference* mapSRS, float radius, double looptime)
{
	// set up the animation path 
	osg::AnimationPath* animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;

	double delta = osg::PI * 2.0 / (double)numSamples;

	//Get the center point in geocentric
	GeoPoint mapPos = pos.transform(mapSRS);
	osg::Vec3d centerWorld;
	mapPos.toWorld( centerWorld );

	bool isProjected = mapSRS->isProjected();

	osg::Vec3d up = isProjected ? osg::Vec3d(0,0,1) : centerWorld;
	up.normalize();

	//Get the "side" vector
	osg::Vec3d side = isProjected ? osg::Vec3d(1,0,0) : up ^ osg::Vec3d(0,0,1);


	double time=0.0f;
	double time_delta = looptime/(double)numSamples;

	osg::Vec3d firstPosition;
	osg::Quat firstRotation;

	for (unsigned int i = 0; i < (unsigned int)numSamples; i++)
	{
		double angle = delta * (double)i;
		osg::Quat quat(angle, up );
		osg::Vec3d spoke = quat * (side * radius);
		osg::Vec3d end = centerWorld + spoke;                

		osg::Quat makeUp;
		makeUp.makeRotate(osg::Vec3d(0,0,1), up);

		osg::Quat rot = makeUp;
		animationPath->insert(time,osg::AnimationPath::ControlPoint(end,rot));
		if (i == 0)
		{
			firstPosition = end;
			firstRotation = rot;
		}
		time += time_delta;            
	}

	animationPath->insert(time, osg::AnimationPath::ControlPoint(firstPosition, firstRotation));

	return animationPath;    
}