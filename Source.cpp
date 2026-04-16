#include "variable.h"

int main()
{
	int startyear, endyear, tableexecute = 1;
	dataplace = "TSX&NIKKEI_2016-2025";
	Algorithmname = "MOEAD";
	fitexptime = 3;
	generations = 999;
	fitpaticle = 100;
	fitstocknum = 0;
	slidingdown = 1, slidingup = 14;
	startyear = 2015, endyear = 2025;
	fitchangeyear = endyear - startyear;
	fitchangeyear += 1;
	tableexecute = 1;
	cout.setf(ios_base::fixed | ios::uppercase);
	cout << setprecision(30);
	Create_File(startyear, endyear);
	for (int windowsfor = slidingdown; windowsfor < slidingup; windowsfor++) //滑動視窗
	{
		year = startyear;//幾年開始(訓練期)
		int change = 0;
		Look_YQHM(windowsfor, change);//判斷以年，半年，季或月
		for (int yearfor = 1; yearfor <= fitchangeyear; yearfor++)//換年
		{
			for (int monthfor = 1; monthfor <= change; monthfor++)//換月change
			{
				Num_of_Calc = 0;
				auto starttime = std::chrono::high_resolution_clock::now();
				srand(114);//亂數種子
				io_Data_Place(monthfor, windowsfor);//輸入股價檔&輸出檔案名稱
				out_Pareto_Front_Place(monthfor, windowsfor);
				ifstream input_file;
				input_file.open(in, ios::in);//跳過無法讀取的檔案
				if (!input_file)
					continue;
				input_file.close();
				fitstocknum = 0;
				days = 0;
				for (int i = 0; i < stocknum; i++)
				{
					for (int j = 0; j < day; j++)
					{
						stock[i][j] = "";
						stockrate[i][j] = 0;
					}
				}
				Data_In();//股價輸入
				double WeightTemp[LambdaNum + 1];
				for (int LambdaNumfor = 0; LambdaNumfor < LambdaNum + 1; LambdaNumfor++)
				{
					WeightTemp[LambdaNumfor] = (double)LambdaNumfor / (double)LambdaNum; //[0~1.0] 11個權重
				}
				vector<int>(it1) = { INT_MAX };
				vector<string>(it2) = { "" };
				vector<double>(it3) = { DBL_MIN };
				vector<double>(it4) = { DBL_MAX };
				vector<vector <int>>(it5) = { it1 };
				vector<double>(it6) = { DBL_MAX };
				vector<int>(it7) = { INT_MAX };
				vector<int>(it8) = { INT_MAX };
				vector<int>(it9) = { INT_MAX };
				ParetoFrontStockNum.swap(it1);
				ParetoFrontPickStock.swap(it2);
				ParetoFrontReturn.swap(it3);
				ParetoFrontRisk.swap(it4);
				ParetoFrontStock.swap(it5);
				ParetoFrontTrend.swap(it6);
				ParetoFrontExp.swap(it7);
				ParetoFrontGeneration.swap(it8);
				ParetoFrontNumCalc.swap(it9);
				for (int expfor = 0; expfor < fitexptime; expfor++)
				{
					for (int particlefor = 0; particlefor < fitpaticle; particlefor++)
					{
						Initial(particlefor);
					}
					for (int particlefor = 0; particlefor < fitpaticle; particlefor++)
					{
						Weight_Vectors[0][particlefor] = WeightTemp[rand() % (LambdaNum + 1)];
						Weight_Vectors[1][particlefor] = 1.0 - Weight_Vectors[0][particlefor];
						//cout << Weight_Vectors[0][particlefor] << endl;
					}
					for (int particlefor = 0; particlefor < fitpaticle; particlefor++)
					{
						for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
						{
							double random = (double)rand() / (double)RAND_MAX;
							if (random > 0.5)
							{
								pickstock[particlefor][stocknumfor] = 1;
								pickstocknum[particlefor]++;
							}
							else
								pickstock == 0;
						}
						Calc(particlefor, 0, expfor);//計算
						parentfitness[particlefor] = Weight_Vectors[0][particlefor] * expectedreturn[particlefor] - Weight_Vectors[1][particlefor] * risk[particlefor];
						parentrisk[particlefor] = risk[particlefor];
						parentreturn[particlefor] = expectedreturn[particlefor];
						for (int stokcnumfor = 0; stokcnumfor < fitstocknum; stokcnumfor++)
						{
							parentpickstock[particlefor][stokcnumfor] = pickstock[particlefor][stokcnumfor];
						}
						//cout << expectedreturn[particlefor] << " " << risk[particlefor] << endl;
						Pareto_Front(particlefor, 0, expfor);
					}
					for (int generationsfor = 0; generationsfor < generations; generationsfor++)
					{
						for (int particlefor = 0; particlefor < fitpaticle; particlefor++)//+= 2
						{
							Selection(particlefor);
							Crossover(particlefor);
							Mutation(particlefor);
							Initial(particlefor);
							pickstocknum[particlefor] = 0;
							for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
							{
								pickstock[particlefor][stocknumfor] = Childpickstock[particlefor][stocknumfor];
								if (pickstock[particlefor][stocknumfor] == 1)
									pickstocknum[particlefor]++;
							}
							/*if (particlefor == 0)
							{
								for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
								{
									cout << pickstock[particlefor][stocknumfor] << " ";
								}
								cout << endl;
							}*/
							Calc(particlefor, generationsfor, expfor);//計算
							if (particlefor == 1)
							{
								for (int particlefor = 0; particlefor < fitpaticle; particlefor++)//+= 2
								{
									//cout << expectedreturn[particlefor] << " " << risk[particlefor] << endl;
								}
							}
							for (int Neighbourhoodfor = 0; Neighbourhoodfor < NeighbourhoodNum; Neighbourhoodfor++)
							{
								childfitness[particlefor][Neighbourhoodfor] = Weight_Vectors[0][Neighbourhood[particlefor][Neighbourhoodfor]] * expectedreturn[particlefor] - Weight_Vectors[1][Neighbourhood[particlefor][Neighbourhoodfor]] * risk[particlefor];
							}
							Update(particlefor);
							/*if (risk[particlefor] == 103258.00499907990160863847)
							{
								cout << endl;
							}*/
							/*for (int particlefor = 0; particlefor < fitpaticle; particlefor++)//+= 2
							{
								for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
								{
									//cout << parentpickstock[particlefor][stocknumfor] << " ";
								}
								//cout << endl;
							}*/
							if (particlefor == 98)
							{
								//cout << endl;
							}
							Pareto_Front(particlefor, generationsfor, expfor);
							add = 0;
						}
						for (int particlefor = 0; particlefor < fitpaticle; particlefor++)//+= 2
						{
							//cout << endl << risk[particlefor] << " " << expectedreturn[particlefor] << endl;
						}
					}
				}
				Quick_Sort(0, ParetoFrontRisk.size() - 1);
				out_Pareto_Front(starttime);
				if (tableexecute == 1)
				{
					out_Summart_Table(windowsfor, starttime);
				}
			}
			year++;
		}
	}
}

