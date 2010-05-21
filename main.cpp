// main.cpp - rxc
// 23rd April 2001
//
// the start of (hopefully) a replicant series in the vein of neko
// but with much more!

#ifndef __replitxtAPP_H__
#include "replitxtApp.h"
#endif

void Usage();
void ListReplicants();
void TextReplicant(int32 replicant_id, const char* string);


int
main(int argc, char **argv)
{
	int list=false, text=false;
	uint32 replicant_id = 0;
	// does the user want anything?
	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"list", 0, 0, 'l'},
			{"text", 1, 0, 't'},
			{"help", 0, 0, 'h'},
			{0,0,0,0}
		};
		int c = getopt_long (argc, argv, "lh:", long_options, &option_index);
		if (c == -1) break;
		
		switch (c) {
			case 'l':
				list = true;
				break;
			case 't':
				text = true;
				if (argv[optind] == NULL) {
					printf("Missing argument for --text option\n");
					Usage();
				}
				
				replicant_id = strtoul(optarg, NULL, 0);
				break;
			case 'h':
			case '?':
				Usage();
				break;
		}
	}

	if (list) {
		ListReplicants();
	} else if (text) {
		TextReplicant(replicant_id, argv[optind]);
	} else {	
		replitxtApp *replitxt = new replitxtApp();
	
		replitxt->Run();
		delete replitxt;
		return 0;
	}
}

void
Usage() {
	printf("Usage: replitxt [--list] [--text replicant_id \"lots_of_text\"]\n");/* [-w width] [--width=width]\n"
	       "\t[--deskbar] [--normal] [--framecolor 0xrrggbb]\n"
	       "\t[--activecolor 0xrrggbb] [--idlecolor 0xrrggbb]\n");*/
	exit(0);
}

BMessenger
MessengerForShelf()
{
	BMessenger	tracker_messenger;
	BMessenger	messenger;
	BMessage	request(B_GET_PROPERTY);	
	BMessage	reply;
	
	request.AddSpecifier("Messenger");
	request.AddSpecifier("Shelf");
	request.AddSpecifier("View", "PoseView");
	request.AddSpecifier("Window", "/boot/home/Desktop");

	// create a messenger for the tracker
	tracker_messenger = BMessenger("application/x-vnd.Be-TRAK", -1);
	
	// send the request to the tracker
	tracker_messenger.SendMessage(&request, &reply);
	reply.FindMessenger("result", &messenger);

	return messenger;
}

void
ListReplicants() {
	BMessenger	shelf_messenger;
	BMessage	request(B_GET_PROPERTY);
	BMessage	reply;
	status_t	status = B_OK;
	uint32		index=0;
	uint32		replitxt_index=0;

	// get a messenger for the shelf
	shelf_messenger = MessengerForShelf();

	while (status == B_OK) {
		int32 uid;
		const char* name;
		const char* string;
			
		reply.MakeEmpty();
		BMessage id_request(B_GET_PROPERTY);		// We're getting the ID property

		id_request.AddSpecifier("ID");					// want the ID
		id_request.AddSpecifier("Replicant", index);	// of the index'th replicant
	
		status = shelf_messenger.SendMessage(&id_request, &reply);
		if (status == B_OK) {
			status = reply.FindInt32("result", &uid);
		} else
			printf("shelf_messenger not found (%s)\n", strerror(status));

		BMessage name_request(B_GET_PROPERTY);
		name_request.AddSpecifier("Name");		// ask for the Name of the replicant
	
		BMessage uid_specifier(B_ID_SPECIFIER);	// specifying via ID
		uid_specifier.AddInt32("id", uid);
		uid_specifier.AddString("property", "Replicant");
		name_request.AddSpecifier(&uid_specifier);
			
		shelf_messenger.SendMessage(&name_request, &reply);

		if (reply.FindString("result", &name) == B_OK) {
			if (strcmp(name, "replitxt") == 0) {
//					printf("%ld: %s\n", uid, name);

				BMessage *string_request = new BMessage('vivi');
				string_request->AddInt32("id", uid);
				string_request->AddInt32("index", index);
				string_request->AddBool("get_text", true);
				shelf_messenger.SendMessage(string_request, &reply);
			
				if (reply.FindString("text", &string) == B_OK) {
					printf("%ld: %s\n", uid, string);
				}
					
				replitxt_index++;
			}
		}

		index++;
	}
	
	printf("There are %ld replitxt replicants out of %ld replicants.\n", replitxt_index, index-1);

	exit(0);
}

void
TextReplicant(int32 replicant_id, const char* string)
{
	printf("should send \"%s\" to replicant [%ld]\n", string, replicant_id);

	BMessenger	shelf_messenger;
	BMessage	reply;
	BMessage	request(B_GET_PROPERTY);
	status_t	status = B_OK;
	int32		index=0;
	bool		message_sent = false;
		
	// get a messenger for the shelf
	shelf_messenger = MessengerForShelf();

	while (status == B_OK) {
		int32 uid;
		const char* name;
			
		reply.MakeEmpty();
		BMessage id_request(B_GET_PROPERTY);		// We're getting the ID property

		id_request.AddSpecifier("ID");					// want the ID
		id_request.AddSpecifier("Replicant", index);	// of the index'th replicant
	
		status = shelf_messenger.SendMessage(&id_request, &reply);
		if (status == B_OK) {
			status = reply.FindInt32("result", &uid);
		} else
			printf("shelf_messenger not found (%s)\n", strerror(status));

//		printf("uid: %ld\n", uid);
		if (uid == replicant_id) {
			//printf("should send \"%s\" to replicant [%ld]\n", string, index);

			BMessage *string_request = new BMessage('vivi');
			string_request->AddInt32("id", replicant_id);
			string_request->AddInt32("index", index);
			string_request->AddBool("set_text", true);
			string_request->AddString("text", strdup(string));
			shelf_messenger.SendMessage(string_request);
			
			message_sent = true;
			break;
			//exit(0);
		}
		
		index++;
	}
	
	if (!message_sent)
		printf("Replicant [%ld] not found. Try replitxt --list\n", replicant_id);
	exit(0);
}
