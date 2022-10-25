#pragma once
#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
/////////////////////////////////////////////////////////////////////////////
// Disable unavoidable warning messages:

//  4103: used #pragma pack to change alignment
//  4114: same type qualifier used more than once
//  4201: nonstandard extension used : nameless struct/union
//  4237: "keyword" reserved for future use
//  4251: class needs to have dll-interface to export class
//  4275: non DLL-interface class used as base for DLL-interface class
//  4290: C++ Exception Specification ignored
//  4503: decorated name length exceeded, name was truncated
//  4786: string too long - truncated to 255 characters

#pragma warning(disable : 4103 4114 4201 4237 4251 4275 4290 4503 4335 4786)

#endif // WIN32

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

#include <iostream>

#include "UdpClient.h"

osg::Node* createEarth();


class ModelPositionCallback : public osg::NodeCallback,public IUdpRecvCallback
{
public:
	ModelPositionCallback();
	~ModelPositionCallback(void);
public:
	void updateParameters();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	virtual void OnRecv(const char* buf, USHORT len, const char* fromIp, USHORT fromPort) override;
private:
	double                  _latitude;
	double                  _longitude;
	double                  _height;
	double                  _rotation;
	float					_heading;
	double                  _speed;
	osg::ref_ptr<osg::CoordinateSystemNode> csn_;
};

class FindNamedNodeVisitor : public osg::NodeVisitor
{
public:
	FindNamedNodeVisitor(const std::string& name);

	virtual void apply(osg::Node& node);

public:
	typedef std::vector< osg::ref_ptr<osg::Node> > NodeList;
	std::string _name;
	NodeList _foundNodes;
};