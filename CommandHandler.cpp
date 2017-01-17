#include "CommandHandler.h"

using namespace opendnp3;

CommandHandler::CommandHandler()
{

}

CommandHandler::~CommandHandler()
{

}

void CommandHandler::Start()
{

}

void CommandHandler::End()
{

}

CommandStatus CommandHandler::Select(const ControlRelayOutputBlock& command, uint16_t index)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Operate(const ControlRelayOutputBlock& command, uint16_t index, OperateType opType)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Select(const AnalogOutputInt16& command, uint16_t index)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Operate(const AnalogOutputInt16& command, uint16_t index, OperateType opType)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Select(const AnalogOutputInt32& command, uint16_t index)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Operate(const AnalogOutputInt32& command, uint16_t index, OperateType opType)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Select(const AnalogOutputFloat32& command, uint16_t index)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Operate(const AnalogOutputFloat32& command, uint16_t index, OperateType opType)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Select(const AnalogOutputDouble64& command, uint16_t index)
{
    return CommandStatus::NOT_SUPPORTED;
}

CommandStatus CommandHandler::Operate(const AnalogOutputDouble64& command, uint16_t index, OperateType opType)
{
    return CommandStatus::NOT_SUPPORTED;
}
