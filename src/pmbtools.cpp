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
#include"pmbtools.h"

#if defined(ARDUINO_ARCH_ESP32)
    #include "esp_task_wdt.h"
    void        _HAL_feedWatchdog(){ esp_task_wdt_reset(); }
    uint32_t    _HAL_freeHeap(){ return ESP.getFreeHeap();   }
    uint32_t    _HAL_maxHeapBlock(){ return ESP.getMaxAllocHeap(); }
    string      _HAL_uniqueName(const string& prefix){ return string(prefix).append(stringFromInt(ESP.getEfuseMac() & 0xFFFFFF,"%06X")); }
#else
    extern "C" {
        #include "user_interface.h"
    }
    void        _HAL_feedWatchdog(){ ESP.wdtFeed(); }
    uint32_t    _HAL_freeHeap(){ return ESP.getFreeHeap(); }
    uint32_t    _HAL_maxHeapBlock(){ return ESP.getMaxFreeBlockSize(); }
    string      _HAL_uniqueName(const string& prefix){ return string(prefix).append(stringFromInt(ESP.getChipId(),"%06X")); }
#endif

uint32_t _HAL_maxPayloadSize(){ return (_HAL_maxHeapBlock() - PMB_HEAP_SAFETY) / 2; }
//
//
//
#if PMB_DEBUG
void dumpvs(const vector<string>& vs){ for(auto const& v:vs) Serial.printf("%s\n",v.data()); }
void dumpnvp(const std::map<string,string>& ms){ for(auto const& r:ms) Serial.printf("%s=%s\n",r.first.data(),r.second.data()); }
#endif
//
void dumphex(const uint8_t* mem, size_t len) {
    if(mem && len){ // no-crash sanity
        auto W=16;
        uint8_t* src;
        memcpy(&src,&mem,sizeof(uint8_t*));
        Serial.printf("Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
        for(uint32_t i = 0; i < len; i++) {
            if(i % W == 0) Serial.printf("\n[0x%08X] 0x%08X %5d:  ", (ptrdiff_t)src, i,i);
            Serial.printf("%02X ", *src);
            src++;
            //
            if(i % W == W-1 || src==mem+len){
                size_t ff=W-((src-mem) % W);
                for(int p=0;p<(ff % W);p++) Serial.print("   ");
                Serial.print("  "); // stretch this for nice alignment of final fragment
                for(uint8_t* j=src-(W-(ff%W));j<src;j++) Serial.printf("%c", isprint(*j) ? *j:'.');
                _HAL_feedWatchdog();
            }
            _HAL_feedWatchdog();
        }
        Serial.println();
    }
}

uint32_t hex2uint(const uint8_t* str){
    size_t res = 0;
    uint8_t c;
    while (isxdigit(c=*str++)) {
        uint8_t v = (c & 0xF) + (c >> 6) | ((c >> 3) & 0x8);
        res = (res << 4) | (size_t) v;
    }
    return res;
}

string join(const vector<string>& vs,const char* delim) {
	string rv="";
	if(vs.size()){
		string sd(delim);
		for(auto const& v:vs) rv+=v+sd;
		for(int i=0;i<sd.size();i++) rv.pop_back();		
	}
	return rv;
}

//
string encodeUTF8(const string& s){
    string value(s);
    size_t u=value.find("\\u");
    while(u!=string::npos){
        uint32_t cp=hex2uint((const uint8_t*) &value[u+2]);
//        Serial.printf("value %s u=%d CODE POINT!!! %d 0x%06x\n",value.data(),u,cp,cp);
        uint8_t b0=cp&0x3f;
        uint8_t b1=(cp&0xfc0) >> 6;
        uint8_t b2=(cp&0xf000) >> 12;
//        Serial.printf("bytes %02x %02x %02x\n",b2,b1,b0);
        vector<char> reps;
        if(cp>0x7FF){
            reps.push_back(b2 | 0xE0);
            reps.push_back(b1 | 0x80);
            reps.push_back(b0 | 0x80);
        }
        else {
            if(cp>0x7f){
                reps.push_back(b1 | 0xC0);
                reps.push_back(b0 | 0x80);
            } else reps.push_back(b0);
        }
//        string spesh(reps.begin(),reps.end());
        string lhf=value.substr(0,u)+string(reps.begin(),reps.end())+value.substr(u+6,string::npos);
        value=lhf;
        u=value.find("\\u",u+6);
    }
    return value; 
}

std::map<string,string> json2nvp(const string& s){
    std::map<std::string,std::string> J;
    if(s.size() > 7){
        string json=ltrim(rtrim(ltrim(rtrim(s,']'),'['),'}'),'{');
        size_t i=json.find("\":");
        if(i){
            do{
                size_t h=1+json.rfind("\"",i-2);
                size_t j=json.find(",\"",i);
                J[json.substr(h,i-h)]=encodeUTF8(replaceAll(trim(json.substr(i+2,j-(i+2)),'"'),"\\/","/"));
                i=json.find("\":",i+2);
            } while(i!=std::string::npos);
            return J;
        } //else Serial.printf("can't parse json - no components\n");
    } //else Serial.printf("can't parse json - too short \n");
    return {};
}

string lowercase(string s){
   string ucase(s);
   transform(ucase.begin(), ucase.end(),ucase.begin(), [](unsigned char c){ return std::tolower(c); } );
   return ucase;
}

string ltrim(const string& s, const char d){
	string rv(s);
	while(rv.size() && (rv.front()==d)) rv=string(++rv.begin(),rv.end());
	return rv;	
}

string nvp2json(const std::map<string,string>& nvp){
  string j="{";
  for(auto const& m:nvp) j+="\""+m.first+"\":\""+m.second+"\",";
  j.pop_back();
  return j.append("}");
}

string replaceAll(const string& s,const string& f,const string& r){
  string tmp=s;
  size_t pos = tmp.find(f);

  while( pos != string::npos){
    tmp.replace(pos, f.size(), r);
    pos =tmp.find(f, pos + r.size());
  }
return tmp;
}

string rtrim(const string& s, const char d){
	string rv(s);
	while(rv.size() && (rv.back()==d)) rv.pop_back();
	return rv;	
}

vector<string> split(const string& s, const char* delimiter){
	vector<string> vt;
	string delim(delimiter);
	auto len=delim.size();
	auto start = 0U;
	auto end = s.find(delim);
	while (end != string::npos){
		vt.push_back(s.substr(start, end - start));
		start = end + len;
		end = s.find(delim, start);
	}
	string tec=s.substr(start, end);
	if(tec.size()) vt.push_back(tec);		
	return vt;
}

string stringFromInt(int i,const char* fmt){
	char buf[16];
	sprintf(buf,fmt,i);
	return string(buf);
}

bool stringIsAlpha(const string& s){ 
    return !(std::find_if(s.begin(), s.end(),[](char c) { return !std::isalpha(c); }) != s.end());
}

bool stringIsNumeric(const string& s){ 
    string abs=(s[0]=='-') ? string(++s.begin(),s.end()):s; // allow leading minus
    return all_of(abs.begin(), abs.end(), ::isdigit);
}

string trim(const string& s, const char d){ return(ltrim(rtrim(s,d),d)); }

string uppercase(string s){
   string ucase(s);
   transform(ucase.begin(), ucase.end(),ucase.begin(), [](unsigned char c){ return std::toupper(c); } );
   return ucase;
}

string urlencode(const std::string &s){
    static const char lookup[]= "0123456789abcdef";
    std::string e;
    for(int i=0, ix=s.length(); i<ix; i++)
    {
        const char& c = s[i];
        if ( (48 <= c && c <= 57) ||//0-9
             (65 <= c && c <= 90) ||//abc...xyz
             (97 <= c && c <= 122) || //ABC...XYZ
             (c=='-' || c=='_' || c=='.' || c=='~') 
        ) e.push_back(c);
        else
        {
            e.push_back('%');
            e.push_back(lookup[ (c&0xF0)>>4 ]);
            e.push_back(lookup[ (c&0x0F) ]);
        }
    }
    return e;
}