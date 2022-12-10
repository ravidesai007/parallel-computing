                                              
#pragma GCC optimize ("-Ofast")

#include "config.h"

#ifndef Sha1_h
#define Sha1_h

#include <inttypes.h>
#include "Print.h"
#include "sha1/sha1.h"

#ifndef SHA1_DISABLE_WRAPPER
class Sha1Wrapper : public Print
{
	public:
		void init(void);
		uint8_t * result(void);
#ifdef SHA1_ENABLE_HMAC
		void initHmac(const uint8_t * secret, uint16_t secretLength);
		uint8_t * resultHmac(void);
#endif
		virtual size_t write(uint8_t);
		using Print::write;
	private:
		struct sha1_hasher_s _hasher;

};

extern Sha1Wrapper Sha1;
#endif

#endif
