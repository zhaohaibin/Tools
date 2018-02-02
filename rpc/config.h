#ifndef _CONFIG_H

#ifndef RPC_API
	#if defined(_MSC_VER)
#if defined(RPC_DLL)
		#if defined(RPC_EXPORTS)
			#define RPC_API __declspec(dllexport)
		#else
			#define RPC_API __declspec(dllimport)
		#endif//RPC_EXPORTS
#else
	#define RPC_API
#endif
	#else
		#if defined(USE_GCC_VISIBILITY_FLAG)
			#define RPC_API __attribute__ ((visibility("default")))
		#else
			#define RPC_API
		#endif //USE_GCC_VISIBILITY_FLAG
	#endif //_MSC_VER
#endif//RPC_API

#endif ///_RPC_H