/*
 * $Id: Config.C,v 3.10 2012-04-07 09:43:04 sin Exp $
*/

#include "config.h"


Config::Config(const string filename){
    init(filename);
}

Config::Config(){
}

int Config::init(const string filename){
    setDefault();

    ifstream is;
    is.open(filename.c_str());
    if(!is){
        cerr << "Config: Can't open " << filename << "."<< endl;
        exit(1);
    }
    while(!is.eof()){
        string index;
        string value;

        is >> index >> value;

        config_map[index] = value;
    }
    return 0;
}

void Config::set(const string index, const string value){
    config_map[index] = value;
}

string Config::get(const string index){
    if(config_map.find(index) == config_map.end()){
        cerr << "Config: "<< index << "No such config value." << endl;
        return 0;
    }else{
        return config_map[index];
    }
}

string Config::operator[](const string index){
    return get(index);
}

string Config::operator[](const char *index){
    string temp(index);
    return get(temp);
}


void Config::show(){
    for(map<string, string>::iterator it=config_map.begin();it != config_map.end(); it++){
    cout << "config["<<it->first<<"] = "<<it->second << endl;
    }
}

void Config::setDefault(){
    config_map["ip_packet_mode"] = "false";
    config_map["rule_file"] = "";
    config_map["pcap_filter"] = "";
    config_map["savemode"] = "packet";
    config_map["mallocmode"] = "malloc";
    config_map["gc_remove_time"] = "10000000";
    config_map["sim_time"] = "2592000"; //60*60*24*3
    config_map["dbpass"] = "";
    config_map["dbname"] = "";
    config_map["dbuser"] = "";
    config_map["dbhost"] = "";
    config_map["save_all"] = "0";
}
