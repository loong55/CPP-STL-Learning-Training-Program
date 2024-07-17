#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include<map>
#include<speeker.h>
#include<algorithm>
#include<deque>
#include<numeric>
#include<functional>
#include<fstream>

//演讲管理类
class SpeechManager
{
public:	
	SpeechManager();//构造函数 

    void show_Menu();//展示菜单
	
    void exitSystem();//退出系统
    
	vector<int>v1;//比赛选手 容器  12人，保存选手编号	
	vector<int>v2;//第一轮晋级容器  6人，保存选手编号	
	vector<int>vVictory;//胜利前三名容器  3人，保存选手编号
	
	map<int, Speaker> m_Speaker;//具体选手 key代表编号，value记录姓名、分数;
    
	void initSpeech();//初始化属性
    
	int m_Index;//比赛轮数

	void createSpeaker();//初始化创建12名选手
    
	void startSpeech();//开始比赛 - 比赛流程控制
    
		void speechDraw();//抽签    
		void speechContest();//比赛	
		void showScore();//显示比赛结果
	
	void saveRecord();//保存记录
	
	void loadRecord();//读取记录
	
	bool fileIsEmpty;//文件为空的标志
	
	map<int, vector<string>> m_Record;//往届记录，key代表第几届，value记录具体的信息
	
	void showRecord();//显示往届得分
	
	void clearRecord();//清空记录	
	~SpeechManager();//析构函数
};