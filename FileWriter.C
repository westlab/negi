/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: FileWriter.C,v 1.3 2012-04-30 07:15:00 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "FileWriter.H"

FileWriter::FileWriter(){
	ofs.open("./output.txt", ios::trunc | ios::binary);
	return;
}


void FileWriter::Write(const string& str){
	ofs << str << endl;
}

