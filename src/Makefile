#
# OMNeT++/OMNEST Makefile for Ncorp-INETMANET
#
# This file was generated with the command:
#  opp_makemake -f --deep -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/kodo/src -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/bundle_dependencies/boost-e92f30/master -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/bundle_dependencies/fifi-6ca972/master/src -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/bundle_dependencies/sak-602ce9/master/src -I/home/ivocalado/inetmanet/src -I/home/ivocalado/inetmanet/src/networklayer/common -I/home/ivocalado/inetmanet/src/transport/tcp_common -I/home/ivocalado/inetmanet/src/wpan/networklayer/contract -I/home/ivocalado/inetmanet/src/linklayer/common -I/home/ivocalado/inetmanet/src/networklayer/manetrouting/base -I/home/ivocalado/inetmanet/src/networklayer/autorouting/ipv4 -I/home/ivocalado/inetmanet/src/util/headerserializers/tcp -I/home/ivocalado/inetmanet/src/networklayer/ipv6tunneling -I/home/ivocalado/inetmanet/src/mobility -I/home/ivocalado/inetmanet/src/wpan/linklayer/ieee802154/mac -I/home/ivocalado/inetmanet/src/world/radio -I/home/ivocalado/inetmanet/src/linklayer/ieee80211/mac -I/home/ivocalado/inetmanet/src/networklayer/ipv6 -I/home/ivocalado/inetmanet/src/networklayer/icmpv6 -I/home/ivocalado/inetmanet/src/world/powercontrol -I/home/ivocalado/inetmanet/src/linklayer/ieee80211mesh/locator -I/home/ivocalado/inetmanet/src/networklayer/ipv4 -I/home/ivocalado/inetmanet/src/util -I/home/ivocalado/inetmanet/src/linklayer/ieee80211/radio/errormodel -I/home/ivocalado/inetmanet/src/util/headerserializers/ipv4 -I/home/ivocalado/inetmanet/src/wpan/linklayer/ieee802154/contract -I/home/ivocalado/inetmanet/src/util/headerserializers -I/home/ivocalado/inetmanet/src/battery/models -I/home/ivocalado/inetmanet/src/linklayer/radio/propagation -I/home/ivocalado/inetmanet/src/networklayer/contract -I/home/ivocalado/inetmanet/src/world/obstacles -I/home/ivocalado/inetmanet/src/linklayer/ieee80211/radio -I/home/ivocalado/inetmanet/src/networklayer/arp -I/home/ivocalado/inetmanet/src/applications -I/home/ivocalado/inetmanet/src/transport/sctp -I/home/ivocalado/inetmanet/src/base -I/home/ivocalado/inetmanet/src/util/headerserializers/sctp -I/home/ivocalado/inetmanet/src/linklayer/contract -I/home/ivocalado/inetmanet/src/transport/udp -I/home/ivocalado/inetmanet/src/applications/pingapp -I/home/ivocalado/inetmanet/src/linklayer/radio -I/home/ivocalado/inetmanet/src/transport/contract -I/home/ivocalado/inetmanet/src/linklayer/ethernet -I/home/ivocalado/inetmanet/src/util/headerserializers/udp -I/home/ivocalado/inetmanet/src/linklayer/ieee80211/mgmt -I/home/ivocalado/inetmanet/src/wpan/phyLayer/ieee802154 -I/home/ivocalado/inetmanet/src/status -I/home/ivocalado/inetmanet/src/networklayer/xmipv6 -I/home/ivocalado/inetmanet/src/mobility/models -L/home/ivocalado/inetmanet/out/$(CONFIGNAME)/src -linet -DINET_IMPORT -KINETMANET_2_0_PROJ=/home/ivocalado/inetmanet
#

