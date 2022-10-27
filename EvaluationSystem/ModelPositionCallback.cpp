#include "StdAfx.h"
#include "ModelPositionCallback.h"
#include "NavigationZoneAndLineStruct.h"


ModelPositionCallback::ModelPositionCallback():
	_latitude(39.510251),// Ĭ��λ��Ϊ�������˻���
	_longitude(116.410745),
	_height(35.0f), // ���ڷɻ��ķ��и߶Ⱥ��ٶ�,��������ع涨��,һ��������600�׿�ʼ��8400��Ϊֹ
	_speed(400.0f), // һ����ԣ��񺽷ɻ���Ѳ���ٶ���800-900����/Сʱ���ң������Ե�1000�����½ʱ��ʱ����230��������
	_heading(0.0),
	csn_(0),
	p_udp_client_(nullptr)
{	
	csn_ = new osg::CoordinateSystemNode; // ��������ϵ�ڵ�
	csn_->setEllipsoidModel(new osg::EllipsoidModel()); // ������Բ��ģ��

	// todo
	p_udp_client_ = new CUdpClient(this);
	p_udp_client_->Start(3000);
	// _rotation.makeRotate(osg::DegreesToRadians(90.0),0.0,0.0,1.0);
}

void ModelPositionCallback::updateParameters()
{
	// _longitude += _speed * ((2.0*osg::PI)/360.0)/20.0;
}

void ModelPositionCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	//updateParameters();
	osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);
	if (mt){
		osg::Matrix matrix;
		csn_->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(_latitude), osg::DegreesToRadians(_longitude), fabs(500.0f), matrix);
		matrix.preMult(osg::Matrix::scale(10, 10, 10)*osg::Matrix::rotate(osg::inDegrees(144.0f), osg::Vec3(0, 0, 1)));
		mt->setMatrix(matrix);
		_longitude -= 0.00005;
		_latitude -= 0.00005;
		_rotation += 0.00;
	}
	traverse(node,nv);

}

void ModelPositionCallback::OnRecv(const char* buf, USHORT len, const char* fromIp, USHORT fromPort)
{
	aircraftStatusData *dlc_data = (aircraftStatusData *)buf;
	TRACE("����:%0.8f,ά��:%0.8f,�߶�:%0.8f,\
		  �����:%0.8f,������:%0.8f,������:%0.8f,\
		  ӭ��:%0.8f,�໬��:%0.8f,����:%0.8f \n",\
		  dlc_data->dbLong,dlc_data->dbLati,dlc_data->fHeight,\
		  dlc_data->fPsi,dlc_data->fTheta,dlc_data->fGama,\
		  dlc_data->fAlpha,dlc_data->fBeta,dlc_data->fVel);
	// _height = dlc_data->fHeight;
	if ( fabs(dlc_data->dbLong) > 0 && fabs(dlc_data->dbLong) < 180 && \
		 fabs(dlc_data->dbLati) > 0 && fabs(dlc_data->dbLati) < 90
		)
	{
		/*_longitude = dlc_data->dbLong;
		_latitude = dlc_data->dbLati;
		_height = dlc_data->fHeight;
		_speed = dlc_data->fVel;
		_heading = dlc_data->fPsi;*/
	}
	// osg::Quat               _rotation;
	//double                  _speed;
	// earth_manipulator_->setViewpoint(osgEarth::Viewpoint("view_point5", dlc_data->dbLong, dlc_data->dbLati, 4000, -60, -90, 1000), 1.0);
}

ModelPositionCallback::~ModelPositionCallback(void)
{
	if (p_udp_client_!=nullptr){
		delete p_udp_client_;
		p_udp_client_ = nullptr;
	}
}


FindNamedNodeVisitor::FindNamedNodeVisitor(const std::string& name)
	: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
	_name(name) 
{}

void FindNamedNodeVisitor::apply(osg::Node& node)
{
	if (node.getName()==_name)
	{
		_foundNodes.push_back(&node);
	}
	traverse(node);
}
