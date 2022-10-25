//FindNodeVisitor.h

#ifndef FIND_NODE_VISITOR_H
#define FIND_NODE_VISITOR_H

#include <osg/NodeVisitor>
#include <osg/Node>

#include <osgSim/DOFTransform>
#include <iostream>
#include <vector>
#include <string>

//节点查找访问器，继承自osg::NodeVisitor
class FindNodeVisitor:public osg::NodeVisitor
{
public:

	//构造函数，参数为需要查找的节点名
	FindNodeVisitor(const std::string& searchName);

	//重载apply()方法
	virtual void apply(osg::Node& searchNode);

	virtual void apply(osg::Geode& geode);

	virtual void apply(osg::Transform& searchNode);

	//设置要查找的节点名
	void setNameToFind(const std::string& searchName);

	//得到查找节点向量的第一个节点
	osg::Node* getFirst();

	//定义一个节点向量
	typedef std::vector<osg::Node*> nodeListType;

	//得到查找节点向量
	nodeListType& getNodeList()
	{
		return foundNodeList;
	}

private:

	//节点名
	std::string searchForName;

	//用来保存查找的节点
	nodeListType foundNodeList;

};

#endif