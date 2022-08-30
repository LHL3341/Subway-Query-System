#include"Graph.h"

Graph::Graph()
{
	dao = new Dao;
	if (dao->GetNode() && dao->GetEdge())
	{
	}
}

Graph::~Graph()
{
	dao->ResetNode();
	dao->RewriteEdge();
}

int Graph::AddNode(const wstring& str)
{
	bool flag = dao->UpdateNode(str);
	return flag;
}

void Graph::DelNode(const wstring& str)
{
	dao->DelNode(str);
}

int Graph::AddEdge(const wstring& s, const wstring& e, const int distance, const wstring& l)
{
	bool flag = dao->UpdateEdge(Edge(s, e, distance, l));
	return flag;
}

void Graph::DelEdge(const wstring& s, const wstring& e)
{
	dao->DelEdge(s, e);
}

QStringList Graph::Shortest(const wstring& start, const wstring& e)
{
	int n = dao->GetNodeNum();
	vector<bool> s(n);      //是否经过
	vector<int> dis(n);     //到该点的最短距离
	vector<int> pre(n);     //上个节点序号
	for (int i = 0; i < n; i++)
	{
		dis[i] = dao->GetDis(dao->GetNodeIdx(start), i);
		if (dis[i] < MAXNUM)
			pre[i] = dao->GetNodeIdx(start);
		else
			pre[i] = -1;
	}
	dis[dao->GetNodeIdx(start)] = 0;
	s[dao->GetNodeIdx(start)] = true;
	for (int i = 1; i < n; i++)
	{
		int best = dao->GetNodeIdx(start);
		int temp = MAXNUM;
		for (int j = 0; j < n; j++)//找到最小的距离
		{
			if (!s[j] && dis[j] < temp)
			{
				temp = dis[j];
				best = j;
			}
		}
		s[best] = true;
		for (int j = 0; j < n; j++)//更新dist和pre
		{
			if (!s[j] && dao->GetDis(best, j) != MAXNUM)
			{
				int newdis = dis[best] + dao->GetDis(best, j);
				if (newdis < dis[j])
				{
					dis[j] = newdis;
					pre[j] = best;
				}
			}
		}
	}

	int distance = 1;
	double time = 1;
	int end = dao->GetNodeIdx(e);
	int tmp = end;
	bool flag = 0;
	QStringList t;
	while (tmp != dao->GetNodeIdx(start))
	{
		t.append(QString::fromStdWString(dao->FindNodebyIdx(tmp)));
		tmp = pre[end];
		if (tmp == -1)
		{
			flag = 1;
			break;
		}
		time += dao->GetTime(tmp, end)+0.4;
		distance += dao->GetDis(tmp, end);
		end = tmp;
	}
	//wcout << start << "->";
	//for (int i = t.size(); i > 1; i--)
	//{
	//	wcout << dao->FindNodebyIdx(t[i - 1]) << "->";
	//}
	//cout << t[0];
	if (!flag)
	{
		int trans = 0;
		int len = t.size();

		wstring str = dao->GetCurLine(t[len - 1].toStdWString(), start);
		wstring ss(str);
		for (int i = len - 1; i > 0; i--)
		{
			wstring s = dao->GetCurLine(t[i].toStdWString(), t[i - 1].toStdWString());
			if (i == len - 1 && dao->GetSecondLine(t[len - 1].toStdWString(), start) == s)
			{
				str = dao->GetSecondLine(t[len - 1].toStdWString(), start);
			}
			if (str != s && dao->GetSecondLine(t[i].toStdWString(), t[i - 1].toStdWString()) == L"")
			{
				t.insert(i, "<==**换乘" + QString::fromStdWString(s) + "**");
				str = s;
				len++;
				trans++;
				time += 2.5;
			}
		}
		int totaltime = ceil(time);
		int min = totaltime % 60;
		int h = totaltime / 60;
		t.insert(0, "距离: " + QString::number(distance) + "m");
		t.insert(0, "需换乘 " + QString::number(trans) + "次");
		t.insert(0, "预计最短用时: " + QString::number(h) + "h" + QString::number(min) + "min");
		t.insert(0, "搭乘" + QString::fromStdWString(ss));
		return t;
	}
	return QStringList();
}

