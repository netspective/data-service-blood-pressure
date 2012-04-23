#include "ccpp_bp.h"
#include "Functions.h"
#include <bloodPressure.h>

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

char buf[BUFFERSIZE], c;
int spawn,flag,sizebuf,port;
string domainid,deviceid,loginfo,logdata,logconfpath,hostip;
stringstream prtemp;
char* bpRandomData()
{

        char send_data[1024],temp[1024];
        int sys, dias, pulse;
        sys = rand()%30+110;
        dias = rand()%40+55;
        pulse = rand()%40+55;


        strcpy(send_data,"");
        strcpy(temp,"");

        sprintf(temp, "%d", time(0));
        strcat(send_data,temp);


        sprintf(temp, "%d", sys);
        strcat(send_data,":");
        strcat(send_data,temp);


        sprintf(temp, "%d", dias);
        strcat(send_data,":");
        strcat(send_data,temp);


        sprintf(temp, "%d", pulse);
        strcat(send_data,":");
        strcat(send_data,temp);
	sleep(1);
        return send_data;


}


int publishBloodPressure(string deviceid,string domainid,string loginfo,string logdata,string logconfpath)
{

/*Importing log4cpp configuration and Creating category*/
	log4cpp::Category &log_root = log4cpp::Category::getRoot();
    	log4cpp::Category &bloodInfo = log4cpp::Category::getInstance( std::string(loginfo));
    	log4cpp::Category &bloodData = log4cpp::Category::getInstance( std::string(logdata));
	log4cpp::PropertyConfigurator::configure(logconfpath);
	bloodInfo.notice(" Blood Pressure Publisher Started "+deviceid);
	
	/*Initializing SimpleDDS library*/
	AbstractDataService *simpledds;
	BloodPressureTypeSupport_var typesupport;
	DataWriter_ptr writer;
	BloodPressureDataWriter_var bpWriter;

	/*Setting QoS Properties for Topic*/
	DDS::TopicQos tQos;
	getQos(tQos);
	
	simpledds = new OpenSpliceDataService(tQos);
	typesupport = new BloodPressureTypeSupport();
	writer = simpledds->publish(typesupport);
	bpWriter = BloodPressureDataWriter::_narrow(writer);
	
	flag=0;
	BloodPressure data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	bloodInfo.notice("Blood Pressure Started Publishing Data In DDS");
	bloodInfo.notice("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, SYSTOLIC, DIASTOLIC, PULSERATE");	
	while (1) 
	{
		
			strcpy(buf,bpRandomData());
			char * pch;
			prtemp<<domainid<<COMMA<<deviceid<<COMMA;
			pch = strtok (buf,SEMI);
			data.timeOfMeasurement = atol(pch);
			prtemp<<data.timeOfMeasurement<<COMMA;
			pch = strtok (NULL, SEMI);
			data.systolicPressure = (short)atoi(pch);		
			prtemp<<data.systolicPressure<<COMMA;
			pch = strtok (NULL, SEMI);
			data.diastolicPressure = (short)atoi(pch);
			prtemp<<data.diastolicPressure<<COMMA;
			pch = strtok (NULL, SEMI);
			data.pulseRatePerMinute = (short)atoi (pch);
			prtemp<<data.pulseRatePerMinute;
			bloodData.info(prtemp.str().c_str());
			bpWriter->write(data, NULL);
			prtemp.str(CLEAN);
		

	}
	
	/*Deleting SimpleDDS Instance*/
	bloodInfo.notice(" BloodPressure Publisher Ends");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;

}

//int main(int argc, char* argv[]) 
//{
//	if (!parse_args_pub(argc, argv,hostip, port,domainid,deviceid,loginfo,logdata,logconfpath))
//    	return 1;
//	publishBloodPressure(hostip,port,deviceid,domainid,loginfo,logdata,logconfpath);
	
//	return 0;
//}

