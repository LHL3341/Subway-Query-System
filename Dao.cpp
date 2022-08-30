#include "Dao.h"

Dao::~Dao()
{
}

bool Dao::ResetNode()
{
	{
		wofstream fout;
		fout.imbue(locale("zh_CN"));
		fout.open(filepath + L"Nodes.txt", ios::out);
		if (!fout.is_open())
		{
			std::cerr << "cannot open the file";
			return false;
		}
		int n = node.size();
		for (auto& n : node)
			fout << n.first << endl;
		fout << endl;
		fout.close();
		return true;
	}
}

bool Dao::SetEdge(const Edge& e)
{
	if (GetSecondLine(e.start,e.end)!=L"")
		return false;
	wofstream fout;
	fout.imbue(locale("zh_CN"));
	fout.open(filepath + L"Edges.txt", ios::out | ios::app);
	if (!fout.is_open())
	{
		std::cerr << "cannot open the file";
		return false;
	}
	char arr[10];
	itoa(e.dis, arr, 10);
	fout << e.start << L"," << e.end << L"," << e.line << L"," << arr << endl;
	fout.close();
	return true;
}

int Dao::GetNode()
{
	wstring path = filepath + L"Nodes.txt";
	if (_waccess(path.c_str(), 0) == -1)
	{
		wofstream fout;
		fout.open(path, ios::out);
		fout.close();
		return 101;
	}
	wifstream fin;
	fin.open(path, ios::in);
	fin.imbue(locale("zh_CN"));
	if (!fin.is_open())
	{
		return 0;
	}
	node.clear();
	int i = 0;
	while (!fin.eof())
	{
		wchar_t s[20];
		fin.getline(s, 20);
		wstring str(s);
		if (!node.count(str) && str != L"")
		{
			node[str] = i;
			i++;
		}
	}
	fin.close();
	return 1;
}

int Dao::GetEdge()
{
	wstring path = filepath + L"Edges.txt";
	if (_waccess(path.c_str(), 0) == -1)
	{
		wofstream fout;
		fout.open(path, ios::out);
		fout.close();
		return 101;
	}
	wifstream fin;
	fin.open(path, ios::in);
	fin.imbue(locale("zh_CN"));
	if (!fin.is_open())
	{
		std::cerr << "cannot open the file";
		return 0;
	}
	adj.clear();
	int len = node.size();
	adj.resize(len);
	for (auto& each : adj)
	{
		each.resize(len);
	}

	while (!fin.eof())
	{
		wchar_t str[100];
		fin.getline(str, 100);
		wstring s(str);
		wstring start = s.substr(0, s.find_first_of(L","));
		s = s.substr(s.find_first_of(L",") + 1, s.size());
		wstring end = s.substr(0, s.find_first_of(L","));
		s = s.substr(s.find_first_of(L",") + 1, s.size());
		wstring line = s.substr(0, s.find_first_of(L","));
		wstring dis = s.substr(s.find_first_of(L",") + 1, s.size());
		for (int i = 0; i < dis.size(); i++)
		{
			if (dis[i] == L',')
				dis.erase(i);
		}
		int distance = _wtoi(dis.c_str());
		if (node.count(start) && node.count(end) && Lines.count(line))
		{
			wstring curline = GetCurLine(start, end);
			if (curline == L"")
			{

			adj[node[start]][node[end]] = Adj(distance, line,L"");
			adj[node[end]][node[start]] = Adj(distance, line,L"");
			}
			else if (curline != L"" && GetSecondLine(start, end) != L"")
			{
				;
			}
			else
			{
				if (curline != line)
				{
					adj[node[start]][node[end]] = Adj(distance, curline, line);
					adj[node[end]][node[start]] = Adj(distance, curline,line);
				}
			}
		}
		else if (start == L"" || end == L"" || distance == 0 || line == L"")
		{
		}
		else
		{
		}
	}
	fin.close();
	return 1;
}

void Dao::RewriteEdge()
{
	wofstream fout;
	fout.imbue(locale("zh_CN"));
	fout.open(filepath + L"Edges.txt", ios::out);
	if (!fout.is_open())
	{
		std::cerr << "cannot open the file";
		return;
	}

	int n = adj.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			if (adj[i][j].dis != MAXNUM && adj[i][j].line != L"")
			{
				char arr[10];
				itoa(adj[i][j].dis, arr, 10);
				fout << FindNodebyIdx(i) << L"," << FindNodebyIdx(j) << L","
					<< adj[i][j].line << L"," << arr << endl;
				if (adj[i][j].line2 != L"")
				{
					char arr2[10];
					itoa(adj[i][j].dis, arr2, 10);
					fout << FindNodebyIdx(i) << L"," << FindNodebyIdx(j) << L","
						<< adj[i][j].line2 << L"," << arr2 << endl;
				}
			}
		}
	}
	fout.close();
}

bool Dao::UpdateNode(const wstring& str)
{
	if (node.count(str))
		return false;
	int n = node.size();
	node[str] = n;
	for (auto& each : adj)
	{
		Adj a;
		each.push_back(a);
	}
	vector<Adj> v;
	v.resize(n + 1);
	adj.push_back(v);
	return ResetNode();
}

