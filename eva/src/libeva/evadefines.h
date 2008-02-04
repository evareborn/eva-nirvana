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
 
#ifndef LIBEVADEFINES_H
#define LIBEVADEFINES_H

/** add a type define for uint in case that some platform dose not has type uint */
typedef unsigned int uint;

/** Visual Studio .NET 2002/2003 Additional Definitions */
#ifdef WIN32
#pragma warning(disable: 4309 4800)
typedef unsigned char uint8_t;
#include <malloc.h>
#endif

/**
 * define all constants for eva use */
/** the unknown part in login request appended data, after this, all 0x00s*/
static const unsigned char login_appended_2_72[71]= {
			0x01, 0x2e, 0x2c, 0x69,  0xb4, 0x00, 0x10, 0x6b,
			0xc3, 0xe9, 0x92, 0xc3,  0x50, 0x7a, 0x3a, 0x77,
			0x4d, 0xad, 0x69, 0x0c,  0x04, 0xc3, 0x5f, 0x00,
			0x00, 0x00, 0x00, 0x00,  0x01, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
			0x02, 0xc5, 0x17, 0xea,  0x05, 0x00, 0x10, 0x5d,
			0xef, 0x8a, 0xfe, 0xe6,  0x10, 0x37, 0xf8, 0xff,
			0xcc, 0x52, 0x58, 0x62,  0x42, 0xa0, 0x91
			};


/**  eva, URL of applying free qq acount */
static const char QQ_APPLY_FREE_ACOUNT_URL[] =
        "http://jump.qq.com/clienturl_239?ADUIN=0&ADSESSION=0&ADTAG=CLIENT.QQ.1595_LoginWindow.0";

static const char QQ_FORGET_PASSWORD_URL[] =
        "http://dna.qq.com/cgi-bin/console_forgetPW?reLogin=true&ADUIN=0&ADSESSION=0&ADTAG=CLIENT.QQ.1595_LoginWindow.0";

static const char QQ_FORGET_PW_URL[] =
        "http://service.tencent.com/reg/getpassword.shtml?ADUIN=0&ADSESSION=0&ADTAG=CLIENT.QQ.1595_LoginWindow.0";

static const char QQ_PW_PROTECT_URL[] =
        "http://jump.qq.com/clienturl_8?source=8&ADUIN=0&ADSESSION=0&ADTAG=CLIENT.QQ.1595_LoginWindow.0";


/// all version numbers takes 2 bytes( short)

/**  client GB QQ2000c build 630 */
#define QQ_CLIENT_062E = 0x062E
/** client En QQ2000c build 305 */
#define QQ_CLIENT_072E   0x072E
/** client En QQ2000c build 630 */
#define QQ_CLIENT_0801   0x0801
/** client Gb QQ2003c build 0808 */
#define QQ_CLIENT_0A1D   0x0A1D
/** client Gb QQ2003c build 0925 */
#define QQ_CLIENT_0B07   0x0B07
/** client GB QQ2003iii build 0117 */
#define QQ_CLIENT_0B2F   0x0B2F
/** GB QQ2003iii build 0304 (offical release) */
#define QQ_CLIENT_0B35   0x0B35
/** QQ20003  */
#define QQ_CLIENT_0B37   0x0B37
/** QQ2004 */
#define QQ_CLIENT_0C0B   0x0C0B
/** QQ2004 preview*/
#define QQ_CLIENT_0C0D   0x0C0D
/** QQ2004 */
#define QQ_CLIENT_0C21   0x0C21
/** QQ2004II */
#define QQ_CLIENT_0C49   0x0C49
/** QQ2005 beta1 */
#define QQ_CLIENT_0D05   0x0D05
/** QQ2005 beta2 */
#define QQ_CLIENT_0D51   0x0D51
/** QQ2005 */
#define QQ_CLIENT_0D61   0x0D61
/** QQ2005 ? */
#define QQ_CLIENT_0E1B   0x0E1B
/** ? */
#define QQ_CLIENT_05A5   0x05A5
/** QQ2006 Spring Festival */
#define QQ_CLIENT_05F1   0x0F15