void Selection(int particlefor)
{
	unordered_set<int> particleset;
	for (int particlefor = 0; particlefor < fitpaticle; ++particlefor)
	{
		particleset.insert(particlefor);
	}
	int particlefor1 = 0;
	for (int Neighbourhoodfor = 0; Neighbourhoodfor < NeighbourhoodNum; Neighbourhoodfor++)
	{
		vector<int> paticlesetarray(particleset.begin(), particleset.end());
		double MinWeight = DBL_MAX;
		int MinWeightNum = 10000;
		for (int count = 0; count < particleset.size(); count++)
		{
			double temp = pow(Weight_Vectors[0][particlefor] - (Weight_Vectors[0][paticlesetarray[count]]), 2) + pow(Weight_Vectors[1][particlefor] - (Weight_Vectors[1][paticlesetarray[count]]), 2);
			temp = sqrt(temp);
			//cout << temp << endl;
			/*if (temp == MinWeight)
			{
				MinWeightNum = MinWeightNum;
			}
			else */
			if (temp < MinWeight)
			{
				MinWeight = sqrt(pow((Weight_Vectors[0][particlefor] - Weight_Vectors[0][paticlesetarray[count]]), 2) + pow(Weight_Vectors[1][particlefor] - (Weight_Vectors[1][paticlesetarray[count]]), 2));
				MinWeightNum = paticlesetarray[count];
			}
		}
		Neighbourhood[particlefor][Neighbourhoodfor] = MinWeightNum;
		for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
		{
			Neighbourhoodpickstock[particlefor][Neighbourhoodfor][stocknumfor] = parentpickstock[MinWeightNum][stocknumfor];
		}
		particleset.erase(MinWeightNum);
		particlefor1++;
	}
	int random = 0, random1 = 0;
	random = rand() % NeighbourhoodNum;
	Parent[particlefor][0] = Neighbourhood[particlefor][random];
	random = rand() % NeighbourhoodNum;
	Parent[particlefor][1] = Neighbourhood[particlefor][random];
	/*/if (particlefor == 1)
	{
		for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
		{
			cout << parentpickstock[Parent[particlefor][0]][stocknumfor] << ",";
		}
		cout << endl;
		for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
		{
			cout << parentpickstock[Parent[particlefor][1]][stocknumfor] << ",";
		}
		cout << endl;
	}*/
	//cout << Parent[particlefor][0] << " " << Parent[particlefor][1] << endl;
}

