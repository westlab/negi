/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Color.H,v 5.0 2010-10-13 08:47:15 sin Exp $
 * Note: ¥Ç¥Ð¥Ã¥°½ÐÎÏ¤Î¿§ÉÕ¤±¥Õ¥¡¥¤¥ë
*/

/*
#define BLACK if(get_colorflag()){ cout << "[30m"; };
#define RED if(get_colorflag()){ cout << "[31m"; };
#define GREEN if(get_colorflag()){ cout << "[32m"; };
#define YELLOW if(get_colorflag()){ cout << "[33m"; };
#define BLUE if(get_colorflag()){ cout << "[34m"; };
#define MAJENDA if(get_colorflag()){ cout << "[35m"; };
#define CYAN if(get_colorflag()){ cout << "[36m"; };
#define WHITE if(get_colorflag()){ cout << "[37m"; };

#define UBLACK if(get_colorflag()){ cout << "[40m"; };
#define URED if(get_colorflag()){ cout << "[41m"; };
#define UGREEN if(get_colorflag()){ cout << "[42m"; };
#define UYELLOW if(get_colorflag()){ cout << "[43m"; };
#define UBLUE if(get_colorflag()){ cout << "[44m"; };
#define UMAJENDA if(get_colorflag()){ cout << "[45m"; };
#define UCYAN if(get_colorflag()){ cout << "[46m"; };
#define UWHITE if(get_colorflag()){ cout << "[47m"; };

#define RESET if(get_colorflag()){ cout << "[0m"; }; 
*/
#define BLACK cout << "[30m";
#define RED cout << "[31m";
#define GREEN cout << "[32m";
#define YELLOW cout << "[33m";
#define BLUE cout << "[34m";
#define MAJENDA cout << "[35m";
#define CYAN cout << "[36m";
#define WHITE cout << "[37m";

#define UBLACK cout << "[40m";
#define URED cout << "[41m";
#define UGREEN cout << "[42m";
#define UYELLOW cout << "[43m";
#define UBLUE cout << "[44m";
#define UMAJENDA cout << "[45m";
#define UCYAN cout << "[46m";
#define UWHITE cout << "[47m";

#define RESET cout << "[0m";
