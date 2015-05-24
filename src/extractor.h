/*
 * Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * 
 * Please see the LICENSE file distributed with this source
 * code archive for information covering the modification and
 * redistribution of this file and binaries built from it.
 */

/*
 * $Id: Extractor.H,v 5.0 2010-10-13 08:47:15 sin Exp $
*/
#ifndef EXTRACTOR_H_
#define EXTRACTOR_H_
#pragma once

#include "extractor.h"
#include "packet.h"

class Extractor {
 private:
 public:
      Extractor();
      void Proc(Packet *pkt);
};
#endif  // EXTRACTOR_H_
