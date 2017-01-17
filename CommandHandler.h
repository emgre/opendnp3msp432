#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "opendnp3/outstation/ICommandHandler.h"

class CommandHandler : public opendnp3::ICommandHandler
{
public:
	CommandHandler();
	virtual ~CommandHandler();

	virtual void Start();
	virtual void End();

	virtual opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& command, uint16_t index);
	virtual opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, opendnp3::OperateType opType);

	virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& command, uint16_t index);
	virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& command, uint16_t index, opendnp3::OperateType opType);

	virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& command, uint16_t index);
	virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::OperateType opType);

	virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index);
	virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, opendnp3::OperateType opType);

	virtual opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& command, uint16_t index);
	virtual opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, opendnp3::OperateType opType);
};

#endif // COMMANDHANDLER_H