/** QQ2006 Beta 3  */
#define QQ_CLIENT_0F4B   0x0F4B

#define QQ_LOGIN_DATA_0F4B_LENGTH         456
#define QQ_LOGIN_RESERVE_0F4B_LENGTH    0x13
#define QQ_LOGIN_APPEND_0F4B_LENGTH     0x0140

/** QQ2006 standard */
#define QQ_CLIENT_0F5F   0x0F5F

#define QQ_LOGIN_DATA_0F5F_LENGTH         449
#define QQ_LOGIN_RESERVE_0F5F_LENGTH    0x13
#define QQ_LOGIN_APPEND_0F5F_LENGTH     0x0140


/** a random key used for login request packet
   Bring Good luck to anyone who uses Eva, that's what 66
   means. :)
*/
static const unsigned char QQ_Client_Key[16] = {
			0x66, 0x66,   0x66,  0x66, 0x66, 0x66, 0x66, 0x66,
			0x66, 0x66,   0x66,  0x66, 0x66, 0x66, 0x66, 0x66,
			}; 

static const unsigned char QQ_Hash_0F4B[16] = {
			0x3f,   0xf2,   0x31,  0x72,  0x30, 0xc8, 0x95, 0x7d,
			0xd8,  0x6a,  0x49,  0x0d,  0xf9,  0x8e, 0x44, 0xea
			};

static const unsigned char QQ_Hash_0F5F[16] = {
			0x41,  0xd1,  0x18,  0xac,  0x14,  0x78,  0x58,  0xf1,
			0xd0,  0x81,  0x4d,  0x7d,  0x7d,  0x7b,  0xd9,  0x1f
			};

static const unsigned char Computer_UUID_0F4B[16] = {
			0x88, 0xef, 0x95, 0xa6 , 0xd4, 0x04, 0xeb, 0x49,
			0x9f, 0x56, 0xb5, 0x5f  , 0x2e, 0x3d, 0xac, 0x6a
			};

static const unsigned char Computer_UUID_0F5F[16] = {
			0x00, 0xe3, 0xae, 0x4c, 0xd4, 0xff, 0x4b, 0x88,
			0x42, 0xb5, 0x66, 0xc5, 0x3a, 0x0d, 0xb6, 0x6f
			};

/** QQ server  number*/
#define QQ_SERVER_0100   0x0100
/** QQ Token server number*/
#define QQ_SERVER_0000   0x0000

/**  default client version*/
#define QQ_CLIENT_VERSION   QQ_CLIENT_0F5F
/** default qq.exe md5 hash code */
#define QQ_Hash   QQ_Hash_0F5F
/** default computer UUID */
#define Computer_UUID      Computer_UUID_0F5F
/** default qq reserved length for login request */
#define QQ_LOGIN_RESERVED_LENGTH   QQ_LOGIN_RESERVE_0F5F_LENGTH

/** QQ login data  length */
#define QQ_LOGIN_DATA_OLD_LENGTH     416

/** QQ default login request packet length */
#define QQ_LOGIN_DATA_LENGTH          QQ_LOGIN_DATA_0F5F_LENGTH
/** QQ default login request last trunck */
#define QQ_LOGIN_APPEND_LENGTH      QQ_LOGIN_APPEND_0F5F_LENGTH


/**  qq header tag, unsigned char ( 1 byte) */
#define QQ_PACKET_TAG  0x02
/** QQ tail byte*/
#define QQ_PACKET_TAIL  0x03


/** eva system message types, unsigned short ( 2 bytes) */
#define SYSTEM_MESSAGE_NORMAL  0x0001

/** udp  */
#define QQ_LOGIN_UDP  0
/** tcp */
#define QQ_LOGIN_TCP  1

/** QQ UDP packet header length*/
#define QQ_UDP_HEADER_LENGTH  7
/** QQ TCP packet header length */
#define QQ_TCP_HEADER_LENGTH  9
/** QQ packet tail length*/
#define QQ_TAIL_LENGTH  1

/**  the number of send times*/ 
#define QQ_ACK_PACKET_SEND_TIME  4


