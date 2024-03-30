#ifndef DEBUGCOMMAND_H
#define DEBUGCOMMAND_H

#include "DebugCommandBase.h"
#include <functional>
#include <string>

// Clase DebugCommand para funciones sin argumentos
class DebugCommand : public DebugCommandBase {
private:
    std::function<void()> _command;

public:
    DebugCommand(const std::string& id, const std::string& description, const std::string& format, const std::function<void()>& command)
        : DebugCommandBase(id, description, format), _command(command) {}

    void Invoke() {
        _command();
    }
};

// Clase DebugCommand para funciones con argumentos
template <typename T>
class DebugCommandArg : public DebugCommandBase {
private:
    std::function<void(T)> _command;

public:
    DebugCommandArg(const std::string& id, const std::string& description, const std::string& format, const std::function<void(T)>& command)
        : DebugCommandBase(id, description, format), _command(command) {}


    void Invoke() override {
        throw std::runtime_error("DebugCommandArg::Invoke() no se puede llamar sin dar alg�n par�metro");
    }

    // Implementaci�n del m�todo Invoke para llamar a la funci�n con el argumento
    void Invoke(T value) {
        _command(value);
    }

    // Implementaci�n de las funciones virtuales puras de DebugCommandBase
    std::string GetCommandId() const {
        return DebugCommandBase::GetCommandId(); // Llamar al m�todo de la clase base
    }

    std::string GetCommandDescription() const {
        return DebugCommandBase::GetCommandDescription(); // Llamar al m�todo de la clase base
    }

    std::string GetCommandFormat() const {
        return DebugCommandBase::GetCommandFormat(); // Llamar al m�todo de la clase base
    }
};

#endif // DEBUGCOMMAND_H