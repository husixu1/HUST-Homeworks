/***********************************************************************
*    NDDIMS - Natural Disasters Data and Information Management System
*    Copyright (C) 2016  HuSixu	 Email:husixu1@hotmail.com
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

/** developer tools, don't use it **/
/** developer tools, don't use it **/

#include "file_operation.h"
#include "node_operation.h"
#include "data_structure.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const char type[7]			=	{'1','2','3','4','5','6','7'};
static const char name[7][8][20]	={	{"tropical storm","tornado","thunder","hot dry storm","heavy rain","freeze","cold","drought"},
										{"tsunami","tide","red tide","sea level raise","tsunami","tide","red tide","sea level raise"},
										{"rain flood","river flood","rain flood","river flood","rain flood","river flood","rain flood","river flood"},
										{"collapse","landslide","debris flow","crack","volcano eruption","ground subsidence","desertification","salinization"},
										{"liquefaction","sandblasting","urban fires","bank burst","liquefaction","sandblasting","urban fires","bank burst"},
										{"agri-insect","rodent","atm-disasters","env-disasters","insect","rodent","atm-disasters","env-disasters"},
										{"insect","rat disaster","forest fire","insect","rat disaster","forest fire","insect","rat disaster"}	};
static const char location[270][20]	=	{"Beijing","Tianjin","Shanghai","Chongqing","Yinchuan","Shizuishan","Wuzhong","Guyuan","Zhongwei",
										"wulumuqi","kelamayi","lasa ","huhehaote","Baotou","Wuhai","Chifeng","Tongliao","eerduosi","hulunbeier",
										"Bayannaoer","Wulanchabu","Nanning","Liuzhou","Guilin","Wuzhou","beihai","Chongzuo","laibin","Hezhou",
										"Yulin","baise","Hechi","Qinzhou ","fangchenggang","Guigang","haerbin","Daqing","qiqihaer","jiamusi","Jixi",
										"Hegang","Shuangyashan","Mudanjiang","Yichun","Qitaihe","Heihe","Suihua","Changchun","Jilin","Siping","Liaoyuan",
										"Tonghua","baishan","Songyuan","Baicheng","Shenyang","Dalian","Anshan","Fushun","Benxi","Dandong","Jinzhou",
										"Yingkou","Fuxin","Liaoyang","Panjin","Tieling","Chaoyang","Huludao ","Shijiazhuang","Tangshan","Handan ",
										"Qinghuangdao","Baoding ","Zhangjiakou","Chengde","Langfang","Cangzhou","Hengshui","Xingtai","Jinan","Qingdao",
										"Zibo ","Zaozhuang","Dongying","Yantai","Weifang","Jining","Taian","Weihai","rizhao","Laiwu","Linyi","dezhou",
										"Liaocheng","Heze","Binzhou","Nanjing","Zhenjiang","Changzhou","Wuxi","Suzhou","Xuzhou ","Lianyungang","Huaian",
										"Yancheng","Yangzhou","Taizhou ","Nantong","Suqian","Hefei","Bengbu","Wuhu","Huainan","Bozhou","Fuyang","Huaibei",
										"Suzhou","Chuzhou","Anqing","Chaohu","Maanshan","Xuancheng","Huangshan","Chizhou","Tongling","Hangzhou","Jiaxing",
										"Huzhou","Ningbo","Jinhua","Wenzhou","Lishui","Shaoxing","Quzhou","Zhoushan","Taizhou","Fuzhou","Xiamen","Quanzhou",
										"Sanming","Nanping","Zhangzhou","Putian","Ningde","Longyan","Guangzhou","Shenzhen","Shantou","Huizhou","Zhuhai",
										"Jieyang","Foshan","Heyuan","Yangjiang","Maoming","Zhanjiang","Meizhou","Zhaoqing","Shaoguan","Chaozhou","Dongguan ",
										"Zhonghan","Qingyuan","Jiangmen","Shanwei","Yunfu","Haikou","Sanya","Kunming","Qujing","Yuxi","Baoshan","Zhaotong",
										"Lijiang","Puer","Lincang","Guiyang","Liupanshui","Zunyi","Anshun","Chengdu","Mianyang","Deyang","Guangyuan","Zigong",
										"Panzhihua","Leshan","Nanchong","Neijiang","Suining","Guangan","Luzhou","Dazhou","Meishan","Yibin","Yaan","Ziyang",
										"Changsha","Zhuzhou","Xiangtan","Hengyang","Yueyang","Chenzhou","Yongzhou","Shaoyang","Huaihua","Changde","Yiyang ",
										"Zhangjiajie","Loudi","Wuhan","Xiangfan","Yichang","Huangshi","Ezhou ","Suizhou","Jingzhou","Jingmen","Shiyan",
										"Xiaogan","Huanggang","Xianning","Zhengzhou","Luoyang","Kaifeng","Luohe","Anyang","Xinxiang","Zhoukou","Sanmenxia",
										"Jiaozuo ","Pingdingshan","Xinyang","Nanyang","Hebi","Puyang","Xuchang","Shangqiu","Zhumadian","Taiyuan","DaTong",
										"Xinzhou","Yangquan","Changzhi","Jincheng","Shuozhou","Jinzhong","Yuncheng","Linfen","Lvliang","Xian ","Xianyang",
										"Tongchuan","Yanan","Baoji","Weinan","Hanzhoung","Ankang","Shangluo","Yulin","Lanzhou","Tianshui","Pingliang",
										"Jiuquan","Jiayuguan","Jinchang","baiyiin","Wuwei","Zhangye"};
