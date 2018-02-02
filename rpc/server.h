#ifndef _SERVER_H
#define _SERVER_H
#include <string>
#include "../include/config.h"
#include "../src/service/service.h"

namespace rpc
{
	class rpc_server_impl;

	class RPC_API rpc_server
	{
	public:
		rpc_server(const std::string& ip, int port);
		~rpc_server();

	public:
		void bindService(rpc_service& se, const std::string& name = "");
		void setThreadMaxCount(unsigned int count);
		void start();
		void stop();
	private:
		rpc_server_impl* m_pImpl;
	};
}
#endif //_SERVER_H