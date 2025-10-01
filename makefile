# Makefile adaptado para seu ambiente (/opt/ACE_wrappers)
ACE_ROOT ?= /opt/ACE_wrappers
TAO_ROOT ?= $(ACE_ROOT)/TAO

CXX = g++
CXXFLAGS = -std=c++11 -I. -Iservidor -Icliente \
    -I$(ACE_ROOT) \
    -I$(ACE_ROOT)/ACE/include \
    -I$(TAO_ROOT)/include \
    -I$(TAO_ROOT)/orbsvcs \
    -I$(TAO_ROOT)/orbsvcs/orbsvcs

LDFLAGS = -L$(ACE_ROOT)/lib -L$(TAO_ROOT)/lib
LDLIBS = -lTAO -lACE

GEN_SRCS = LoggerC.cpp LoggerS.cpp

SERVER_SRCS = servidor/servidor.cpp servidor/LoggerI.cpp $(GEN_SRCS)
CLIENT_SRCS = cliente/cliente.cpp LoggerC.cpp

all: servidor cliente

servidor: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SERVER_SRCS) $(LDFLAGS) $(LDLIBS)

cliente: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(CLIENT_SRCS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f servidor cliente *.o

.PHONY: all clean