void Update(int particlefor)
{
	for (int Neighbourhoodfor = 0; Neighbourhoodfor < NeighbourhoodNum; Neighbourhoodfor++)
	{
		if (particlefor == 4)
		{
			//cout << childfitness[particlefor][Neighbourhoodfor] << " " << parentfitness[Neighbourhood[particlefor][Neighbourhoodfor]] << endl;
		}
		if (childfitness[particlefor][Neighbourhoodfor] >= parentfitness[Neighbourhood[particlefor][Neighbourhoodfor]])
		{
			parentfitness[Neighbourhood[particlefor][Neighbourhoodfor]] = childfitness[particlefor][Neighbourhoodfor];
			parentrisk[Neighbourhood[particlefor][Neighbourhoodfor]] = risk[particlefor];
			parentreturn[Neighbourhood[particlefor][Neighbourhoodfor]] = expectedreturn[particlefor];
			for (int stokcnumfor = 0; stokcnumfor < fitstocknum; stokcnumfor++)
			{
				parentpickstock[Neighbourhood[particlefor][Neighbourhoodfor]][stokcnumfor] = pickstock[particlefor][stokcnumfor];
			}
			/*parentfitness[Neighbourhood[particlefor][Neighbourhoodfor]] = childfitness[particlefor][Neighbourhoodfor];
			parentrisk[Neighbourhood[particlefor][Neighbourhoodfor]] = risk[particlefor];
			parentreturn[Neighbourhood[particlefor][Neighbourhoodfor]] = expectedreturn[particlefor];
			for (int stokcnumfor = 0; stokcnumfor < fitstocknum; stokcnumfor++)
			{
				parentpickstock[Neighbourhood[particlefor][Neighbourhoodfor]][stokcnumfor] = pickstock[particlefor][stokcnumfor];
			}*/
		}
	}
}

