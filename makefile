# Caminhos (ajuste se necessário)
SERVER_DIR = servidor
CLIENT_DIR = cliente

CXX = g++
CXXFLAGS = -std=c++11 -I. -I$(SERVER_DIR) -I$(CLIENT_DIR) \
           -I$(ACE_ROOT)/include -I$(TAO_ROOT)/include
LDFLAGS = -L$(ACE_ROOT)/lib -L$(TAO_ROOT)/lib
LDLIBS = -lTAO -lACE

# Arquivos gerados pelo tao_idl
GEN_SRCS = LoggerC.cpp LoggerS.cpp
GEN_HDRS = LoggerC.h LoggerS.h

# Fontes
SERVER_SRCS = $(SERVER_DIR)/servidor.cpp $(SERVER_DIR)/LoggerI.cpp $(GEN_SRCS)
CLIENT_SRCS = $(CLIENT_DIR)/cliente.cpp LoggerC.cpp

# Alvos
all: servidor cliente

servidor: $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SERVER_SRCS) $(LDFLAGS) $(LDLIBS)

cliente: $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(CLIENT_SRCS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f servidor cliente *.o
