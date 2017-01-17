#ifndef LINKLISTENER_H
#define LINKLISTENER_H

#include <opendnp3/link/ILinkListener.h>
#include <opendnp3/gen/LinkStatus.h>

class LinkListener : public opendnp3::ILinkListener
{
public:
	virtual void OnStateChange(opendnp3::LinkStatus value) {}
	virtual void OnKeepAliveInitiated() {}
	virtual void OnKeepAliveFailure() {}
	virtual void OnKeepAliveSuccess() {}
};

#endif // LINKLISTENER_H