void Quick_Sort(int left, int right) {
	if (left < right) {
		// 选取枢纽元素，这里选择中间元素作为枢纽元素
		double pivot = ParetoFrontRisk[(left + right) / 2];
		int i = left;
		int j = right;

		// 划分数组
		while (i <= j) {
			while (ParetoFrontRisk[i] < pivot) {
				i++;
			}
			while (ParetoFrontRisk[j] > pivot) {
				j--;
			}
			if (i <= j) {
				swap(ParetoFrontStock[i], ParetoFrontStock[j]);
				swap(ParetoFrontStockNum[i], ParetoFrontStockNum[j]);
				swap(ParetoFrontPickStock[i], ParetoFrontPickStock[j]);
				swap(ParetoFrontReturn[i], ParetoFrontReturn[j]);
				swap(ParetoFrontRisk[i], ParetoFrontRisk[j]);
				swap(ParetoFrontTrend[i], ParetoFrontTrend[j]);
				swap(ParetoFrontExp[i], ParetoFrontExp[j]);
				swap(ParetoFrontGeneration[i], ParetoFrontGeneration[j]);
				swap(ParetoFrontNumCalc[i], ParetoFrontNumCalc[j]);
				i++;
				j--;
			}
		}
		// 递归调用快速排序
		Quick_Sort(left, j);
		Quick_Sort(i, right);
	}
}

void Pareto_Front(int particlefor, int generationsfor, int expfor)
{
	for (int i = 0; i < ParetoFrontRisk.size(); i++)
	{
		if (expectedreturn[particlefor] > ParetoFrontReturn[i] && risk[particlefor] < ParetoFrontRisk[i])
		{
			ParetoFrontStock.erase(ParetoFrontStock.begin() + i);
			ParetoFrontStockNum.erase(ParetoFrontStockNum.begin() + i);
			ParetoFrontPickStock.erase(ParetoFrontPickStock.begin() + i);
			ParetoFrontReturn.erase(ParetoFrontReturn.begin() + i);
			ParetoFrontRisk.erase(ParetoFrontRisk.begin() + i);
			ParetoFrontTrend.erase(ParetoFrontTrend.begin() + i);
			ParetoFrontExp.erase(ParetoFrontExp.begin() + i);
			ParetoFrontGeneration.erase(ParetoFrontGeneration.begin() + i);
			ParetoFrontNumCalc.erase(ParetoFrontNumCalc.begin() + i);
			add = 1;
			--i;
		}
		else if (expectedreturn[particlefor] > ParetoFrontReturn[i] || risk[particlefor] < ParetoFrontRisk[i])
		{
			add = 1;
		}
		else if (expectedreturn[particlefor] <= ParetoFrontReturn[i] && risk[particlefor] >= ParetoFrontRisk[i])
		{
			add = 0;
			break;
		}
	}
	if (add == 1)
	{
		string paretostocktmp = "";
		ParetoFrontStockNum.push_back(pickstocknum[particlefor]);
		vector<int>paretotmp;
		for (int i = 0; i < fitstocknum; i++)
		{
			if (pickstock[particlefor][i] == 1)
			{
				paretostocktmp = paretostocktmp + stock[i][0] + " ";
				paretotmp.push_back(1);
			}
			else
			{
				paretotmp.push_back(0);
			}
		}
		ParetoFrontStock.push_back(paretotmp);
		ParetoFrontPickStock.push_back(paretostocktmp);
		ParetoFrontReturn.push_back(expectedreturn[particlefor]);
		ParetoFrontRisk.push_back(risk[particlefor]);
		ParetoFrontTrend.push_back(trend[particlefor]);
		ParetoFrontExp.push_back(expfor);
		ParetoFrontGeneration.push_back(generationsfor);
		ParetoFrontNumCalc.push_back(Num_of_Calc);
	}
}

void Crossover(int particlefor)//,int Neighbourhoodfor)
{
	int Crossplace;       // 交叉點
	Crossplace = rand() % fitstocknum; // 隨機交叉點
	//cout << Crossplace << endl;
	//cout << "Crossplace: " << Crossplace << endl;
	for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++) {
		if (stocknumfor <= Crossplace)
		{
			Childpickstock[particlefor][stocknumfor] = parentpickstock[Parent[particlefor][0]][stocknumfor];
		}
		else
		{
			Childpickstock[particlefor][stocknumfor] = parentpickstock[Parent[particlefor][1]][stocknumfor];
		}
	}
	for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++) {
		//cout << parentpickstock[Parent[particlefor][0]][stocknumfor] << " ";
	}
	//cout << endl;
	for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++) {
		//cout << parentpickstock[Parent[particlefor][1]][stocknumfor] << " ";
	}
	//cout << endl;
}

