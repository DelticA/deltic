#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>
#include <cmath>

#define GRIDSIZE 8
#define OBSTACLE 2
#define judge_black 0
#define judge_white 1
#define grid_black 1
#define grid_white -1
#define MaxNum 3000
#define MinNum -3000
#define FirstAdv 0.17

using namespace std;

int currBotColor; 
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; 
int DeltaX[] = { -1,-1,-1,0,0,1,1,1 };
int DeltaY[] = { -1,0,1,-1,1,-1,0,1 };
int mx[4], my[4], yx[4], yy[4];
int turnID, EndDepth;
int AnsX1, AnsX2, AnsX3, AnsY1, AnsY2, AnsY3;
int MyKingMove[8][8], YourKingMove[8][8], MyQueenMove[8][8], YourQueenMove[8][8], mm;
int SearchMem[8][8];
int SerialX[65], SerialY[65], SDepth[65], SearchStart, SearchEnd;

inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE) 
		return false;
	return true;
}

void GenerateAmazon()
{
	gridInfo[0][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[(GRIDSIZE - 1) / 3][0] = grid_black;
	gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0] = grid_black;
	gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;
	gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1] = grid_white;
	gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1] = grid_white;
	gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;
	cin >> turnID;
	if (turnID < 12)
		EndDepth = 1;
	else if (turnID < 18)
		EndDepth = 2;
	else if (turnID < 24)
		EndDepth = 3;
	else
		EndDepth = 4;
	int x1, y1, x2, y2, x3, y3;
	cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
	if (x1 == -1)
	{
		currBotColor = grid_black;
		if (turnID == 1)
		{
			cout << "5 0 5 6 2 3";//"5 0 5 3 2 3"  "5 0 5 6 2 3" "5 0 5 5 5 3" "5 0 5 5 2 5"
			exit(0);
		}
	}
	else
	{
		currBotColor = grid_white;
		gridInfo[x2][y2] = gridInfo[x1][y1];
		gridInfo[x1][y1] = 0;
		gridInfo[x3][y3] = OBSTACLE;
	}
	for (int i = 1; i <= turnID * 2 - 2; i++)
	{
		cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
		gridInfo[x2][y2] = gridInfo[x1][y1];
		gridInfo[x1][y1] = 0;
		gridInfo[x3][y3] = OBSTACLE;
	}
	int p1 = 0, p2 = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (gridInfo[i][j] == currBotColor)
			{
				mx[p1] = i;
				my[p1] = j;
				p1++;
			}
			if (gridInfo[i][j] == -currBotColor)
			{
				yx[p2] = i;
				yy[p2] = j;
				p2++;
			}
		}
}