/** QQ packet maximum size*/
#define MAX_PACKET_SIZE  65535
/** the maximum bytes to send at once*/
#define MAX_SEND_IM_SIZE  400
/**  QQ key length*/
#define QQ_KEY_LENGTH  16

/** QQ default encoding, short ( 2 bytes) */
#define QQ_IM_ENCODING_GB  0x8602

/** packet queue time out */
#define QQ_SENDQUEUE_TIMEOUT  5000
/** the times of maximum re-send a packet */
#define QQ_RESEND_MAX  5
/** Keep Alive time interval   unit: ms */
#define QQ_KEEP_ALIVE_INTERVAL  60000

#define QQ_MAX_GROUP_NAME_BYTE  16
#define QQ_NAME_LENGTH_MAX          250
#define QQ_MSG_IM_MAX          1500
#define QQ_CONTACT_FIELDS    37
#define QQ_MEMO_FIELDS         7



/// unsigned char ( 1 bytes)
#define QQ_LOGIN_MODE_NORMAL  0x0A
#define QQ_LOGIN_MODE_INVISIBLE  0x28

#define QQ_FRIEND_GENDER_GG   0x00
#define QQ_FRIEND_GENDER_MM  0x01

#define QQ_FRIEND_GENDER_UNKNOWN  0xFF

#define QQ_FRIEND_FLAG_QQ_MEMBER  0x01
#define QQ_FRIEND_FLAG_MOBILE           0x10
#define QQ_FRIEND_FLAG_BIND_MOBILE  0x20

// should we change the status into Hex expression?
#define QQ_FRIEND_STATUS_ONLINE     10
#define QQ_FRIEND_STATUS_OFFLINE   20
#define QQ_FRIEND_STATUS_LEAVE      30
#define QQ_FRIEND_STATUS_INVISIBLE  40

// int ( 4 bytes)
#define QQ_MISC_STATUS_HAVING_VIDEO  0x00000001

/// unsigned char ( 1 bytes)
#define QQ_REQUEST_LOGIN_TOKEN_REPLY_OK  0x00
#define QQ_LOGIN_REPLY_OK                0x00
#define QQ_LOGIN_REPLY_REDIRECT          0x01
#define QQ_LOGIN_REPLY_PWD_ERROR         0x05
#define QQ_LOGIN_REPLY_NEED_REACTIVATE   0x06
#define QQ_LOGIN_REPLY_PWD_ERROR_EX      0x09
#define QQ_LOGIN_REPLY_REDIRECT_EX       0x0A
// eva defines this
#define QQ_LOGIN_REPLY_MISC_ERROR                0xFF

#define QQ_UPLOAD_GROUP_FRIEND_OK     0x00
#define QQ_UPLOAD_FRIEND_REMARK_OK  0x00
#define QQ_CHANGE_STATUS_REPLY_OK    0x30

#define QQ_SEND_IM_REPLY_OK   0x00

#define QQ_ADD_FRIEND_AUTH_REPLY_OK  0x30
#define QQ_DELETE_FRIEND_REPLY_OK       0x00
#define QQ_DELETE_ME_REPLY_OK              0x00

#define QQ_REQUEST_KEY_REPLY_OK          0x00


// unsigned short
#define QQ_FRIEND_LIST_POSITION_START  0x0000
#define QQ_FRIEND_LIST_POSITION_END  0xFFFF
// unsigned char
#define QQ_FRIEND_ONLINE_LIST_POSITION_START  0x00
#define QQ_FRIEND_ONLINE_LIST_POSITION_END     0xFF
#define QQ_FRIEND_LIST_UNSORTED   0x00
#define QQ_FRIEND_LIST_SORTED       0x01


