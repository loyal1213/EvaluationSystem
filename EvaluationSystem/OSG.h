#pragma once

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgEarth/MapNode>
#include <string>


#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarth/Map>



#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/CoordinateSystemNode>

#include <osgDB/FileUtils>
#include <osgDB/fstream>
#include <osgDB/ReadFile>

#include <osgText/Text>

#include <osg/CoordinateSystemNode>

#include <osgSim/OverlayNode>
#include <osgSim/SphereSegment>

#include <osgGA/NodeTrackerManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <osgParticle/FireEffect>

#include <osgEarthUtil/Controls>

#include <iostream>


#include "LabelControlEventHandler.h"
#include "BuildRader.h"

class COSG
{
public:
    COSG(HWND hWnd);
    ~COSG();

    void InitOSG(std::string filename);

	void InitManipulators(void);

    bool InitSceneGraph(void);

    void InitCameraConfig(void);

 
    void PreFrameUpdate(void);

    void PostFrameUpdate(void);

    void Done(bool value) { mDone = value; }

    bool Done(void) { return mDone; }
    //static void Render(void* ptr);

    osg::ref_ptr<osgViewer::Viewer> getViewer() { return mViewer; }
protected:
	// 创建星空
	int CreateStarrySky(void);
	// 添加标签控件，显示经纬度高度
	void AddViewPointLable();
	// 添加指南针
	osg::MatrixTransform *COSG::createCompassPart(const std::string &image, float radius, float height);

private:
    bool mDone;
    std::string m_ModelName;
    HWND m_hWnd;
    osg::ref_ptr<osgViewer::Viewer> mViewer;
    osg::ref_ptr<osg::Group> mRoot;
    osg::ref_ptr<osg::Node> mModel;
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
private:
	osg::ref_ptr<osgEarth::MapNode>  earth_map_node_;
	// 指南针
	osg::ref_ptr<osg::MatrixTransform> mt_compass_;

	// 经纬度显示标签
	CLabelControlEventHandler* label_event_;
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> canvas_;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earth_manipulator_;

private:
	osg::ref_ptr<osg::CoordinateSystemNode> coordinate_system_node_;// 地表的矩阵操作

	osg::ref_ptr<osg::MatrixTransform> mtAirport_;

	osg::ref_ptr<osg::Node> airport_;

	osg::ref_ptr<osg::Node> node_airfly_;

	osg::ref_ptr<osg::MatrixTransform> mt_airfly_;
	
	osg::Vec3 m_vec3RibbonColor_;
public:
	osg::ref_ptr<osg::MatrixTransform> GetXform(){return xform_;}
	osg::ref_ptr<osg::MatrixTransform>  xform_;
	osg::ref_ptr<osg::AnimationPath> apc_;

private:
	// 主要用来处理雷达相关操作
	CBuildRader m_pBuildRader;
public:
	int AddAirPlane(void);
	void DoPreLineNow();
	int AddAnnotation(void);

	void DrawChartlet (osg::Group* annoGroup, osg::Group* editGroup);

	void DrawStereoPolygon(const SpatialReference* geoSRS, osg::Group* annoGroup);

	void DrawRectangle(const SpatialReference* geoSRS, osg::Group* annoGroup, osg::Group* editGroup);

	void DrawStereoEllipse(const SpatialReference* geoSRS, osg::Group* annoGroup, osg::Group* editGroup);

	void DrawCircle(const SpatialReference* geoSRS, osg::Group* annoGroup, osg::Group* editGroup);

	void DrawLine(const SpatialReference* geoSRS, osg::Group* annoGroup, osg::Group* labelGroup, Style labelStyle);

	void DrawPolygon(const SpatialReference* geoSRS, osg::Group* annoGroup);

	// 在地球上绘制地标
	void DrawLandMark(osg::Group* labelGroup, const SpatialReference* geoSRS, Style labelStyle);

	osg::ref_ptr<osg::AnimationPath> CreateAnimationPath(const GeoPoint& pos, const SpatialReference* mapSRS, float radius, double looptime);
	osg::ref_ptr<osg::Node> CreatePlane(osg::Node* node, const GeoPoint& pos, const SpatialReference* mapSRS, double radius, double time);
public:
	// 创建飞机历史航迹
	void AddBuildHistoryRoute(osg::MatrixTransform* scaler, float lineWidth);
	// 创建飞机飞行彩带
	void AddBuildRibbon(int size, osg::MatrixTransform* scaler, int ribbonWidth);
	// 在左上角添加经度度标签，鼠标移动时候显示  
	void AddLonLatLabel();
	// 设置飞机视角跟随 接收 udp 数据，实时仿真
	void RealTimeSimulation(bool is_track=true);
	// 在右上角显示指南针
	void AddCompass();
public:
	int createControls(void);
	int FlyTo(double longitude, double latitude, double altitude, double height, double heading, double pitch, double range);
	int StartFly(void);
	// 添加帧率标签
	int AddFrameRateLabel(void);
	// 显示多维态势
	int DisplaySituation(void);
};

class CRenderingThread : public OpenThreads::Thread
{
public:
    CRenderingThread( COSG* ptr );
    virtual ~CRenderingThread();

    virtual void run();

protected:
    COSG* _ptr;
    bool _done;
};


class UpdateCompass : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};



class GetFrameRate :public osgGA::GUIEventHandler
{
public:
	GetFrameRate(osgEarth::Util::LabelControl* fpsLabelContral);

	bool handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter&aa);

public:
	double curFrameNum, curTime, nextFrameNum, nextTime;
	double fps;
	osgEarth::Util::LabelControl* fpsLabel;
};
