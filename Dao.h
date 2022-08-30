#pragma once
//#define _CRT_SECURE_NO_WARNINGS;
//#pragma warning(disable:4996);
#include<iostream>
#include <windows.h>
#include <stdlib.h>
#include<io.h>
#include<fstream>
#include <direct.h>
#include<vector>
#include<unordered_map>
//#include<D:\1\Python39_64\include\Python.h>
//#pragma comment(lib,"D:\1\Python39_64\libs\python39.lib")
#include"Qt.h"
using namespace std;

#define MAXNUM 100001
struct Edge
{
	wstring start;	//起点
	wstring end;	//终点
	int dis;  //距离
	wstring line;//几号线
	Edge(const wstring& s, const wstring& e, const int distance, const wstring& l)
	{
		start = s;
		end = e;
		dis = distance;
		line = l;
	}
};
struct Adj
{
	int dis;
	wstring line;
	wstring line2;
	Adj(int distance = MAXNUM, wstring l = L"",wstring l2=L"")
	{
		dis = distance;
		line = l;
		line2 = l2;
	}
};
class Dao
{
public:
	Dao()
	{
		if (_waccess(filepath.c_str(), 0) == -1)
		{
			CreateDirectory(LPCWSTR(filepath.c_str()), NULL);
		}
		Lines[L"1号线/八通线"] = 75 *0.6;				//1111
		Lines[L"2号线"] = 80 * 0.6;						 //11111
		Lines[L"4号线/大兴线"] = 80 * 0.6;				 //11111
		Lines[L"5号线"] = 80 * 0.6;						 //11111
		Lines[L"6号线"] = 100 * 0.6;					 //111111
		Lines[L"7号线"] = 80 * 0.6;						 //111111
		Lines[L"8号线"] = 80 * 0.6;						 //111111
		Lines[L"9号线"] = 80 * 0.6;						 //111111
		Lines[L"10号线"] = 80 * 0.6;					 //11111
		Lines[L"11号线"] = 100 * 0.6;					 //111111
		Lines[L"13号线"] = 80 * 0.6;					 //111111
		Lines[L"14号线"] = 80 * 0.6;					 //111111
		Lines[L"15号线"] = 100 * 0.6;					 //11111
		Lines[L"16号线"] = 80 * 0.6;					 //11111
		Lines[L"17号线"] = 100 * 0.6;					 //11111
		Lines[L"19号线"] = 120 * 0.6;					 //
		Lines[L"亦庄线"] = 80 * 0.6;					 //111111
		Lines[L"房山线"] = 100 * 0.6;					 //11111
		Lines[L"燕房线"] = 80 * 0.6;					 //111111
		Lines[L"昌平线"] = 100 * 0.6;					 //11111
		Lines[L"S1线"] = 100 * 0.6;						 //111111
		Lines[L"西郊线"] = 70 * 0.6;					 //111
		Lines[L"亦庄T1线"] = 70 * 0.6;					 //111
		Lines[L"首都机场线"] = 110 * 0.6;				 // 111
		Lines[L"大兴机场线"] = 160 * 0.5;				 //111
	}
	~Dao();

	bool ResetNode();										 //重写节点信息到文件中
	bool SetEdge(const Edge& e);							 //添加新边
	int GetNode();											 //从文件中获取节点
	int GetEdge();											 //从文件中获取边
	void RewriteEdge();										 //重写边信息到文件中
	bool UpdateNode(const wstring& str);					 //添加节点
	bool UpdateEdge(const Edge& e);							 //添加边
	void DelNode(const wstring& str);						 //删除节点
	void DelEdge(const wstring& s, const wstring& e);		 //删除边
															 //
	int GetNodeNum();										 //获取节点数
	int GetDis(const int i, const int j);					 //获取站点间距离
	double GetTime(const int i, const int j);				 //获取时间
	wstring GetCurLine(const int s, const int e);			 //
	wstring GetCurLine(const wstring& s, const wstring& e);	 //获取站点间线路
															 //
	int GetNodeIdx(const wstring& str);						 //获取节点编号
	vector<wstring> GetNodeList();							 //获取节点列表
	wstring FindNodebyIdx(const int idx);					 //通过编号查找节点
	vector<wstring> GetLines();								 //获取边
	bool  GetDefault();										 //获取默认配置信息
	wstring GetSecondLine(const wstring& s, const wstring& e);	 //获取站点间线路
	wstring GetSecondLine(const int i, const int j);
private:													 //
	wstring filepath = L"data/";							 //数据文件路径
	wstring defaultfilepath = L".default/";					 //默认数据路径
	vector<vector<Adj>> adj;								 //邻接矩阵
	unordered_map<wstring, int> node;						 //节点
	unordered_map<wstring, int> Lines;						 //线路
};