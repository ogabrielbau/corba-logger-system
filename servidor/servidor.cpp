#include <iostream>
#include <string>
#include <orbsvcs/CosNamingC.h>
#include "LoggerI.h"

int main(int argc, char* argv[])
{
    try
    {
        // Inicializa ORB
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        // Obtém RootPOA e POA Manager
        CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var root_poa = PortableServer::POA::_narrow(poa_obj.in());
        PortableServer::POAManager_var poa_manager = root_poa->the_POAManager();

        // Cria instancia do servant
        Logger_i* logger_servant = new Logger_i();

        // Aceita o servant no POA e obtém referência CORBA
        PortableServer::ObjectId_var id = root_poa->activate_object(logger_servant);
        CORBA::Object_var logger_ref = root_poa->id_to_reference(id.in());

        // Narrow para o tipo Logger
        LoggerModule::Logger_var logger = LoggerModule::Logger::_narrow(logger_ref.in());

        // Publique a IOR no Servidor de Nomes (CosNaming)
        CORBA::Object_var obj = orb->resolve_initial_references("NameService");
        CosNaming::NamingContext_var naming_context = CosNaming::NamingContext::_narrow(obj.in());
        if (CORBA::is_nil(naming_context.in()))
        {
            std::cerr << "Erro ao obter Naming Service" << std::endl;
            return 1;
        }

        // Nome no Naming Service: "LoggerService"
        CosNaming::Name name;
        name.length(1);
        name[0].id = CORBA::string_dup("LoggerService");
        name[0].kind = CORBA::string_dup("");

        naming_context->rebind(name, logger.in()); // rebind para sobrescrever se existir

        // Ativa POA Manager e entra em loop
        poa_manager->activate();
        std::cout << "Servidor Logger pronto e registrado no Naming Service como 'LoggerService'." << std::endl;

        // Mantem ORB rodando (aceitando requisições)
        orb->run();

        // cleanup (quando orb->shutdown for chamado)
        root_poa->destroy(1, 1);
        orb->destroy();
    }
    catch (const CORBA::Exception& ex)
    {
        std::cerr << "Excecao CORBA no servidor: ";
        ex._tao_print_exception("Exception");
        return 1;
    }

    return 0;
}
