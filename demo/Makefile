# Makefile 

# The top level directory
TOPDIR := $(shell pwd)
# The source directory locations
IDL_DIR=$(TOPDIR)/idl
CPP_DIR=$(TOPDIR)
CPP_LIB_DIR=$(TOPDIR)/lib
BP_LIB=$(TOPDIR)/../lib

# The C/C++ compilers to use
#CPP=/usr/bin/g++ 
CPP=/usr/bin/g++
IDLPP= idlpp

# The C/C++ compiler flags
CPPFLAGS= -Wl,-allow-multiple-definition -g -Wall -I/opt/mongo-cxx-driver-v1.8/mongo/client -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
LDFLAGS= -L/opt/mongo-cxx-driver-v1.8 -L/lib64 -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib -Wl,-allow-multiple-definition
LIBS=  -lboost_thread -lboost_program_options -lboost_system -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  /lib/libsimpledds.a /lib/libbloodpressure.a /lib/liblogger.a
#LIBS=  -lboost_thread -lboost_program_options -lboost_system -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  /lib/libbloodpressure.a

# Boost Libraries is used by the  publisher/generator examples
#BOOST_LIBS= -lboost_thread -lboost_program_options -lboost_system

# The target programs to build
TARGETS= \
	$(CPP_DIR)/bin/bp-pub \
	$(CPP_DIR)/bin/bp-echo \
	$(CPP_DIR)/bin/bp-alarm \
	$(CPP_DIR)/bin/bp-persist



all: $(TARGETS)

clean: 
	rm -f $(TOPDIR)/bin/*; \
	rm -f $(TOPDIR)/src/*.o;


# Compile the bloodPressurePublish binary
$(CPP_DIR)/bin/bp-pub:
	cd $(CPP_DIR)/src; \
	$(CPP) $(CPPFLAGS) -c $(CPP_DIR)/src/bloodPressure-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -o $(TOPDIR)/bin/bp-pub $(CPP_DIR)/src/bloodPressure-pub.o /lib/libbloodpressure.a $(BP_LIB)/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressurePublish binary
$(CPP_DIR)/bin/bp-echo:
	cd $(CPP_DIR)/src; \
        $(CPP) $(CPPFLAGS) -c $(CPP_DIR)/src/bloodPressure-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -o $(TOPDIR)/bin/bp-echo $(CPP_DIR)/src/bloodPressure-echo.o /lib/libbloodpressure.a $(BP_LIB)/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressurePublish binary
$(CPP_DIR)/bin/bp-alarm:
	cd $(CPP_DIR)/src; \
        $(CPP) $(CPPFLAGS) -c $(CPP_DIR)/src/bloodPressure-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -o $(TOPDIR)/bin/bp-alarm $(CPP_DIR)/src/bloodPressure-alarm.o /lib/libbloodpressure.a $(BP_LIB)/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressurePublish binary
$(CPP_DIR)/bin/bp-persist:
	cd $(CPP_DIR)/src; \
        $(CPP) $(CPPFLAGS) -c $(CPP_DIR)/src/bloodPressure-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lmongoclient -o $(TOPDIR)/bin/bp-persist $(CPP_DIR)/src/bloodPressure-persist.o /lib/libbloodpressure.a $(BP_LIB)/gen/bp/libbp.a $(LIBS);
 
