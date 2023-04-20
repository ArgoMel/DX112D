#pragma once
#include "Client.h"

class CClientManager
{
public:
	CClientManager();
	~CClientManager();
	bool Init(HINSTANCE hInst);
	int Run();
};

