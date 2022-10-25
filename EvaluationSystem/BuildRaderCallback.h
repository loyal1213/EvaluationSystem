// BuildRader/BuildRaderCallback 主要用来处理雷达相关操作
#pragma once

#include <osgViewer/Viewer>
#include <osgEarth/MapNode>
#include <osg/AnimationPath>
#include <osgEarth/Utils>
#include <osgEarthUtil/EarthManipulator>
#include <osgParticle/FireEffect>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventHandler>
#include <math.h>
#include <iostream>
#include <fstream>

//自定义回调类
class CBuildRaderCallback:public osg::NodeCallback
{
public:
	CBuildRaderCallback(float fRotateSpeed,float fRotateRadius,float fRotateHeight);
	~CBuildRaderCallback();
	virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
private:
	float m_fSpeed; //旋转速度
	float m_fRadius; //距（0，0，0）距离
	float m_fHeight;
};
