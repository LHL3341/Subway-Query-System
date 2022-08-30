#pragma once
#include"Dao.h"

using namespace std;

class Graph
{
public:
	Graph();																			  //
	~Graph();																			  //
	int AddNode(const wstring& str);													  //添加站点
	void DelNode(const wstring& str);													  //删除站点
	int AddEdge(const wstring& s, const wstring& e, const int distance, const wstring& l);//添加路线
	void DelEdge(const wstring& s, const wstring& e);									  //删除路线
																						  //
	QStringList Shortest(const wstring& s, const wstring& e);							  //最短距离
	QStringList Fastest(const wstring& s, const wstring& e);							  //最短时间
	QStringList LeastTrans(const wstring& s, const wstring& e);							  //最少换乘
	int LeastStation(const wstring& s, const wstring& e);								  //最少站
	void Cheapest(const wstring& s, const wstring& e);									  //最便宜
	int Check();																		  //检查
	QStringList GetNodes();																  //获取站点列表
	QStringList GetLines();																  //获取线路列表
	QStringList GetPath(const wstring& s, const wstring& e, QString str);				  //获取路径(最短距离\最少换乘....)
	bool Recover();																		  //恢复默认
private:																				  //
	int dfs(int maxtrans, int trans, int* visited, int cur, int pre, int e);			  //深搜
	Dao* dao;																			  //
};
