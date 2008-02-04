/***************************************************************************
 *   Copyright (C) 2005 by yunfan                                          *
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

#include "evahtmlparser.h"
#include "evapacket.h"
#include <arpa/inet.h>
#include <qregexp.h>
#include <qfile.h>
#include <qpixmap.h>

unsigned int EvaHtmlParser::tmpNum = 0;

std::list<CustomizedPic> EvaHtmlParser::convertToHtml(QString &txt, bool isURLOn, bool isAbsImgPath, bool useRealFileName)
{
	// FIXME  by this way, some html code may not be displayed properly
	txt.replace("&amp;", "&amp;amp;");
	txt.replace("&lt;", "&lt;lt;");
	txt.replace("&gt;", "&gt;gt;");
	txt.replace("&quot;", "&quot;quot;");

	txt.replace("/<D>", "/dd");
	txt.replace("/<J>", "/mn");
	txt.replace("/<H>", "/hl");
	txt.replace("/<M>", "/mamao");
	txt.replace("/<QQ>", "/qz");
	txt.replace("/<L>", "/fw");
	txt.replace("/<O>", "/oh");
	txt.replace("/<B>", "/bj");
	txt.replace("/<U>", "/qsh");
	txt.replace("/<W>", "/xig");
	txt.replace("/<!!>", "/xy");
	txt.replace("/<~>", "/duoy");
	txt.replace("/<Z>", "/xr");
	txt.replace("/<*>", "/xixing");
	txt.replace("/<00>", "/nv");
	txt.replace("/<11>", "/nan");
	txt.replace("/<&", "/ztj");
	txt.replace("/&>", "/ytj");
	
	txt.replace("<", "&lt;");
	txt.replace(">", "&gt;");
	txt.replace("\"", "&quot;");
	txt.replace("\n", "<br>");

// 	QRegExp rx2("/(:o|jy|:~|pz|:\\*|se|:\\||fd|dy|:<|ll|:$|hx|:x|bz|:z|shui|:'|dk"
// 				"|:\\-\\||gg|:@|fn|:P|tp|:D|cy|:\\)|wx|:\\(|ng|:\\+|kuk|:#|feid"
// 				"|:Q|zk|:t|tu|;P|tx|;\\-D|ka|;d|baiy|;o|am|:g|jie|\\|\\-\\)|kun"
// 				"|:\\!|jk|:L|lh|:\\>|hanx|:;|db|;f|fendou|:\\-S|zhm|\\?|yiw"
// 				"|;x|xu|;@|yun|:8|zhem|;\\!|shuai|\\!\\!\\!|kl|xx|qiao|bye|zj"
// 				"|go|shan|shake|fad|love|aiq|jump|tiao|find|zhao|&|mm|pig"
// 				"|zt|cat|maom|dog|xg|hug|yb|$|qianc|\\(\\!\\)|dp|cup|bei"
// 				"|cake|dg|li|shd|bome|zhd|kn|dao|footb|zq|music|yy|shit"
// 				"|bb|coffee|kf|eat|fa|pill|yw|rose|mg|fade|dx|kiss|wen"
// 				"|heart|xin|break|xs|meeting|hy|gift|lw|phone|dh|time|sj"
// 				"|email|yj|tv|ds|sun|ty|moon|yl|strong|qiang|weak|ruo"
// 				"|share|ws|v|shl|dd|mn|hl|mamao|qz|fw|oh|bj|qsh|xig|xy|duoy"
// 				"|xr|xixing|nv|nan|"
// 				"|lengh|ch|wb|gz|qdl|huaixiao|zhh|yhh|hq|bs"
// 				"|wq|kk|yx|qq|xia|kelian|cd|pj|lq|pp|sa|pc|bq"
// 				"|gy|qt|cj|an|NO|OK|zhuanquan|kt|ht|ts|hs|jd"
// 				"|jw|xw|zuotaiji|youtaiji)");
	QRegExp rx2(	"/(:\\)|wx|:~|pz|:\\*|se|:\\||fd|8-\\)|dy|:<|ll|:$|hx|:x|bz|:z|shui|:'|dk"
				"|:\\-\\||gg|:@|fn|:P|tp|:D|cy|:o|jy|:\\(|ng|:\\+|kuk|\\-\\-b|lengh|:Q|zk|:t|tu"
				"|;P|tx|;\\-D|ka|;d|baiy|;o|am|:g|jie|\\|\\-\\)|kun|:\\!|jk|:L|lh|:>|hanx|:;|db"
				"|;f|fendou|:\\-S|zhm|\\?|yiw|;x|xu|;@|yun|;8|zhem|;\\!|shuai|\\!\\!\\!|kl|xx|qiao|bye|zj"
				"|wipe|ch|dig|kb|handclap|gz|&\\-\\(|qd|B\\-\\)|huaix|<@|zhh|@>|yhh|:\\-O|hq|>\\-\\||bs|P\\-\\(|wq"
				"|:'\\||kk|X\\-\\)|yx|:\\*|qq|@x|xia|8\\*|kel|pd|cd|<W>|xig|beer|pj|basketb|lq|oo|pp"
				"|coffee|kf|eat|fan|pig|zt|rose|mg|fade|dx|showlove|sa|heart|xin|break|xs|cake|dg|li"
				"|shd|bome|zhd|kn|dao|footb|zq|ladybug|pc|shit|bb|moon|yl|sun|ty|gift|lw|hug|yb|strong|qiang"
				"|weak|ruo|share|ws|v|shl|@\\)|bq|jj|gy|@@|qt|bad|cj|loveu|aini|no|bu|ok|hd"
				"|love|aiq|<L>|fw|jump|tiao|shake|fad|<O>|oh|circle|zhq|kotow|kt|turn|ht|skip|tsh|oY|hsh"
				"|#\\-O|jd|hiphop|jw|kiss|xw|<&|ztj|&>|ytj"
				"|:#|feid|go|shan|find|zhao|&|mm|cat|maom"
				"|dog|xg|\\$|qianc|\\(\\!\\)|dp|cup|bei|music|yy|pill|yw|kiss|wen|metting|hy|phone|dh|time|sj"
				"|email|yj|tv|ds|<D>|dd|<J>|mn|<H>|hl|<M>|mamao|<QQ>|qz|<B>|bj|<U>|qsh|<\\!\\!>|xy"
				"|<~>|duoy|<Z>|xr|<\\*>|xixing|<00>|nv|<11>|nan)");
	QRegExp rx4("(http|ftp)://[a-z0-9._%-]+\\.[a-z0-9._%\\-]+\\.[a-z0-9]{1,4}[a-z0-9._%\\-/\\?=~&#]*");
		rx4.setCaseSensitive(false);
	int pos=0;
	int pos1=0;
	QString smiley;
	int fileIndex;
	QString img;
	QString url;
	QString buffer;

	while(pos>=0){
		pos1 = pos;
		pos = rx4.search(txt, pos);
		url = rx4.cap(0);
		if (pos > -1) {
			buffer = txt.mid(pos1, pos - pos1);
			int pos2 = 0;
			while( pos2 >=0 )
			{
				pos2 = rx2.search(buffer, pos2);
				if (pos2 > -1) {
					smiley = rx2.cap(0);
					QChar suffix = txt[pos + smiley.length() ];
					if( !suffix.isNull() && ( (suffix >= 'a' && suffix <= 'z' ) || (suffix >= 'A' && suffix <= 'Z' ) )){
						pos2 += smiley.length();
						continue;
					}
					fileIndex = EvaUtil::textToFileIndex(smiley.ascii());
					if(fileIndex!=-1){
						img = "<img src=\""+ (isAbsImgPath?(absImagePath+"/"):"") + QString::number(fileIndex)+".gif\">";
						buffer.replace(pos2, smiley.length(), img);
						pos2 += img.length();
					}else
						pos2 += smiley.length();

				}
			}
			txt.replace(pos1, pos - pos1, buffer);
			pos = pos1 + buffer.length();

			pos += strlen(url.ascii());
		} else {

			pos = rx2.search(txt, pos1);
			if (pos > -1) {
				smiley = rx2.cap(0);
				QChar suffix = txt[pos + smiley.length() ];
				if(!suffix.isNull() && ( (suffix >= 'a' && suffix <= 'z' )  || (suffix >= 'A' && suffix <= 'Z' ) )){
					pos += smiley.length();
					continue;
				}
				fileIndex = EvaUtil::textToFileIndex(smiley.ascii());
				if(fileIndex!=-1){
					img = "<img src=\""+ (isAbsImgPath?(absImagePath+"/"):"") + QString::number(fileIndex)+".gif\">";
					txt.replace(pos, smiley.length(), img);
					pos += img.length();
				}else
					pos += smiley.length();
			}
		}
	}
	std::list< CustomizedPic > picList = convertCustomizedPictures(txt, useRealFileName);
	if(isURLOn){
		QString email;
		QString wraped;
		QString aRefBegin = "<a href=\"";
		QString aRefEnd = "</a>";
		QString emailTag = "mailto:";

		QRegExp rx3("\\b[a-z0-9._%-]+@[a-z0-9._%-]+\\.[a-z]{2,4}\\b");
		rx3.setCaseSensitive(false);
		pos = 0;
		while(pos>=0){
			pos = rx3.search(txt, pos);
			if (pos > -1) {
				email = rx3.cap(0);			
				email.stripWhiteSpace();
				wraped = aRefBegin + emailTag + email + "\">" + email + aRefEnd;
				txt.replace(pos, strlen(email.ascii()), wraped);
				pos += strlen(wraped.ascii());
			}
		}
		QString url;
			pos = 0;
			while(pos>=0){
				pos = rx4.search(txt, pos);
				if (pos > -1) {
					url = rx4.cap(0);
					url.stripWhiteSpace();
					wraped = aRefBegin + url + "\">" + url + aRefEnd;
					txt.replace(pos, strlen(url.ascii()), wraped);
					pos += strlen(wraped.ascii());
				}
			}
	}
	bool isIgnore = false;
	for(pos = 0; pos < (int)txt.length(); pos ++ ){
		QChar ch = txt.at(pos);//printf("ch(%d): %c\t", pos,  ch.latin1());
		if(ch == '<') {
			isIgnore = true;
			continue;
		}
		if(ch == '>')  {
			isIgnore = false;
			continue;
		}
		if(ch == ' ' && !isIgnore && isAbsImgPath){
			txt.replace(pos, 1, "&nbsp;");
			pos+= (QString("&nbsp;").length() -1 );
		}
		if(ch == '\t' && !isIgnore && isAbsImgPath){
			txt.replace(pos, 1, "&nbsp;&nbsp;&nbsp;&nbsp;");
			pos+= (QString("&nbsp;&nbsp;&nbsp;&nbsp;").length() - 1);
		}
	}
	return picList;
}

// for personal chatting only
const int EvaHtmlParser::convertToPlainTxt(QString &html, QString &sendFileNameBase)
{
	QRegExp rx("<img src=\\d\\d?\\d?\\.gif >");
	QRegExp rx1("\\d\\d?\\d?");
	QRegExp picRx("<img src=[a-zA-Z0-9_/\\.\\s\\-\\{\\}\\[\\]\\(\\)]{10,100}\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG) >");
	QRegExp picRx1("[a-zA-Z0-9_\\.\\s\\-\\{\\}\\[\\]\\(\\)]{10,100}\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG)");

	int pos=0;
	QString imgTxt, strFile, smiley;
	int fileIndex, len;
	while(pos>=0){
		pos = rx.search(html, pos);
		if (pos > -1) {
			imgTxt = rx.cap(0);
			len = rx.matchedLength();
			int p=0;
			p = rx1.search(imgTxt, 0);
			if(p>-1) 
				strFile = rx1.cap(0);
			fileIndex = strFile.toInt();
			if(fileIndex<0 || fileIndex > QQ_SMILEY_AMOUNT-1) {
				pos += len;
				continue;
			}
			smiley = QString(EvaUtil::fileIndexToText(fileIndex).c_str()) + " ";
			html.replace(imgTxt, smiley);
			pos += smiley.length();
		}
	}
	pos =0;
	int numImage = 0;
	sendFileNameBase = "";
	while(pos>=0){
		pos = picRx.search(html, pos);
		if(pos > -1) {
			imgTxt = picRx.cap(0);
			len = picRx.matchedLength();
			int p=0;
			p = picRx1.search(imgTxt, 0);
			if(p>-1){
				strFile = picRx1.cap(0);
				numImage++;
				if(numImage == 1)
					sendFileNameBase = strFile.left(strFile.length() - 4);;
			}
			smiley = generateSendFormat32(sendFileNameBase, strFile, numImage);
			html.replace(imgTxt, smiley);
			pos += smiley.length();
		}
	}
	
	QString start = "<p>";
	pos = html.find(start, 0);
	if(pos!= -1){
		html = html.right(html.length() - pos - start.length());
	}
	html.replace("<br /></p>\n</body></html>", "");
	html.replace("</p>\n</body></html>", "");
	html.replace("\n","");
	html.replace("<br>", "\n");
	html.replace("<p>", "");
	html.replace("<br />","\n");
	html.replace("</p>", "\n");
	html.replace("&amp;", "&");
	html.replace("&lt;", "<");
	html.replace("&gt;", ">");
	html.replace("&quot;", "\"");
	html.replace("&nbsp;", " ");

	return numImage;
}

// imgNo starts from 1
QString EvaHtmlParser::generateSendFormat32(const QString sendFileNameBase, const QString &file, const int imgNo)
{
	QPixmap pix(absCustomCachesPath + "/" +  file);
	pix.save(absCustomCachesPath + "/" + sendFileNameBase + QString::number(imgNo - 1) + ".jpg", "JPEG", 100);
	//QFile info(absCustomCachesPath + "/" + file);
	//info.remove();

	QString ext = file.right(4); 
	QString extCode;
	if(ext == ".jpg")
		extCode = "A";
	else if(ext == ".gif")
		extCode = "C";
	else
		extCode = "A";

	QChar code = 0x40 + imgNo;  // note that imgNo starts from 1
	QString noCode = code;
	
	QString contents;
	if(imgNo == 1)               // for the first occurred we put its name here
		contents = sendFileNameBase + QString::number(imgNo - 1) + ".jpg"; 
	contents += (extCode + noCode);
	
	return "[ZDY][32]"+contents+"[/32][/ZDY]";
}

std::list< CustomizedPic > EvaHtmlParser::convertCustomizedPictures( QString & text, bool useRealFileName)
{
	QRegExp rx("\\[ZDY\\]\\[[0-9][0-9]\\][^/]+\\[/[0-9][0-9]\\]\\[/ZDY\\]");
	int pos=0;
	QString contents;
	QString img;
	int type;
	while(pos>=0){
		pos = rx.search(text, pos);
		if (pos > -1) {
			contents = rx.cap(0);
			bool ok;
			type = contents.mid(6,2).toInt(&ok);
			CustomizedPic args;
			args.type = 0; // 0 means error;
			args.isExisted = false;
			if(ok){
				switch(type){
				case 32:
					img = processPic32(contents, &args);
					break;
				case 33:
					img = processPic33(contents, &args);
					tmpNum++;
					break;
				case 34:
					img = processPic34(contents);
					break;
				case 36:
					img = processPic36(contents, &args);
					tmpNum++;
					break;
				case 37:
					img = processPic37(contents);
					break;
				default:
					printf("unknown code: %d\n", type);
					img = "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches")+"/unknown.png\">";
				}
				if(useRealFileName){
					text.replace(contents, "<img src=\"" + args.fileName+ "\">");
					pos += args.fileName.length();
				}else{
					text.replace(contents, img);
					pos += img.length();
				}
				if(type == 33 || type == 36) picList.push_back(args);
			} else
				pos+=contents.length();
		}
	}
	std::list< CustomizedPic > retList;
	std::list< CustomizedPic >::iterator iter;
	for(iter=picList.begin(); iter!=picList.end(); ++iter){
		if(!iter->isExisted)
			retList.push_back(*iter);
	}
	return retList;
}

// for 0x32
QString EvaHtmlParser::processPic32( const QString &src, CustomizedPic * args )
{
	QString imgName = src.mid(9, src.length()-9-11);
	
	QChar dot = imgName.at(imgName.length() - 4);
	args->type = 32;
	args->fileName = imgName.left(imgName.length() - ((dot=='.')?0:2));
	//printf("EvaHtmlParser::processPic32 -- args->fileName: %s\n", args->fileName.ascii());
	QFile file(absCustomCachesPath + "/" + imgName);
	if(file.exists()){
		args->tmpFileName = absCustomCachesPath + "/" + imgName;
		args->isExisted = true;
	}else
		args->tmpFileName = "/t_" + imgName;
	
	return "<img src=\"" + args->tmpFileName + "\">";
}

// for 33, only fileName and shortCutName are useful.
QString EvaHtmlParser::processPic33( const QString &src, CustomizedPic * args )
{
	QString contents = src.mid(9, src.length()-9-11);
	//args->isExisted = true;
	args->type = 33;
	args->fileName = contents.mid(0, 36);
	args->shortCutName = contents.right(contents.length() - 36);
	args->tmpFileName = ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/" + "tmp" + QString::number(tmpNum) + ".png";
	QString ret =  "<img src=\"" + args->tmpFileName +"\">";
	return ret;
}

QString EvaHtmlParser::processPic34( const QString &src)
{
	QString contents = src.mid(9, src.length()-9-11);

	int occurredIndex = contents.latin1()[0] - 'A'; 
	if(occurredIndex >= (int)(picList.size())){
		return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown.png\">";
	}
	int index=0;
	std::list< CustomizedPic >::iterator iter;
	for(iter=picList.begin(); iter!=picList.end(); ++iter){
		if(index == occurredIndex){
			return "<img src=\"" + iter->tmpFileName + "\">";
		}
		index++;
	}
	return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown.png\">";
}

QString EvaHtmlParser::processPic36( const QString &src, CustomizedPic * args )
{
	QString contents = src.mid(10, src.length()-9-11-1);// cause this always be first, so ignore first 'e'
	//args->isFirst = true;
	args->type = 36;
	uint pos = 0;
	Q_UINT8 shortLen = contents.latin1()[pos++] - 'A';
	
	// start getting session key,  it's 4 bytes long but represented in ascii expression of hex with 8 bytes long
	bool ok;
	Q_UINT16 sessionLen = contents.mid(pos, 2).toInt(&ok, 16) - 16 - 2; // we have to minus 2, because we need get rid ot these 2 bytes. 16 is ip(8 bytes) & port(8 bytes)
	pos+=2;
	if(!ok){
		args->type = 0;
		return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") +"/unknown_sessionLen2int.png\">";
	}
	// FIXME: we should use sessionLen to get the session string not fix value 8
	QString strSession = contents.mid(pos, 8);
	uint tmp4 = strSession.stripWhiteSpace().toInt(&ok, 16);
	if(!ok){
		args->type = 0;
		return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown_session2int.png\">";
	}
	args->sessionID = tmp4;
	pos+=8;// note sessionLen is 8 
	
	// we sort ip out now
	QString strIP = contents.mid(pos, 8);
	strIP.replace(' ', '0'); // in the case of "10.0.0.1", which these 0s are presented as white spaces
	args->ip = htonl(strIP.toUInt(&ok, 16));
	if(!ok){
		args->type = 0;
		return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown_ip2int.png\">";
	}
	pos+=8;
	// port 
	QString strPort = contents.mid(pos, 8).stripWhiteSpace();
	args->port = strPort.toInt(&ok, 16) & 0xffff; 
	if(!ok){
		args->type = 0;
		return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown_port2int.png\">";
	}
	pos+=8;
	
	QString strFileAgentKey = contents.mid(pos, 16);
	memcpy(args->fileAgentKey, strFileAgentKey.latin1(), 16);
	pos+=16;
	
	args->fileName = contents.mid(pos, strlen(contents.ascii()) - pos - shortLen - 1); // we have to minus the short cut and the last byte 
	
	pos+=strlen(args->fileName.ascii());

	args->shortCutName = contents.mid(pos, shortLen);
	// if file already existed/downloaded just display it :)
	if(absCustomCachesPath){
		QFile file(absCustomCachesPath + "/" + args->fileName);
		if(file.exists()){
			args->tmpFileName = absCustomCachesPath + "/" + args->fileName;
			args->isExisted = true;
		}else
			args->tmpFileName = absCustomCachesPath + "/tmp" + QString::number(tmpNum) + ".png";
	}else
		args->tmpFileName = "~/.eva/customCaches/tmp" + QString::number(tmpNum) + ".png";
	//args->tmpFileName = ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/" + "tmp" + QString::number(tmpNum) + ".png";
	QString ret =  "<img src=\"" + args->tmpFileName +"\">";
	return ret;
}

QString EvaHtmlParser::processPic37( const QString &src)
{
	QString contents = src.mid(9, src.length()-9-11);
	uint pos = 0;
	int occurredIndex = contents.latin1()[pos++] - 'A'; 
	
	//Q_UINT8 shortLen = contents.latin1()[pos++] - 'A';
	
	if(occurredIndex >= (int)picList.size()){
		return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown.png\">";
	}
	int index = 0;
	std::list< CustomizedPic >::iterator iter;
	for(iter=picList.begin(); iter!=picList.end(); ++iter){
		if(index == occurredIndex){
			return "<img src=\"" + iter->tmpFileName + "\">";
		}
		index++;
	}
	return "<img src=\"" + ((absCustomCachesPath)?absCustomCachesPath:"~/.eva/customCaches") + "/unknown.png\">";
}

void EvaHtmlParser::convertToPlainTxt( QString & html, const unsigned int agentSessionID, 
						const unsigned int agentIP, const unsigned short agentPort )
{
	QRegExp rx("<img src=\\d\\d?\\d?\\.gif >");
	QRegExp rx1("\\d\\d?\\d?");
	QRegExp picRx("<img src=[a-zA-Z0-9_/\\.\\s\\-\\{\\}\\[\\]\\(\\)]{10,100}\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG) >");
	QRegExp picRx1("[a-zA-Z0-9_\\.\\s\\-\\{\\}\\[\\]\\(\\)]{10,100}\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG)");
	int pos=0;
	QString imgTxt, strFile, smiley;
	int fileIndex, len;
	
	while(pos>=0){
		pos = rx.search(html, pos);
		if (pos > -1) {
			imgTxt = rx.cap(0);
			len = rx.matchedLength();
			int p=0;
			p = rx1.search(imgTxt, 0);
			if(p>-1) 
				strFile = rx1.cap(0);
			fileIndex = strFile.toInt();
			if(fileIndex<0 || fileIndex > QQ_SMILEY_AMOUNT-1) {
				pos += len;
				continue;
			}
			smiley = QString(EvaUtil::fileIndexToText(fileIndex).c_str()) + " ";
			html.replace(imgTxt, smiley);
			pos += imgTxt.length();
		}
	}
	
	pos =0;
	while(pos>=0){
		pos = picRx.search(html, pos);
		if(pos > -1) {
			imgTxt = picRx.cap(0);
			len = picRx.matchedLength();
			int p=0;
			p = picRx1.search(imgTxt, 0);
			if(p>-1){
				strFile = picRx1.cap(0);
			}
			smiley = generateSendFormat(strFile, agentSessionID, agentIP, agentPort);
			html.replace(imgTxt, smiley);
			pos += smiley.length();
		}
	}
	
	QString start = "<p>";
	pos = html.find(start, 0);
	if(pos!= -1){
		html = html.right(html.length() - pos - start.length());
	}
	html.replace("<br /></p>\n</body></html>", "");
	html.replace("</p>\n</body></html>", "");
	html.replace("\n","");
	html.replace("<br>", "\n");
	html.replace("<p>", "");
	html.replace("<br />","\n");
	html.replace("</p>", "\n");
	html.replace("&amp;", "&");
	html.replace("&lt;", "<");
	html.replace("&gt;", ">");
	html.replace("&quot;", "\"");
}

QString EvaHtmlParser::generateSendFormat( QString & fileName, const unsigned int agentSessionID, 
			const unsigned int agentIP, const unsigned short agentPort )
{
	QString shortcutStr = "";
	if(!fileName.startsWith("{"))
		shortcutStr = (strlen(fileName.ascii()) > 7)?(fileName.mid(1,6)):(fileName);
	//QString shortcutStr = "abc";
	QString lenStr;
	lenStr.sprintf("%3d",strlen(fileName.ascii()) + 50 + strlen(shortcutStr.ascii()));
	
	QString sessionStr;
	sessionStr.sprintf("%8x", agentSessionID);
	
	QString ipStr;
	QString tmpStr;
	char *tmp = new char[4];
	memcpy(tmp, &agentIP, 4);
	for(int i = 0; i <4; i++){
		tmpStr.sprintf("%02x", (Q_UINT8)tmp[i]);
		ipStr+=tmpStr;
	}
	delete []tmp;
	
	QString portStr;
	portStr.sprintf("%8x", agentPort);
	
	QString contents = "[36]";
	contents+="e"; // new pic
	int scLen = strlen(shortcutStr.ascii());
	char scCh = 'A' + scLen;
	contents+=scCh; // length of short cut.
	contents+="1A"; // following length
	contents+=sessionStr;
	contents+=ipStr;
	contents+=portStr;
	tmp = new char[17];
	memcpy(tmp, Packet::getFileAgentKey(), 16);
	tmp[16]=0x00;
	contents+=QString(tmp);
	delete []tmp;
	contents+=fileName;
	contents+=shortcutStr;
	contents+="A";
	
	return "[ZDY]"+contents+"[/36][/ZDY]";
}

void EvaHtmlParser::parseToAbsPath( QString & html, const QString absPath )
{
	QRegExp rx("<img src=\\d\\d?\\d?\\.gif >");
	QRegExp rx1("\\d\\d?\\d?");
	QRegExp picRx("<img src=[a-zA-Z0-9_/\\.\\s\\-\\{\\}\\[\\]\\(\\)]+\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG) >");
	QRegExp picRx1("[a-zA-Z0-9_\\.\\s\\-\\{\\}\\[\\]\\(\\)]+\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG)");
	int pos=0, len = 0, fileIndex = 0;
	QString imgTxt, strFile, smiley;
	
	while(pos>=0){
		pos = rx.search(html, pos);
		if (pos > -1) {
			imgTxt = rx.cap(0);
			len = rx.matchedLength();
			int p=0;
			p = rx1.search(imgTxt, 0);
			if(p>-1) 
				strFile = rx1.cap(0);
			smiley = "<img src=\"" + absImagePath + "/" + strFile + ".gif\">";
			html.replace(imgTxt, smiley);
			pos += smiley.length();
		}
	}
	pos = 0;
	while(pos>=0){
		pos = picRx.search(html, pos);
		if(pos > -1) {
			imgTxt = picRx.cap(0);
			int p=0;
			p = picRx1.search(imgTxt, 0);
			if(p>-1)
				strFile = picRx1.cap(0);
			smiley = absPath + "/" + strFile;
			html.replace(strFile, smiley);
			pos += smiley.length();
		}
	}
	QString start = "<p>";
	pos = html.find(start, 0);
	if(pos!= -1){
		html = html.right(html.length() - pos - start.length());
	}
	html.replace("<br /></p>\n</body></html>", "");
	html.replace("</p>\n</body></html>", "");
}

std::list< QString > EvaHtmlParser::getCustomImages( const QString html )
{
	std::list<QString> picList;
	QRegExp picRx("<img src=[a-zA-Z0-9_\\.\\s\\-\\{\\}\\[\\]\\(\\)]{10,100}\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG) >");
	QRegExp picRx1("[a-zA-Z0-9_\\.\\s\\-\\{\\}\\[\\]\\(\\)]{10,100}\\.(jpg|JPG|gif|GIF|bmp|BMP|jpeg|JPEG)");
	int pos=0;
	QString imgTxt, strFile, smiley;
	while(pos>=0){
		pos = picRx.search(html, pos);
		if(pos > -1) {
			imgTxt = picRx.cap(0);
			int p=0;
			p = picRx1.search(imgTxt, 0);
			if(p>-1){
				strFile = picRx1.cap(0);
				picList.push_back(strFile);
			}
			pos += strFile.length();
		}
	}
	return picList;
}

