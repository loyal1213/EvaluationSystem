#pragma once

#include <osgViewer/Viewer>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osgText/Text>

#include <sstream>

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osgFX/Scribe>
#include <osgFX/Outline>
#include <osgViewer/ViewerEventHandlers>
//pick事件处理器
class CPickHandler : public osgGA::GUIEventHandler 
{
public: 

	//构造函数
	CPickHandler(osg::ref_ptr<osgText::Text> updateText):
	  _updateText(updateText) {
		  //
	  }
 
	  //析构函数
	  ~CPickHandler() 
	  {
		  //
	  }

	  //事件处理
	  bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	  //pick
	  virtual void pick(osg::ref_ptr<osgViewer::Viewer> viewer, const osgGA::GUIEventAdapter& ea);

	 

	  //设置显示内容
	  void setLabel(const std::string& name)
	  {
		  _updateText->setText(name);
	  }

protected:
	//用到得到当前视图矩阵
	osg::Vec3 position ;
	osg::Vec3 center ;
	osg::Vec3 up ;

	//传递一个文字对象
	osg::ref_ptr<osgText::Text>  _updateText;
};

//HUD
class CreateHUD
{
public:
	CreateHUD(){
		//
	}

	~CreateHUD(){
		//
	}

	//创建HUD
	osg::ref_ptr<osg::Node> createHUD(osg::ref_ptr<osgText::Text> updateText);
	
} ;