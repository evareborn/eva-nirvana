/***************************************************************************
 *   Copyright (C) 2004 by yunfan                                          *
 *   yunfan_zg@163.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "evautil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include "md5.h"
#include <cstring>
#include <cstdlib>

// const char EvaUtil::smileyMap[QQ_SMILEY_AMOUNT] = {
// 	0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,
// 	0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x73,
// 	0x74,0x75,0x76,0x77,0x8a,0x8b,0x8c,0x8d,
// 	0x8e,0x8f,0x78,0x79,0x7a,0x7b,0x90,0x91,
// 	0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,
// 	0x59,0x5a,0x5c,0x58,0x57,0x55,0x7c,0x7d,
// 	0x7e,0x7f,0x9a,0x9b,0x60,0x67,0x9c,0x9d,
// 	0x9e,0x5e,0x9f,0x89,0x80,0x81,0x82,0x62,
// 	0x63,0x64,0x65,0x66,0x83,0x68,0x84,0x85,
// 	0x86,0x87,0x6b,0x6e,0x6f,0x70,0x88,0xa0,
// 	0x50,0x51,0x52,0x53,0x54,0x56,0x5b,0x5d,
// 	0x5f,0x61,0x69,0x6a,0x6c,0x6d,0x71,0x72,
// 	0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,
// 	0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,
// 	0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,
// 	0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,
// 	0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7
// };

const char EvaUtil::smileyMap[QQ_SMILEY_AMOUNT] = {
	0x4f, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x41, 0x73,
	0x74, 0xa1, 0x76, 0x77, 0x8a, 0x8b, 0x8c, 0x8d,
	0x8e, 0x8f, 0x78, 0x79, 0x7a, 0x7b, 0x90, 0x91,
	0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
	0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9,
	0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1,
	0x61, 0xb2, 0xb3, 0xb4, 0x80, 0x81, 0x7c, 0x62,
	0x63, 0xb5, 0x65, 0x66, 0x67, 0x9c, 0x9d, 0x9e,
	0x5e, 0xb6, 0x89, 0x6e, 0x6b, 0x68, 0x7f, 0x6f,
	0x70, 0x88, 0xa0, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
	0xbc, 0xbd, 0x5c, 0x56, 0x58, 0x5a, 0x5b, 0xbe,
	0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6,
	0xc7,
	// the rest are not selectable in QQ2007II final, but
	// still visible for incoming message
	0x75, 0x59, 0x57, 0x55, 0x7d, 0x7e, 0x9a, 0x9b,
	0x60, 0x9f, 0x82, 0x64, 0x83, 0x84, 0x85, 0x86,
	0x87, 0x50, 0x51, 0x52, 0x53, 0x54, 0x5d, 0x5f,
	0x69, 0x6a, 0x6c, 0x6d, 0x71, 0x72
};
char EvaUtil::md5Buf[KEY_LENGTH];

EvaUtil::textMap EvaUtil::map[QQ_SMILEY_AMOUNT];

EvaUtil::EvaUtil()
{
	initMap();
}

char *EvaUtil::doMd5(char *in, int len)
{
	md5_state_t ctx;
	md5_init(&ctx);
	md5_append(&ctx, (md5_byte_t *)in, len);
	md5_finish(&ctx, (md5_byte_t *)md5Buf);
	return md5Buf;
}

char *EvaUtil::doMd5Md5(char *in, int len)
{
	doMd5(in, len);
	doMd5(md5Buf, KEY_LENGTH);
	return md5Buf;
}

std::string EvaUtil::smileyToText(const char smileyCode)
{
	printf("0x%2x, ", 0xff&smileyCode);
	int i = smileyToFileIndex(smileyCode);
	std::string text = map[i].py;
	return text;
}

char EvaUtil::textToSmiley(const std::string &textTag)
{
	std::string strEn, strPy;
	for(int i=0; i<QQ_SMILEY_AMOUNT; i++){
		strEn = map[i].en;
		strPy = map[i].py;
		if(strEn == textTag || strPy == textTag){
			return smileyMap[i];
		}
	}
	return 0;
}

int EvaUtil::textToFileIndex(const std::string &textTag)
{
	char s = textToSmiley(textTag);
	return smileyToFileIndex(s);
}

int EvaUtil::smileyToFileIndex( const char smileyCode)
{
	for(int i=0; i<QQ_SMILEY_AMOUNT; i++){
		if(smileyMap[i]==smileyCode)
			return i;
	}
        return -1;
}

std::string EvaUtil::fileIndexToText( const int fileIndex)
{
	if(fileIndex<0 || fileIndex > QQ_SMILEY_AMOUNT - 1 ) return "";
	return map[fileIndex].py;
}

std::string EvaUtil::convertToSend( const std::string & text)
{
	std::string converted = "";
	int offset=0;
	char smileyTag = 0x14;
	char customTag = 0x15;
	bool isFirst32 = true;
	std::string code32FileTag = "";
	char seperator32_1 = 0x13;
	char seperator32_2 = 0x4c;
	for(uint i=0; i< text.length(); i++){
		if(text[i] == '/'){
			offset = i;
			while(text[offset] != 0x00 && text[++offset]!=' ');
			if((offset - i)< 14){
				char code = textToSmiley(text.substr(i,offset-i));
				if(code){
					converted += smileyTag;
					converted += code;
					i=offset;
					continue;
				}
			}
			converted+=text[i];
			continue;
		}else{
			if(text[i] == '['){
				std::string zdyTag = text.substr(i, 5);
				if(zdyTag == "[ZDY]"){
					offset = text.find("[/ZDY]", i);
					std::string zdyType = text.substr(i+6, 2);
					zdyTag = text.substr(i+5+4, offset-i-14);
					std::string sendFormat;
					sendFormat += customTag;
					if(zdyType == "32"){
						if(isFirst32){
							code32FileTag = zdyTag.substr(0, zdyTag.length() - 7);
							code32FileTag += seperator32_1;
							code32FileTag += seperator32_2;
							isFirst32 = false;
						}
						sendFormat += "2"; // 0x32
						sendFormat += zdyTag.substr(zdyTag.length() - 2, 2);
						sendFormat += "999999";
					} else	if(zdyType == "36"){
						sendFormat += "6"; // note: at the moment, we only use type 6
						int len = zdyTag.length() + 5; // the len includes tag(1) and type(1) and itself(3)
						char *strLen = new char[4];
						sprintf(strLen, "%3d", len);
						sendFormat += strLen;
						delete strLen;
						sendFormat += zdyTag;
					}
					converted += sendFormat;//printf("sendFormat:%s\n",sendFormat.c_str());
					i += (offset - i + 5);
					continue;
				}
			}
		}
		converted+=text[i];
	}
	if(!isFirst32)
		converted = code32FileTag + converted; 
	return converted;
}

std::string EvaUtil::convertToSend(const std::string &text, bool *hasImage)
{
	std::string converted = "";
	int offset=0;
	char smileyTag = 0x14;
	char customTag = 0x15;
	bool isFirst32 = true;
	std::string code32FileTag = "";
	char seperator32_1 = 0x13;
	char seperator32_2 = 0x4c;
	for(uint i=0; i< text.length(); i++){
		if(text[i] == '/'){
			offset = i;
			while(text[offset] != 0x00 && text[++offset]!=' ');
			if((offset - i)<14){
				char code = textToSmiley(text.substr(i,offset-i));
				if(code){
					converted += smileyTag;
					converted += code;
					i=offset;
					continue;
				}
			}
			converted+=text[i];
			continue;
		}else{
			if(text[i] == '['){
				std::string zdyTag = text.substr(i, 5);
				if(zdyTag == "[ZDY]"){
					offset = text.find("[/ZDY]", i);
					std::string zdyType = text.substr(i+6, 2);
					zdyTag = text.substr(i+5+4, offset-i-14);
					std::string sendFormat;
					sendFormat += customTag;
					if(zdyType == "32"){
						if(isFirst32){
							code32FileTag = zdyTag.substr(0, zdyTag.length() - 7);
							code32FileTag += seperator32_1;
							code32FileTag += seperator32_2;
							isFirst32 = false;
						}
						sendFormat += "2"; // 0x32
						sendFormat += zdyTag.substr(zdyTag.length() - 2, 2);
						sendFormat += "999999";
					} else	if(zdyType == "36"){
						sendFormat += "6"; // note: at the moment, we only use type 6
						int len = zdyTag.length() + 5; // the len includes tag(1) and type(1) and itself(3)
						char *strLen = new char[4];
						sprintf(strLen, "%3d", len);
						sendFormat += strLen;
						delete strLen;
						sendFormat += zdyTag;
					}
					converted += sendFormat;
					i += (offset - i + 5);
					continue;
				}
			}
		}
		converted+=text[i];
	}
	if(!isFirst32){
		converted = code32FileTag + converted; 
		*hasImage = true;
	} else
		*hasImage = false;
	return converted;
}

void EvaUtil::initMap()
{
	strcpy(map[0].en, "/:)"); strcpy(map[0].py, "/wx");
	strcpy(map[1].en, "/:~"); strcpy(map[1].py, "/pz");
	strcpy(map[2].en, "/:*"); strcpy(map[2].py, "/se");
	strcpy(map[3].en, "/:|"); strcpy(map[3].py, "/fd");
	strcpy(map[4].en, "/8-)"); strcpy(map[4].py, "/dy");
	strcpy(map[5].en, "/:<"); strcpy(map[5].py, "/ll");
	strcpy(map[6].en, "/:$"); strcpy(map[6].py, "/hx");
	strcpy(map[7].en, "/:x"); strcpy(map[7].py, "/bz");
	strcpy(map[8].en, "/:z"); strcpy(map[8].py, "/shui");
	strcpy(map[9].en, "/:'"); strcpy(map[9].py, "/dk");
	
	strcpy(map[10].en, "/:-|"); strcpy(map[10].py, "/gg");
	strcpy(map[11].en, "/:@"); strcpy(map[11].py, "/fn");
	strcpy(map[12].en, "/:P"); strcpy(map[12].py, "/tp");
	strcpy(map[13].en, "/:D"); strcpy(map[13].py, "/cy");
	strcpy(map[14].en,"/:o"); strcpy(map[14].py, "/jy");
	strcpy(map[15].en, "/:("); strcpy(map[15].py, "/ng");
	strcpy(map[16].en, "/:+"); strcpy(map[16].py, "/kuk");
	strcpy(map[17].en, "/--b"); strcpy(map[17].py, "/lengh"); // 鍐锋睏锛�/lh" inuse	
	strcpy(map[18].en, "/:Q"); strcpy(map[18].py, "/zk");
	strcpy(map[19].en, "/:t"); strcpy(map[19].py, "/tu");
	
		
	strcpy(map[20].en, "/;P"); strcpy(map[20].py, "/tx");
	strcpy(map[21].en, "/;-D"); strcpy(map[21].py, "/ka");
	strcpy(map[22].en, "/;d"); strcpy(map[22].py, "/baiy");
	strcpy(map[23].en, "/;o"); strcpy(map[23].py, "/am");
	strcpy(map[24].en, "/:g"); strcpy(map[24].py, "/jie");
	strcpy(map[25].en, "/|-)"); strcpy(map[25].py, "/kun");
	strcpy(map[26].en, "/:!"); strcpy(map[26].py, "/jk");
	strcpy(map[27].en, "/:L"); strcpy(map[27].py, "/lh");
	strcpy(map[28].en, "/:>"); strcpy(map[28].py, "/hanx");
	strcpy(map[29].en, "/:;"); strcpy(map[29].py, "/db");
	
	strcpy(map[30].en, "/;f"); strcpy(map[30].py, "/fendou");
	strcpy(map[31].en, "/:-S"); strcpy(map[31].py, "/zhm");
	strcpy(map[32].en, "/?"); strcpy(map[32].py, "/yiw");
	strcpy(map[33].en, "/;x"); strcpy(map[33].py, "/xu");
	strcpy(map[34].en, "/;@"); strcpy(map[34].py, "/yun");
	strcpy(map[35].en, "/:8"); strcpy(map[35].py, "/zhem");
	strcpy(map[36].en, "/;!"); strcpy(map[36].py, "/shuai");
	strcpy(map[37].en, "/!!!"); strcpy(map[37].py, "/kl");
	strcpy(map[38].en, "/xx"); strcpy(map[38].py, "/qiao");
	strcpy(map[39].en, "/bye"); strcpy(map[39].py, "/zj");
	
	strcpy(map[40].en, "/wipe"); strcpy(map[40].py, "/ch"); // 鎿︽睏
	strcpy(map[41].en, "/dig"); strcpy(map[41].py, "/kb"); // 鎸栭蓟
	strcpy(map[42].en, "/handclap"); strcpy(map[42].py, "/gz"); // 榧撴帉
	strcpy(map[43].en, "/&-("); strcpy(map[43].py, "/qd"); // 绯楀ぇ浜�	strcpy(map[44].en, "/B-)"); strcpy(map[44].py, "/huaix"); // 澹炵瑧锛�/hx" inuse
	strcpy(map[45].en, "/<@"); strcpy(map[45].py, "/zhh"); // 宸﹀摷鍝�	strcpy(map[46].en, "/@>"); strcpy(map[46].py, "/yhh"); // 鍙冲摷鍝�	strcpy(map[47].en, "/:-O"); strcpy(map[47].py, "/hq"); // 鍝堟瑺
	strcpy(map[48].en, "/>-|"); strcpy(map[48].py, "/bs"); // 閯欒
	strcpy(map[49].en, "/P-("); strcpy(map[49].py, "/wq"); // 濮斿眻

	strcpy(map[50].en, "/:'|"); strcpy(map[50].py, "/kk"); // 蹇摥浜�	strcpy(map[51].en, "/X-)"); strcpy(map[51].py, "/yx"); // 闄伴毆
	strcpy(map[52].en, "/:*"); strcpy(map[52].py, "/qq"); // 瑕Κ
	strcpy(map[53].en, "/@x"); strcpy(map[53].py, "/xia"); // 鍤�	strcpy(map[54].en, "/8*"); strcpy(map[54].py, "/kel"); // 鍙啇
	strcpy(map[55].en, "/pd"); strcpy(map[55].py, "/cd"); // 鑿滃垁	
	strcpy(map[56].en, "/<W>"); strcpy(map[56].py, "/xig");
	strcpy(map[57].en, "/beer"); strcpy(map[57].py, "/pj"); // 鍟ら厭
	strcpy(map[58].en, "/basketb"); strcpy(map[58].py, "/lq"); // 绫冪悆
	strcpy(map[59].en, "/oo"); strcpy(map[59].py, "/pp"); // 涔掍箵
	
		
	strcpy(map[60].en, "/coffee"); strcpy(map[60].py, "/kf");
	strcpy(map[61].en, "/eat"); strcpy(map[61].py, "/fan");
	strcpy(map[62].en, "/pig"); strcpy(map[62].py, "/zt");
	strcpy(map[63].en, "/rose"); strcpy(map[63].py, "/mg");
	strcpy(map[64].en, "/fade"); strcpy(map[64].py, "/dx");
	strcpy(map[65].en, "/showlove"); strcpy(map[65].py, "/sa"); // 绀烘剾
	strcpy(map[66].en, "/heart"); strcpy(map[66].py, "/xin");
	strcpy(map[67].en, "/break"); strcpy(map[67].py, "/xs");
	strcpy(map[68].en, "/cake"); strcpy(map[68].py, "/dg");
	strcpy(map[69].en, "/li"); strcpy(map[69].py, "/shd");
	
	strcpy(map[70].en, "/bome"); strcpy(map[70].py, "/zhd");
	strcpy(map[71].en, "/kn"); strcpy(map[71].py, "/dao");
	strcpy(map[72].en, "/footb"); strcpy(map[72].py, "/zq");
	strcpy(map[73].en, "/ladybug"); strcpy(map[73].py, "/pc"); // 鐡㈣煵
	strcpy(map[74].en, "/shit"); strcpy(map[74].py, "/bb");
	strcpy(map[75].en, "/moon"); strcpy(map[75].py, "/yl");
	strcpy(map[76].en, "/sun"); strcpy(map[76].py, "/ty");
	strcpy(map[77].en, "/gift"); strcpy(map[77].py, "/lw");
	strcpy(map[78].en, "/hug"); strcpy(map[78].py, "/yb");
	strcpy(map[79].en, "/strong"); strcpy(map[79].py, "/qiang");
	
		
	strcpy(map[80].en, "/weak"); strcpy(map[80].py, "/ruo");
	strcpy(map[81].en, "/share"); strcpy(map[81].py, "/ws");
	strcpy(map[82].en, "/v"); strcpy(map[82].py, "/shl");
	strcpy(map[83].en, "/@)"); strcpy(map[83].py, "/bq"); // 鎶辨嫵
	strcpy(map[84].en, "/jj"); strcpy(map[84].py, "/gy"); // 鍕惧紩
	strcpy(map[85].en, "/@@"); strcpy(map[85].py, "/qt"); // 鎷抽牠
	strcpy(map[86].en, "/bad"); strcpy(map[86].py, "/cj"); // 宸媮
	strcpy(map[87].en, "/loveu"); strcpy(map[87].py, "/aini"); // 鎰涗綘
	strcpy(map[88].en, "/no"); strcpy(map[88].py, "/bu"); // NO
	strcpy(map[89].en, "/ok"); strcpy(map[89].py, "/hd"); // OK
	
	strcpy(map[90].en, "/love"); strcpy(map[90].py, "/aiq");
	strcpy(map[91].en, "/<L>"); strcpy(map[91].py, "/fw");
	strcpy(map[92].en, "/jump"); strcpy(map[92].py, "/tiao");
	strcpy(map[93].en, "/shake"); strcpy(map[93].py, "/fad");
	strcpy(map[94].en, "/<O>"); strcpy(map[94].py, "/oh");
	strcpy(map[95].en, "/circle"); strcpy(map[95].py, "/zhq"); // 杞夊湀
	strcpy(map[96].en, "/kotow"); strcpy(map[96].py, "/kt"); // 纾曢牠
	strcpy(map[97].en, "/turn"); strcpy(map[97].py, "/ht"); // 鍥為牠
	strcpy(map[98].en, "/skip"); strcpy(map[98].py, "/tsh"); // 璺崇供
	strcpy(map[99].en, "/oY"); strcpy(map[99].py, "/hsh"); // 鎻墜

	strcpy(map[100].en, "/#-O"); strcpy(map[100].py, "/jd"); // 婵�鍕�	strcpy(map[101].en, "/hiphop"); strcpy(map[101].py, "/jw"); // 琛楄垶
	strcpy(map[102].en, "/kiss"); strcpy(map[102].py, "/xw"); // 鐛诲惢
	strcpy(map[103].en, "/<&"); strcpy(map[103].py, "/ztj"); // 宸﹀お妤�	strcpy(map[104].en, "/&>"); strcpy(map[104].py, "/ytj"); // 鍙冲お妤�
	strcpy(map[105].en, "/:#"); strcpy(map[105].py, "/feid");
	strcpy(map[106].en, "/go"); strcpy(map[106].py, "/shan");
	strcpy(map[107].en, "/find"); strcpy(map[107].py, "/zhao");
	strcpy(map[108].en, "/&"); strcpy(map[108].py, "/mm");
	strcpy(map[109].en, "/cat"); strcpy(map[109].py, "/maom");

	strcpy(map[110].en, "/dog"); strcpy(map[110].py, "/xg");
	strcpy(map[111].en, "/$"); strcpy(map[111].py, "/qianc");
	strcpy(map[112].en, "/(!)"); strcpy(map[112].py, "/dp");
	strcpy(map[113].en, "/cup"); strcpy(map[113].py, "/bei");
	strcpy(map[114].en, "/music"); strcpy(map[114].py, "/yy");
	strcpy(map[115].en, "/pill"); strcpy(map[115].py, "/yw");
	strcpy(map[116].en, "/kiss"); strcpy(map[116].py, "/wen");
	strcpy(map[117].en, "/meeting"); strcpy(map[117].py, "/hy");
	strcpy(map[118].en, "/phone"); strcpy(map[118].py, "/dh");
	strcpy(map[119].en, "/time"); strcpy(map[119].py, "/sj");

	strcpy(map[120].en, "/email"); strcpy(map[120].py, "/yj");
	strcpy(map[121].en, "/tv"); strcpy(map[121].py, "/ds");
	strcpy(map[122].en, "/<D>"); strcpy(map[122].py, "/dd");
	strcpy(map[123].en, "/<J>"); strcpy(map[123].py, "/mn");
	strcpy(map[124].en, "/<H>"); strcpy(map[124].py, "/hl");
	strcpy(map[125].en, "/<M>"); strcpy(map[125].py, "/mamao");
	strcpy(map[126].en, "/<QQ>"); strcpy(map[126].py, "/qz");
	strcpy(map[127].en, "/<B>"); strcpy(map[127].py, "/bj");
	strcpy(map[128].en, "/<U>"); strcpy(map[128].py, "/qsh");
	strcpy(map[129].en, "/<!!>"); strcpy(map[129].py, "/xy");

	strcpy(map[130].en, "/<~>"); strcpy(map[130].py, "/duoy");
	strcpy(map[131].en, "/<Z>"); strcpy(map[131].py, "/xr");
	strcpy(map[132].en, "/<*>"); strcpy(map[132].py, "/xixing");
	strcpy(map[133].en, "/<00>"); strcpy(map[133].py, "/nv");
	strcpy(map[134].en, "/<11>"); strcpy(map[134].py, "/nan");


};

void EvaUtil::calcSuns( const unsigned short level, int * suns, int * moons, int * stars )
{
	*suns = 0;
	*moons = 0;
	*stars = 0;
	
	*suns = level/16;
	*moons = (level%16)/4;
	*stars = level%4;
}

std::string EvaUtil::customSmileyToText( const char * buf, int * smileyLength, const char *uuid)
{
	if(buf[0] != 0x15) return "";
	
	char *strTmp = new char[1000];
	std::string strRet = "[ZDY]";
	int pos=1;
	switch( buf[pos++] ){
	case 0x32:{
		if(!uuid) break;
		char ext = buf[pos++]; // ignore 1 byte, 'A':jpg, 'C':gif
		char seq = buf[pos++] - 0x11; // the image tail sequence
		pos += 6; // "999999", always be

		strRet += "[32]";
		strRet += uuid;
		strRet += seq;
		switch(ext){
		case 'A':
			strRet += ".jpg";
			break;
		case 'C':
			strRet += ".gif";
		default:
			printf("EvaUtil::customSmileyToText -- unknown file extension : 0x%2x\n", 0xff&ext);
			break;
		}
		strRet += "[/32]";
		}
		break;
	case 0x33:{
		pos++; // ignore 1 unknown byte
		memcpy(strTmp, buf+pos, 32 + 1 + 3); // copy filename, contains 32(md5 of the file) + 1('.') + 3("GIF')
		strTmp[32 + 1 + 3] = 0x00;
		pos += (32+1+3);
		
		strRet += "[33]";
		strRet += std::string(strTmp);
		
		int len = (buf[pos++] - 'A') & 0xff;
		memcpy(strTmp, buf+pos, len);
		strTmp[len] = 0x00;
		pos += len; 
		
		strRet += std::string(strTmp);
		strRet += "[/33]";
		}
		break;
	case 0x34:{
		strRet += "[34]";
		strRet += buf[pos++];
		strRet += "[/34]";
		}
		break;
	case 0x36:{
		char *partLen = new char[4];
		int lenLen = 0;
		while(buf[pos + lenLen] == 0x20){
			partLen[lenLen++] = '0';
		};
		memcpy(partLen + lenLen, buf + pos + lenLen, 3 - lenLen);
		partLen[3]=0x00;
		int len = atoi(partLen);printf("36 len:%d\n",len);
		delete partLen;
		
		strRet += "[36]";
		memcpy(strTmp, buf+pos+3, len - pos - 3);
		strTmp[len - pos - 3] = 0x00;
		strRet += strTmp;
		strRet += "[/36]";
		
		pos = len; // ignore 0x15 and 0x36 and 3 bytes(length of this part)
		}
		break;
	case 0x37:{
		char *partLen = new char[4];
		int lenLen = 0;
		while(buf[pos + lenLen] == 0x20){
			partLen[lenLen++] = '0';
		};
		memcpy(partLen + lenLen, buf + pos + lenLen, 3 - lenLen);
		partLen[3]=0x00;
		int len = atoi(partLen);printf("37 len:%d\n",len);
		delete partLen;
		
		strRet += "[37]";
		memcpy(strTmp, buf+pos+3, len -pos-3);
		strTmp[len - pos - 3] = 0x00;
		strRet += strTmp;
		strRet += "[/37]";
		
		pos = len;
		}
		break;
	default:
		sprintf(strTmp,"%d", buf[pos-1]);
		strRet += "[";
		strRet += strTmp;
		strRet += "]";
		break;
	}
	delete strTmp;
	*smileyLength = pos;
	strRet += "[/ZDY]";
// 	printf("DONE\n--%s\n", strRet.c_str());
	return strRet;
}

int EvaUtil::write16(unsigned char *buf, const unsigned short value )
{
	unsigned short tmp = htons(value);
	memcpy(buf, &tmp, 2);
	return 2;
}

int EvaUtil::write32( unsigned char *buf, const unsigned int value )
{
	unsigned int tmp = htonl(value);
	memcpy(buf, &tmp, 4);
	return 4;
}

unsigned short EvaUtil::read16( const unsigned char * buf )
{
	unsigned short tmp;
	memcpy(&tmp, buf, 2);
	return ntohs(tmp);
}

unsigned int EvaUtil::read32( const unsigned char * buf )
{
	unsigned int tmp;
	memcpy(&tmp, buf, 4);
	return ntohl(tmp);
}
