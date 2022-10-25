#pragma once
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgUtil/LineSegmentIntersector>
#include <osgEarth/MapNode>
#include <osgEarthUtil/Controls>
#include <osgEarth/MapNode>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgEarth/GeoData>
#include <osgEarthUtil/Formatter>
#include <Windows.h>
#include <iostream>

using namespace osgEarth::Util;

class CLabelControlEventHandler : public osgGA::GUIEventHandler
{
public:
	struct Callback : public osg::Referenced
	{
		virtual void set(const osgEarth::GeoPoint& coords, osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode) = 0;

		virtual void reset(/*osg::View*/osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode) = 0;

		virtual ~Callback() { }
	};

public:
	CLabelControlEventHandler(MapNode* mapNode, osgEarth::Util::Controls::LabelControl* mouse_label = 0L,\
		osgEarth::Util::Controls::LabelControl* view_label = 0L,\
		osgEarth::Util::Controls::LabelControl* fly_label = 0L,\
		Formatter* formatter = 0L);

	virtual ~CLabelControlEventHandler() { }

	void addCallback(Callback* callback);


public:
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
protected:
	osgEarth::MapNode*      _mapNode;
	osg::NodePath _mapNodePath;
	typedef std::vector<osg::ref_ptr<Callback>> Callbacks;
	Callbacks _callbacks;
};


class MouseCoordsLabelCallback : public CLabelControlEventHandler::Callback
{
public:
	MouseCoordsLabelCallback(osgEarth::Util::Controls::LabelControl* mouse_label = 0L,\
		osgEarth::Util::Controls::LabelControl* view_label = 0L,\
		osgEarth::Util::Controls::LabelControl* fly_label = 0L,\
		Formatter* formatter = 0L);

	virtual ~MouseCoordsLabelCallback() { }

	virtual void set(const osgEarth::GeoPoint& coords, osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode);
	virtual void reset(osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode);

protected:
	osg::observer_ptr<osgEarth::Util::Controls::LabelControl> mouse_label_;
	osg::observer_ptr<osgEarth::Util::Controls::LabelControl> view_label_;
	osg::observer_ptr<osgEarth::Util::Controls::LabelControl> fly_label_;
	osg::ref_ptr<osgEarth::Util::Formatter>         _formatter;
};



//转换为中文字符进行显示   
void gb2312ToUnicode(const std::string& src, std::wstring& result);
void unicodeToUTF8(const std::wstring &src, std::string& result);
void gb2312ToUtf8(const std::string& src, std::string& result);