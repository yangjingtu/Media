#include "StdAfx.h"
#include "AACEncoder.h"
#include "AACEncodeImpl.h"

CAACEncoder::CAACEncoder(void)
{
}

CAACEncoder::~CAACEncoder(void)
{
}

CAACEncoder* CAACEncoder::CreateEncoder()
{
	return new CAACEncodeImpl();
}

void CAACEncoder::DeleteEncoder(CAACEncoder* decoder)
{
	delete decoder;
}


