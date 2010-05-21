// replitxtApp.cpp

#ifndef __replitxtAPP_H__
#include "replitxtApp.h"
#endif

#ifndef __replitxtWINDOW_H__
#include "replitxtWindow.h"
#endif

replitxtApp::replitxtApp()
	: BApplication("application/x-vnd.rxc-replitxt")
{
	replitxtWindow *replitxt_w = new replitxtWindow();
	
	replitxt_w->Show();
}