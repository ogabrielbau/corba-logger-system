#include "LoggerI.h"
#include <iomanip>
#include <ctime>

Logger_i::Logger_i()
 : verbose_(true) // por padrão imprime, você pode mudar
{
}

Logger_i::~Logger_i()
{
}

void Logger_i::log(LoggerModule::Severity severidade,
                   const char* endereco,
                   CORBA::UShort pid,
                   CORBA::ULong hora,
                   const char* msg)
{
    // Se verbose_ == true, imprime a mensagem recebida
    if (verbose_)
    {
        // converte hora (segundos desde epoch) para string legível
        std::time_t t = static_cast<std::time_t>(hora);
        std::tm tm_buf;
#if defined(_WIN32)
        gmtime_s(&tm_buf, &t);
#else
        gmtime_r(&t, &tm_buf);
#endif
        char timebuf[64];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S (UTC)", &tm_buf);

        const char* sev_str = "UNKNOWN";
        switch (severidade)
        {
            case LoggerModule::DEBUG: sev_str = "DEBUG"; break;
            case LoggerModule::WARNING: sev_str = "WARNING"; break;
            case LoggerModule::ERROR: sev_str = "ERROR"; break;
            case LoggerModule::CRITICAL: sev_str = "CRITICAL"; break;
        }

        std::cout << "---- Logger Notification ----\n";
        std::cout << "Severidade: " << sev_str << "\n";
        std::cout << "Endereco:   " << endereco << "\n";
        std::cout << "PID:        " << pid << "\n";
        std::cout << "Hora:       " << timebuf << " (" << hora << "s since epoch)\n";
        std::cout << "Msg:        " << msg << "\n";
        std::cout << "-----------------------------" << std::endl;
    }
    // Se verbose_ == false, não imprime (a operação ainda retorna normalmente)
}

CORBA::Boolean Logger_i::verbose()
{
    return verbose_;
}

void Logger_i::verbose(CORBA::Boolean v)
{
    verbose_ = v;
}