void Mutation(int particlefor)//,int Neighbourhoodfor)
{
	for (int i = 0; i < fitstocknum; i++)
	{
		double MutationRate = (double)1.0 / (double)fitstocknum;
		double randomnum;
		randomnum = ((double)rand() / (double)RAND_MAX);
		if (randomnum < MutationRate)
		{
			if (Childpickstock[particlefor][i] == 0)
			{
				//cout << "0 " << i << endl;
				Childpickstock[particlefor][i] = 1;
			}
			else
			{
				//cout << "1 " << i << endl;
				Childpickstock[particlefor][i] = 0;
			}
		}
	}
}

void Create_File(int startyear, int endyear)
{
	string AverageLocation, SummaryTableLocation, SingleStockLocation;
	if (startfunds == 10000000)
		fundname = "10M";
	else if (startfunds == 1000000000)
		fundname = "1B";
	FileLocation = "./" + dataplace + "_" + Algorithmname + "_" + to_string(startyear) + "-" + to_string(endyear) + "(" + to_string(fitchangeyear) + ")_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname;
	_mkdir(FileLocation.c_str());
	SummaryTableLocation = FileLocation + "\\" + "Summart_Table";
	_mkdir(SummaryTableLocation.c_str());
	AverageLocation = SummaryTableLocation + "\\" + "Avearge";
	_mkdir(AverageLocation.c_str());
	for (int i = slidingdown; i < slidingup; i++)
	{
		string SlidingLocation, FundtrainLocation, ParetoFrontLocation, ProbabilityLocation, PickStockLocation, ParetoDetailLocation, EL_Pareto_DetailLocation;
		SlidingLocation = FileLocation + "\\" + Sliding_Windows[i - 1];
		_mkdir(SlidingLocation.c_str());
		FundtrainLocation = FileLocation + "\\" + Sliding_Windows[i - 1] + "\\" + "Fund_Train";
		_mkdir(FundtrainLocation.c_str());
		ParetoFrontLocation = FileLocation + "\\" + Sliding_Windows[i - 1] + "\\" + "Pareto_Front";
		_mkdir(ParetoFrontLocation.c_str());
		SingleStockLocation = FileLocation + "\\" + Sliding_Windows[i - 1] + "\\" + "Single_Stock";
		_mkdir(SingleStockLocation.c_str());
		PickStockLocation = FileLocation + "\\" + Sliding_Windows[i - 1] + "\\" + "Pick_Stock";
		_mkdir(PickStockLocation.c_str());
		ParetoDetailLocation = FileLocation + "\\" + Sliding_Windows[i - 1] + "\\" + "Pareto_Detail";
		_mkdir(ParetoDetailLocation.c_str());
	}
	ofstream outstockavg1;
	outavg = "./" + dataplace + "\\Summary_Table\\Avearge\\Stock_Avg.csv";
	outstockavg1.open(outavg, ios::out);//創建文檔
	outstockavg1.close();
}

void Initial(int particlefor)
{
	//fitness[particlefor] = 0;//指數&UBAH需改-DBL_MAX
	trend[particlefor] = 0;
	expectedreturn[particlefor] = 0;
	risk[particlefor] = 0;
	pickstocknum[particlefor] = 0;
	for (int j = 0; j < stocknum; j++)
	{
		pickstock[particlefor][j] = 0;
		leftfunds[particlefor][j] = 0;
		buystocknum[particlefor][j] = 0;
	}
	for (int dayfor = 0; dayfor < day; dayfor++)
	{
		totallevel[particlefor][dayfor] = 0;
		for (int j = 0; j < stocknum; j++)
		{
			level[particlefor][j][dayfor] = 0;
		}
	}
}

