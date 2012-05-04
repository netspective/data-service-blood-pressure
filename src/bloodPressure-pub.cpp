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

	/*Initializing SimpleDDS library*/
	DataService *simpledds;
	BloodPressureTypeSupport_var typesupport;
	DataWriter_ptr writer;
	BloodPressureDataWriter_var bpWriter;

	/*Setting QoS Properties for Topic*/
	DDS::TopicQos tQos;
	getQos(tQos);
	
	simpledds = new OpenSpliceDataService(tQos,loginfo,logconfpath);
	simpledds->logger->info("Blood Pressure Publisher Started %s",deviceid.c_str());
	typesupport = new BloodPressureTypeSupport();
	writer = simpledds->publish(typesupport);
	
	bpWriter = BloodPressureDataWriter::_narrow(writer);
	
	flag=0;
	BloodPressure data;

	/*Storing Domain and Device ID*/
	data.deviceID = DDS::string_dup(deviceid.c_str());
	data.deviceDomain = DDS::string_dup(domainid.c_str());
	
	simpledds->logger->info("Blood Pressure Started Publishing Data In DDS");
	simpledds->logger->info("Format: DOMAIN, DEVICEID, TIMEOFMEASURED, SYSTOLIC, DIASTOLIC, PULSERATE");	
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
			simpledds->logger->data(prtemp.str().c_str());
			bpWriter->write(data, NULL);
			prtemp.str(CLEAN);
		

	}
	
	/*Deleting SimpleDDS Instance*/
	simpledds->logger->info(" BloodPressure Publisher Ends");
	simpledds->deleteWriter(writer);
	delete simpledds;
	return 0;

}