/* QQ commands, unsigned short( 2 bytes) */
#define QQ_CMD_LOGOUT                             0x0001
#define QQ_CMD_KEEP_ALIVE                         0x0002
#define QQ_CMD_MODIFY_INFO                        0x0004
#define QQ_CMD_SEARCH_USER                        0x0005
#define QQ_CMD_GET_USER_INFO                      0x0006
#define QQ_CMD_ADD_FRIEND                         0x0009
#define QQ_CMD_DELETE_FRIEND                      0x000A
#define QQ_CMD_ADD_FRIEND_AUTH                    0x000B
#define QQ_CMD_CHANGE_STATUS                      0x000D
#define QQ_CMD_ACK_SYS_MSG                        0x0012
#define QQ_CMD_SEND_IM                            0x0016
#define QQ_CMD_RECV_IM                            0x0017
#define QQ_CMD_DELETE_ME                          0x001C
#define QQ_CMD_REQUEST_KEY                        0x001D
#define QQ_CMD_CELL_PHONE_1                       0x0021
#define QQ_CMD_LOGIN                              0x0022
#define QQ_CMD_GET_FRIEND_LIST                    0x0026
#define QQ_CMD_GET_FRIEND_ONLINE                  0x0027
#define QQ_CMD_CELL_PHONE_2                       0x0029
#define QQ_CMD_QUN_CMD                            0x0030
#define QQ_CMD_TEST                               0x0031
#define QQ_CMD_GROUP_NAME_OP                      0x003C
#define QQ_CMD_UPLOAD_GROUP_FRIEND                0x003D
#define QQ_CMD_MEMO_OP                            0x003E
#define QQ_CMD_DOWNLOAD_GROUP_FRIEND              0x0058
#define QQ_CMD_GET_LEVEL                          0x005C
#define QQ_CMD_ADVANCED_SEARCH                    0x0061
#define QQ_CMD_REQUEST_LOGIN_TOKEN                0x0062
#define QQ_CMD_REQUEST_EXTRA_INFORMATION          0x0065
#define QQ_CMD_TEMP_SESSION_OP                    0x0066
#define QQ_CMD_SIGNATURE_OP                       0x0067
#define QQ_CMD_RECV_MSG_SYS                       0x0080
#define QQ_CMD_RECV_MSG_FRIEND_CHANGE_STATUS      0x0081
#define QQ_CMD_SERVER_DETECT                      0x0091
#define QQ_CMD_WEATHER                            0x00A6
#define QQ_CMD_ADD_FRIEND_EX                      0x00A7
#define QQ_CMD_ADD_FRIEND_AUTH_EX                 0x00A8
#define QQ_CMD_ADD_FRIEND_AUTH_INFO               0x00AE
#define QQ_CMD_VERIFY_ADDING_MSG                  0x00B5
#define QQ_CMD_ADD_FRIEND_AUTH_QUESTION           0x00B7
#define QQ_CMD_REQUEST_LOGIN_TOKEN_EX             0x00BA
#define QQ_CMD_LOGIN_LOCATION_CHECK               0x00DA

//unsigned char(1 byte)
#define QQ_REQUEST_UNKNOWN_KEY     0x03
#define QQ_REQUEST_FILE_AGENT_KEY  0x04

//unsigned char(1 byte)
#define QQ_IM_NORMAL_REPLY  0x01
#define QQ_IM_AUTO_REPLY       0x02
#define QQ_IM_IMAGE_REPLY      0x05

/** flags of QQ_CMD_REQUEST_EXTRA_INFORMATION return, unsigned int (4 bytes)
    actually, the flag is not 4 bytes long, its length depends on the byte before, in QQ 2006
    it's 0x05 and it was 0x04 in QQ 2005, 0x03 in early QQ 2005 version. currently, the last
    byte of the 5s is ignored which is alway 0x00, we still use 4 bytes, so that we don't need
    to change other part of eva, only the parsing process got changed
*/
// QQ friends center flag
#define QQ_EXTAR_INFO_PAL         0x0400L
// QQ customized face flag, QQ calls them user head, because all pictures stored in UserHead directory
#define QQ_EXTAR_INFO_USER_HEAD   0x1000L
// QQ ablum flag
#define QQ_EXTAR_INFO_ALBUM      0x0001L
// QQ tang flag, is QQ tang a sort of game? not sure what it is.
#define QQ_EXTAR_INFO_TANG        0x0008L
// QQ personal signature flag
#define QQ_EXTAR_INFO_SIGNATURE   0x0040L

