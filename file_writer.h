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

#ifndef FileWriter_H
#define FileWriter_H

class FileWriter
{
	private:
		ofstream ofs;
	public:
		FileWriter();
		void Write(const string& str);
};

#endif
