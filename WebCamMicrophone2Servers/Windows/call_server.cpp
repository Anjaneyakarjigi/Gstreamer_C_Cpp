#include <iostream>
#include "VideoAudioServer.h"

/* arguments [0]exe name , [1]1->filestream : 2->UDP stream : 3-> TCP stream 4 -> RTP, [3] IPaddress, [4] Port */
int main(int argc, char *argv[])
{

	if (argc > 4 || argc < 4)
	{
		printf("Error Arguments, please input all arguments");
		return 1;
	}

	ServerConfig ss;
	
	if (argv[3] > 0)
		ss.port = atoi(argv[3]);	// port
	strcpy(ss.IpAddress , argv[2]); // Ipaddress
	ss.OutputConfig = atoi(argv[1]); // Output config

	gst_init(&argc, &argv);
	//ss.OutputConfig = 4;
	ServerSetUp *ObjServer = new ServerSetUp(&ss);
	/* Set video confiuration */
	VideoConfig v;
	v.pass = 4;		v.framerate = 25;	v.bitrate = 500000;

	ObjServer->SetUpVideo(&v);

	/* set audio configuration, for future addition */
	AudioConfig	a = { };
	ObjServer->SetUpAudio(&a);

	ObjServer->startServer();
	
	while (1)
	{
		system("cls");
		printf("Select key options :\nE : stop\nP : pause\nS : start\n");
		char c = getchar();
		if (c == 'E')
		{
			ObjServer->stopServer();
			break;
		}
		else if (c == 'P')
		{
			ObjServer->pauseServer();
		}
		else if (c == 'S')
		{
			ObjServer->playServer();
		}
		else
		{
			printf("Enter Proper Input\n");
		}
	}

	getchar();
	
	delete ObjServer;

	return 0;
}