#include "StdAfx.h"
#include "PickHandler.h"

//�¼�������
bool CPickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*> (&aa);
	switch(ea.getEventType()){
		//ÿһ֡
	case(osgGA::GUIEventAdapter::FRAME):{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		//�õ���ͼ����
		viewer ->getCamera() ->getViewMatrixAsLookAt(position, center, up) ;
		if (viewer){	
			//ִ��PICK����
			pick(viewer.get(),ea);
		}
		return false;
										}
	case osgGA::GUIEventAdapter::PUSH: {

		/*if (ea.getButton()==1) {  
			Pick(ea.getX(),ea.getY());//��ͨ���¼�ea��������������  
		}  */

		osgUtil::LineSegmentIntersector::Intersections intersections;
		osg::ref_ptr<osg::Node> node = new osg::Node();
		osg::ref_ptr<osg::Group> parent = new osg::Group();
		// ��handle�������ȡ�ض�group0�����Լ����������е������йأ��˴�Ϊroot�ĵ�һ���ӽڵ㣩
		// osg::ref_ptr<osg::Group> group0 = dynamic_cast<osg::Group*>( viewer->getSceneData()->asGroup()->getChild(0));
		if (viewer->computeIntersections(ea.getX(), ea.getY(), intersections)){
			//�õ�ѡ��Ľڵ�
			osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
			osg::NodePath& nodePath = intersection.nodePath;	
			node = nodePath.back(); // ȡnodepath�е����һ����Ϊ��ǰ�ڵ㣺node = nodePath.back();

			//osg::ref_ptr<osg::Group> group =dynamic_cast<osg::Group*>( nodePath[2]);							
			//if (group==group0){
				//����ڵ��л�����
				parent = dynamic_cast<osg::Group*> (nodePath[nodePath.size() - 2]);//��ǰѡ��ڵ�ĸ��ڵ�
				osgFX::Outline *ot = dynamic_cast<osgFX::Outline*>(parent.get());
				if (!ot) {	//��ot�����ڣ�δ������ (node->parent)=>(node->outline->parent)
					osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
					outline->setColor(osg::Vec4(1, 1, 0, 1));
					outline->setWidth(5);
					outline->addChild(node);
					parent->replaceChild(node, outline);
				}else{	//��ot���ڣ��������ҳ���ǰoutline�ĸ��ڵ㣨node->outline->*itr��=>(node->*itr)
					osg::Node::ParentList parentList = ot->getParents();
					osg::Node::ParentList::iterator itr = parentList.begin();
					(*itr)->replaceChild(ot, node);
				}
			}
		//}


		return false;  
	 
									   }
	default:
		return false;
	}

}
//PICK����
void CPickHandler::pick(osg::ref_ptr<osgViewer::Viewer> viewer, const osgGA::GUIEventAdapter& ea)
{
	//����һ���߶ν���������
	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	//����һ����
	std::ostringstream os;
	//�õ�����λ��
	float x = ea.getX();
	float y = ea.getY();
	//���û�з����������㣬�����û�е�������
	if (viewer->computeIntersections(x,y,intersections))
	{
		//�õ��ཻ�����Ľ���
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			//������
			os<<"Mouse in World  X:"<< hitr->getWorldIntersectPoint().x()<<"     Y: "<<
				hitr->getWorldIntersectPoint().y()<<"     Z: "<< hitr->getWorldIntersectPoint().z()<<std::endl ;
		}
	}
	//������
	os<<"Viewer Position X: "<<position[0]<<"     Y: "<<position[1]<<"     Z: "<<position[2]<<std::endl ;

	gdlist += os.str();

	//������ʾ����
	setLabel(gdlist);
}




osg::ref_ptr<osg::Node> CreateHUD::createHUD(osg::ref_ptr<osgText::Text> updateText)
{
	//����һ�����
	osg::ref_ptr<osg::Camera> hudCamera = new osg::Camera;
	//���þ���֡����
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//������ͶӰ����
	hudCamera->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
	//������ͼ����
	hudCamera->setViewMatrix(osg::Matrix::identity());
	//������Ⱦ˳��ΪPOST
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	//�����Ȼ���
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	//��������
	std::string timesFont("fonts//cour.ttf");

	//����λ��
	osg::Vec3 position(700,900,0.0);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	//�رչ���
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	//�ر���Ȳ���
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	geode->addDrawable(updateText.get());
	hudCamera->addChild(geode.get());

	updateText->setCharacterSize(20.0f);
	updateText->setFont(timesFont);
	updateText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	updateText->setText("");
	updateText->setPosition(position);
	//�������ݱ���ΪDYNAMIC
	updateText->setDataVariance(osg::Object::DYNAMIC);

	return hudCamera.get();

}