QStringList Graph::Fastest(const wstring& start, const wstring& e)
{
	int n = dao->GetNodeNum();
	vector<bool> s(n);      //是否访问过
	vector<double> dis(n);     //到该点的最短时间
	vector<int> pre(n);     //上个节点序号
	for (int i = 0; i < n; i++)
	{
		dis[i] = dao->GetTime(dao->GetNodeIdx(start), i);
		if (dis[i] < MAXNUM)
			pre[i] = dao->GetNodeIdx(start);
		else
			pre[i] = -1;
	}
	dis[dao->GetNodeIdx(start)] = 0;
	s[dao->GetNodeIdx(start)] = true;
	for (int i = 1; i < n; i++)//总的迭代次数
	{
		int best = dao->GetNodeIdx(start);
		int temp = MAXNUM;
		for (int j = 0; j < n; j++)//找到最小的距离
		{
			if (!s[j] && dis[j] < temp)
			{
				temp = dis[j];
				best = j;
			}
		}
		s[best] = true;
		for (int j = 0; j < n; j++)//更新dist和pre
		{
			if (!s[j] && dao->GetTime(best, j) < MAXNUM)
			{
				int newdist = dis[best] + dao->GetTime(best, j);
				if (newdist < dis[j])
				{
					dis[j] = newdist;
					pre[j] = best;
				}
			}
		}
	}
	int distance = 1;
	double time = 1;
	int end = dao->GetNodeIdx(e);
	int tmp = end;
	bool flag = 0;
	QStringList t;
	while (tmp != dao->GetNodeIdx(start))
	{
		t.append(QString::fromStdWString(dao->FindNodebyIdx(tmp)));
		tmp = pre[end];
		if (tmp == -1)
		{
			flag = 1;
			break;
		}
		time += dao->GetTime(tmp, end) + 0.4;
		distance += dao->GetDis(tmp, end);
		end = tmp;
	}
	//wcout << start << "->";
	//for (int i = t.size(); i > 1; i--)
	//{
	//	wcout << dao->FindNodebyIdx(t[i - 1]) << "->";
	//}
	//cout << t[0];
	if (!flag)
	{
		int trans = 0;
		int len = t.size();

		wstring str = dao->GetCurLine(t[len - 1].toStdWString(), start);
		wstring ss(str);
		for (int i = len - 1; i > 0; i--)
		{
			wstring s = dao->GetCurLine(t[i].toStdWString(), t[i - 1].toStdWString());
			if (i==len-1&&dao->GetSecondLine(t[len - 1].toStdWString(), start)== s)
			{
				str = dao->GetSecondLine(t[len - 1].toStdWString(), start);
			}
			if (str != s&&dao->GetSecondLine(t[i].toStdWString(), t[i - 1].toStdWString()) == L"")
			{
				t.insert(i , "<==**换乘" + QString::fromStdWString(s) + "**");
				str = s;
				len++;
				trans++;
				time += 2.5;
			}
		}
		int totaltime = ceil(time);
		int min = totaltime % 60;
		int h = totaltime / 60;
		t.insert(0, "距离: " + QString::number(distance) + "m");
		t.insert(0, "需换乘 " + QString::number(trans) + "次");
		t.insert(0, "预计最短用时: " + QString::number(h) + "h" + QString::number(min) + "min");
		t.insert(0, "搭乘" + QString::fromStdWString(ss));
		return t;
	}
	return QStringList();
}
QStringList Graph::LeastTrans(const wstring& s, const wstring& e)
{
	int n = dao->GetNodeNum();
	int* g = new int[n];
	memset(g, -1, sizeof(int) * n);
	int flag = 0;
	int start = dao->GetNodeIdx(s);
	int end = dao->GetNodeIdx(e);
	for (int i = 0; i < 6; i++)
	{
		if (flag = dfs(i, 0, g, start, start, end))
			break;
	}
	if (flag)
	{
		int distance = 1;
		double time = 1;
		int tmp = end;
		QStringList t;
		while (tmp != start)
		{
			t.append(QString::fromStdWString(dao->FindNodebyIdx(tmp)));
			tmp = g[end];
			time += dao->GetTime(tmp, end)+0.4;
			distance += dao->GetDis(tmp, end);
			end = tmp;
		}
		int len = t.size();
		wstring str = dao->GetCurLine(t[len - 1].toStdWString(), s);
		wstring ss(str);
		int trans = 0;
		for (int i = len - 1; i > 0; i--)
		{
			wstring line = dao->GetCurLine(t[i].toStdWString(), t[i - 1].toStdWString());
			if (i==len-1&&dao->GetSecondLine(t[len - 1].toStdWString(), s)== line)
			{
				str = dao->GetSecondLine(t[len - 1].toStdWString(), s);
			}
			if (str != line&&dao->GetSecondLine(t[i].toStdWString(), t[i - 1].toStdWString()) == L"")
			{
				t.insert(i , "<==**换乘" + QString::fromStdWString(line) + "**");
				str = line;
				len++;
				trans++;
				time += 2.5;
			}
		}
		int totaltime = ceil(time);
		int min = totaltime % 60;
		int h = totaltime / 60;
		t.insert(0, "距离: " + QString::number(distance) + "m");
		t.insert(0, "需换乘 " + QString::number(trans) + "次");
		t.insert(0, "预计最短用时: " + QString::number(h) + "h" + QString::number(min) + "min");
		t.insert(0, "搭乘" + QString::fromStdWString(ss));
		return t;
	}
	return QStringList();
}