double GetValue()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			MyKingMove[i][j] = MaxNum;
			YourKingMove[i][j] = MaxNum;
			MyQueenMove[i][j] = MaxNum;
			YourQueenMove[i][j] = MaxNum;
		}
	double t1 = 0, t2 = 0, p1 = 0, p2 = 0, m = 0;
	for (int z = 0; z < 4; z++)
	{
		memset(SearchMem, 0, sizeof(SearchMem));
		SearchStart = 0;
		SearchEnd = 1;
		SerialX[0] = mx[z];
		SerialY[0] = my[z];
		SDepth[0] = 0;
		SearchMem[mx[z]][my[z]] = 1;
		while (SearchStart != SearchEnd)
		{
			int x = SerialX[SearchStart];
			int y = SerialY[SearchStart];
			int Depth = SDepth[SearchStart];
			MyKingMove[x][y] = min(MyKingMove[x][y], Depth);
			for (int i = 0; i < 8; i++)
			{
				if (!inMap(x + DeltaX[i], y + DeltaY[i]))
					continue; 
				if (SearchMem[x + DeltaX[i]][y + DeltaY[i]])
					continue;
				if (gridInfo[x + DeltaX[i]][y + DeltaY[i]])
					continue;
				SerialX[SearchEnd] = x + DeltaX[i];
				SerialY[SearchEnd] = y + DeltaY[i];
				SDepth[SearchEnd] = Depth + 1;
				SearchMem[x + DeltaX[i]][y + DeltaY[i]] = 1;
				SearchEnd++;
			}
			SearchStart++;
		}
		MyKingMove[mx[z]][my[z]] = MaxNum;
	}
	for (int z = 0; z < 4; z++)
	{
		memset(SearchMem, 0, sizeof(SearchMem));
		SearchStart = 0;
		SearchEnd = 1;
		SerialX[0] = yx[z];
		SerialY[0] = yy[z];
		SDepth[0] = 0;
		SearchMem[yx[z]][yy[z]] = 1;
		while (SearchStart != SearchEnd)
		{
			int x = SerialX[SearchStart];
			int y = SerialY[SearchStart];
			int Depth = SDepth[SearchStart];
			YourKingMove[x][y] = min(YourKingMove[x][y], Depth);
			for (int i = 0; i < 8; i++)
			{
				if (!inMap(x + DeltaX[i], y + DeltaY[i]))
					continue;
				if (SearchMem[x + DeltaX[i]][y + DeltaY[i]])
					continue;
				if (gridInfo[x + DeltaX[i]][y + DeltaY[i]])
					continue;
				SerialX[SearchEnd] = x + DeltaX[i];
				SerialY[SearchEnd] = y + DeltaY[i];
				SDepth[SearchEnd] = Depth + 1;
				SearchMem[x + DeltaX[i]][y + DeltaY[i]] = 1;
				SearchEnd++;
			}
			SearchStart++;
		}
		YourKingMove[yx[z]][yy[z]] = MaxNum;
	}
	for (int z = 0; z < 4; z++)
	{
		memset(SearchMem, 0, sizeof(SearchMem));
		SearchStart = 0;
		SearchEnd = 1;
		SerialX[0] = mx[z];
		SerialY[0] = my[z];
		SDepth[0] = 0;
		SearchMem[mx[z]][my[z]] = 1;
		while (SearchStart != SearchEnd)
		{
			int x = SerialX[SearchStart];
			int y = SerialY[SearchStart];
			int Depth = SDepth[SearchStart];
			MyQueenMove[x][y] = min(MyQueenMove[x][y], Depth);
			for (int i = 0; i < 8; i++)
			{
				for (int p = 1;; p++)
				{
					if (!inMap(x + p * DeltaX[i], y + p * DeltaY[i]))
						break;
					if (SearchMem[x + p * DeltaX[i]][y + p * DeltaY[i]])
						continue;
					if (gridInfo[x + p * DeltaX[i]][y + p * DeltaY[i]])
						break;
					SerialX[SearchEnd] = x + p * DeltaX[i];
					SerialY[SearchEnd] = y + p * DeltaY[i];
					SDepth[SearchEnd] = Depth + 1;
					SearchMem[x + p * DeltaX[i]][y + p * DeltaY[i]] = 1;
					SearchEnd++;
				}
			}
			SearchStart++;
		}
		MyQueenMove[mx[z]][my[z]] = MaxNum;
	}
	for (int z = 0; z < 4; z++)
	{
		memset(SearchMem, 0, sizeof(SearchMem));
		SearchStart = 0;
		SearchEnd = 1;
		SerialX[0] = yx[z];
		SerialY[0] = yy[z];
		SDepth[0] = 0;
		SearchMem[yx[z]][yy[z]] = 1;
		while (SearchStart != SearchEnd)
		{
			int x = SerialX[SearchStart];
			int y = SerialY[SearchStart];
			int Depth = SDepth[SearchStart];
			YourQueenMove[x][y] = min(YourQueenMove[x][y], Depth);
			for (int i = 0; i < 8; i++)
			{
				for (int p = 1;; p++)
				{
					if (!inMap(x + p * DeltaX[i], y + p * DeltaY[i]))
						break;
					if (SearchMem[x + p * DeltaX[i]][y + p * DeltaY[i]])
						continue;
					if (gridInfo[x + p * DeltaX[i]][y + p * DeltaY[i]])
						break;
					SerialX[SearchEnd] = x + p * DeltaX[i];
					SerialY[SearchEnd] = y + p * DeltaY[i];
					SDepth[SearchEnd] = Depth + 1;
					SearchMem[x + p * DeltaX[i]][y + p * DeltaY[i]] = 1;
					SearchEnd++;
				}
			}
			SearchStart++;
		}
		YourQueenMove[yx[z]][yy[z]] = MaxNum;
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (MyQueenMove[j][i] > 1)
				continue;
			mm = 0;
			for (int p = 0; p < 8; p++)
			{
				if (!inMap(i + DeltaX[p], j + DeltaY[p]))
					continue;
				if (gridInfo[i + DeltaX[p]][j + DeltaY[p]])
					continue;
				mm++;
			}
			m += mm / MyKingMove[i][j];
		}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			if (MyKingMove[i][j] > YourKingMove[i][j])
				t1 += -1;
			else if (MyKingMove[i][j] < YourKingMove[i][j])
				t1 += 1;
			else if (MyKingMove[i][j] == MaxNum)
				t1 += 0;
			else
				t1 += FirstAdv;

			if (MyQueenMove[i][j] > YourQueenMove[i][j])
				t2 += -1;
			else if (MyQueenMove[i][j] < YourQueenMove[i][j])
				t2 += 1;
			else if (MyQueenMove[i][j] == MaxNum)
				t2 += 0;
			else
				t2 += FirstAdv;
			p1 += 2.0 * (pow(2.0, -1 * (double)MyQueenMove[i][j]) - pow(2.0, -1 * (double)YourQueenMove[i][j]));
			p2 += min(1.0, max(-1.0, ((double)YourKingMove[i][j] - (double)MyKingMove[i][j]) / 6.0));
		}

	double evaluation;
	if (turnID <= 4) evaluation = 0.15 * t1 + 0.50 * t2 + 0.10 * p1 + 0.10 * p2 + 0.30 * m;
	else if (turnID <= 7) evaluation = 0.24 * t1 + 0.47 * t2 + 0.13 * p1 + 0.13 * p2 + 0.20 * m;
	else if (turnID <= 16) evaluation = 0.30 * t1 + 0.25 * t2 + 0.30 * p1 + 0.30 * p2 + 0.05 * m;
	else evaluation = 0.8 * t1 + 0.1 * t2 + 0.1 * p1 + 0.1 * p2;
	return evaluation;
}