/** sub-command of QQ_CMD_REQUEST_SIGNATURE, unsigned char(1 byte) */
#define QQ_SIGNATURE_MODIFY  0x01
#define QQ_SIGNATURE_DELETE  0x02
#define QQ_SIGNATURE_REQUEST  0x03

/**  sub-command of Qun, unsigned char(1 byte)*/
#define QQ_QUN_CMD_CREATE_QUN               0x01
#define QQ_QUN_CMD_MODIFY_MEMBER            0x02
#define QQ_QUN_CMD_MODIFY_QUN_INFO          0x03
#define QQ_QUN_CMD_GET_QUN_INFO             0x04
#define QQ_QUN_CMD_ACTIVATE_QUN             0x05
#define QQ_QUN_CMD_SEARCH_QUN               0x06
#define QQ_QUN_CMD_JOIN_QUN                 0x07
#define QQ_QUN_CMD_JOIN_QUN_AUTH            0x08
#define QQ_QUN_CMD_EXIT_QUN                 0x09
#define QQ_QUN_CMD_SEND_IM                  0x0A
#define QQ_QUN_CMD_GET_ONLINE_MEMBER        0x0B
#define QQ_QUN_CMD_GET_MEMBER_INFO          0x0C
#define QQ_QUN_CMD_MODIFY_CARD              0x0E
#define QQ_QUN_CMD_REQUEST_ALL_REALNAMES    0x0F
#define QQ_QUN_CMD_REQUEST_CARD             0x10
#define QQ_QUN_CMD_SEND_IM_EX               0x1A
#define QQ_QUN_CMD_ADMIN                    0x1B
#define QQ_QUN_CMD_TRANSFER                 0x1C
#define QQ_QUN_CMD_CREATE_TEMP_QUN          0x30
#define QQ_QUN_CMD_MODIFY_TEMP_QUN_MEMBER   0x31
#define QQ_QUN_CMD_EXIT_TEMP_QUN            0x32
#define QQ_QUN_CMD_GET_TEMP_QUN_INFO        0x33
#define QQ_QUN_CMD_SEND_TEMP_QUN_IM         0x35
#define QQ_QUN_CMD_GET_TEMP_QUN_MEMBERS     0x37
#define QQ_QUN_CMD_REPLY_OK                   0x00
#define QQ_QUN_CMD_REPLY_NO_SUCH_QUN          0x02
#define QQ_QUN_CMD_REPLY_TEMP_QUN_REMOVED     0x03
#define QQ_QUN_CMD_REPLY_NOT_TEMP_QUN_MEMBER  0x04
#define QQ_QUN_CMD_REPLY_NOT_QUN_MEMBER       0x0a
#define QQ_QUN_TYPE_PERMANENT                 0x01
#define QQ_TEMP_QUN_TYPE_MULTI_TALK           0x01
#define QQ_TEMP_QUN_TYPE_DISCUSSION_GROUP     0x02
#define QQ_QUN_MEMBER_ADD                     0x01
#define QQ_QUN_MEMBER_REMOVE                  0x02
#define QQ_QUN_NO_AUTH                        0x01
#define QQ_QUN_NEED_AUTH                      0x02
#define QQ_QUN_NO_ADD                         0x03
#define QQ_QUN_AUTH_REQUEST                   0x01
#define QQ_QUN_AUTH_APPROVE                   0x02
#define QQ_QUN_AUTH_REJECT                    0x03
#define QQ_QUN_JOIN_OK                        0x01
#define QQ_QUN_JOIN_NEED_AUTH                 0x02
#define QQ_QUN_JOIN_DENIED                    0x03
#define QQ_SEARCH_QUN_BY_ID                   0x01
#define QQ_SEARCH_DEMO_QUN                    0x02
#define QQ_QUN_UNSET_ADMIN                    0x00
#define QQ_QUN_SET_ADMIN                      0x01


/* group operation commands, unsigned char( 1 bytes) */
#define QQ_UPLOAD_GROUP_NAME        0x02
#define QQ_DOWNLOAD_GROUP_NAME  0x01

