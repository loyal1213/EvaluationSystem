#include "StdAfx.h"
#include "LabelControlEventHandler.h"
#include <osgEarth/Terrain>
#include <osgEarthUtil/EarthManipulator>

CLabelControlEventHandler::CLabelControlEventHandler(MapNode* mapNode, \
	osgEarth::Util::Controls::LabelControl* mouse_label, \
	osgEarth::Util::Controls::LabelControl* view_label, \
	osgEarth::Util::Controls::LabelControl* fly_label, \
	Formatter* formatter) :_mapNode(mapNode)
{
	_mapNodePath.push_back((osg::Node *)mapNode->getTerrainEngine());

	if (mouse_label && view_label && fly_label){
		addCallback(new MouseCoordsLabelCallback(mouse_label,view_label,fly_label));
	}

	/*if (view_label){
		addCallback(new MouseCoordsLabelCallback());
	}*/
}

void CLabelControlEventHandler::addCallback(CLabelControlEventHandler::Callback* cb)
{
	_callbacks.push_back(cb);
}

bool CLabelControlEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{	

	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer *>(&aa);
	if (ea.getEventType() == ea.MOVE || ea.getEventType() == ea.DRAG || ea.getEventType() == ea.SCROLL ){
		osg::Vec3d world;
		if (_mapNode->getTerrain()->getWorldCoordsUnderMouse(aa.asView(), ea.getX(), ea.getY(), world)){
			osgEarth::GeoPoint map;
			map.fromWorld(_mapNode->getMapSRS(), world);

			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->set(map, viewer/*aa.asView()*/, _mapNode);
		}else{
			for (Callbacks::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
				i->get()->reset(viewer, _mapNode);
		}
	}

	if (ea.getEventType() == ea.FRAME){	
		osgEarth::Util::EarthManipulator * em = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
		if (em == nullptr){
			return  false;
		}
		osgEarth::Viewpoint view_point = em->getViewpoint();
		double fx = view_point.focalPoint().mutable_value().x();
		double fy = view_point.focalPoint().mutable_value().y();
		double fz = view_point.focalPoint().mutable_value().z();
		double heading_deg = view_point.getHeading();
		double pitch = view_point.getPitch();
		double rangle = view_point.getRange();
		switch(ea.getEventType()){
		case osgGA::GUIEventAdapter::DOUBLECLICK: break;
		case osgGA::GUIEventAdapter::PUSH: break;
		case osgGA::GUIEventAdapter::RELEASE: break;
		case osgGA::GUIEventAdapter::SCROLL:break;
		case osgGA::GUIEventAdapter::KEYDOWN: {
				if (ea.getKey() == 'X'){		// focalPoint	���㣺����Ľ���λ�ã���һ���������꣨���̣߳�
					fx++;	
				}else if(ea.getKey() == 'x'){
					fx--;
				}else if (ea.getKey() == 'Y'){
					fy++;
				}else if (ea.getKey() == 'y'){
					fy--;
				}else if (ea.getKey() == 'Z'){
					fz++;
				}else if (ea.getKey() == 'z'){
					fz--;
				}else if (ea.getKey() == 'H'){ // heading	ˮƽ��λ�ǣ�0-360��ֵ�����Ƶ�ͼˮƽ��ת����λ�Ƕȡ�
					heading_deg++;
				}else if (ea.getKey() == 'h'){
					heading_deg--;
				}else if (ea.getKey() == 'P'){ // pitch	�����ǣ�-90��0��ֵ����λ�Ƕ�
					pitch++;
				}else if (ea.getKey() == 'p'){
					pitch--;
				}else if (ea.getKey() == 'R'){ // range	���ࣺ���λ�õ�����ľ��룬��λ���ס�
					rangle+=1000;
				}else if (ea.getKey() == 'r'){
					rangle-=1000;
				}
				// 112.44 33.75 444.02 -15.84 -53.01 402812.75
				em->setViewpoint(osgEarth::Viewpoint("viepoint_1",fx,fy,fz,heading_deg,pitch,rangle));
			}
		}

	}

	return false;
}

//-----------------------------------------------------------------------

MouseCoordsLabelCallback::MouseCoordsLabelCallback(osgEarth::Util::Controls::LabelControl* mouse_label, \
	osgEarth::Util::Controls::LabelControl* view_label, \
	osgEarth::Util::Controls::LabelControl* fly_label, \
	Formatter* formatter) :
	mouse_label_(mouse_label), view_label_(view_label), fly_label_(fly_label),_formatter(formatter){}

void MouseCoordsLabelCallback::set(const osgEarth::GeoPoint& mapCoords, osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode)
{
	if (!viewer){
		return;
	}

	osgEarth::Util::EarthManipulator * em = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
	if (em == nullptr){
		return ;
	}

	if (mouse_label_.valid()){
		char allStr[512] ={0};
		sprintf(allStr, "%s:%.8f %s:%.8f %s:%.8f", "����",mapCoords.x(), "ά��", mapCoords.y(), "����",mapCoords.z());
		std::string allStrUtf8;
		gb2312ToUtf8(allStr, allStrUtf8);//����ת��
		mouse_label_->setText(allStrUtf8);
	}
	
	if (view_label_.valid()){
		char allStr[512] = {0};
		 // longtutude latitude  heading pitch range
		sprintf(allStr, "%.8f %.8f | (%.3f %.3f %.3f)",\
			em->getViewpoint().focalPoint().mutable_value().x(), \
			em->getViewpoint().focalPoint().mutable_value().y(),\
			em->getViewpoint().getHeading(),\
			em->getViewpoint().getPitch(),\
			em->getViewpoint().getRange());
		std::string allStrUtf8;
		gb2312ToUtf8(allStr, allStrUtf8);//����ת��
		view_label_->setText(allStrUtf8);
	}

	if (fly_label_.valid()){
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform *>(em->getViewpoint().getNode().get());
		if (mt){
			char allStr[512] = {0};
			std::string name = mt->getName();
			/*osg::Vec3d position = mt->getMatrix().getTrans(),lla(0,0,0),targetPosXYZ(0,0,0);
			
			targetPosXYZ = osg::Vec3d((mt->getBound().center()* osg::computeLocalToWorld(mt->getParentalNodePaths()[0])).x(),
				(mt->getBound().center()* osg::computeLocalToWorld(mt->getParentalNodePaths()[0])).y(),
				(mt->getBound().center()* osg::computeLocalToWorld(mt->getParentalNodePaths()[0])).z());

			mapNode->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(targetPosXYZ.x(),targetPosXYZ.y(),targetPosXYZ.z(),lla.x(),lla.y(),lla.z());*/
			osg::Vec3d targetPosXYZ(0,0,0),geography(0,0,0);
			osg::ref_ptr<osg::Node> node = mt->getChild(0);
			targetPosXYZ = osg::Vec3d((node->getBound().center()* osg::computeLocalToWorld(node->getParentalNodePaths()[0])).x(),
				(node->getBound().center()* osg::computeLocalToWorld(node->getParentalNodePaths()[0])).y(),
				(node->getBound().center()* osg::computeLocalToWorld(node->getParentalNodePaths()[0])).z());
			// ע��γ����ǰ��
			mapNode->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(targetPosXYZ.x(), targetPosXYZ.y(), targetPosXYZ.z(), geography[0], geography[1], geography[2]);
			sprintf(allStr,"�ɻ��ͺ�:%s\n ��    ��:%.8f\n γ    ��:%.8f\n ��    ��:%.8f",mt->getName().c_str(),osg::DegreesToRadians(geography.x()),osg::DegreesToRadians(geography.y()),geography.z());
			std::string allStrUtf8;
			gb2312ToUtf8(allStr, allStrUtf8);//����ת��
			fly_label_->setText(allStrUtf8);
		}
		
	}

}

void MouseCoordsLabelCallback::reset(osgViewer::Viewer* viewer, osgEarth::MapNode* mapNode)
{
	if (mouse_label_.valid()){
		mouse_label_->setText("");
	}
	if (view_label_.valid()){
		view_label_->setText("");
	}
	if (fly_label_.valid()){
		fly_label_->setText("");
	}
}



//ת��Ϊ�����ַ�������ʾ   
void gb2312ToUnicode(const std::string& src, std::wstring& result)
{
	int n = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	result.resize(n);
	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}

void unicodeToUTF8(const std::wstring &src, std::string& result)
{
	int n = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0);
	result.resize(n);
	::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0);
}

void gb2312ToUtf8(const std::string& src, std::string& result)
{
	std::wstring strWideChar;
	gb2312ToUnicode(src, strWideChar);
	unicodeToUTF8(strWideChar, result);
}