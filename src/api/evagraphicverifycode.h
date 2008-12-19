/**
 * Copyright (C)2008 George Ang (gnap.an AT gmail.com)
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */ 

#ifndef API_EVAGRAPHICVERIFYCODE_H_
#define API_EVAGRAPHICVERIFYCODE_H_
 

class GraphicVerifyCode {
public:
	GraphicVerifyCode() :
		m_SessionTokenLen ( 0),
		m_SessionToken (NULL),
		m_DataLen  (0),
		m_Data (NULL)
	{
	};
	GraphicVerifyCode(const GraphicVerifyCode &rhs)  :
		m_SessionTokenLen ( 0),
		m_SessionToken (NULL),
		m_DataLen  (0),
		m_Data (NULL)
	{
		*this = rhs; 
	}
	~GraphicVerifyCode(){
		if(m_SessionToken) delete [] m_SessionToken;
		if(m_Data) delete [] m_Data;
	}
	GraphicVerifyCode &operator=(const GraphicVerifyCode &rhs){
		setSessionToken(rhs.m_SessionToken, rhs.m_SessionTokenLen);
		setData(rhs.m_Data, rhs.m_DataLen);
		return *this;
	}
	void setSessionToken(const unsigned char *token, const unsigned short len){
		if(m_SessionToken) delete []m_SessionToken;
		m_SessionToken = new unsigned char [len];
		memcpy(m_SessionToken, token, len);
		m_SessionTokenLen = len;
	};

	void setData(const unsigned char *data, const unsigned short len){
		if(m_Data) delete []m_Data;
		m_Data = new unsigned char [len];
		memcpy(m_Data, data, len);
		m_DataLen = len;
	};

	unsigned short m_SessionTokenLen;
	unsigned char *m_SessionToken;
	unsigned short m_DataLen;
	unsigned char *m_Data;
};



#endif // API_EVAGRAPHICVERIFYCODE_H_
