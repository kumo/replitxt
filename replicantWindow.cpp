// replicantWindow.cpp

#ifndef __replicantWINDOW_H__
#include "replicantWindow.h"
#endif

#ifndef __replitxtREPLICANT_H__
#include "replitxtReplicant.h"
#endif

replicantWindow::replicantWindow(const char* text)
	: BWindow(BRect(70, 70, 230, 170), "Replicant!",
			B_TITLED_WINDOW, B_NOT_ZOOMABLE)
{
	InitGUI(text);
}

void
replicantWindow::InitGUI(const char* text)
{
	BView *background = new BView(Bounds(), "background", B_FOLLOW_ALL, 0);
	AddChild(background);
	
	replitxtReplicant *replicant = new replitxtReplicant(text);
	background->AddChild(replicant);
	
	BRect bounds = replicant->Bounds();
	bounds.InsetBy(-20, -20);
	ResizeTo(bounds.right, bounds.bottom);
}

bool
replicantWindow::QuitRequested()
{
	return true;
}