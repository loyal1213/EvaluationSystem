#include "StdAfx.h"
#include "PickHandler.h"

//事件处理函数
bool CPickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*> (&aa);
	switch(ea.getEventType()){
		//每一帧
	case(osgGA::GUIEventAdapter::FRAME):{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		//得到视图矩阵
		viewer ->getCamera() ->getViewMatrixAsLookAt(position, center, up) ;
		if (viewer){	
			//执行PICK动作
			pick(viewer.get(),ea);
		}
		return false;
										}
	case osgGA::GUIEventAdapter::PUSH: {

		/*if (ea.getButton()==1) {  
			Pick(ea.getX(),ea.getY());//可通过事件ea获得鼠标点击的坐标  
		}  */

		osgUtil::LineSegmentIntersector::Intersections intersections;
		osg::ref_ptr<osg::Node> node = new osg::Node();
		osg::ref_ptr<osg::Group> parent = new osg::Group();
		// 在handle函数里获取特定group0（与自己在主函数中的设置有关，此处为root的第一个子节点）
		// osg::ref_ptr<osg::Group> group0 = dynamic_cast<osg::Group*>( viewer->getSceneData()->asGroup()->getChild(0));
		if (viewer->computeIntersections(ea.getX(), ea.getY(), intersections)){
			//得到选择的节点
			osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
			osg::NodePath& nodePath = intersection.nodePath;	
			node = nodePath.back(); // 取nodepath中的最后一个即为当前节点：node = nodePath.back();

			//osg::ref_ptr<osg::Group> group =dynamic_cast<osg::Group*>( nodePath[2]);							
			//if (group==group0){
				//点击节点切换高亮
				parent = dynamic_cast<osg::Group*> (nodePath[nodePath.size() - 2]);//当前选择节点的父节点
				osgFX::Outline *ot = dynamic_cast<osgFX::Outline*>(parent.get());
				if (!ot) {	//若ot不存在（未高亮） (node->parent)=>(node->outline->parent)
					osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
					outline->setColor(osg::Vec4(1, 1, 0, 1));
					outline->setWidth(5);
					outline->addChild(node);
					parent->replaceChild(node, outline);
				}else{	//若ot存在（高亮）找出当前outline的父节点（node->outline->*itr）=>(node->*itr)
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
//PICK动作
void CPickHandler::pick(osg::ref_ptr<osgViewer::Viewer> viewer, const osgGA::GUIEventAdapter& ea)
{
	//创建一个线段交集检测对象
	osgUtil::LineSegmentIntersector::Intersections intersections;

	std::string gdlist="";
	//申请一个流
	std::ostringstream os;
	//得到鼠标的位置
	float x = ea.getX();
	float y = ea.getY();
	//如果没有发生交集运算，及鼠标没有点中物体
	if (viewer->computeIntersections(x,y,intersections))
	{
		//得到相交交集的交点
		for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			hitr != intersections.end();
			++hitr)
		{
			//输入流
			os<<"Mouse in World  X:"<< hitr->getWorldIntersectPoint().x()<<"     Y: "<<
				hitr->getWorldIntersectPoint().y()<<"     Z: "<< hitr->getWorldIntersectPoint().z()<<std::endl ;
		}
	}
	//输入流
	os<<"Viewer Position X: "<<position[0]<<"     Y: "<<position[1]<<"     Z: "<<position[2]<<std::endl ;

	gdlist += os.str();

	//设置显示内容
	setLabel(gdlist);
}




osg::ref_ptr<osg::Node> CreateHUD::createHUD(osg::ref_ptr<osgText::Text> updateText)
{
	//创建一个相机
	osg::ref_ptr<osg::Camera> hudCamera = new osg::Camera;
	//设置绝对帧引用
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//设置正投影矩阵
	hudCamera->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
	//设置视图矩阵
	hudCamera->setViewMatrix(osg::Matrix::identity());
	//设置渲染顺序为POST
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	//清除深度缓存
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	//设置字体
	std::string timesFont("fonts//cour.ttf");

	//设置位置
	osg::Vec3 position(700,900,0.0);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	//关闭关照
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	//关闭深度测试
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	geode->addDrawable(updateText.get());
	hudCamera->addChild(geode.get());

	updateText->setCharacterSize(20.0f);
	updateText->setFont(timesFont);
	updateText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	updateText->setText("");
	updateText->setPosition(position);
	//设置数据变量为DYNAMIC
	updateText->setDataVariance(osg::Object::DYNAMIC);

	return hudCamera.get();

}