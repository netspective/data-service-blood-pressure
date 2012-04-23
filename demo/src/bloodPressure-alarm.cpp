#include <boost/program_options.hpp>
#include <bloodPressure.h>


string _domainid,_deviceid,_loginfo,_logdata,_logconfpath;
int _sysmin,_sysmax,_pulsemin,_pulsemax,_dismin,_dismax;

int main(int argc, char* argv[]) 
{
	if (!parse_args_bp_alarm(argc, argv,_domainid,_deviceid,_sysmin,_sysmax,_dismin,_dismax,_pulsemin,_pulsemax,_loginfo,_logdata,_logconfpath))
    	return 1;
	
	alarmSubscribeBloodPressure(_domainid,_deviceid,_sysmin,_sysmax,_dismin,_dismax,_pulsemin,_pulsemax,_loginfo,_logdata,_logconfpath);
	return 0;
}