int Graph::LeastStation(const wstring& s, const wstring& e)
{
	int n = dao->GetNodeNum();
	vector<vector<int >> v;
	for (int i = 0; i < n; i++)
	{
		vector<int > row(n);
		v.push_back(row);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (dao->GetTime(i, j) != -1)
			{
				v[i][j] = 1;
			}
		}
	}
	vector<vector<int >> v2(v);
	int count = 0;
	while (!v[dao->GetNodeIdx(s)][dao->GetNodeIdx(e)] || !v[dao->GetNodeIdx(s)][dao->GetNodeIdx(e)])
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++)
					v2[i][j] += v[i][k] * v2[k][j];
			}
	}
	count++;
	return count;
}

int Graph::Check()
{
	if (dao->GetNodeNum() < 50)
	{
		return 1;
	}
	return 0;
}

QStringList Graph::GetNodes()
{
	vector<wstring> nodes = dao->GetNodeList();
	QStringList list;
	for (wstring& n : nodes)
	{
		QString str = QString::fromStdWString(n);
		list.append(str);
	}
	return list;
}

QStringList Graph::GetLines()
{
	vector<wstring> lines = dao->GetLines();
	QStringList list;
	for (wstring& n : lines)
	{
		QString str = QString::fromStdWString(n);
		list.append(str);
	}
	return list;
}

QStringList Graph::GetPath(const wstring& s, const wstring& e, QString str)
{
	if (dao->GetNodeIdx(s) == -1 || dao->GetNodeIdx(e) == -1)
		return QStringList();
	if (str == "路程最短")
	{
		return Shortest(s, e);
	}
	else if (str == "用时最短")
	{
		return Fastest(s, e);
	}
	else
	{
		return LeastTrans(s, e);
	}
}

bool Graph::Recover()
{
	if (dao->GetDefault())
	{
		dao->ResetNode();
		dao->RewriteEdge();
		return true;
	}
	return false;
}

int Graph::dfs(int maxtrans, int trans, int* visited, int cur, int pre, int e)
{
	if (trans == maxtrans)
	{
		visited[cur] = -1;
		return false;
	}
	if (cur == e)
	{
		return true;
	}
	int n = dao->GetNodeNum();
	for (int i = 0; i < n; i++)
	{
		if (visited[i] == -1 && dao->GetDis(cur, i) != MAXNUM)
		{
			visited[i] = cur;
			if (dao->GetCurLine(pre, cur) == dao->GetCurLine(cur, i))
			{
				if (dfs(maxtrans, trans, visited, i, cur, e))
					return true;
			}
			else
			{
				if ((dao->GetSecondLine(pre, cur) == L"") && (dao->GetSecondLine(cur, i) == L""))
				{
					if (dfs(maxtrans, trans + 1, visited, i, cur, e))
						return true;
				}
				else if ((dao->GetSecondLine(pre, cur) != L"") && (dao->GetSecondLine(cur, i) != L""))
				{
					if ((dao->GetSecondLine(pre, cur) == dao->GetCurLine(cur, i)) ||
						(dao->GetSecondLine(pre, cur) == dao->GetSecondLine(cur, i)) ||
						(dao->GetCurLine(pre, cur) == dao->GetSecondLine(cur, i)))
					{
						if (dfs(maxtrans, trans, visited, i, cur, e))
							return true;
					}
					else
					{
						if (dfs(maxtrans, trans + 1, visited, i, cur, e))
							return true;
					}
				}
				else if ((dao->GetSecondLine(pre, cur) != L""))
				{
					if (dao->GetSecondLine(pre, cur) == dao->GetCurLine(cur, i))
					{
						if (dfs(maxtrans, trans, visited, i, cur, e))
							return true;
					}
					else
					{
						if (dfs(maxtrans, trans + 1, visited, i, cur, e))
							return true;
					}
				}
				else if ((dao->GetSecondLine(cur, i) != L""))
				{
					if (dao->GetSecondLine(cur, i) == dao->GetCurLine(pre, cur))
					{
						if (dfs(maxtrans, trans, visited, i, cur, e))
							return true;
					}
					else
					{
						if (dfs(maxtrans, trans + 1, visited, i, cur, e))
							return true;
					}
				}
			}
		}
	}
	visited[cur] = -1;
	return false;
}