# Name of target to be created (-o option)
TARGET = Ncorp-INETMANET$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/kodo/src \
    -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/bundle_dependencies/boost-e92f30/master \
    -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/bundle_dependencies/fifi-6ca972/master/src \
    -I/home/ivocalado/work/UFCG/doutorado/workspace/dependencies/bundle_dependencies/sak-602ce9/master/src \
    -I$(INETMANET_2_0_PROJ)/src \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/common \
    -I$(INETMANET_2_0_PROJ)/src/transport/tcp_common \
    -I$(INETMANET_2_0_PROJ)/src/wpan/networklayer/contract \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/common \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/manetrouting/base \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/autorouting/ipv4 \
    -I$(INETMANET_2_0_PROJ)/src/util/headerserializers/tcp \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/ipv6tunneling \
    -I$(INETMANET_2_0_PROJ)/src/mobility \
    -I$(INETMANET_2_0_PROJ)/src/wpan/linklayer/ieee802154/mac \
    -I$(INETMANET_2_0_PROJ)/src/world/radio \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mac \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/ipv6 \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/icmpv6 \
    -I$(INETMANET_2_0_PROJ)/src/world/powercontrol \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211mesh/locator \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/ipv4 \
    -I$(INETMANET_2_0_PROJ)/src/util \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/radio/errormodel \
    -I$(INETMANET_2_0_PROJ)/src/util/headerserializers/ipv4 \
    -I$(INETMANET_2_0_PROJ)/src/wpan/linklayer/ieee802154/contract \
    -I$(INETMANET_2_0_PROJ)/src/util/headerserializers \
    -I$(INETMANET_2_0_PROJ)/src/battery/models \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/radio/propagation \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/contract \
    -I$(INETMANET_2_0_PROJ)/src/world/obstacles \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/radio \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/arp \
    -I$(INETMANET_2_0_PROJ)/src/applications \
    -I$(INETMANET_2_0_PROJ)/src/transport/sctp \
    -I$(INETMANET_2_0_PROJ)/src/base \
    -I$(INETMANET_2_0_PROJ)/src/util/headerserializers/sctp \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/contract \
    -I$(INETMANET_2_0_PROJ)/src/transport/udp \
    -I$(INETMANET_2_0_PROJ)/src/applications/pingapp \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/radio \
    -I$(INETMANET_2_0_PROJ)/src/transport/contract \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/ethernet \
    -I$(INETMANET_2_0_PROJ)/src/util/headerserializers/udp \
    -I$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mgmt \
    -I$(INETMANET_2_0_PROJ)/src/wpan/phyLayer/ieee802154 \
    -I$(INETMANET_2_0_PROJ)/src/status \
    -I$(INETMANET_2_0_PROJ)/src/networklayer/xmipv6 \
    -I$(INETMANET_2_0_PROJ)/src/mobility/models \
    -I. \
    -Ietx \
    -Ihandler \
    -Ihelper \
    -Imessages \
    -Itransmitter

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(INETMANET_2_0_PROJ)/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath $(INETMANET_2_0_PROJ)/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/Ncorp.o \
    $O/etx/ETXMetric.o \
    $O/handler/EventHandler.o \
    $O/helper/helper-functions-impl.o \
    $O/transmitter/CcackBaseline.o \
    $O/transmitter/HashMatrices.o \
    $O/transmitter/Flow.o \
    $O/transmitter/Binary8MatrixHelpers.o \
    $O/transmitter/DiagonalRandomMatrix.o \
    $O/transmitter/Generation.o \
    $O/messages/packets_m.o

# Message files
MSGFILES = \
    messages/packets.msg

