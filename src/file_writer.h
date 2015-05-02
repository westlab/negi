/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: FileWriter.H,v 1.1 2011-11-18 04:36:54 sin Exp $
*/

#ifndef FILE_WRITER_H_
#define FILE_WRITER_H_

class FileWriter {
 private:
        ofstream ofs_;
 public:
        FileWriter();
        void Write(const string& str);
};

#endif  // FILE_WRITER_H_
