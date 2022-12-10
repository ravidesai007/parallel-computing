      
#pragma GCC optimize ("-Ofast")

#include "sha256.h"

#ifndef SHA1_DISABLED
#ifndef SHA1_DISABLE_WRAPPER
void Sha1Wrapper::init(void)
{
	sha1_hasher_init(&_hasher);
}

#ifdef SHA1_ENABLE_HMAC
void Sha1Wrapper::initHmac(const uint8_t * secret, uint16_t secretLength)
{
	sha1_hasher_init_hmac(&_hasher, secret, secretLength);
}

uint8_t * Sha1Wrapper::resultHmac(void)
{
	return sha1_hasher_gethmac(&_hasher);
}
#endif


size_t Sha1Wrapper::write(uint8_t byte)
{
	if(sha1_hasher_putc(&_hasher, byte) == byte)
	{
		return 1;
	}
	return 0;
}

uint8_t * Sha1Wrapper::result(void)
{
	return sha1_hasher_gethash(&_hasher);
}

Sha1Wrapper Sha1;

#endif
#endif
