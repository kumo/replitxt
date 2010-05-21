// replitxtReplicant: implementation of a replitxt replicant

#ifndef __replitxtREPLICANT_H__
#include "replitxtReplicant.h"
#endif

const char *app_signature = "application/x-vnd.rxc-replitxt";
const char *replicant_name = "replitxt";

replitxtReplicant *global_pointer;

filter_result filter(BMessage *message, BHandler **target, BMessageFilter *messageFilter);

// *******************************
// * replitxtReplicant()
// * default constructor
// *******************************
replitxtReplicant::replitxtReplicant(const char* text)
	: xReplicant(BRect(10, 10, 151, 41), replicant_name)
{
	SetText(text);
	
	fNeedID = true;
}

// *******************************
// * replitxtReplicant()
// * replicant constructor
// *******************************
replitxtReplicant::replitxtReplicant(BMessage *data)
	: xReplicant(data)
{
	SetViewColor(B_TRANSPARENT_32_BIT);

	fString = data->FindString("text");
	font_ascent = data->FindFloat("ascent");

	fNeedID = data->FindBool("need_id");
	if (!fNeedID)
		fID = data->FindInt32("id");
	
	// tell me where the mouse always is!
//	SetEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY);

	global_pointer = this;
}

#pragma mark -

// archive the replitxt replicant
status_t
replitxtReplicant::Archive(BMessage *data, bool deep) const
{
	xReplicant::Archive(data, deep);

	data->AddString("add_on", app_signature);
	data->AddString("class", replicant_name);
	data->AddString("text", fString);
	data->AddFloat("ascent", font_ascent);
	data->AddBool("need_id", fNeedID);
	
	if (!fNeedID)
		data->AddInt32("id", fID);

	return B_OK;
}

// method to instantiate the view from a message
BArchivable*
replitxtReplicant::Instantiate(BMessage* inArchive)
{
	if (!validate_instantiation(inArchive, replicant_name)) return NULL;

	return new replitxtReplicant(inArchive);
}

#pragma mark -

void
replitxtReplicant::AttachedToWindow()
{
	xReplicant::AttachedToWindow();
	if (Replicated()) {
		Parent()->Looper()->Lock();
		fMessageFilter = new BMessageFilter('vivi', filter);
		Parent()->Looper()->AddFilter(fMessageFilter);
//		printf("there are %ld filters\n", Parent()->Looper()->FilterList()->CountItems());
		Parent()->Looper()->Unlock();
		
		fReplicantNum = Parent()->CountChildren()-1;
	} 
}

void
replitxtReplicant::DetachedFromWindow()
{
	if (Replicated()) {
		xReplicant::DetachedFromWindow();
		
		Parent()->Looper()->Lock();
		Parent()->Looper()->RemoveFilter(fMessageFilter);
		Parent()->Looper()->Unlock();
	} 
}

// *******************************
// * Draw()
// *******************************
void
replitxtReplicant::Draw(BRect updateRect)
{
	xReplicant::Draw(updateRect);
	// write some text

	SetDrawingMode(B_OP_ALPHA);
	
	SetHighColor(0,0,0,40);
	FillRect(Bounds());
	SetHighColor(0,0,80,100);
	StrokeRect(Bounds());
//	SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);	
	MovePenTo(6,2+font_ascent);
	DrawString(fString);
}

// *******************************
// * SetText()
// *******************************
void
replitxtReplicant::SetText(const char* text)
{
	float width;
	font_height FontAttributes;
	
	fString = strdup(text);
	
	// find out the width in pixels
	width = be_plain_font->StringWidth(fString);
	
	// and the height of the font
	be_plain_font->GetHeight(&FontAttributes);
	font_ascent = ceil(FontAttributes.ascent);
	float font_height = font_ascent + ceil(FontAttributes.descent);
	
	// now we should resize ourselves!
	xResizeTo(5+width+5, 2+font_height+2);
	
	Invalidate();
	
//	printf("resizing to %.2f\n", width+20);
}

const char*
replitxtReplicant::GetText() const
{
	return (strdup(fString));
}

filter_result
filter(BMessage *message, BHandler **target, BMessageFilter *messageFilter)
{
	filter_result result = B_DISPATCH_MESSAGE;
	replitxtReplicant *replicant;
	
	if (message->what == 'vivi') {
		int32 id;
		int32 index;
		bool set_text;
		
		index = message->FindInt32("index");
		id = message->FindInt32("id");
		set_text = message->FindBool("set_text");
		printf("v");
	
		replicant = ((replitxtReplicant*)((BWindow*)messageFilter->Looper())->ChildAt(0)->ChildAt(index));
		
		printf("%s\n", replicant->Name());
		printf("[%ld:%ld, %c, %ld:%ld (%s)]\n", index, replicant->Index(), (replicant->NeedID()?'t':'f'), id, replicant->ID(), global_pointer->GetText());
		
		if (replicant->NeedID())
			if (index == replicant->Index()) {
				replicant->SetID(id);
				result = B_SKIP_MESSAGE;
			}
				
		// is the message for us?
		if (replicant->ID() == id)
			// should we return the string?
			if (!set_text) {
				BMessage *msg = new BMessage('vivi');
				msg->AddString("text", replicant->GetText());
			
				message->SendReply(msg);
			} else { // we set the string
				replicant->SetText(message->FindString("text"));
				printf("should set text to %s\n", message->FindString("text"));
			}
			result = B_SKIP_MESSAGE;
	} else
		printf(".");

	return result;
}

// ------------------------------------------------------------- xReplicant.cpp --