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

class cOSG
{
public:
    cOSG(HWND hWnd);
    ~cOSG();

    void InitOSG(std::string filename);

	void InitManipulators(void);
    void InitSceneGraph(void);
    void InitCameraConfig(void);
    void SetupWindow(void);
    void SetupCamera(void);
    void PreFrameUpdate(void);
    void PostFrameUpdate(void);
    void Done(bool value) { mDone = value; }
    bool Done(void) { return mDone; }
    //static void Render(void* ptr);

    osgViewer::Viewer* getViewer() { return mViewer; }
protected:
	// �����ǿ�
	int CreateStarrySky(void);
	// ��ӱ�ǩ�ؼ�����ʾ��γ�ȸ߶�
	void AddViewPointLable();
	// ���ָ����
	osg::MatrixTransform *cOSG::createCompassPart(const std::string &image, float radius, float height);

protected:
	// virtual void OnRecv(const char* buf, USHORT len, const char* fromIp, USHORT fromPort) override;

private:
    bool mDone;
    std::string m_ModelName;
    HWND m_hWnd;
    osgViewer::Viewer* mViewer;
    osg::ref_ptr<osg::Group> mRoot;
    osg::ref_ptr<osg::Node> mModel;
    osg::ref_ptr<osgGA::TrackballManipulator> trackball;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator;
private:
	osg::ref_ptr<osgEarth::MapNode>  earth_map_node_;
	// ָ����
	osg::ref_ptr<osg::MatrixTransform> mt_compass_;

	// ��γ����ʾ��ǩ
	CLabelControlEventHandler* label_event_;
	osg::ref_ptr<osgEarth::Util::Controls::ControlCanvas> canvas_;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earth_manipulator_;

private:
	osg::ref_ptr<osg::CoordinateSystemNode> coordinate_system_node_;// �ر�ľ������

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
	// ��Ҫ���������״���ز���
	CBuildRader m_pBuildRader;

public:
	int AddAirPlane(void);
	void DoPreLineNow();
//	osg::Node* loadFly();
	int AddAnnotation(void);
public:
	// �����ɻ���ʷ����
	void AddBuildHistoryRoute(osg::MatrixTransform* scaler, float lineWidth);
	// �����ɻ����вʴ�
	void AddBuildRibbon(int size, osg::MatrixTransform* scaler, int ribbonWidth);
	// �����Ͻ���Ӿ��ȶȱ�ǩ������ƶ�ʱ����ʾ  
	void AddLonLatLabel();
	// ���÷ɻ��ӽǸ��� ���� udp ���ݣ�ʵʱ����
	void RealTimeSimulation(bool is_track=true);
	// �����Ͻ���ʾָ����
	void AddCompass();
	// �����ʾ�ӵ���Ϣ�Ŀؼ�
//	void AddViewPointLabel(void);
public:
	int createControls(void);
	int FlyTo(double longitude, double latitude, double altitude, double height, double heading, double pitch, double range);
	int StartFly(void);
};

class CRenderingThread : public OpenThreads::Thread
{
public:
    CRenderingThread( cOSG* ptr );
    virtual ~CRenderingThread();

    virtual void run();

protected:
    cOSG* _ptr;
    bool _done;
};


class UpdateCompass : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};
