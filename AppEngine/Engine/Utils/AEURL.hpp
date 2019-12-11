//
//  AEURL.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/18.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEURL_hpp
#define AEURL_hpp

#include "AEPlatform.hpp"

class AEURL {
public:
    std::string url();
    std::string scheme();
    std::string host();
    std::string port();
    std::string path();
    std::string query();
    std::string fragment();
    
public:
    void clear();
    void reset(std::string url);
    std::string absoluteURL(std::string name);
    
protected:
    std::string _url;
    std::string _scheme;
    std::string _host;
    std::string _port;
    std::string _path;
    std::string _query;
    std::string _fragment;
};

#endif /* AEURL_hpp */
