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
//pick�¼�������
class CPickHandler : public osgGA::GUIEventHandler 
{
public: 

	//���캯��
	CPickHandler(osg::ref_ptr<osgText::Text> updateText):
	  _updateText(updateText) {
		  //
	  }
 
	  //��������
	  ~CPickHandler() 
	  {
		  //
	  }

	  //�¼�����
	  bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	  //pick
	  virtual void pick(osg::ref_ptr<osgViewer::Viewer> viewer, const osgGA::GUIEventAdapter& ea);

	 

	  //������ʾ����
	  void setLabel(const std::string& name)
	  {
		  _updateText->setText(name);
	  }

protected:
	//�õ��õ���ǰ��ͼ����
	osg::Vec3 position ;
	osg::Vec3 center ;
	osg::Vec3 up ;

	//����һ�����ֶ���
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

	//����HUD
	osg::ref_ptr<osg::Node> createHUD(osg::ref_ptr<osgText::Text> updateText);
	
} ;