// replitxtWindow.h: definition of the simple replitxt window  class

#ifndef __replitxtWINDOW_H__
#define __replitxtWINDOW_H__

#include <Window.h>
#include <View.h>
#include <TextControl.h>
#include <Button.h>
#include <Application.h>
#include <Box.h>

class replitxtWindow : public BWindow
{
public:
					replitxtWindow();
	
	bool			QuitRequested();
	
protected:
	void			InitGUI();
	virtual void	MessageReceived(BMessage *msg);
private:
	BTextControl 	*textView;
};
#endif