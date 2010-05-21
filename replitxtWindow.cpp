// replitxtWindow.cpp

#ifndef __replitxtWINDOW_H__
#include "replitxtWindow.h"
#endif

#ifndef __replicantWINDOW_H__
#include "replicantWindow.h"
#endif

#ifndef __replitxtREPLICANT_H__
#include "replitxtReplicant.h"
#endif

#define M_CREATE_REPLICANT 'crea'

replitxtWindow::replitxtWindow()
	: BWindow(BRect(70, 70, 240, 180), "replitxt",
			B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	InitGUI();
}

void
replitxtWindow::InitGUI()
{
	BRect rect(Bounds());
	
	BView *background = new BView(Bounds(), "background", B_FOLLOW_ALL, 0);
	background->SetViewColor(216, 216, 216, 0);
	AddChild(background);
	
	rect.InsetBy(10,10);

	BBox *box = new BBox(rect);
	background->AddChild(box);
	box->SetLabel("Type text here ...");
		
	rect.InsetBy(10,10);
	rect.OffsetBy(-10, 0);
	textView = new BTextControl(rect, "text_view", "", "", new BMessage(M_CREATE_REPLICANT));
	textView->SetDivider(0.0);
	box->AddChild(textView);
	textView->MakeFocus(true);
	
	// resize the box
	box->ResizeTo(box->Bounds().right, 10+textView->Bounds().bottom + 20);

	rect.Set(10, 25+box->Bounds().bottom, 40, 46);
	rect.InsetBy(10,0);
	
	BButton *button = new BButton(rect, "create", "Create replicant...", new BMessage(M_CREATE_REPLICANT));
	button->ResizeToPreferred();
	background->AddChild(button);
}

void
replitxtWindow::MessageReceived(BMessage *msg)
{
	replicantWindow *repWin;
	switch (msg->what) {
		case M_CREATE_REPLICANT:
			repWin = new replicantWindow(textView->Text());
			repWin->Show();
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}

bool
replitxtWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}