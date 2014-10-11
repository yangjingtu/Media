#include "StdAfx.h"
#include "AACDecoder.h"
#include "AACDecodeImpl.h"

CAACDecoder::CAACDecoder(void)
{
}

CAACDecoder::~CAACDecoder(void)
{
}

CAACDecoder* CAACDecoder::CreateDecoder()
{
	return new CAACDecodeImpl();
}

void CAACDecoder::DeleteDecoder(CAACDecoder* decoder)
{
	if(decoder)
	{
		delete decoder;
		decoder = NULL;
	}
}
