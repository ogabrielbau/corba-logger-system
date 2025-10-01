#include <iostream>
#include <string>
#include "LoggerC.h"
#include "LoggerS.h"
#include <tao/corba.h>
#include <tao/ORB_Core.h>
#include <CosNamingC.h>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        CORBA::Object_var obj = orb->string_to_object("corbaloc::localhost:1050/NameService");
        CosNaming::NamingContext_var nc = CosNaming::NamingContext::_narrow(obj);

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("LoggerService");

        Logger_var logger = Logger::_narrow(nc->resolve(name));

        if (CORBA::is_nil(logger)) {
            cerr << "Erro: LoggerService não encontrado!" << endl;
            return 1;
        }

        cout << "Cliente conectado ao LoggerService." << endl;

        // Loop para enviar múltiplas mensagens
        string severity, message;
        int pid = 1234; // PID fictício
        string ip = "192.168.0.10";

        while (true) {
            cout << "Digite a severidade (INFO, WARN, DEBUG, ERROR) ou 'sair' para encerrar: ";
            cin >> severity;
            if (severity == "sair") break;

            cin.ignore(); // Limpa buffer
            cout << "Digite a mensagem: ";
            getline(cin, message);

            try {
                logger->log(severity.c_str(), ip.c_str(), pid, message.c_str());
                cout << "Mensagem enviada!" << endl;
            } catch (const CORBA::Exception& ex) {
                cerr << "Falha ao enviar log: " << ex._name() << endl;
            }
        }

        orb->destroy();
    } catch (const CORBA::Exception& ex) {
        cerr << "Exceção CORBA: " << ex._name() << endl;
        return 1;
    }
    return 0;
}
