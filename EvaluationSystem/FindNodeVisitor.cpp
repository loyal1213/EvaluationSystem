#include "StdAfx.h"
#include "FindNodeVisitor.h"

//FindNodeVisitor.cpp

#include "FindNodeVisitor.h"

//���캯������ʼ�������ñ������е��ӽڵ�
FindNodeVisitor::FindNodeVisitor(const std::string& searchName):
osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
	searchForName(searchName)
{

}


//����apply()����
void FindNodeVisitor::apply(osg::Node& searchNode)
{
	//�жϽڵ������Ƿ�����ҵĽڵ�����һ��
	if(searchNode.getName() == searchForName)
	{
		//��ӵ��ڵ�ϵ��
		foundNodeList.push_back(&searchNode);
	}

	//��������
	traverse(searchNode);
}

//����apply()����
void FindNodeVisitor::apply(osg::Transform& searchNode)
{
	//�жϽڵ������Ƿ�����ҵĽڵ�����һ��
	if(searchNode.getName() == searchForName)
	{
		//��ӵ��ڵ�ϵ��
		foundNodeList.push_back(&searchNode);
	}

	//��������
	traverse(searchNode);
}

//����apply()����
void FindNodeVisitor::apply(osg::Geode& geode)
{
	apply((osg::Node&)geode);
	traverse((osg::Node&)geode);
}

//����Ҫ���ҵĽڵ�����
void FindNodeVisitor::setNameToFind(const std::string& searchName)
{

	searchForName = searchName;
	foundNodeList.clear();
}

//�õ����ҽڵ������еĵ�һ���ڵ�
osg::Node* FindNodeVisitor::getFirst()
{
	return *(foundNodeList.begin());
}