//觀察是什麼區間
int Look_YQHM(int windowsfor, int& change)
{
	if (windowsfor == 3 || windowsfor == 5 || windowsfor == 6 || windowsfor == 8 || windowsfor == 11)
	{
		change = 12;
	}
	if (windowsfor == 1 || windowsfor == 2 || windowsfor == 10)
	{
		change = 2;
	}
	if (windowsfor == 4 || windowsfor == 7 || windowsfor == 9 || windowsfor == 12)
	{
		change = 4;
	}
	if (windowsfor == 13)
	{
		change = 1;
	}
	return change;
}

//計算
void Calc(int particlefor, int generationsfor, int expfor)
{
	double //趨勢值計算參數
		funds = 0,//分配資金
		fundsleft = 0,//投資組合剩餘資金
		expectedreturntemp = 0, //預期報酬暫存
		power = 0, //天數平方
		Yi = 0, //一次回歸趨勢線
		tmp = 0;//
	double //夏普值計算參數
		sptemptotallevel,
		spreturntemp = 0,
		splevelavg,
		sptemp[day],
		allsptemp = 0,
		risktemp;
	if (pickstocknum[particlefor] != 0)
	{
		funds = startfunds / pickstocknum[particlefor];
		fundsleft = startfunds % pickstocknum[particlefor];
	}
	for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
	{
		if (pickstock[particlefor][stocknumfor] == 1)
		{
			buystocknum[particlefor][stocknumfor] = funds / stockrate[stocknumfor][0];
			leftfunds[particlefor][stocknumfor] = funds - (stockrate[stocknumfor][0] * buystocknum[particlefor][stocknumfor]);
			for (int daysfor = 0; daysfor < days; daysfor++)
			{
				level[particlefor][stocknumfor][daysfor] = stockrate[stocknumfor][daysfor] * buystocknum[particlefor][stocknumfor] + leftfunds[particlefor][stocknumfor];//每股資金水位
			}
		}
	}
	for (int daysfor = 0; daysfor < days; daysfor++)
	{
		for (int k = 0; k < fitstocknum; k++)
		{
			totallevel[particlefor][daysfor] += level[particlefor][k][daysfor];//總資金水位
		}
		totallevel[particlefor][daysfor] += fundsleft;
		expectedreturntemp += (((double)daysfor + 1) * totallevel[particlefor][daysfor]) - (((double)daysfor + 1) * startfunds);
		power += pow(daysfor + 1, 2);
	}
	expectedreturn[particlefor] = expectedreturntemp / power;//每日預期報酬
	for (int daysfor = 0; daysfor < days; daysfor++)
	{
		Yi = (expectedreturn[particlefor] * (daysfor + 1)) + startfunds;//預期資金水位(一次回歸趨勢線）
		tmp += pow((totallevel[particlefor][daysfor] - Yi), 2);//背離趨勢線的波動
	}
	risk[particlefor] = sqrt(tmp / days);//每日風險
	truereturn[particlefor] = totallevel[particlefor][days - 1] - startfunds;
	trend[particlefor] = expectedreturn[particlefor] / risk[particlefor];//趨勢值
	Num_of_Calc += 1;
	if (trend[particlefor] < 0)
	{
		trend[particlefor] = expectedreturn[particlefor] * risk[particlefor];
	}
	if (pickstocknum[particlefor] == 0)
	{
		risk[particlefor] = 0;
		expectedreturn[particlefor] = 0;
		trend[particlefor] = 0;
	}
	/*double r1;
	if (UBHstatus == false)
		r1 = ((generations+1) / (generationsfor + 1)) * risk[particlefor];
	else
		r1 = risk[particlefor];*/
		/*if (expfor == 0)
			fitness[particlefor] = expectedreturn[particlefor];// expectedreturn[particlefor] / risk[particlefor];//計算適應
		else if (expfor == 1)
			fitness[particlefor] = 1 / risk[particlefor];
		else*/
	//趨勢值導引計算夏普值
	//cout << fitness[particlefor] << endl;
	sptemptotallevel = totallevel[particlefor][days - 1] - startfunds;
	trendspreturn[particlefor] = sptemptotallevel / startfunds;
	for (int i = 0; i < days; i++)
	{
		spreturntemp += totallevel[particlefor][i];
	}
	splevelavg = spreturntemp / days;
	for (int i = 0; i < days; i++)
	{
		sptemp[i] = totallevel[particlefor][i] - splevelavg;
		allsptemp += pow(sptemp[i], 2);
	}
	risktemp = sqrt(allsptemp / days);
	trendspsharperisk[particlefor] = risktemp / splevelavg;
	trendspsharpe[particlefor] = (trendspreturn[particlefor] - riskfreerate) / trendspsharperisk[particlefor];//夏普值
}

