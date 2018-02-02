#ifndef _RPC_SERVICE_HPP
#define _RPC_SERVICE_HPP

#include <string>
#include "RCF/Idl.hpp"
#include "../include/client.h"
using namespace std;
namespace rpc
{
	RCF_BEGIN(i_rpc_service, "i_rpc_service")
		RCF_METHOD_V3(void, rpcCall,const std::string&, const rpc_request&, rpc_response&);
	RCF_END(i_rpc_service);
}
#endif