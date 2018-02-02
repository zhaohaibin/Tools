#ifndef _CLIENT_H
#define _CLIENT_H
#include "../include/config.h"
#include <string>
//#include "boost/serialization/access.hpp"
using namespace std;
namespace rpc
{
	class rpc_client_impl;
	
	class RPC_API rpc_request
	{
	public:
		//friend class boost::serialization::access;
		template<class Archive>  
		void serialize(Archive& ar)  
		{  
			ar & (*m_pContentStr); 
		}  
	public:
		rpc_request();
		rpc_request(const std::string& str);
		~rpc_request();

		void setContent(const std::string& str);
		const string& getContent() const;
	private:
		void _clear();
	protected:
		std::string* m_pContentStr;
	};

	class RPC_API rpc_response
	{
	public:
		//friend class boost::serialization::access;
		template<class Archive>  
		void serialize(Archive& ar)  
		{  
			ar & (*m_pContentStr); 
		}
	public:
		rpc_response();
		~rpc_response();

		void setContent(const std::string& str);
		const string& getContent() const;
	private:
		void _clear();
	protected:
		std::string* m_pContentStr;
	};
	class RPC_API rpc_client
	{
	public:
		rpc_client(const std::string& ip, int port);
		~rpc_client();
		void invoke(const std::string& funcName, 
			const rpc_request& rpcRequest, rpc_response& rpcResponse, 
			int timeOutSec = 60);
	private:
		rpc_client_impl* m_impl;
	};
}
#endif