//fundtrain輸出位置
void io_Data_Place(int monthfor, int windowsfor)
{
	windowsplace = FileLocation + "\\" + Sliding_Windows[windowsfor - 1] + "\\";
	string tempdataplace = "./" + dataplace + "\\" + Sliding_Windows[windowsfor - 1];
	if (windowsfor == 1 || windowsfor == 2)//H#,H2H
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q1-Q2(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q1-Q2(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 2)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q3-Q4(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q3-Q4(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 3)//H2M
	{
		if (monthfor < 5)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
		}
		else if (monthfor >= 5 && monthfor < 8)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
		}
		else if (monthfor >= 8 && monthfor < 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
		}
		else if (monthfor >= 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "\\Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
		}
	}
	if (windowsfor == 4)//H2Q
	{
		if (monthfor < 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "-Q" + to_string(monthfor + 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "-Q" + to_string(monthfor + 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor + 1 - 4) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor + 1 - 4) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 5 || windowsfor == 6)//M#,M2M
	{
		if (monthfor <= 9)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 9)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 7 || windowsfor == 9)//Q#.Q2Q
	{
		if (monthfor <= 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 8)//Q2M
	{
		if (monthfor <= 7)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 7 && monthfor < 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + " - " + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 2 - 12) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 2 - 12) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 10)//Y2H
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 2)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q3~" + to_string(year + 1) + "_Q2(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q3~" + to_string(year + 1) + "_Q2(" + to_string(year) + " Q1).csv";
		}
	}
	if (windowsfor == 11)//Y2M
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 1 && monthfor < 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 12)//Y2Q
	{
		if (monthfor < 2)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor >= 2 && monthfor <= 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 13)//Y2Y
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	cout << in << endl;
	cout << out << endl;
}

//柏拉圖前沿輸出位置
void out_Pareto_Front_Place(int monthfor, int windowsfor)
{
	if (windowsfor == 1 || windowsfor == 2)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q1-Q2(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 2)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q3-Q4(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 3)
	{
		if (monthfor < 5)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1)" + ".csv";
		}
		else if (monthfor >= 5 && monthfor < 8)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1)" + ".csv";
		}
		else if (monthfor >= 8 && monthfor < 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1)" + ".csv";
		}
		else if (monthfor >= 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1)" + ".csv";
		}
	}
	if (windowsfor == 4)
	{
		if (monthfor < 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "-Q" + to_string(monthfor + 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor + 1 - 4) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 5 || windowsfor == 6)
	{
		if (monthfor <= 9)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 9)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 7 || windowsfor == 9)
	{
		if (monthfor <= 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 8)
	{
		if (monthfor <= 7)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 7 && monthfor < 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 2 - 12) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 10)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 2)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q3~" + to_string(year + 1) + "_Q2(" + to_string(year) + " Q1)" + ".csv";
		}
	}
	if (windowsfor == 11)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 1 && monthfor < 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 12)
	{
		if (monthfor < 2)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor >= 2 && monthfor <= 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 13)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitpaticle) + "_" + to_string(NeighbourhoodNum) + "_" + to_string(LambdaNum) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	cout << paretofront_outfile << endl;
}

