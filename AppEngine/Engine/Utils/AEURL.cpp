//
//  AEURL.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/18.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEURL.hpp"

std::string AEURL::url() {
    return _url;
}
std::string AEURL::scheme() {
    return _scheme;
}
std::string AEURL::host() {
    return _host;
}
std::string AEURL::port() {
    return _port;
}
std::string AEURL::path() {
    return _path;
}
std::string AEURL::query() {
    return _query;
}
std::string AEURL::fragment() {
    return _fragment;
}

void AEURL::clear() {
    _url.clear();
    _scheme.clear();
    _host.clear();
    _port.clear();
    _path.clear();
    _query.clear();
    _fragment.clear();
}
void AEURL::reset(std::string url) {
    clear();
    this->_url = url;
    size_t idx1 = 0, idx2 = 0;
    
    idx1 = 0;
    idx2 = url.find_first_of(":") + 1;
    this->_scheme = url.substr(idx1, idx2 - idx1);
    
    idx1 = idx2 + 2;
    idx2 = url.find("/", idx1);
    std::string host = url.substr(idx1, idx2 - idx1);
    if (host.find(":") == std::string::npos) {
        this->_host = host;
    }
    else {
        size_t idx = host.find(":");
        this->_host = host.substr(0, idx);
        this->_port = host.substr(idx + 1);
    }
    
    idx1 = idx2 + 0;
    idx2 = url.find("?", idx1);
    if (idx2 == std::string::npos) {
        this->_path = url.substr(idx1);
    }
    else {
        this->_path = url.substr(idx1, idx2 - idx1);
        
        idx1 = idx2;
        idx2 = url.find("#", idx1);
        if (idx2 == std::string::npos) {
            this->_query = url.substr(idx1);
        }
        else {
            this->_query = url.substr(idx1, idx2 - idx1);
            this->_fragment = url.substr(idx2);
        }
    }
}
std::string AEURL::absoluteURL(std::string name) {
    if (name.find("http:") == 0 || name.find("https:") == 0 || name.find("data:") == 0 || name.find("file:") == 0 || name.find("camera:") == 0) {
        return name;
    }
    std::string host = _port.empty() ? _host : (_host + ":" + _port);
    if (name.find("/") == 0) {
        return _scheme + "//" + host + name;
    }
    else {
        size_t idx = _path.find_last_of("/") + 1;
        return _scheme + "//" + host + _path.substr(0, idx) + name;
    }
}
