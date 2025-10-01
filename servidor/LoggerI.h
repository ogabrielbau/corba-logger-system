#ifndef LOGGER_I_H
#define LOGGER_I_H

#include <iostream>
#include <string>
#include "LoggerS.h"   // gerado pelo tao_idl

class Logger_i : public POA_LoggerModule::Logger
{
public:
    Logger_i();
    virtual ~Logger_i();

    // operação remota
    virtual void log(LoggerModule::Severity severidade,
                     std::string endereco,
                     CORBA::UShort pid,
                     CORBA::ULong hora,
                     std::string msg) override;

    // atributo verbose - getter / setter
    virtual CORBA::Boolean verbose() override;
    virtual void verbose(CORBA::Boolean v) override;

private:
    CORBA::Boolean verbose_;
};

#endif // LOGGER_I_H
