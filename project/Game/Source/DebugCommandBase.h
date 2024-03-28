#ifndef __DEBUGCOMMANDBASE_H__
#define __DEBUGCOMMANDBASE_H__

#include <iostream>
#include <functional>
#include <string>

class DebugCommandBase
{
private:
    std::string _commandId;
    std::string _commandDescription;
    std::string _commandFormat;

public:
    DebugCommandBase(const std::string& id, const std::string& description, const std::string& format)
        : _commandId(id), _commandDescription(description), _commandFormat(format) {}

    DebugCommandBase() {}

    std::string GetCommandId() const { return _commandId; }
    std::string GetCommandDescription() const { return _commandDescription; }
    std::string GetCommandFormat() const { return _commandFormat; }
};



#endif // __DEBUGCOMMANDBASE_H__