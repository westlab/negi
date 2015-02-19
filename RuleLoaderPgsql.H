/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: RuleLoaderPgsql.H,v 5.1 2012-05-19 15:01:31 sin Exp $
*/

#pragma once

#include "Stream.H"
using namespace pqxx;

class RuleLoaderPgsql
{
	private:
	public:
		RuleLoaderPgsql();
		int Proc();
		int UpdateCheck();
};
