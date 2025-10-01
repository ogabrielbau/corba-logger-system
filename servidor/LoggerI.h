#ifndef LOGGER_I_H
#define LOGGER_I_H

#include <iostream>
#include "LoggerS.h"   // gerado por tao_idl

class Logger_i : public POA_LoggerModule::Logger
{
public:
    Logger_i();
    virtual ~Logger_i();

    // operação remota
    virtual void log(LoggerModule::Severity severidade,
                     const char* endereco,
                     CORBA::UShort pid,
                     CORBA::ULong hora,
                     const char* msg);

    // atributo verbose - getter / setter
    virtual CORBA::Boolean verbose();
    virtual void verbose(CORBA::Boolean v);

private:
    CORBA::Boolean verbose_;
};

#endif // LOGGER_I_H
