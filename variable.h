#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <random>
#include <direct.h>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#define stocknum 60//多少檔
#define day 280//天
#define exptime 3//實驗次數//
#define	startfunds  10000000//初始資金
#define paticle 100//10顆粒子//
#define changeyear 10//多少年滑動視窗(測試期)//
#define windows 13
#define upper 0.001250//自適應角度上限
#define lower 0.000200//自適應角度下限
#define riskfreerate 0.0087
#define NeighbourhoodNum 10
#define LambdaNum 10

using namespace std;
using std::string;

void Create_File(int startyear, int endyear);
int Look_YQHM(int windowsfor, int& change);
void Data_In();
void Initial(int particlefor);
void Calc(int particlefor, int generationsfor, int expfor);
void Selection(int particlefor);//,int Neighbourhoodfor);
void Crossover(int particlefor);//,int Neighbourhoodfor);
void Mutation(int particlefor);//,int Neighbourhoodfor);
void Quick_Sort(int left, int right);
void Update(int particlefor);
void Pareto_Front(int particlefor, int generationsfor, int expfor);
void io_Data_Place(int monthfor, int windowsfor);
void out_Pareto_Front(chrono::time_point<chrono::high_resolution_clock> time);
void out_Pareto_Front_Place(int monthfor, int windowsfor);
void out_Summart_Table(int h, chrono::time_point<chrono::high_resolution_clock> time);

string fundname;
string FileLocation;
string Algorithmname;
string datapalce;
string Sliding_Windows[windows] = { "H#", "H2H" , "H2M", "H2Q", "M#", "M2M", "Q#", "Q2M", "Q2Q", "Y2H", "Y2M", "Y2Q", "Y2Y" };//13個滑動視窗
string stock[stocknum][day] = {};//股價含名字
string in;//股價檔案名稱
string out;//輸出檔案名稱
string dataplace;//股價檔案路徑&位置
string outavg;//平均輸出路徑&檔案名稱
string windowsplace;//輸出檔案路徑&位置
string paretofront_outfile;
string totaltableplace;//總表輸出檔案位置

int days,
fitstocknum,
slidingdown,
slidingup,
fitexptime,
generations,
fitpaticle,
fitchangeyear,
pickstocknum[paticle] = { 0 },
pickstock[paticle][stocknum] = { 0 },//選到的股票
buystocknum[paticle][stocknum],//股票購買的數量
year = 0,
Childpickstock[paticle][stocknum],
parentpickstock[paticle][stocknum],
add,//前緣更新檢測
Neighbourhood[paticle][NeighbourhoodNum],
Neighbourhoodpickstock[paticle][NeighbourhoodNum][stocknum],
Parent[paticle][2],//[10顆粒子][0是父代、1是母代]
Num_of_Calc;

double
Weight_Vectors[2][paticle],//2個目標&
stockrate[stocknum][day] = { 0 },//股價
level[paticle][stocknum][day] = { 0 },//資金水位
trend[paticle], //趨勢值
truereturn[paticle],
risk[paticle],
expectedreturn[paticle],
totallevel[paticle][day] = { 0 },
leftfunds[paticle][stocknum] = { 0 },//單檔股票剩餘資金
trendspsharperisk[paticle],//趨勢值導引計算夏普值風險
trendspsharpe[paticle],//趨勢值導引計算夏普值
trendspreturn[paticle],//趨勢值導引計算夏普值報酬
parentfitness[paticle],//乘上lamda後適應值
parentrisk[paticle],
parentreturn[paticle],
childfitness[paticle][NeighbourhoodNum];

vector<int>ParetoFrontStockNum = { INT_MAX },
ParetoFrontExp,
ParetoFrontGeneration,
ParetoFrontNumCalc;
vector<string> ParetoFrontPickStock = { "" };
vector<vector<int>> ParetoFrontStock = { ParetoFrontStockNum };
vector<double>
ParetoFrontTrend = { DBL_MIN },
ParetoFrontRisk = { DBL_MAX },
ParetoFrontReturn = { DBL_MIN };