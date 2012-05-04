# Makefile 

# The top level directory
TOPDIR := $(shell pwd)

# The source directory locations
IDL_DIR=$(TOPDIR)/idl
CPP_DIR=$(TOPDIR)
CPP_LIB_DIR=$(TOPDIR)/lib

ifeq ($(strip $(PREFIX)),)

        TARGET_PATH=$(TOPDIR)
else
        TARGET_PATH=$(PREFIX)
endif


# The C/C++ compilers to use
#CPP=/usr/bin/g++ 
CPP=/usr/bin/g++
IDLPP= idlpp

# The C/C++ compiler flags
CPPFLAGS= -g -Wall -I/opt/mongo-cxx-driver-v1.8/mongo/client -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
LDFLAGS= -L/opt/mongo-cxx-driver-v1.8 -L/lib64 -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
LIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lsimpledds

# Boost Libraries is used by the  publisher/generator examples
BOOST_LIBS= -lboost_thread -lboost_program_options -lboost_system

# The target programs to build
TARGETS= \
	$(CPP_DIR)/bin/bp-pub \
	$(CPP_DIR)/bin/bp-sub-echo \
	$(CPP_DIR)/bin/bp-sub-alarm \
	$(CPP_DIR)/bin/bp-sub-persist \
	$(CPP_DIR)/bin/lib

INSTALL_TARGETS= \
	bp_install



# Build all targets
all: check_idlpp $(TARGETS)

install: $(INSTALL_TARGETS)

# Clean all files, including C++ generated from IDL
clean:
	rm -f /lib//libbloodpressure.so;  \
	rm -f /usr/include/bloodPressure.h;  \
	rm -f /usr/include/Functions.h;  \
	rm -f $(CPP_DIR)/lib/gen/bp/*;  \
	rm -f $(CPP_DIR)/*.a;  \
	rm -f $(CPP_DIR)/src/*.o;  \
	rm -f $(CPP_DIR)/lib/*.o; \
	rm -f $(TOPDIR)/bin/*
	rm -f $(TARGETS)

# Check that the idlpp compiler is in the current path
check_idlpp:
	@$(IDLPP) | grep Usage > /dev/null;


# Generate the bp C++ source files from the IDL
$(CPP_DIR)/lib/gen/bp/bp.h:
	mkdir -p $(CPP_DIR)/lib/gen/bp; \
	cd ${IDL_DIR}; $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/bp bp.idl; 

# Compile the bp IDL-generated C++ files into libbp.a
$(CPP_DIR)/lib/gen/bp/libbp.a: $(CPP_DIR)/lib/gen/bp/bp.h
	cd $(CPP_DIR)/lib/gen/bp; \
	$(CPP) $(CPPFLAGS) -c bp.cpp; \
        $(CPP) $(CPPFLAGS) -c bpDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c bpDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c bpSplDcps.cpp; \
        ar cr libbp.a bp.o bpDcps.o bpDcps_impl.o bpSplDcps.o; \
        ranlib libbp.a

# Compile the Functions helper class
Functions.o: $(CPP_LIB_DIR)/Functions.cpp
	$(CPP) $(CPPFLAGS) -c $(CPP_LIB_DIR)/Functions.cpp


# Compile the bloodPressurePublish binary
$(CPP_DIR)/bin/bp-pub: $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/src; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/src/bloodPressure-pub.cpp -o  $(CPP_DIR)/src/bloodPressure-pub.o $(LDFLAGS) $(LIBS); 

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/bin/bp-sub-echo: $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/src; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/src/bloodPressure-echo.cpp -o $(CPP_DIR)/src/bloodPressure-echo.o $(LDFLAGS) $(LIBS);

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/bin/bp-sub-alarm: $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/src; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/src/bloodPressure-alarm.cpp -o $(CPP_DIR)/src/bloodPressure-alarm.o  $(LDFLAGS) $(LIBS); 

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/bin/bp-sub-persist: $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o 
	cd $(CPP_DIR)/src; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/src/bloodPressure-persist.cpp -o $(CPP_DIR)/src/bloodPressure-persist.o $(LDFLAGS) $(LIBS);

# Creating Libraries
$(CPP_DIR)/bin/lib:
	cd $(CPP_DIR)/src; \
	ar cr $(CPP_DIR)/libbloodpressure.a $(CPP_DIR)/src/bloodPressure-pub.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/bp/libbp.a \
	$(CPP_DIR)/src/bloodPressure-echo.o $(CPP_DIR)/src/bloodPressure-alarm.o $(CPP_DIR)/src/bloodPressure-persist.o; \
        ranlib $(CPP_DIR)/libbloodpressure.a; 
	
bp_install:
	mkdir -p $(TARGET_PATH)/lib; \
        mkdir -p $(TARGET_PATH)/include; \
	cp $(CPP_DIR)/libbloodpressure.a $(TARGET_PATH)/lib; \
	cp $(CPP_DIR)/libbloodpressure.a /lib; \
        cp $(CPP_DIR)/lib/bloodPressure.h $(TARGET_PATH)/include; \
        cp $(CPP_DIR)/lib/bloodPressure.h /usr/include; \
        cp $(CPP_DIR)/lib/Functions.h /usr/include;

