#include "RCF/RCF.hpp"
#include "rpc_init.h"
namespace rpc
{
	static RCF::RcfInitDeinit*  pRcfInit = NULL;
	rpc_init::rpc_init()
	{
		if(pRcfInit == NULL)
			pRcfInit = new RCF::RcfInitDeinit();
	}

	rpc_init::~rpc_init()
	{
	}
}


