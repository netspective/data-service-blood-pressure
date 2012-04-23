# Blood Pressure Library

BloodPressure library is intended to greatly simplify the use of BloodPressure device simulation with DDS. As such, BloodPressure library have functions that reduces by several times the line of code that need to be written for a creating BloodPressure Simulation DDS application. 

# Compilation and Installation
Clone the library from the github

	git clone git@github.com:netspective/data-service-blood-pressure.git
	
	cd data-service-blood-pressure

Compiling
	
	make

Installing
	
	make install 

	or 

	make install PREFIX=<installation path>



## Simplified Functions
Following are BloodPressure functions and their descriptions

Note: Refer the demo programs for reference

#### Running Publisher 

SYNOPSIS

	int publishBloodPressure(string deviceid,string domainid,string loginfo,string logdata,string logconfpath);

DESCRIPTION

	The publish function used to publish the Blood Pressure data in the DDS. 

ARGUMENTS

	domain             Device Domain 

	device-id          Device ID - for device identification

	log-info           Log information category

	lod-data           Log data file 

	log4cpp-conf       Log configuration and format specification file


#### Echo Subscribers

SYNOPSIS

	int echoSubscribeBloodPressure(string deviceid,string domainid,string loginfo,string logdata,string logconfpath);

DESCRIPTION

This function is used to subscribe and join the BloodPressure topic. Log4cpp config is used to maintain the logs and thier path.
 
ARGUMENTS

	domain             Device Domain 

	device-id          Device ID - for device identification

	log-info           Log information category

	lod-data           Log data file 

	log4cpp-conf       Log configuration and format specification file


#### Alarm Subscriber

SYNOPSIS

	int alarmSubscribeBloodPressure(string  domainid,string deviceid,int sysmin,int sysmax,int dismin,int dismax,int pulsemin,int pulsemax,string loginfo,string logdata,string logconfpath);


DESCRIPTION

This function is used to subscribe and join the BloodPressure topic with alarm notification. Log4cpp config is used to maintain the logs and thier path.

 
ARGUMENTS


	domain               Device Domain

	device-id            Device ID for identification

	log-info             Log information category

	log-data             Log data category 

	log4cpp-conf         Log configuration and format specification file

	systolic-low         Systolic low pressure alarm specification - default <90

	systolic-high        Systolic high Pressure alarm specification - default >140

	diatolic-low         Diatolic low pressure alarm specification - default <60

	diatolic-high        Diatolic high pressure alarm specification - default>90

	pulse-rate-low       Pulse low rate alarm specification - default <60

	pulse-rate-high      Pulse high rate alarm specification - default >90

#### Persistance Subscriber

SYNOPSIS

	int persistSubscribeBloodPressure(string domainid,string deviceid,string loginfo,string logdata,string logconfpath,string host,string database);

DESCRIPTION

This function is used to subscribe and join the BloodPressure topic and store the output in the MongoDB database notification. Log4cpp config is used to maintain the logs and thier path.
 
ARGUMENTS

	domain           Device Domain

	device-id        Device ID for identification

	host             Host Ip_Address

	database         Database Name

	log-info         Log information category

	log-data         Log data category

	log4cpp-conf     Log configuration and format specification file
