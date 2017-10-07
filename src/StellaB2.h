// StellaB2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CStellaB2App:
// See StellaB2.cpp for the implementation of this class
//

class CStellaB2App : public CWinApp
{
public:
	CStellaB2App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CStellaB2App theApp;