#include "ccpp_bp.h"
#include "Functions.h"
#include <bloodPressure.h>
#define devid "deviceID"

using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

std::stringstream temp,prtemp;
//string domainid,deviceid,loginfo,logdata,logconfpath;
int echoSubscribeBloodPressure(string domainid,string deviceid,string loginfo,string logdata,string logconfpath)
{

	

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
         tQos.durability_service.history_depth= 1024;

	 /*Initializing Subscriber and DataWriter*/
	 simpledds = new OpenSpliceDataService(tQos,loginfo,logconfpath);
	 typesupport = new BloodPressureTypeSupport();
    	 simpledds->logger->info(" Blood Pressure Subscriber Started %s" ,deviceid.c_str());
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);
	
	 bpReader = BloodPressureDataReader::_narrow(content_reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 
	 simpledds->logger->info("Blood Pressure Subscriber For %s",deviceid.c_str());
	 simpledds->logger->info("Format: DOMAIN_ID, DEVICE_ID, MEASURED_TIME, SYSTOLIC, DIASTOLIC, PULSERATE");
	 /*Receiving Data from DDS */
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
			prtemp <<bpList[i].deviceDomain<<COMMA;
		        prtemp <<bpList[i].deviceID <<COMMA<<bpList[i].timeOfMeasurement<<COMMA<< bpList[i].systolicPressure;
			prtemp <<COMMA<<bpList[i].diastolicPressure<<COMMA<<bpList[i].pulseRatePerMinute;
			 simpledds->logger->info(prtemp.str().c_str());
			prtemp.str(CLEAN);
			}
	  	}
		status = bpReader->return_loan(bpList, infoSeq);
       		checkStatus(status, "return_loan");
       
    	}

        /* We're done.  Delete everything */
	simpledds->logger->info("Blood Pressure Subscriber Ends");	
        simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;


}