static const char name_d[29][20]	=	{
									"liu yu ting ",
									"liu jing ting",
									"zhang shu qi",
									"xiong qian",
									"chen zhi hao ",
									"dong zi heng ",
									"liu ming zhe ",
									"li rui guang ",
									"cheng huan ",
									"he zhuo ",
									"wu tong ",
									"hu si xu",
									"ma wei liang ",
									"wei shuo ",
									"yang ci ",
									"zhang xiao hui ",
									"zhang jun hua",
									"zhang ming ",
									"li mu chen ",
									"tang hui chuan ",
									"hu dong ming ",
									"peng da ",
									"huang zhi qiang ",
									"zhan zhong chen ",
									"guo xue yang ",
									"huang wen jie ",
									"deng yi fu ",
									"cui cun ze ",
									"chen ying fang "};
void __generate_random_data(int N1,int N2,int N3){
	Node *temp1;
	Node *temp2;
	Node *temp3;
	N1%=300;
	N2%=999;
	N3%=999;
	char buffer1[30],buffer2[30],buffer3[30];
	int temp;
	int counter = 0;
	srand(time(0));
	for(int i=0;counter<12;i++){
		srand(time(0));
		node_init(&temp1,node_type_classify);
		temp	=	rand()%7;
		type_classify(temp1)	=	type[temp];
		srand(time(0));
		strcpy(buffer1,name[temp][rand()%8]);
		strcpy(name_classify(temp1),buffer1);
		if(node_insert(temp1)==error_duplicate)
			continue;
		else
			counter++;
		for(int j=0;j<N2/2+rand()%N2;j++){
			node_init(&temp2,node_type_event);
			strcpy(name_event(temp2),buffer1);
			sprintf(buffer2,"%4d%2d%2d%3d",rand()%30+1990,rand()%12+1,rand()%31+1,j+1);
			if(buffer2[4]==' ')	buffer2[4]='0';
			if(buffer2[6]==' ')	buffer2[6]='0';
			if(buffer2[8]==' ')	buffer2[8]='0';
			if(buffer2[9]==' ')	buffer2[9]='0';
			strcpy(serial_event(temp2),buffer2);
			strncpy(buffer3,buffer2,8);
			buffer3[8]='\0';
			sprintf(time_event(temp2),"%8s-%2d:%2d",buffer3,rand()%12+1,rand()%59+1);
			if(time_event(temp2)[9]==' ')	time_event(temp2)[9]='0';
			if(time_event(temp2)[12]==' ')	time_event(temp2)[12]='0';
			level_event(temp2)	=	rand()%4+'1';
			strcpy(location_event(temp2),location[rand()%270]);
			deads_event(temp2)			=	rand()%1000000;
			house_destroyed_event(temp2)=	rand()%1000000;
			pencuniary_loss_event(temp2)=	rand()%10000;
			if(node_insert(temp2))
				continue;
			for(int k=0;k<N3/2+rand()%N3;k++){
				node_init(&temp3,node_type_donate);
				strcpy(serial_donate(temp3),buffer2);
				strcpy(classify_name_donate(temp3),buffer1);
				sprintf(time_donate(temp3),"%4d%2d%2d",rand()%30+1990,rand()%12+1,rand()%31+1);
				if(time_donate(temp3)[4]==' ')	time_donate(temp3)[4]='0';
				if(time_donate(temp3)[6]==' ')	time_donate(temp3)[6]='0';
				type_donate(temp3)	=	rand()%2+'1';
				amount_donate(temp3)=	rand()%1000000;
				strcpy(name_donate(temp3),name_d[rand()%29]);
				if(node_insert(temp3))
					continue;
			}
		}
	}
}
