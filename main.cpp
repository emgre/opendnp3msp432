#include "driverlib.h"
#include <stdint.h>

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/OutstationContext.h>
#include <opendnp3/outstation/IOutstationApplication.h>

#include "Executor.h"
#include "CommandHandler.h"
#include "LinkRouter.h"
#include "LinkListener.h"

using namespace opendnp3;
using namespace openpal;

constexpr uint16_t NUM_BINARY = 5;

constexpr uint32_t MAX_APDU_SIZE = 249;

void setupHardware();

OutstationConfig GetOutstationConfig()
{
	OutstationConfig config;
	config.params.maxTxFragSize = MAX_APDU_SIZE;
	config.params.maxRxFragSize = MAX_APDU_SIZE;
	config.eventBufferConfig = EventBufferConfig(5,0,2);
	config.params.allowUnsolicited = true;		
	return config;
}

int main(void)
{
    // Stop watchdog
    MAP_WDT_A_holdTimer();

    setupHardware();

	auto exe = std::make_shared<Executor>(5, 5);
	Logger log(nullptr, "root", 0);
	auto listener = std::make_shared<LinkListener>();
	TransportStack stack(log, exe, listener, MAX_APDU_SIZE, LinkConfig(false, false));

	auto commandHandler = std::make_shared<CommandHandler>();

	OContext outstation(
		GetOutstationConfig(), 
		DatabaseSizes::BinaryOnly(NUM_BINARY),
		log,	
		exe,
		stack.transport,
		commandHandler,
		DefaultOutstationApplication::Create()
	);

	stack.transport->SetAppLayer(outstation);

	LinkRouter router(log, exe, stack.link);
	stack.link->SetRouter(router);	
	stack.link->OnLowerLayerUp();

	exe->Init();
	router.Init();

	MAP_Interrupt_enableSleepOnIsrExit();
	MAP_Interrupt_enableMaster();

    for(;;)
    {
		router.CheckRxTx();
		exe->Run();

    	MAP_PCM_gotoLPM0();
    }
}

void setupHardware()
{
    // Enable Floating Point Unit
	MAP_FPU_enableModule();

	// Set Core Clock to 1.5 MHz
	/*MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_1_5);

	// Setup the clocks
	MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // 48 Mhz
	MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_2);  // 24 Mhz
	MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);	 // 48 Mhz
	MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);*/
}