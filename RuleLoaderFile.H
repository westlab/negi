/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: RuleLoaderFile.H,v 5.1 2012-08-22 08:13:16 sin Exp $
*/

#pragma once

#include "Stream.H"

#ifdef USE_POSTGRES
using namespace pqxx;
#endif

class RuleLoaderFile
{
	private:
	public:
		RuleLoaderFile();
		int Proc();
};
