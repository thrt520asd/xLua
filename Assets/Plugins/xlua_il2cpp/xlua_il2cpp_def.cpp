#include "xlua_il2cpp_def.h"

namespace xlua{

    LogCallback GLogCallback = nullptr;

    void SetLogHandler(LogCallback log){
        GLogCallback = log;
    }

    void GLogFormatted(const char* format, ...) {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, BUFFER_SIZE, format, args);
        va_end(args);
        
        if(GLogCallback){
             GLogCallback(buffer);
        }
    }

    void GWarnFormatted(const char* format, ...) {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, BUFFER_SIZE, format, args);
        va_end(args);

        if (GLogCallback) {
            GLogCallback(buffer);
        }
    }

    void ErrorFormatted(const char* format, ...) {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, BUFFER_SIZE, format, args);
        va_end(args);

        if (GLogCallback) {
            GLogCallback(buffer);
        }
    }
}