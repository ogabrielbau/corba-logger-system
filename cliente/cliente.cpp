#include <iostream>
#include <string>
#include <orbsvcs/CosNamingC.h>
#include "LoggerC.h"

int main(int argc, char* argv[])
{
    try
    {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        // Resolve Naming Service
        CORBA::Object_var obj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var naming_context = CosNaming::NamingContext::_narrow(obj.in());

        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("LoggerService");
        name[0].kind = CORBA::string_dup("");

        CORBA::Object_var logger_obj = naming_context->resolve(name);

        LoggerModule::Logger_var logger = LoggerModule::Logger::_narrow(logger_obj.in());
        if (CORBA::is_nil(logger.in()))
        {
            std::cerr << "Nao foi possivel obter referencia ao Logger." << std::endl;
            return 1;
        }

        // Exemplo: alterar verbose remotamente para true (imprimir)
        std::cout << "Setando verbose = true no servidor..." << std::endl;
        logger->verbose(true);

        // Chamar log() com dados fictícios
        LoggerModule::Severity sev = LoggerModule::ERROR;
        const char* endereco = "192.168.0.10:4321";
        CORBA::UShort pid = 1234;
        CORBA::ULong hora = static_cast<CORBA::ULong>(time(nullptr)); // segundos desde epoch
        const char* msg = "Exemplo de erro ficticio vindo do cliente.";

        logger->log(sev, endereco, pid, hora, msg);
        std::cout << "log() chamado (verbose=true)." << std::endl;

        // Agora desliga a impressão no servidor
        std::cout << "Setando verbose = false no servidor..." << std::endl;
        logger->verbose(false);

        // Chamar log() que agora nao deve ser impresso
        logger->log(LoggerModule::WARNING, "10.0.0.5:5555", 4321, hora, "Aviso ficticio - Nao deve aparecer se verbose==false");
        std::cout << "log() chamado (verbose=false)." << std::endl;

        orb->destroy();
    }
    catch (const CORBA::Exception& ex)
    {
        std::cerr << "Excecao CORBA no cliente: ";
        ex._tao_print_exception("Exception");
        return 1;
    }

    return 0;
}
