
#include "yuukouhai.h"


Yuukouhai::Yuukouhai(void){
	string num[10] = {"零","一","二","三","四","五","六","七","八","九"};
	for(int i=1;i<=9;i++){
		hainame[i]		= num[i]+ "萬";
		hainame[i+10]	= num[i] + "筒";
		hainame[i+20]	= num[i] + "索";
	}
	hainame[31] = "東";
	hainame[32] = "南";
	hainame[33] = "西";
	hainame[34] = "北";
	hainame[35] = "白";
	hainame[36] = "撥";
	hainame[37] = "中";
}

//手牌セット
void Yuukouhai::set_tehai(int t[])
{
	for(int i=0;i<38;i++){
		tehai[i]=t[i];
	}
	fuurosuu=0;
}

//エラーチェック
bool Yuukouhai::error_check()
{
	int tmp=0;
	for(int i=0;i<38;i++){
		tmp+=tehai[i];
	}
	if(tmp%3!=1||tmp>=14||tmp<1) return true;
	return false;
}

bool Yuukouhai::error_check14()
{
	int tmp=0;
	for(int i=0;i<38;i++){
		tmp+=tehai[i];
	}
	if(tmp!=14) return true;
	return false;
}



void Yuukouhai::NotKoritu ()
{
	int work[38];//作業用
	int i=0,p=0;
	//配列初期化
	memset(work,0,sizeof(work));
	memset(not_koritu,0,sizeof(not_koritu));
	//計算
	for(;i<=37;i++){ 
		for(;!tehai[i];i++);if(i>=38) continue;
		//数牌
		if(i<30){
			if(i%10==1) work[i]=work[i+1]=work[i+2]=1;
			else if(i%10==2) work[i]=work[i+1]=work[i+2]=work[i-1]=1;
			else if(i%10>=3 &&i%10<=7) work[i]=work[i-1]=work[i+1]=work[i-2]=work[i+2] =1;
			else if(i%10==8) work[i]=work[i+1]=work[i-2]=work[i-1]=1;
			else if(i%10==9) work[i]=work[i-1]=work[i-2]=1;             
		}
		//字牌
		else if(i>30) work[i]=1;
	}
	//結果を格納
	for(i=0;i<=37;i++){ 
		for(;!work[i];i++);if(i>=38) continue;
		not_koritu[p++]=i;
	}
}

/*
* 通常手の有効牌
*/ 
bool Yuukouhai::NormalYuukou (vector<int>  NormalYuukou[])
{
	if(error_check()) return false;
	for(int i=0;i<38;i++){
		tehaitmp[i] = tehai[i];
	}
	int p_koritu=0;		//配列の添え字ポインタ（孤立牌）
	int p_yuukou=0;		//配列の添え字ポインタ（有効牌）
	int syanten=NormalSyanten();   	//現在の牌のシャンテン数 
	NormalYuukou->clear();//初期化 
	NotKoritu ();
	while(not_koritu[p_koritu] != 0){
		//否孤立牌を追加
		tehai[not_koritu[p_koritu]]++;
		//現在値と比較
		if(NormalSyanten() <syanten)
			NormalYuukou->push_back(not_koritu[p_koritu]);
		//追加した否孤立牌をマイナス
		tehai[not_koritu[p_koritu]]--;
		p_koritu++;
	}

	for(int i=0;i<38;i++){
		tehai[i] = tehaitmp[i];
	}
	return true;
}

/*
* チートイツの有効牌
*/
bool Yuukouhai::TiitoiYuukou (vector<int>  TiitoiYuukou [])
{
	if(error_check()) return false;

	int i=0;//配列の添え字ポインタ（孤立牌）
	int syanten=TiitoituSyanten();  //現在の牌のシャンテン数 
	TiitoiYuukou->clear();//初期化 
	while(i<=37){
		//否孤立牌を追加
		for(;!tehai[i];i++);if(i>=38) continue;
		tehai[i]++;
		//現在値と比較
		if(TiitoituSyanten() <syanten)
			TiitoiYuukou->push_back(i);
		//追加した否孤立牌をマイナス
		tehai[i]--;
		i++;
	}	
	return true;
}

/*
* 国士無双の有効牌
*/ 
bool Yuukouhai::KokusiYuukou (vector<int>  KokusiYuukou [])
{
	if(error_check()) return false;

	int i=0;
	int syanten=KokusiSyanten();//現在の牌のシャンテン数
	KokusiYuukou->clear();//初期化 
	int yaotyuu[13]={1,9,11,19,21,29,31,32,33,34,35,36,37};
	for(;i<13;i++){
		//否孤立牌を追加
		tehai[yaotyuu[i]]++;
		//現在値と比較
		if(KokusiSyanten() <syanten)
			KokusiYuukou->push_back(yaotyuu[i]);
		//追加した否孤立牌をマイナス
		tehai[yaotyuu[i]]--;
	}
	return true;
}

