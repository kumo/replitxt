// replitxtReplicant.h: definition of a transparent replicant class

#ifndef __replitxtREPLICANT_H__
#define __replitxtREPLICANT_H__

#include <View.h>
#include <Screen.h>
#include <Message.h>
#include <Dragger.h>
#include <String.h>
#include <Application.h>
#include <Shelf.h>
#include "stdio.h"
#include <MessageFilter.h>
#include <OS.h>

#ifndef __xREPLICANT_H__
#include "xReplicant.h"
#endif

// this should be exported by the derived class
class _EXPORT replitxtReplicant;

class replitxtReplicant : public xReplicant
{
public:
						replitxtReplicant(const char* text);
						replitxtReplicant(BMessage *data);
//						~replitxtReplicant();

	void				SetText(const char* text);
	const char*			GetText() const;
	
	int32				Index() const { return fReplicantNum; };
	void				SetID(int32 id) { fID = id; fNeedID = false; printf("I am %ld\n", fID); };
	int32				ID() const { return fID; };
	bool				NeedID() const { return fNeedID; };
protected:
	virtual void		AttachedToWindow();
	virtual void		DetachedFromWindow();

	static BArchivable* Instantiate(BMessage *data);
	virtual status_t 	Archive(BMessage *data, bool deep = true) const;
	
	virtual void		Draw(BRect updateRect);
	
private:
	const char*			fString;
	float				font_ascent;
	BMessageFilter*		fMessageFilter;
	
	int32				fID;
	int32				fReplicantNum;
	bool				fNeedID;

friend	filter_result		filter(BMessage *message, BHandler **target, BMessageFilter *messageFilter);
};
#endif