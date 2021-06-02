/*
 MIT License

Copyright (c) 2019 Phil Bowles <H48266@gmail.com>
   github     https://github.com/philbowles/H4
   blog       https://8266iot.blogspot.com
   groups     https://www.facebook.com/groups/esp8266questions/
              https://www.facebook.com/H4-Esp8266-Firmware-Support-2338535503093896/


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include"pmb_config.h"

#include<Arduino.h>

#include<string>
#include<vector>
#include<map>
#include<unordered_map>
#include<functional>

#define RECORD_SEPARATOR "|"
#define UNIT_SEPARATOR "~"

void            _HAL_feedWatchdog();
uint32_t        _HAL_freeHeap();
uint32_t        _HAL_maxHeapBlock();
size_t          _HAL_maxPayloadSize();
#ifdef ARDUINO_ARCH_ESP32
std::string          _HAL_uniqueName(const std::string& prefix);
#else
std::string          _HAL_uniqueName(const std::string& prefix);
#endif

#if PMB_DEBUG
void            dumpvs(const std::vector<std::string>& vs);
void            dumpnvp(const std::map<std::string,std::string>& ms);
#endif

void                    dumphex(const uint8_t* mem, size_t len);
std::string             encodeUTF8(const std::string &);
uint32_t                hex2uint(const uint8_t* str);
std::string 		    join(const std::vector<std::string>& vs,const char* delim="\n");
std::map<std::string,std::string> json2nvp(const std::string& s);
std::string             lowercase(std::string);
std::string             ltrim(const std::string& s, const char d=' ');
std::string             nvp2json(const std::map<std::string,std::string>& nvp);
std::string             replaceAll(const std::string& s,const std::string& f,const std::string& r);
std::string             rtrim(const std::string& s, const char d=' ');
std::vector<std::string>     split(const std::string& s, const char* delimiter="\n");
std::string		        stringFromInt(int i,const char* fmt="%d");
bool		            stringIsAlpha(const std::string& s);
bool		            stringIsNumeric(const std::string& s);
std::string             trim(const std::string& s, const char d=' ');
std::string             uppercase(std::string);
std::string             urlencode(const std::string &s);
//
template<typename T>
std::string flattenMap(const T& m,const std::string& fs=UNIT_SEPARATOR,const std::string& rs=RECORD_SEPARATOR,std::function<std::string(const std::string&)> f=[](const std::string& s){ return s; }){
    std::string flat;
    for(auto const& nvp:m) flat+=f(nvp.first)+fs+f(nvp.second)+rs;
    flat.pop_back();
    return flat;
}