bool Dao::UpdateEdge(const Edge& e)
{
	if (adj[node[e.end]][node[e.start]].line2 != L"" && adj[node[e.end]][node[e.start]].line != L"")
		return false;
	if (adj[node[e.end]][node[e.start]].line == e.line)
		return false;
	bool flag;
	if (flag = SetEdge(e))
	{

		if (adj[node[e.end]][node[e.start]].line == L"")
		{
			adj[node[e.start]][node[e.end]] = Adj(e.dis, e.line,L"");
			adj[node[e.end]][node[e.start]] = Adj(e.dis, e.line,L"");
		}
		else
		{
			adj[node[e.start]][node[e.end]].line2 = e.line;
			adj[node[e.end]][node[e.start]].line2 = e.line;
		}
	}
	return flag;
}

void Dao::DelNode(const wstring& str)
{
	node.erase(str);
	ResetNode();
	GetNode();
	GetEdge();
}

void Dao::DelEdge(const wstring& s, const wstring& e)
{
	adj[node[s]][node[e]] = Adj(MAXNUM, L"",L"");
	adj[node[e]][node[s]] = Adj(MAXNUM, L"",L"");
	RewriteEdge();
}

int Dao::GetNodeNum()
{
	return node.size();
}

vector<wstring> Dao::GetNodeList()
{
	vector<wstring> v;
	for (auto& each : node)
	{
		v.push_back(each.first);
	}
	return v;
}

int Dao::GetDis(const int i, const int j)
{
	return adj[i][j].dis;
}

double Dao::GetTime(const int i, const int j)
{
	if (!Lines.count(adj[i][j].line))
		return MAXNUM;
	double time = double(adj[i][j].dis) /double(Lines[adj[i][j].line]) * 0.06;
	return time;
}

int Dao::GetNodeIdx(const wstring& str)
{
	if (!node.count(str))
		return -1;
	return node[str];
}

wstring Dao::FindNodebyIdx(const int idx)
{
	for (auto& each : node)
	{
		if (each.second == idx)
			return each.first;
	}
	return L"";
}


wstring Dao::GetCurLine(const int s, const int e)
{
	return adj[s][e].line;
}
wstring Dao::GetCurLine(const wstring& i, const wstring& j)
{
	return adj[node[i]][node[j]].line;
}

vector<wstring> Dao::GetLines()
{
	vector<wstring> v;
	for (auto& line : Lines)
	{
		v.push_back(line.first);
	}
	return v;
}

bool Dao::GetDefault()
{
	wstring path = defaultfilepath + L"Nodes.txt";
	if (_waccess(path.c_str(), 0) == -1)
	{
		return false;
	}
	wifstream fin;
	fin.open(path, ios::in);
	fin.imbue(locale("zh_CN"));
	if (!fin.is_open())
	{
		return false;
	}
	node.clear();
	int i = 0;
	while (!fin.eof())
	{
		wchar_t s[20];
		fin.getline(s, 20);
		wstring str(s);
		if (!node.count(str) && str != L"")
		{
			node[str] = i;
			i++;
		}
	}
	fin.close();
	path = defaultfilepath + L"Edges.txt";
	if (_waccess(path.c_str(), 0) == -1)
	{
		return false;
	}
	fin.open(path, ios::in);
	fin.imbue(locale("zh_CN"));
	if (!fin.is_open())
	{
		std::cerr << "cannot open the file";
		return 0;
	}
	adj.clear();
int len = node.size();
	adj.resize(len);
	for (auto& each : adj)
	{
		each.resize(len);
	}

	while (!fin.eof())
	{
		wchar_t str[100];
		fin.getline(str, 100);
		wstring s(str);
		wstring start = s.substr(0, s.find_first_of(L","));
		s = s.substr(s.find_first_of(L",") + 1, s.size());
		wstring end = s.substr(0, s.find_first_of(L","));
		s = s.substr(s.find_first_of(L",") + 1, s.size());
		wstring line = s.substr(0, s.find_first_of(L","));
		wstring dis = s.substr(s.find_first_of(L",") + 1, s.size());
		for (int i = 0; i < dis.size(); i++)
		{
			if (dis[i] == L',')
				dis.erase(i);
		}
		int distance = _wtoi(dis.c_str());
		if (node.count(start) && node.count(end) && Lines.count(line))
		{
			wstring curline = GetCurLine(start, end);
			if (curline == L"")
			{

			adj[node[start]][node[end]] = Adj(distance, line,L"");
			adj[node[end]][node[start]] = Adj(distance, line,L"");
			}
			else if (curline != L"" && GetSecondLine(start, end) != L"")
			{
				;
			}
			else
			{
				if (curline != line)
				{
					adj[node[start]][node[end]].line2 = line;
					adj[node[end]][node[start]].line2 = line;
				}
			}
		}
		else if (start == L"" || end == L"" || distance == 0 || line == L"")
		{
		}
		else
		{
		}
	}
	fin.close();
	return 1;
}

wstring Dao::GetSecondLine(const wstring& s, const wstring& e)
{
	return adj[node[s]][node[e]].line2;
}

wstring Dao::GetSecondLine(const int i, const int j)
{
	return adj[i][j].line2;
}
