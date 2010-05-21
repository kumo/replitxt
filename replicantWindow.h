// replicantWindow.h: definition of the simple replicant window  class

#ifndef __replicantWINDOW_H__
#define __replicantWINDOW_H__

#include <Window.h>
#include <View.h>
#include <Application.h>

class replicantWindow : public BWindow
{
public:
			replicantWindow(const char* text);
	
	bool	QuitRequested();
	
protected:
	void	InitGUI(const char* text);
};
#endif