double MoveSearch(int Depth, double lastMaxMin)
{
	if (Depth == EndDepth)
		return GetValue();
	double MaxMin;
	if (Depth % 2 == 0)
	{
		MaxMin = MinNum;
		int x1, y1, x2, y2, x3, y3;
		for (int z = 0; z <= 3; z++)
		{
			x1 = mx[z];
			y1 = my[z];
			for (int p = 0; p < 8; p++)
			{
				for (int i = 1;i<=8; i++)
				{
					if (!inMap(x1 + i * DeltaX[p], y1 + i * DeltaY[p]))
						break;
					if (gridInfo[x1 + i * DeltaX[p]][y1 + i * DeltaY[p]])
						break;
					x2 = x1 + i * DeltaX[p];
					y2 = y1 + i * DeltaY[p];
					mx[z] = x2;
					my[z] = y2;
					gridInfo[x1][y1] = 0;
					gridInfo[x2][y2] = currBotColor;
					for (int p = 0; p < 8; p++)
					{
						for (int i = 1;; i++)
						{
							if (!inMap(x2 + i * DeltaX[p], y2 + i * DeltaY[p]))
								break;
							if (gridInfo[x2 + i * DeltaX[p]][y2 + i * DeltaY[p]])
								break;
							x3 = x2 + i * DeltaX[p];
							y3 = y2 + i * DeltaY[p];
							gridInfo[x3][y3] = OBSTACLE;
							double newval = MoveSearch(Depth + 1, MaxMin);
							gridInfo[x3][y3] = 0;
							if (Depth != 0)
								MaxMin = max(newval, MaxMin);
							else if (newval > MaxMin)
							{
								MaxMin = newval;
								AnsX1 = x1;
								AnsX2 = x2;
								AnsX3 = x3;
								AnsY1 = y1;
								AnsY2 = y2;
								AnsY3 = y3;
							}
							if (MaxMin >= lastMaxMin)
							{
								gridInfo[x1][y1] = currBotColor;
								gridInfo[x2][y2] = 0;
								mx[z] = x1;
								my[z] = y1;
								return MaxMin;
							}
						}
					}
					gridInfo[x1][y1] = currBotColor;
					gridInfo[x2][y2] = 0;
					mx[z] = x1;
					my[z] = y1;
				}
			}
		}
		if (MaxMin == MinNum)
			return GetValue();
	}
	else
	{
		MaxMin = MaxNum;
		int x1, y1, x2, y2, x3, y3;
		for (int z = 0; z <= 3; z++)
		{
			x1 = yx[z];
			y1 = yy[z];
			for (int p = 0; p < 8; p++)
			{
				for (int i = 1;; i++)
				{
					if (!inMap(x1 + i * DeltaX[p], y1 + i * DeltaY[p]))
						break;
					if (gridInfo[x1 + i * DeltaX[p]][y1 + i * DeltaY[p]])
						break;
					x2 = x1 + i * DeltaX[p];
					y2 = y1 + i * DeltaY[p];
					yx[z] = x2;
					yy[z] = y2;
					gridInfo[x1][y1] = 0;
					gridInfo[x2][y2] = -currBotColor;
					for (int p = 0; p < 8; p++)
					{
						for (int i = 1;; i++)
						{
							if (!inMap(x2 + i * DeltaX[p], y2 + i * DeltaY[p]))
								break;
							if (gridInfo[x2 + i * DeltaX[p]][y2 + i * DeltaY[p]])
								break;
							x3 = x2 + i * DeltaX[p];
							y3 = y2 + i * DeltaY[p];
							gridInfo[x3][y3] = OBSTACLE;
							double newval = MoveSearch(Depth + 1, MaxMin);
							gridInfo[x3][y3] = 0;
							MaxMin = min(newval, MaxMin);
							if (MaxMin <= lastMaxMin)
							{
								gridInfo[x1][y1] = -currBotColor;
								gridInfo[x2][y2] = 0;
								yx[z] = x1;
								yy[z] = y1;
								return MaxMin;
							}
						}
					}
					gridInfo[x1][y1] = -currBotColor;
					gridInfo[x2][y2] = 0;
					yx[z] = x1;
					yy[z] = y1;
				}
			}
		}
		if (MaxMin == MaxNum)
			return GetValue();
	}
	return MaxMin;
}

int main()
{
	GenerateAmazon();
	MoveSearch(0, MaxNum);
	cout << AnsX1 << " " << AnsY1 << " " << AnsX2 << " " << AnsY2 << " " << AnsX3 << " " << AnsY3 << endl;
	return 0;
}
