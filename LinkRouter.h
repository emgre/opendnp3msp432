#ifndef LINKROUTER_H
#define LINKROUTER_H

#include <opendnp3/link/ILinkTx.h>

#include <openpal/logging/Logger.h>
#include <openpal/container/RingBuffer.h>
#include <openpal/executor/IExecutor.h>
#include <opendnp3/link/ILinkSession.h>
#include <opendnp3/link/LinkLayerParser.h>

class LinkRouter : public opendnp3::ILinkTx
{
public:
	LinkRouter(openpal::Logger& logger, const std::shared_ptr<openpal::IExecutor>& exe, const std::shared_ptr<opendnp3::ILinkSession>& context);
	
	virtual void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& pContext) final override;	
	
	void Receive(uint8_t rxByte);
	bool GetTx(uint8_t& txByte);
	
	void Init();
	void CheckRxTx();
	
private:
	void CheckTx();
	void CheckRx();
							
	openpal::RingBuffer<16> m_rxBuffer;
	openpal::RingBuffer<16> m_txBuffer;
	
	bool m_isTransmitting;
	openpal::RSlice m_transmission;		
	
	std::shared_ptr<openpal::IExecutor> m_pExecutor;
	std::shared_ptr<opendnp3::ILinkSession> m_pContext;		
	opendnp3::LinkLayerParser m_parser;
};

#endif // LINKROUTER_H