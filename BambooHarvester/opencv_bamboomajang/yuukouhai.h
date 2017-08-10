﻿
#pragma once
#ifndef __YUUKOUHAIKEISAN
#define __YUUKOUHAIKEISAN

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;



//有効牌を求めるクラス
class Yuukouhai{

private:
	
	int KokusiSyanten();//国士シャンテン
	int TiitoituSyanten();//チートイシャンテン
	void mentu_cut(int);//メンツ抜き関数
	void taatu_cut(int);//ターツ抜き関数

	int mentu;		//メンツ数
	int toitu;		//トイツ数
	int kouho;		//ターツ数
	int fuurosuu;		//フーロ数
	int temp;		//シャンテン数（計算用）
	int syanten_normal;	//シャンテン数（結果用）

	void NotKoritu();
	int tehai[38];
	int tehaitmp[38];//計算用
	int not_koritu[38];//孤立牌でない牌格納用

	bool error_check();
	bool error_check14();
	string hainame[38];

public:
	int NormalSyanten();//通常手シャンテン
	bool NormalYuukou (vector<int> NormalYuukou[]);//通常手有効牌
	bool TiitoiYuukou (vector<int> TiitoiYuukou[]);//国士有効牌
	bool KokusiYuukou (vector<int> KokusiYuukou[]);//チートイ有効牌
	bool kiruhai(void);
	bool showYuukou(string);
	void set_tehai(int t[]);//手牌セット
	Yuukouhai(void);
};


#endif