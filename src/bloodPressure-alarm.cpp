#include "ccpp_bp.h"
#define devid "deviceID"
#include <bloodPressure.h>

using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
//int sysmin,sysmax,pulsemin,pulsemax,dismin,dismax;

std::stringstream temp,prtemp;
//string domainid,deviceid,loginfo,logdata,logconfpath;
int alarmSubscribeBloodPressure(string  domainid,string deviceid,int sysmin,int sysmax,int dismin,int dismax,int pulsemin,int pulsemax,string loginfo,string logdata,string logconfpath)
{
	 sysmin = 90;
	 sysmax = 140;
	 dismin = 60;
	 dismax = 90;
	 pulsemin = 60;
	 pulsemax = 90;
	
	


	 /*Initializing SimpleDDS library*/
	 DataService *simpledds;
	 BloodPressureTypeSupport_var typesupport;
    	 DataReader_ptr content_reader;
    	 BloodPressureDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;
         
	 /*Setting QoS Properties for Topic*/
	 DDS::TopicQos tQos;
	 getQos(tQos);

	 simpledds = new OpenSpliceDataService(tQos,loginfo,logconfpath);
	 typesupport = new BloodPressureTypeSupport();
         simpledds->logger->info(" Blood Pressure Alarm Subscriber Started %s" ,deviceid.c_str());
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);

    	 bpReader = BloodPressureDataReader::_narrow(content_reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;

	 simpledds->logger->info("Blood Pressure alarm Subscriber for %s",deviceid.c_str());
simpledds->logger->info("Format: DOMAIN_ID, DEVICE_ID, MEASURED_TIME, SYSTOLIC(LEVEL), DIASTOLIC(LEVEL), PULSERATE(LEVEL)");
	 while (1) 
	 {
         	status = bpReader->take(
            	bpList,
            	infoSeq,
            	LENGTH_UNLIMITED,
            	ANY_SAMPLE_STATE,
           	ANY_VIEW_STATE,
            	ANY_INSTANCE_STATE);
         	checkStatus(status, "take");
          	if (status == RETCODE_NO_DATA) 
		{
          		continue;
          	}
		for (i = 0; i < bpList.length(); i++) 
		{
			
			if(infoSeq[i].valid_data)
			{
			if (bpList[i].systolicPressure < sysmin || bpList[i].systolicPressure > sysmax || bpList[i].diastolicPressure < dismin || bpList[i].diastolicPressure > dismax || bpList[i].pulseRatePerMinute < pulsemin || bpList[i].pulseRatePerMinute > pulsemax)
			{
				prtemp <<bpList[i].deviceDomain<<COMMA<<bpList[i].deviceID<<COMMA;
				prtemp <<bpList[i].timeOfMeasurement<<COMMA<<alarmString(bpList[i].systolicPressure,sysmin,sysmax);
				prtemp <<COMMA<<alarmString(bpList[i].diastolicPressure,dismin,dismax)<<COMMA;
				prtemp <<alarmString(bpList[i].pulseRatePerMinute,pulsemin,pulsemax);
				simpledds->logger->info(prtemp.str().c_str());
				prtemp.str(CLEAN);
			}
			
			}
		}
		status = bpReader->return_loan(bpList, infoSeq);
        	checkStatus(status, "return_loan");
	 	
    	}
	simpledds->logger->info("Blood Pressure alarm Subscriber Ends");	
        /* We're done.  Delete everything */
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;


}

