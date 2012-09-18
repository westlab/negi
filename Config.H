/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Config.H,v 3.8 2012-03-21 14:56:24 sin Exp $
 * Type: Support Class
 * Name: Config
 * Note: configulation class. read swsim.conf.
*/

#ifndef CONFIG_H
#define CONFIG_H 

#include "Include.H"

class Config
{
	private:
		map<string, string> config_map;
		void setDefault();

	public:
		Config(const string filename);
		Config();
		int init(const string filename);
		string get(const string index);
		void set(const string index, const string value);
		string operator[](const string index);
		string operator[](const char *);
		void show();
};
#endif
