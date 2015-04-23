/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: RuleLoaderFile.C,v 5.2 2012/05/23 05:23:48 sin Exp $
*/

#include "include.h"
#include "global.h"
#include "stream.h"

#ifndef USE_POSTGRES

#include "rule_loader_file.h"
#include "glog/logging.h"

RuleLoaderFile::RuleLoaderFile(){
    return;
}

int RuleLoaderFile::Proc(){
    rule_pool->Clear();

    ifstream is;
    is.open( config->get("rule_file").c_str());

    if(!is){
        cerr << "Config: Can't open " << config->get("rule_file") << "."<< endl;
        exit(1);
    }
    while(!is.eof()){
        int id;
        string rule;

        is >> id >> rule;
        cout << rule << endl;
        LOG(INFO) << "Start add rule loop";
        rule_pool->AddRule(id, rule);

    }
    rule_pool->DeleteLastRule();
    stream_id_prev = 0;



    return 0;
}

#endif //USE_POSTGRES