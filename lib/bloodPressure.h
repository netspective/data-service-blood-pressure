#ifndef __BLOODPRESSURE_H_
#define __BLOODPRESSURE_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
//#include "SimpleDDS.h"
#include <OpenSpliceDataService.h>

#include "Functions.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <ctime> 
#include <boost/program_options.hpp>

/*Log4cpp Library*/
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Configurator.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/SimpleLayout.hh>


using namespace std;

int publishBloodPressure(string deviceid,string domainid,string loginfo,string logdata,string logconfpath);
int echoSubscribeBloodPressure(string deviceid,string domainid,string loginfo,string logdata,string logconfpath);
int alarmSubscribeBloodPressure(string  domainid,string deviceid,int sysmin,int sysmax,int dismin,int dismax,int pulsemin,int pulsemax,string loginfo,string logdata,string logconfpath);
int persistSubscribeBloodPressure(string domainid,string deviceid,string loginfo,string logdata,string logconfpath,string host,string database);
#endif


