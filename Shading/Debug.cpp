#include "Debug.h"

void Debug::Log( const char*message)  
{
	printf("[Engine] Log : %s\n",message);
}

void Debug::LogWarning( const char*message )   
{
	printf("[Engine] Warning : %s\n",message);
}

void Debug::LogError( const char*message )
{
	printf("[Engine] Error : %s\n",message);
}