# Other makefile variables (-K)
INETMANET_2_0_PROJ=/home/ivocalado/inetmanet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f Ncorp-INETMANET Ncorp-INETMANET.exe libNcorp-INETMANET.so libNcorp-INETMANET.a libNcorp-INETMANET.dll libNcorp-INETMANET.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f etx/*_m.cc etx/*_m.h
	$(Q)-rm -f handler/*_m.cc handler/*_m.h
	$(Q)-rm -f helper/*_m.cc helper/*_m.h
	$(Q)-rm -f messages/*_m.cc messages/*_m.h
	$(Q)-rm -f transmitter/*_m.cc transmitter/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc etx/*.cc handler/*.cc helper/*.cc messages/*.cc transmitter/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Ncorp.o: Ncorp.cc \
	Ncorp.h \
	etx/ETXMetric.h \
	handler/EventHandler.h \
	helper/constants.h \
	helper/helper-functions.h \
	messagecodes.h \
	messages/PacketSizeConfigurator.h \
	messages/packets_m.h \
	transmitter/Binary8Arithmetic.h \
	transmitter/Binary8MatrixHelpers.h \
	transmitter/Buffers.h \
	transmitter/CcackBaseline.h \
	transmitter/Flow.h \
	transmitter/Generation.h \
	$(INETMANET_2_0_PROJ)/src/applications/AppBase.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/FSMA.h \
	$(INETMANET_2_0_PROJ)/src/base/ILifecycle.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/base/INotifiable.h \
	$(INETMANET_2_0_PROJ)/src/base/IPassiveQueue.h \
	$(INETMANET_2_0_PROJ)/src/base/LifecycleOperation.h \
	$(INETMANET_2_0_PROJ)/src/base/ModuleAccess.h \
	$(INETMANET_2_0_PROJ)/src/base/NodeOperations.h \
	$(INETMANET_2_0_PROJ)/src/base/NotificationBoard.h \
	$(INETMANET_2_0_PROJ)/src/base/NotifierConsts.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/common/MACBase.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/common/WirelessMacBase.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/contract/RadioState.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mac/IQoSClassifier.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mac/Ieee80211Consts.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mac/Ieee80211Frame_m.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mac/Ieee80211Mac.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/mac/WifiMode.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/ieee80211/radio/WifiPreambleType.h \
	$(INETMANET_2_0_PROJ)/src/linklayer/radio/ModulationType.h \
	$(INETMANET_2_0_PROJ)/src/mobility/IMobility.h \
	$(INETMANET_2_0_PROJ)/src/mobility/models/MobilityAccess.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/ipv4/IPv4Route.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/ipv4/IPv4RouteRule.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INETMANET_2_0_PROJ)/src/status/NodeStatus.h \
	$(INETMANET_2_0_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INETMANET_2_0_PROJ)/src/transport/contract/UDPSocket.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/etx/ETXMetric.o: etx/ETXMetric.cc \
	etx/ETXMetric.h \
	handler/EventHandler.h \
	helper/constants.h \
	helper/helper-functions.h \
	messages/packets_m.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/handler/EventHandler.o: handler/EventHandler.cc \
	Ncorp.h \
	handler/EventHandler.h \
	messagecodes.h \
	messages/packets_m.h \
	transmitter/Binary8Arithmetic.h \
	transmitter/Binary8MatrixHelpers.h \
	transmitter/Buffers.h \
	transmitter/CcackBaseline.h \
	transmitter/Flow.h \
	transmitter/Generation.h \
	$(INETMANET_2_0_PROJ)/src/applications/AppBase.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/ILifecycle.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/status/NodeStatus.h \
	$(INETMANET_2_0_PROJ)/src/transport/contract/UDPSocket.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/helper/helper-functions-impl.o: helper/helper-functions-impl.cc \
	helper/constants.h \
	helper/helper-functions.h
$O/messages/packets_m.o: messages/packets_m.cc \
	messages/packets_m.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/transmitter/Binary8MatrixHelpers.o: transmitter/Binary8MatrixHelpers.cc \
	transmitter/Binary8Arithmetic.h \
	transmitter/Binary8MatrixHelpers.h
$O/transmitter/CcackBaseline.o: transmitter/CcackBaseline.cc \
	Ncorp.h \
	helper/constants.h \
	helper/helper-functions.h \
	messagecodes.h \
	messages/packets_m.h \
	transmitter/Binary8Arithmetic.h \
	transmitter/Binary8MatrixHelpers.h \
	transmitter/Buffers.h \
	transmitter/CcackBaseline.h \
	transmitter/Flow.h \
	transmitter/Generation.h \
	$(INETMANET_2_0_PROJ)/src/applications/AppBase.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/ILifecycle.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/status/NodeStatus.h \
	$(INETMANET_2_0_PROJ)/src/transport/contract/UDPSocket.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/transmitter/DiagonalRandomMatrix.o: transmitter/DiagonalRandomMatrix.cc \
	transmitter/DiagonalRandomMatrix.h
$O/transmitter/Flow.o: transmitter/Flow.cc \
	Ncorp.h \
	helper/constants.h \
	helper/helper-functions.h \
	messagecodes.h \
	messages/packets_m.h \
	transmitter/Binary8Arithmetic.h \
	transmitter/Binary8MatrixHelpers.h \
	transmitter/Buffers.h \
	transmitter/CcackBaseline.h \
	transmitter/Flow.h \
	transmitter/Generation.h \
	$(INETMANET_2_0_PROJ)/src/applications/AppBase.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/ILifecycle.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/status/NodeStatus.h \
	$(INETMANET_2_0_PROJ)/src/transport/contract/UDPSocket.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/transmitter/Generation.o: transmitter/Generation.cc \
	helper/constants.h \
	helper/helper-functions.h \
	messagecodes.h \
	messages/packets_m.h \
	transmitter/Binary8Arithmetic.h \
	transmitter/Binary8MatrixHelpers.h \
	transmitter/Buffers.h \
	transmitter/DiagonalRandomMatrix.h \
	transmitter/Generation.h \
	transmitter/HashMatrices.h \
	$(INETMANET_2_0_PROJ)/src/base/Coord.h \
	$(INETMANET_2_0_PROJ)/src/base/INETDefs.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INETMANET_2_0_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INETMANET_2_0_PROJ)/src/util/FWMath.h
$O/transmitter/HashMatrices.o: transmitter/HashMatrices.cc \
	transmitter/DiagonalRandomMatrix.h \
	transmitter/HashMatrices.h