/* remark operation commands */
#define QQ_UPLOAD_FRIEND_REMARK        0x01
#define QQ_DOWNLOAD_FRIEND_REMARK  0x03

/* used in download grouped friends */
#define QQ_ID_IS_FRIEND   0x01
#define QQ_ID_IS_QUN        0x04

/** types of received message, short( 2 bytes) */
#define QQ_RECV_IM_TO_BUDDY             0x0009
#define QQ_RECV_IM_TO_UNKNOWN           0x000A
#define QQ_RECV_IM_NEWS                 0x0018
#define QQ_RECV_IM_FROM_BUDDY_2006      0x0084
#define QQ_RECV_IM_FROM_UNKNOWN_2006    0x0085

/** message from Qun, short( 2 bytes)  */
// this means another member of a Qun send a message to you only
#define QQ_RECV_IM_QUN_IM_TEMP       0x001F
#define QQ_RECV_IM_UNKNOWN_QUN_IM    0x0020
#define QQ_RECV_IM_ADDED_TO_QUN      0x0021
// kicked out from a Qun
#define QQ_RECV_IM_DELETED_FROM_QUN   0x0022
#define QQ_RECV_IM_REQUEST_JOIN_QUN   0x0023
#define QQ_RECV_IM_APPROVE_JOIN_QUN   0x0024
#define QQ_RECV_IM_REJECT_JOIN_QUN    0x0025
#define QQ_RECV_IM_CREATE_QUN         0x0026
#define QQ_RECV_IM_TEMP_QUN_IM        0x002A
#define QQ_RECV_IM_QUN_IM             0x002B
#define QQ_RECV_IM_SET_QUN_ADMIN      0x002C
#define QQ_RECV_IM_SYS_MESSAGE        0x0030
#define QQ_RECV_IM_SIGNATURE_CHANGED  0x0041

/** system message, unsigned char( 1 byte) */
#define QQ_RECV_IM_KICK_OUT  0x01

/** sub types of QQ_RECV_IM_TO_BUDDY, short ( 2 bytes) */
#define QQ_IM_TCP_REQUEST               0x0001
#define QQ_IM_ACCEPT_TCP_REQUEST        0x0003
#define QQ_IM_REJECT_TCP_REQUEST        0x0005
#define QQ_IM_NORMAL_TEXT               0x000B
#define QQ_IM_UDP_REQUEST               0x0035
#define QQ_IM_ACCEPT_UDP_REQUEST        0x0037
#define QQ_IM_REJECT_UDP_REQUEST        0x0039
#define QQ_IM_NOTIFY_IP                 0x003B
#define QQ_IM_ARE_YOU_BEHIND_FIREWALL   0x003F
#define QQ_IM_ARE_YOU_BEHIND_PROXY      0x0041
#define QQ_IM_YES_I_AM_BEHIND_PROXY     0x0042
#define QQ_IM_NOTIFY_FILE_AGENT_INFO    0x004B
#define QQ_IM_REQUEST_CANCELED          0x0049
/// sub-commands started from QQ 2005 
#define QQ_IM_EX_UDP_REQUEST          0x0081
#define QQ_IM_EX_REQUEST_ACCEPTED     0x0083
#define QQ_IM_EX_REQUEST_CANCELLED    0x0085
#define QQ_IM_EX_NOTIFY_IP            0x0087


/** transfer types of file transfer packets */
#define QQ_TRANSFER_FILE		0x65
#define QQ_TRANSFER_IMAGE		0x66
#define QQ_TRANSFER_FACE		0x6B
#define QQ_TRANSFER_CAMERA		0xA1

/** transfer mode maybe :) */
#define QQ_TRANSFER_FILE_UDP		0x00
#define QQ_TRANSFER_FILE_DIRECT_UDP	0x01
#define QQ_TRANSFER_FILE_TCP		0x02
#define QQ_TRANSFER_FILE_DIRECT_TCP	0x03

/** Qun type, unsigned char ( 1 byte) */
#define QUN_TYPE_ADMIN               0x01
#define QUN_TYEP_SHAREHOLDER         0x02

/** authorization, unsigned char ( 1 byte) */


