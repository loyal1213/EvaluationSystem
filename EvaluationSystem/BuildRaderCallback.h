// BuildRader/BuildRaderCallback ��Ҫ���������״���ز���
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

//�Զ���ص���
class CBuildRaderCallback:public osg::NodeCallback
{
public:
	CBuildRaderCallback(float fRotateSpeed,float fRotateRadius,float fRotateHeight);
	~CBuildRaderCallback();
	virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
private:
	float m_fSpeed; //��ת�ٶ�
	float m_fRadius; //�ࣨ0��0��0������
	float m_fHeight;
};
