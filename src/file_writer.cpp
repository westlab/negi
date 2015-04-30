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

#include "include.h"
#include "global.h"
#include "file_writer.h"

FileWriter::FileWriter() {
    ofs_.open("./output.txt", ios::trunc | ios::binary);
    return;
}


void FileWriter::Write(const string& str) {
    ofs_ << str << endl;
}
