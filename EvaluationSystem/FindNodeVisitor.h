//FindNodeVisitor.h

#ifndef FIND_NODE_VISITOR_H
#define FIND_NODE_VISITOR_H

#include <osg/NodeVisitor>
#include <osg/Node>

#include <osgSim/DOFTransform>
#include <iostream>
#include <vector>
#include <string>

//�ڵ���ҷ��������̳���osg::NodeVisitor
class FindNodeVisitor:public osg::NodeVisitor
{
public:

	//���캯��������Ϊ��Ҫ���ҵĽڵ���
	FindNodeVisitor(const std::string& searchName);

	//����apply()����
	virtual void apply(osg::Node& searchNode);

	virtual void apply(osg::Geode& geode);

	virtual void apply(osg::Transform& searchNode);

	//����Ҫ���ҵĽڵ���
	void setNameToFind(const std::string& searchName);

	//�õ����ҽڵ������ĵ�һ���ڵ�
	osg::Node* getFirst();

	//����һ���ڵ�����
	typedef std::vector<osg::Node*> nodeListType;

	//�õ����ҽڵ�����
	nodeListType& getNodeList()
	{
		return foundNodeList;
	}

private:

	//�ڵ���
	std::string searchForName;

	//����������ҵĽڵ�
	nodeListType foundNodeList;

};

#endif