//總表輸出
void out_Summart_Table(int h, chrono::time_point<chrono::high_resolution_clock> time)
{
	static int totaltabletemph = 0;
	totaltableplace = FileLocation + "\\" + "Summart_Table\\" + Sliding_Windows[h - 1] + ".csv";
	cout << totaltableplace << endl;
	ofstream totaltable;
	totaltable.setf(ios_base::fixed | ios::uppercase);
	totaltable << setprecision(20);
	if (h != totaltabletemph)
	{
		totaltable.open(totaltableplace, ios::out);
		totaltabletemph = h;
	}
	else
	{
		totaltable.open(totaltableplace, ios::app);
	}
	auto endtime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - time);
	totaltable << out << "," << "Number of Pareto Front" << "," << ParetoFrontRisk.size() << "," << "Duration" << "," << duration.count();
	totaltable << endl;
	totaltable.close();
}

void out_Pareto_Front(chrono::time_point<chrono::high_resolution_clock> time)//int expfor, 
{
	ofstream outpareto;
	outpareto << fixed << setprecision(20);
	outpareto.open(paretofront_outfile, ios::out);
	outpareto << "Number of Stock in Portfolio" << "," << "Portfolio" << "," << "TrendRatio" << "," << "DailyRisk" << "," << "ExpectedReturn" << "," << "Number of Computations";//TrendRatio ExpectedReturn DailyRisk
	outpareto << endl;
	for (int i = 0; i < ParetoFrontStockNum.size(); i++)
	{
		if (ParetoFrontReturn[i] != DBL_MIN)
			outpareto << ParetoFrontStockNum[i] << "," << ParetoFrontPickStock[i] << "," << ParetoFrontTrend[i] << "," << ParetoFrontRisk[i] << "," << ParetoFrontReturn[i] << "," << ParetoFrontNumCalc[i] << endl;
	}
	outpareto << endl;
	auto endtime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - time);
	outpareto << "Duration" << "," << duration.count() << endl;
	outpareto.close();
}

//資料輸入
void Data_In()
{
	int i = 0, j = 0, itemp = 0;
	ifstream input_file;
	input_file.open(in, ios::in);//讀入的檔名
	string line;
	while (getline(input_file, line))
	{
		string tmp1;
		stringstream ss(line);//字串處理
		while (getline(ss, tmp1, ','))//csv檔是逗號分隔檔，所以透過逗號來讀取檔案裡面的資料
		{
			char* ptr = const_cast<char*>(tmp1.c_str());
			for (int length = 0; length < strlen(ptr); length++)
			{
				if (ptr[length] >= 'a' && ptr[length] <= 'z' || ptr[length] >= 'A' && ptr[length] <= 'Z')
				{
					fitstocknum += 1;
					break;
				}
			}
		}
	}
	input_file.close();

	input_file.open(in, ios::in);//讀入的檔名
	while (getline(input_file, line))
	{
		if (!input_file.is_open())
		{
			break;
		}
		string tmp1;
		stringstream ss(line);//字串處理
		while (getline(ss, tmp1, ','))//csv檔是逗號分隔檔，所以透過逗號來讀取檔案裡面的資料
		{
			char* ptr = const_cast<char*>(tmp1.c_str());
			stock[i][j] = ptr;
			if (i == (fitstocknum - 1))
			{
				j++;
				i = 0;
			}
			else
			{
				i++;
			}
		}
	}
	for (i = 0; i < fitstocknum; i++)
	{
		for (j = 0; j < (day - 1); j++)
		{
			if (stock[i][j + 1] != "")
			{
				stockrate[i][j] = stod(stock[i][j + 1]);
				itemp += 1;
			}
			else
			{
				break;
			}
		}
		days = itemp;
		itemp = 0;
	}
	input_file.close();//開檔要記得關檔
}