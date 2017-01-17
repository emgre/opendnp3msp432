#include "LinkRouter.h"
#include "driverlib.h"

LinkRouter* gLinkParser = nullptr;

const eUSCI_UART_Config uartConfig =
{
	EUSCI_A_UART_CLOCKSOURCE_SMCLK,
	9,
	12,
	34,	// Baud rate of 9600 @ 1.5 MHz
	EUSCI_A_UART_NO_PARITY,
	EUSCI_A_UART_LSB_FIRST,
	EUSCI_A_UART_ONE_STOP_BIT,
	EUSCI_A_UART_MODE,
	EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};

void UART_ISR()
{
	auto status = MAP_UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT|EUSCI_A_UART_TRANSMIT_INTERRUPT);

	if(status | EUSCI_A_UART_RECEIVE_INTERRUPT)
	{
		MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
		auto data = MAP_UART_receiveData(EUSCI_A0_BASE);
		gLinkParser->Receive(data);
	}

	if(status | EUSCI_A_UART_TRANSMIT_INTERRUPT)
	{
		MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);

		uint8_t txByte = 0;
		if(gLinkParser->GetTx(txByte))
		{
			MAP_UART_transmitData(EUSCI_A0_BASE, txByte);
		}

		else
		{
			MAP_UART_disableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
		}
	}
}

LinkRouter::LinkRouter(openpal::Logger& logger,
	const std::shared_ptr<openpal::IExecutor>& exe,
	const std::shared_ptr<opendnp3::ILinkSession>& context)
:m_isTransmitting(false),
m_pExecutor(exe),	
m_pContext(context),
m_parser(logger)
{
	
}

void LinkRouter::Init()
{
	gLinkParser = this;
	
    // Setup the GPIOs
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	// Setup UART
	MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
	MAP_UART_enableModule(EUSCI_A0_BASE);

	// Setup interrupt
	MAP_UART_registerInterrupt(EUSCI_A0_BASE, &UART_ISR);
	MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
}

void LinkRouter::Receive(uint8_t byte)
{	
	m_rxBuffer.Put(byte);
}

bool LinkRouter::GetTx(uint8_t& txByte)
{
	return m_txBuffer.Get(txByte);
}

void LinkRouter::CheckTx()
{	
	if(m_isTransmitting)
	{						
		m_txBuffer.PutMany(m_transmission);
		
		MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
		
		if(m_transmission.IsEmpty() && m_txBuffer.Empty())
		{
			m_isTransmitting = false;						
			m_pContext->OnTransmitResult(true);
		}		
	}	
}

void LinkRouter::CheckRxTx()
{		
	CheckTx();
	CheckRx();
}

void LinkRouter::CheckRx()
{		
	auto buffer = m_parser.WriteBuff();
	uint32_t count = m_rxBuffer.GetMany(buffer);
				
	if(count > 0)
	{
		m_parser.OnRead(count, *m_pContext);
	}
}
	
void LinkRouter::BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& pContext)
{
	if(m_isTransmitting)
	{
		auto failure = [&pContext](){ pContext.OnTransmitResult(false); };
		m_pExecutor->Post(failure);
	}
	else	
	{
		m_isTransmitting = true;
		m_transmission = buffer;		
		
		CheckTx();													
	}	
}