/// used in command 0x00A7
#define QQ_ADD_FRIEND_EX_ADDING_POSSIBLE    0x00
#define QQ_ADD_FRIEND_EX_ALREADY_IN_LIST    0x99
/// used in command 0x00A7
#define QQ_AUTH_NO_AUTH      0x00
#define QQ_AUTH_NEED_AUTH    0x01
#define QQ_AUTH_NO_ADD       0x02
#define QQ_AUTH_HAS_QUESTION 0x03

/// used in command 0x00B7
// 0x01 is to get my question settings including my question and answer
#define AUHT_TYPE_QUESTION_GET        0x01
#define AUTH_TYPE_QUESTION_SET        0x02
#define AUTH_TYPE_QUESTION_REQUEST    0x03
#define AUTH_TYPE_QUESTION_ANSWER     0x04

/// used in command 0x00A8
#define ADDING_AUTH_TYPE_ANYONE        0x00
#define ADDING_AUTH_TYPE_AUTH          0x02
#define ADDING_AUTH_TYPE_ACCEPT_ADD    0x03
#define ADDING_AUTH_TYPE_ACCEPT        0x04
#define ADDING_AUTH_TYPE_REJECT        0x05
#define ADDING_AUTH_TYPE_COMPOUND      0x10

/// used in command 0x00AE
#define AUTH_INFO_CMD_INFO  0x01
#define AUTH_INFO_CMD_CODE  0x02
/// used in command 0x00AE
#define AUTH_INFO_TYPE_CODE     0x00
#define AUTH_INFO_TYPE_GRAPHIC  0x01


#define QQ_MY_AUTH_APPROVE  0x30
#define QQ_MY_AUTH_REJECT   0x31
#define QQ_MY_AUTH_REQUEST  0x32


/// short ( 1 bytes)
#define QQ_MSG_SYS_BEING_ADDED            0x01
#define QQ_MSG_SYS_ADD_FRIEND_REQUEST     0x02
#define QQ_MSG_SYS_ADD_FRIEND_APPROVED    0x03
#define QQ_MSG_SYS_ADD_FRIEND_REJECTED    0x04
#define QQ_MSG_SYS_BROADCAST              0x06
#define QQ_MSG_SYS_UPDATE_HINT            0x09


#define QQ_MSG_SYS_BEING_ADDED_EX              40
#define QQ_MSG_SYS_ADD_FRIEND_REQUEST_EX       41
#define QQ_MSG_SYS_ADDED_BY_CORRECT_ANSWER     42
#define QQ_MSG_SYS_ADD_FRIEND_APPROVED_AND_ADD 43

#define Q_MSG_SYS_EVA_QQ_NEWS                0xff

#define QQ_MSG_SYS_ADDING_ALLOW_REVERSE      0x01
#define QQ_MSG_SYS_ADDING_NO_REVERSE         0x02

/// unsigned char ( 1 byte) 
#define QQ_SEARCH_ALL        0x31
#define QQ_SEARCH_CUSTOM     0x30
#define QQ_SEARCH_NICK       0x32
#define QQ_SEARCH_QQ         0x33

/** Memo , unsigned char ( 1 byte) **/
#define QQ_MEMO_UPLOAD        0x01
#define QQ_MEMO_REMOVE       0x02
#define QQ_MEMO_DOWNLOAD  0x03

/** sub-command of  QQ_CMD_REQUEST_LOGIN_TOKEN_EX */
#define QQ_LOGIN_TOKEN_REQUEST     0x01
#define QQ_LOGIN_TOKEN_VERIFY      0x02
#define QQ_LOGIN_TOKEN_OK          0x00
#define QQ_LOGIN_TOKEN_NEED_VERI   0x01

/** rechieve weather information, sub-command of QQ_CMD_WEATHER */
#define QQ_SUB_CMD_GET_WEATHER 0x01

/** reply code for QQ_CMD_SERVER_DETECT */
#define QQ_CMD_SERVER_DETECT_REPLY_OK           0x0000
#define QQ_CMD_SERVER_DETECT_REPLY_REDIRECT     0x0001
#endif