//国士シャンテン
int Yuukouhai::KokusiSyanten()
{
	int kokusi_toitu=0,syanten_kokusi=13,i;             
	//老頭牌
	for(i=1;i<30;i++){        
		if(i%10==1||i%10==9||i%20==1||i%20==9){	   
			if(tehai[i])
				syanten_kokusi--;
			if(tehai[i] >=2 && kokusi_toitu==0)
				kokusi_toitu=1;	
		}
	}             
	//字牌
	for(i=31;i<38;i++){      
		if(tehai[i]){	        
			syanten_kokusi--;
			if(tehai[i] >=2 && kokusi_toitu==0)
				kokusi_toitu=1;			            
		}
	}             
	//頭
	syanten_kokusi-= kokusi_toitu;             
	return syanten_kokusi;
}

//チートイシャンテン
int Yuukouhai::TiitoituSyanten()
{
	int i=1,toitu=0,syurui=0,syanten_tiitoi;
	//トイツ数を数える
	for(;i<=37;i++){ 
		for(;!tehai[i];i++);
		if(i>=38) continue;
		syurui++;
		if(tehai[i] >=2)
			toitu++;
	}
	syanten_tiitoi=6-toitu;
	//４枚持ちを考慮
	if(syurui<7)
		syanten_tiitoi+= 7-syurui;
	return syanten_tiitoi;
}


//通常手シャンテン
int Yuukouhai::NormalSyanten()
{
	//初期化
	mentu=0;
	toitu=0;
	kouho=0;
	temp=0;
	syanten_normal=8;
	for(int i=1;i<38;i++)
	{
		//頭抜き出し
		if(2 <= tehai[i])
		{            
			toitu++;
			tehai[i] -= 2;
			mentu_cut(1);
			tehai[i] += 2;
			toitu--;
		}
	}
	mentu_cut(1);   //頭無しと仮定して計算

	//死に孤立字牌
	bool ji=false;
	for(int i=31;i<38;i++){if(tehai[i]==4)ji=true;}
	return syanten_normal-fuurosuu*2+ji;	//最終的な結果
}

//メンツ抜き出し
void Yuukouhai::mentu_cut(int i)
{
	for(;!tehai[i];i++);
	if(i>=38){taatu_cut(1);return;}//メンツを抜き終わったのでターツ抜きへ
	//コーツ
	if(tehai[i]>=3)
	{
		mentu++;
		tehai[i]-=3;
		mentu_cut(i);
		tehai[i]+=3;
		mentu--;
	}
	//シュンツ
	if(tehai[i+1]&&tehai[i+2]&&i<30)
	{
		mentu++;
		tehai[i]--,tehai[i+1]--,tehai[i+2]--;
		mentu_cut(i);
		tehai[i]++,tehai[i+1]++,tehai[i+2]++;
		mentu--;
	}
	//メンツ無しと仮定
	mentu_cut(i+1);
}
//ターツ抜き出し
void Yuukouhai::taatu_cut(int i)
{													   
	for(;!tehai[i];i++);
	if(i>=38) //抜き出し終了
	{
		temp=8-mentu*2-kouho-toitu;
		if(temp<syanten_normal) { syanten_normal=temp; }
		return;
	}
	if(mentu+kouho<4)
	{            
		//トイツ
		if(tehai[i]==2)
		{
			kouho++;
			tehai[i]-=2;
			taatu_cut(i);
			tehai[i]+=2;
			kouho--;
		}

		//ペンチャンorリャンメン
		if(tehai[i+1]&&i<30)
		{
			kouho++;
			tehai[i]--,tehai[i+1]--;
			taatu_cut(i);
			tehai[i]++,tehai[i+1]++;
			kouho--;
		}

		//カンチャン
		if(tehai[i+2]&&i<30&&i%10<=8)
		{
			kouho++;
			tehai[i]--,tehai[i+2]--;
			taatu_cut(i);
			tehai[i]++,tehai[i+2]++;
			kouho--;
		}
	}
	taatu_cut(i+1);
}

bool Yuukouhai::kiruhai(){
	if(error_check14() == true)
		return false;
	vector<int> result;
	for(int i=0;i<38;i++){
		if(tehai[i] > 0){
			tehai[i] --;
			//std::cout << hainame[i] << "切りの待ち(有効牌):"<<endl;
			showYuukou(hainame[i] + "切りの待ち(有効牌):");
			tehai[i] ++;
		}
	}
	return 0;
}

bool Yuukouhai::showYuukou(string inputstr){
	if(error_check()) return false;
	vector<int> result;
	NormalYuukou(&result);
	std::cout << inputstr;
	for(int i=0; i < result.size();i++){
		cout << hainame[result[i]] <<" ";
	}
	cout << endl;
	cout <<"向聴数:" << NormalSyanten() << endl << endl;;

	return true;
}