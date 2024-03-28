//#include <iostream>
//#include <functional>
//#include <string>
//#include "App.h"
//#include "DebugCommandBase.h"
//
//class DebugCommandBase
//{
//private: 
//	std::string _commandId;
//	std::string _commandDescription;
//	std::string _commandFormat;
//
//public:
//
//	DebugCommandBase(const std::string& id, const std::string& description, const std::string& format)
//		: _commandId(id), _commandDescription(description), _commandFormat(format) {}
//
//	std::string GetCommandId() const { return _commandId; }
//	std::string GetCommandDescription() const { return _commandDescription; }
//	std::string GetCommandFormat() const { return _commandFormat; }
//
//};
//
//
//class DebugCommand : DebugCommandBase {
//
//private:
//	std::function<void()> _command;
//
//public:
//	// Constructor
//	DebugCommand(const std::string& id, const std::string& description, const std::string& format, const std::function<void()>& command)
//		: DebugCommandBase(id, description, format), _command(command) {}
//
//	// Método para invocar el comando
//	void Invoke() {
//		_command();
//	}
//
//};
