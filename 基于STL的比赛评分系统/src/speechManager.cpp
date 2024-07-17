#include "speechManager.h"

SpeechManager::SpeechManager()
{
    //初始化属性
	this->initSpeech();
    
	//创建选手
	this->createSpeaker();

	//获取往届记录
	this->loadRecord();
}

void SpeechManager::show_Menu()
{
	cout << "********************************************" << endl;
	cout << "*************  欢迎参加演讲比赛 ************" << endl;
	cout << "*************  1.开始演讲比赛  *************" << endl;
	cout << "*************  2.查看往届记录  *************" << endl;
	cout << "*************  3.清空比赛记录  *************" << endl;
	cout << "*************  0.退出比赛程序  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

void SpeechManager::exitSystem()
{
	cout << "欢迎下次使用" << endl;
	cout << "按任意键继续..." << endl;
    cin.ignore(); // 清除输入缓冲区
    cin.get(); // 等待用户输入
	exit(0);
}

void SpeechManager::initSpeech()
{
	//容器保证为空
	this->v1.clear();  
	this->v2.clear();
	this->vVictory.clear();
	this->m_Speaker.clear();
	//初始化比赛轮数
	this->m_Index = 1;
	//初始化记录容器
	this->m_Record.clear();
}

void SpeechManager::createSpeaker()
{
	string nameSeed = "ABCDEFGHIJKL";//姓名种子：12个字母
	for (int i = 0; i < nameSeed.size(); i++)
	{
		string name = "选手";
		name += nameSeed[i];

		Speaker sp;
		sp.m_Name = name;
		for (int i = 0; i < 2; i++)
		{
			sp.m_Score[i] = 0;
		}

		//12名选手编号
		this->v1.push_back(i + 10001);

		//选手编号 以及对应的选手 存放到map容器中
		this->m_Speaker.insert(make_pair(i + 10001, sp));
	}
}

//开始比赛-比赛流程
void SpeechManager::startSpeech()
{
	//第一轮比赛
	//1、抽签
    speechDraw();	
	//2、比赛
    speechContest();
	//3、显示晋级结果
	showScore();


	//第二轮比赛
	this->m_Index++;
	//1、抽签
	speechDraw();
	//2、比赛
    speechContest();
	//3、显示最终结果
	showScore();
	//4、保存分数
	saveRecord();

	//重置比赛，获取记录
	//初始化属性
	this->initSpeech();    
	//创建选手
	this->createSpeaker();
	//获取往届记录
	this->loadRecord();

	cout << "本届比赛完毕！" << endl;
	cout << "按任意键继续..." << endl;     
    cin.ignore(); // 清除输入缓冲区     
    cin.get(); // 等待用户输入
	system("clear");
}

void SpeechManager::speechDraw()
{
	cout << "第 << " << this->m_Index << " >> 轮比赛选手正在抽签"<<endl;
	cout << "---------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;
	if (this->m_Index == 1)
	{
		random_shuffle(v1.begin(), v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	else
	{
		random_shuffle(v2.begin(), v2.end());
		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << "---------------------" << endl;
	cout << "按任意键继续..." << endl;     
    cin.ignore(); // 清除输入缓冲区    
    cin.get(); // 等待用户输入
	cout << endl;
}

void SpeechManager::speechContest()
{
	cout << "------------- 第"<< this->m_Index << "轮正式比赛开始：------------- " << endl;

	multimap<double, int, greater<int>> groupScore; //临时6人小组容器，保存key分数 value 选手编号
	//greater<int>关系仿函数作谓词，需要#include <functional>

	int num = 0; //记录人员数，6个为1组

	vector <int>v_Src;   //比赛的人员容器，记录本轮比赛选手编号
	if (this->m_Index == 1)
	{
		v_Src = v1;//第一轮
	}
	else
	{
		v_Src = v2;//第二轮
	}

	//遍历所有参赛选手
	for (vector<int>::iterator it = v_Src.begin(); it != v_Src.end(); it++)
	{
		num++;

		//10个评委打分
		deque<double>d;//双端数组容器
		for (int i = 0; i < 10; i++)
		{
			double score = (rand() % 401 + 600) / 10.f;  // 600 ~ 1000,.f代表小数
			//cout << score << " ";
			d.push_back(score);
		}

		sort(d.begin(), d.end(), greater<double>());				//排序，#include<algorithm>
		d.pop_front();												//去掉最高分
		d.pop_back();												//去掉最低分

		double sum = accumulate(d.begin(), d.end(), 0.0f);			//获取总分,#include<numeric>
		double avg = sum / (double)d.size();						//获取平均分

		//每个人平均分
		//cout << "编号： " << *it  << " 选手： " << this->m_Speaker[*it].m_Name << " 获取平均分为： " << avg << endl;  //打印分数
		this->m_Speaker[*it].m_Score[this->m_Index - 1] = avg;
		
		//6个人一组，用临时容器保存
		groupScore.insert(make_pair(avg, *it));
		if (num % 6 == 0)//每六人打完分，排一次名
		{
			cout << "第" << num / 6 << "小组比赛名次：" << endl;
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin(); it != groupScore.end(); it++)
			{
				cout << "编号: " << it->second << " 姓名： " << this->m_Speaker[it->second].m_Name << " 成绩： " << this->m_Speaker[it->second].m_Score[this->m_Index - 1] << endl;
			}

			int count = 0;
			//每六人，取前三名
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin(); it != groupScore.end() && count < 3; it++, count++)
			{
				if (this->m_Index == 1)
				{
					v2.push_back((*it).second);//第一轮比赛前三名编号，插入v2容器
				}
				else
				{
					vVictory.push_back((*it).second);//第二轮前三名编号，插入vVictory容器
				}
			}

			groupScore.clear();
			cout << endl;
		}
	}
	cout << "------------- 第" << this->m_Index << "轮比赛完毕  ------------- " << endl;
	cout << "按任意键继续..." << endl;     
    cin.ignore(); // 清除输入缓冲区     
    cin.get(); // 等待用户输入
}

void SpeechManager::showScore()
{
	cout << "---------第" << this->m_Index << "轮晋级选手信息如下：-----------" << endl;
	vector<int>v;
	if (this->m_Index == 1)
	{
		v = v2;
	}
	else
	{
		v = vVictory;
	}

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << "选手编号：" << *it << " 姓名： " << m_Speaker[*it].m_Name << " 得分： " << m_Speaker[*it].m_Score[this->m_Index - 1] << endl;
	}
	cout << endl;

	cout << "按任意键继续..." << endl;     
	cin.ignore(); // 清除输入缓冲区    
    cin.get(); // 等待用户输入
	system("clear");
	this->show_Menu(); 
}

void SpeechManager::saveRecord()
{
	ofstream ofs;//#include<fstream>
	ofs.open("speech.csv", ios::out | ios::app); // 用输出的方式打开文件  -- 写文件| 追加写

	//将每个人数据写入到文件中
	for (vector<int>::iterator it = vVictory.begin(); it != vVictory.end(); it++)
	{
		ofs << *it << "," << m_Speaker[*it].m_Score[1] << ",";
	}
	ofs << endl;
    
	//关闭文件
	ofs.close();
    
	cout << "记录已经保存" << endl;
	//有记录了，文件不为空
	this->fileIsEmpty=false;
}

void SpeechManager::loadRecord()
{
	ifstream ifs("speech.csv", ios::in); //输入流对象 读取文件

	if (!ifs.is_open())
	{
		this->fileIsEmpty = true;
		cout << "文件不存在！" << endl;
		ifs.close();
		return;
	}

	char ch;
	ifs >> ch;//流文件读取的第一个字符，存入ch，后面流指向下一个字符
	if (ifs.eof())//此时流如果指向文件尾部
	{
		cout << "文件为空!" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	//文件不为空
	this->fileIsEmpty = false;

	ifs.putback(ch); //流重新指向第一个字符

	string data;
	int index = 0;
	while (ifs >> data)//按行读取数据
	{
		//cout << data << endl;
		vector<string>v;

		int pos = -1;
		int start = 0;

		while (true)
		{
			pos = data.find(",", start); //从0开始查找 ','
			if (pos == -1)
			{
				break; //找不到break返回
			}
			string tmp = data.substr(start, pos - start); //找到了,进行分割 参数1 起始位置，参数2 截取长度
			v.push_back(tmp);//数据分段存入
			start = pos + 1;
		}

		this->m_Record.insert(make_pair(index, v));//key值第几届，velue值存入选手编号和分数
		index++;
	}
	ifs.close();
}

void SpeechManager::showRecord()//显示记录
{
	if(this->fileIsEmpty)
	{
		cout << "文件不存在或者记录为空！" << endl;
	}
	for (int i = 0; i < this->m_Record.size(); i++)
	{
		cout << "第" << i + 1 << "届 " <<
			"冠军编号：" << this->m_Record[i][0] << " 得分：" << this->m_Record[i][1] << " "
			"亚军编号：" << this->m_Record[i][2] << " 得分：" << this->m_Record[i][3] << " "
			"季军编号：" << this->m_Record[i][4] << " 得分：" << this->m_Record[i][5] << endl;
	}
	/*
	this->m_Record 是一个 map 容器，其键是 int 类型，值是 vector<string> 类型。
	这意味着对于每一个键（代表第几届），都有一个 vector<string> 作为值。
	map 容器本身只能使用一个键来访问对应的 vector<string>。
	一旦你通过键访问到了 vector<string>，你就可以使用单个中括号 [] 来访问该 vector 中的元素。
	第一个中括号填入键值，得到对应的vector<string>，第二个中括号
	*/
    cout << "按任意键继续..." << endl;     
	cin.ignore(); // 清除输入缓冲区     
	cin.get(); // 等待用户输入
	system("clear");
}

void SpeechManager::clearRecord()//删除记录
{
	cout << "确认清空？" << endl;
	cout << "1、确认" << endl;
	cout << "2、返回" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
		//打开模式 ios::trunc 如果存在删除文件并重新创建
		ofstream ofs("speech.csv", ios::trunc);
		ofs.close();

		//初始化属性
		this->initSpeech();
		//创建选手
		this->createSpeaker();
		//获取往届记录
		this->loadRecord();
		cout << "清空成功！" << endl;
	}

	cout << "按任意键继续..." << endl;    
	cin.ignore(); // 清除输入缓冲区    
    cin.get(); // 等待用户输入
	system("clear");
}

SpeechManager::~SpeechManager()
{
}