#ifndef DEBUGCOMMAND_H
#define DEBUGCOMMAND_H

#include "DebugCommandBase.h"
#include <functional>
#include <string>

class DebugCommand : public DebugCommandBase {
private:
    std::function<void()> _command;

public:
    // Constructor
    DebugCommand(const std::string& id, const std::string& description, const std::string& format, const std::function<void()>& command)
        : DebugCommandBase(id, description, format), _command(command) {}

    DebugCommand() {}

    // Método para invocar el comando
    void Invoke() {
        _command();
    }
};

#endif // DEBUGCOMMAND_H