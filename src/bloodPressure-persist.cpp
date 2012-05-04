#include "ccpp_bp.h"
//Visolve Added
#include <dbclient.h>
#define devid "deviceID"
#include <bloodPressure.h>

using namespace mongo;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;

std::stringstream temp,prtemp;
string tablename;

void run(string host,long time,short bp,short lbp,short pr,const char *deviceid) 
{
	DBClientConnection c;
	c.connect(host);
	BSONObj p = BSONObjBuilder().append(TIMESTAMP,(int)time).append(SYSTOLIC,bp).append(DIASTOLIC,lbp).append(PULSERATE,pr).append(DEVICEID,deviceid).obj();
	c.insert(tablename, p);

}

int persistSubscribeBloodPressure(string domainid,string deviceid,string loginfo,string logdata,string logconfpath,string host,string database)
{
	 tablename = database+DOT+domainid;
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
	 simpledds->logger->info(" Blood Pressure Persist Subscriber Started %s" ,deviceid.c_str());	
	 /*Creating content Filtered Subscriber*/
	 StringSeq sSeqExpr;
         sSeqExpr.length(0);
	 content_reader = simpledds->filteredSubscribe(typesupport, deviceid ,devid , deviceid,sSeqExpr);

    	 bpReader = BloodPressureDataReader::_narrow(content_reader);
   	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;

	  simpledds->logger->info("Blood Pressure Persist Subscriber for %s in table %s",deviceid.c_str(),tablename.c_str());
	  simpledds->logger->info("MONGODB Format: DEVICE_ID, MEASURED_TIME, SYSTOLIC, DIASTOLIC, PULSERATE");
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
				try 
				{
			temp<<bpList[i].deviceID;
			run(host,bpList[i].timeOfMeasurement,bpList[i].systolicPressure,bpList[i].diastolicPressure,bpList[i].pulseRatePerMinute,temp.str().c_str());
			prtemp <<bpList[i].deviceID <<COMMA<<bpList[i].timeOfMeasurement<<COMMA<< bpList[i].systolicPressure;
			prtemp <<COMMA<<bpList[i].diastolicPressure<<COMMA<<bpList[i].pulseRatePerMinute;
			simpledds->logger->info(prtemp.str().c_str());
			prtemp.str(CLEAN);
				} 
				catch( DBException &e ) 
				{
				temp <<e.what();
				//simpledds->logger->info(temp.str());
				}
				
				temp.str(CLEAN);
			}
		}
		status = bpReader->return_loan(bpList, infoSeq);
       		checkStatus(status, "return_loan");
	}
	 simpledds->logger->info("Blood Pressure Persist Subscriber Ends %s",deviceid.c_str());	
	simpledds->deleteReader(content_reader);
        delete simpledds;
        return 0;
}

