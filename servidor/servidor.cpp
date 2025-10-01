#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "LoggerS.h"
#include "LoggerC.h"
#include "Logger_i.h"
#include <tao/corba.h>
#include <tao/PortableServer/PortableServer.h>
#include <tao/ORB_Core.h>

using namespace std;

ofstream logFile("logger.txt", ios::app); // Arquivo para registrar logs

// Função para imprimir log no console e salvar em arquivo
void log_message(const string &severity, const string &ip, int pid, const string &msg) {
    time_t now = time(nullptr);
    cout << "---- Logger Notification ----" << endl;
    cout << "Severidade: " << severity << endl;
    cout << "Endereco:   " << ip << endl;
    cout << "PID:        " << pid << endl;
    cout << "Hora:       " << ctime(&now);
    cout << "Msg:        " << msg << endl;
    cout << "-----------------------------" << endl;

    if (logFile.is_open()) {
        logFile << severity << "," << ip << "," << pid << "," << now << "," << msg << endl;
    }
}

// Implementação do Logger
class Logger_i : public POA_Logger {
public:
    void log(const char* severity, const char* ip, CORBA::Long pid, const char* msg) override {
        try {
            log_message(severity, ip, pid, msg);
        } catch (const CORBA::Exception& ex) {
            cerr << "Erro ao processar log: " << ex._name() << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        CORBA::Object_var poaObj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var rootPoa = PortableServer::POA::_narrow(poaObj);

        PortableServer::POAManager_var poaManager = rootPoa->the_POAManager();
        poaManager->activate();

        Logger_i loggerImpl;

        CORBA::Object_var loggerObj = loggerImpl._this();

        CORBA::Object_var namingObj = orb->string_to_object("corbaloc::localhost:1050/NameService");
        CosNaming::NamingContext_var namingContext = CosNaming::NamingContext::_narrow(namingObj);

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("LoggerService");

        namingContext->rebind(name, loggerObj);

        cout << "Servidor Logger pronto e registrado no Naming Service como 'LoggerService'." << endl;

        // Mantém o servidor rodando
        orb->run();
    } catch (const CORBA::Exception& ex) {
        cerr << "Exceção CORBA: " << ex._name() << endl;
        return 1;
    }
    return 0;
}
