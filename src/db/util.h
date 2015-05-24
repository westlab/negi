/*
 * ** Copyright (C) 2015 Westlab, All Rights Reserved.
*/

#ifndef DB_UTIL_H_
#define DB_UTIL_H_
#pragma once

#include <iostream>

void find_and_replace(std::string& source,
                      std::string const& find,
                      std::string const& replace);

#endif  // DB_UTIL_H_
