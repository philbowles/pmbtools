![cubans](assets/cuban%20mechanic.jpg)

# ArduinoIDE library of Utility functions and ESP8266/ESP32 HAL serving as base of other libraries

---

# Contents

* [What does it do?](#what-does-it-do)
* [PMB Tools' place in "The menagerie"](#the-menagerie-roadmap)
* [Why "The Cuban Mechanics" ?](#why-the-cuban-mechanics)
* [Problems / Issues](#problems--issues)

---

# What does it do?

PMB Tools is at the heart of several other firmware packages that provide for simple *robust* and rapid ***asynchronous*** IOT development on ESP8266 / ESP32.

Firstly it "smooths out" differences in the hardware of ESP8266 / ESP32 that make writing cross-platform code a lot simpler, faster and easier to debug. This type of thing is usually known as a "Hardware Abstraction Layer" or "HAL"

Secondly it provides a few utility functions of the kind that most programmers collect over the years. They usually include / re-write at the beginning of every project. I make no claims for any of the functions to be special or clever - they are just *my* quirky little way of doing things the way *I* like doing them. They have developed historically and also happen to be called by pretty much every piece of code I write, so - like 'em or not - you gotta have 'em! Finally, there are no doubt many different / better / faster ways of doing some of the tasks: If you think so, then you do them *your* way and spare me any lessons, please.

Perhaps the quickest explanation for this library is to show the *other libraries* that depend on it and where it sits in the grand scheme of rapid development of asynchronous multitasking apps on ESP8266 / ESP32:

# The "menagerie" roadmap

![roadmap](assets/common/menagerieroadmap.jpg)

## The related / dependent libraries

|| Name | Provides | Notes |
| :---: | :----------  | :--- | :--- |
||[Forked AsyncTCP](https://github.com/philbowles/AsyncTCP-master)|"Glue" to LwIP (ESP8266)| Important bugfixes |
||[Forked ESPAsyncTCP](https://github.com/philbowles/ESPAsyncTCP-master)|"Glue" to LwIP(ESP32)| Missing features added |
||[Forked ESPAsyncWebserver](https://github.com/philbowles/ESPAsyncWebServer)| Basis of webUI in H4Plugins| Several major bugfixes |
|![roadmap](assets/common/tools_icon.jpg)|[PMB Tools](https://github.com/philbowles/pmbtools)|'32/'8266 HAL and utility functions| |
|![roadmap](assets/common/aardvark_icon.jpg)|[AardvarkTCP](https://github.com/philbowles/AardvarkTCP)|Simple Large-payload Async TCP| API-compatible with ESPAsyncTCP, seamless TLS/SSL |
|![roadmap](assets/common/pangolin_icon.jpg)|[PangolinMQTT](https://github.com/philbowles/PangolinMQTT)|Async MQTT Client|QoS 0/1/2 Fully 3.1.1 compliant. Large payloads |
|![roadmap](assets/common/armadillo_icon.jpg)|[ArmadilloHTTP](https://github.com/philbowles/ArmadilloHTTP)|Async HTTP/S Client| Simple send/callback of large payloads |
|![roadmap](assets/common/h4_icon.jpg)|[H4](https://github.com/philbowles/H4)|Scheduler/Async Timers| |
|![roadmap](assets/common/h4p_icon.jpg)|[H4/Plugins](https://github.com/philbowles/h4plugins)|Full Async IOT Firmware| Webserver, MQTT, OTA, NTP, HTTP etc etc |

---

# API

```cpp
//
//  HAL
//
void                _HAL_feedWatchdog(); // You should NEVER call this - it's here for completenesss  only
uint32_t            _HAL_maxHeapBlock(); // Maxium size of available memory block that can be allocated from heap
size_t              _HAL_maxPayloadSize(); // calculated from 1/2 of the above after subtracting PMB_HEAP_SAFETY
std::string         _HAL_uniqueName(const std::string& prefix); // prefix defaults to "ESP8266" or "ESP32", appends unique H/W chip ID
//
//  General purpose / string manipulation
//
void                dumphex(const uint8_t* mem, size_t len); // pretty formatted hex dump len bytes at address mem
std::string         join(const std::vector<std::string>& vs,const char* delim="\n"); // flattens/vector/into/string/delimited/by/whatever/u/want
std::string         lowercase(std::string); // does what it says on the tin
std::string         ltrim(const std::string& s, const char d=' '); // trims leftmost character(s)
std::string         replaceAll(const std::string& s,const std::string& f,const std::string& r);
std::string         rtrim(const std::string& s, const char d=' '); // trims rightmost character(s)
std::vector<std::string>  split(const std::string& s, const char* delimiter="\n"); // decomposes "a/b/c..." into {"a","b","c",...}
std::string         stringFromInt(int i,const char* fmt="%d"); // ESP8266 does not have C's itoa etc - this does kinda the same job
bool                stringIsAlpha(const std::string& s); // true if string is entirely "visible ASCII"
bool                stringIsNumeric(const std::string& s); // true if string will covert to a valid integer
std::string         trim(const std::string& s, const char d=' '); // trims both ends, e.g. returs ltrim(rtrim(x))
std::string         uppercase(std::string); // DOES WHAT IT SAYS ON THE TINE
std::string         urlencode(const std::string &s); // pretty standard
```

---

# Why "The Cuban Mechanics"?

Many moons ago I worked for an organisation that expected my small specialist team to work miracles with practically zero resources. It gave rise to quotes such as:

> We the unwilling, led by the unknowing

> Are doing the impossible for the ungrateful

> We have been doing so much for so long with so little

> We are now qualified to anything with nothing

Around that time (late 1990s) I read an article about Cuba. During the 1950s it was the playground of rich Americans and the streest of Havan were filled with many imported glitzy space-age US automobiles. Once Fidel Castro came to power, ties with the USA were severed and trade embargoes put in place which made it practically impossible to get spares to keep the gas guzzlers running.

Cuban mechanics therefore reapidly became adept at improvisation, creativity, bodging, repurposing and nigh-on miracle working to prevent their cars from going to the scrapyard. They thus became the "poster boys" for my beleagured team who truly identified with them.

Hence the backdrop is the flag of CUBA and the "skull and crossbones" motif is to evoke our team's sense of being "a bit out there". The skull's beard is taken from an actual photo of Fidel Castro and deliberately badly 'shopped in place, signifying the lash=up nature of a lot of things we were forced to end up doing. The "bones" of the map of Cuba and the spanner then speak for themselves.

I actually printed dozens of sticky labels and we'd surreptitiously place them anywhere we could on any piece of kit we rescued, rebuilt, repaired or ~~stole~~ "re-allocated".

Serious IT doesn't have to be *all* po-faced. :)

![stickers](assets/stickers.jpg)

## More info

![yt](assets/youtubelink.jpg)

[Watch the video](https://nujournalismincuba2018.wordpress.com/2018/07/21/video-the-magician-mechanics-of-cuba/)

---

# Issues / Support

## IMPORTANT NOTE FOR PLATFORMIO USERS

ArdvarkTCP is an *ArduinoIDE library*, and is 100% compatible with the ArduinoIDE and its build system. PlatformIO, sadly, ***is not***. If PlatformIO has problems with code that compiles and runs correctly under ArduinoIDE, then it is a ***PlatformIO problem***, not an issue with this - or any other - valid Arduino library, so please contact *them*, not *me*.

For this reason, I will not accept any issues relating to build problems with PlatformIO, nor any pull requests nor other suggestions which involve any changes that render this library less than 100% ArduinoIDE compatible.

## Non PlatformIO-related issues

Your **first** point of contact should be one of the facebook groups below, if only to let me know you have raised an issue here. Obviously I will check the issues from time to time, but I do no have the time to check every day.

If you want a rapid response, I am daily moderating those FB groups so you are likely to get a rapid response.

## Before submitting an issue

If you do not provide sufficient information for me to be able to replicate the problem, ***I CANNOT FIX IT***

So, always provide: the MCU/board type, a good description of the problem, how / when / why it happens and how to recreate it, as well as the full source code, relevant Serial output messages and a DECODED stack trace in the event of a crash.

## And finally...

This is open-source, I do it in my own time, for free. If you want professional-level support because you are using *my* work to benefit your own commercial gain, then I'm happy to talk privately about a paid support contract. Or you can support me on [Patreon](https://www.patreon.com/esparto) 

---

## Find me daily in these FB groups

* [ESP8266 & ESP32 Microcontrollers](https://www.facebook.com/groups/2125820374390340/)
* [ESP Developers](https://www.facebook.com/groups/ESP8266/)
* [H4/Plugins support](https://www.facebook.com/groups/h4plugins)

I am always grateful for any $upport on [Patreon](https://www.patreon.com/esparto) :)


(C) 2021 Phil Bowles