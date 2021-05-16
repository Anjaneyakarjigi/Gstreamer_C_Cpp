
#ifndef VIDEOAUDIOSERVER_H
#define VIDEOAUDIOSERVER_H
#include <gst/gst.h>
#include <thread>

//#define FILE_STORE	3	// file = 1, udp = 2, rtp == 3
#define PORT		8554
#define SERVER_ADDR	"127.0.0.1"

struct PipelineStruct
{
	GstElement	*pipeline, *audiosource, *audioconvert, *audioencoder, *audioresample, *audioqueue;
	GstElement  *videosource, *videoconvert, *videoencoder, *videoqueue;
	GstElement	*mux, *payload , *sink;
};

struct ServerConfig
{
	int port = 8554;
	char IpAddress[30] = "127.0.0.1";
	short OutputConfig = 2;
};
struct VideoConfig
{
	short framerate = 25;	// framerate 

	short pass = 0; // set pass value refer below
	/*	cbr (0) – Constant Bitrate Encoding
		quant (4) – Constant Quantizer
		qual (5) – Constant Quality
		pass1 (17) – VBR Encoding - Pass 1
		pass2 (18) – VBR Encoding - Pass 2 */
	int bitrate = 500000;	// if specified variable bit rate , default set to 500000 
};

struct AudioConfig
{
	
};


//template <class T>
class ServerSetUp
{
	gboolean videoflag = false;
	gboolean audioflag = false;
	GstStateChangeReturn ret;
	GMainLoop *loop;	GstBus *bus;
	guint bus_watch_id;

	PipelineStruct *A;
	ServerConfig *ss;
	short OutputConfigFlag = 0;

public:

	ServerSetUp(ServerConfig *s)
	{
		ss = s;
		OutputConfigFlag = ss->OutputConfig;
		/* Initialize GStreamer */
		A = new PipelineStruct;
		loop = g_main_loop_new(NULL, FALSE);

		/* Create the empty pipeline */
		A->pipeline = gst_pipeline_new("AudioVideoServer");
		A->mux = gst_element_factory_make("mpegtsmux", "mux");
		A->payload = gst_element_factory_make("rtpmp2tpay", "payload");
		if (OutputConfigFlag == 1) {
			A->sink = gst_element_factory_make("filesink", "sink");
			g_object_set(A->sink, "location", ss->IpAddress, NULL);
		}
		else if (OutputConfigFlag == 2) {
			A->sink = gst_element_factory_make("udpsink", "sink"); //glimagesink
			g_object_set(G_OBJECT(A->sink), "port", ss->port, "host", ss->IpAddress, "sync", TRUE, "async", FALSE, "qos", TRUE, "qos-dscp", 46, NULL);
			//g_object_set(G_OBJECT(A->sink), "port", PORT, NULL);
		}
		else if (OutputConfigFlag == 4) {
			A->sink = gst_element_factory_make("udpsink", "sink"); //glimagesink
			g_object_set(G_OBJECT(A->sink), "port", ss->port, "host", ss->IpAddress, "sync", TRUE, "async", FALSE, "qos", TRUE, "qos-dscp", 46, NULL);
		}
		else if (OutputConfigFlag == 3) {
			A->sink = gst_element_factory_make("tcpserversink", "sink"); //glimagesink
			g_object_set(G_OBJECT(A->sink), "port", ss->port, "host", ss->IpAddress, "sync", TRUE, "async", FALSE, "qos", TRUE, "qos-dscp", 46, NULL);
		}

		if (!A->pipeline || !A->mux || !A->sink ) {
			g_printerr("Not all general pipeline elements could be created.\n");
		}

		gst_bin_add_many(GST_BIN(A->pipeline), A->mux, A->payload, A->sink, NULL);

	}

	~ServerSetUp(){
		delete A;

	}

	/* Set Up Video Configuration */
	void SetUpVideo(VideoConfig *V)
	{
		videoflag = true;
		A->videosource = gst_element_factory_make("ksvideosrc", "videosource");
		A->videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
		A->videoencoder = gst_element_factory_make("x264enc", "videoencoder");
		A->videoqueue = gst_element_factory_make("queue", "videoqueue");

		if (!A->videosource || !A->videoconvert || !A->videoencoder || !A->videoqueue) {
			g_printerr("Not all video elements could be created.\n");
			return;
		}

		gst_bin_add_many(GST_BIN(A->pipeline), A->videosource, A->videoconvert, A->videoencoder, A->videoqueue, NULL);

		if (gst_element_link_many(A->videosource, A->videoconvert, A->videoencoder, A->videoqueue, NULL) != TRUE) {
			g_printerr("Video elements could not be linked.\n");
			gst_object_unref(A->pipeline);
			return;
		}

		/* Set Caps filter for Encoder */
/*		GstCaps *vidEncCaps = gst_caps_new_simple("video/x-h264",
			"stream-format", G_TYPE_STRING, "byte-stream",
			"profile", G_TYPE_STRING, "main",
			NULL);

		g_object_set(G_OBJECT(v_encCapsfilter), "caps", vidEncCaps, NULL);
		gst_caps_unref(vidEncCaps); */

		g_object_set(G_OBJECT(A->videoencoder), "bitrate", V->bitrate, "ref", 4, "pass", V->pass, "key-int-max", 0, "byte-stream", TRUE, "tune", 0x00000004/*zero-latency*/, "noise-reduction", 1000, NULL);
		g_object_set(A->videoqueue, "max-size-buffers", 0, "max-size-time", 0, NULL);

	}

	/* Set Up Audio Configuration */
	void SetUpAudio(AudioConfig *Aa)
	{
		audioflag = true;
		A->audiosource = gst_element_factory_make("autoaudiosrc", "audiosource");
		A->audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
		A->audioresample = gst_element_factory_make("audioresample", "audioresample");
		A->audioencoder = gst_element_factory_make("avenc_aac", "aac encoder");
		A->audioqueue = gst_element_factory_make("queue", "a_queue"); //wavenc

		if (!A->audiosource || !A->audioconvert || !A->audioresample || !A->audioencoder || !A->audioqueue) {
			g_printerr("Not all audio elements could be created.\n");
			return;
		}

		gst_bin_add_many(GST_BIN(A->pipeline), A->audiosource, A->audioconvert, A->audioresample, A->audioencoder,A->audioqueue, NULL);
		
		if (gst_element_link_many(A->audiosource, A->audioconvert, A->audioresample, A->audioencoder, A->audioqueue, NULL) != TRUE) {
			g_printerr("Audio elements could not be linked.\n");
			gst_object_unref(A->pipeline);
			return;
		}

		//g_object_set(G_OBJECT(audioencoder), "target", 1/* 0 -> quality 1->bitrate */, "cbr", TRUE, "bitrate", 320, NULL); // getting good bitrate
		g_object_set(G_OBJECT(A->audioencoder), "target", 0/* 0 -> quality 1->bitrate */, "quality", 0.0,"encoding-engine-quality", 2,  NULL);// getting good quality
		g_object_set(A->audioqueue, "max-size-buffers", 0, "max-size-time", 0, NULL);

	}

	void startServer()
	{
		if (videoflag = true){
		LinkElementsManually(A->videoqueue, A->mux);
		/*	if (gst_element_link_many(A->videoqueue, mux , NULL) != TRUE) {
		g_printerr("Elements could not be linked.\n");
		gst_object_unref(A->pipeline);
		return -1;
		} */
		}

		if (audioflag == true){
			LinkElementsManually(A->audioqueue, A->mux);
			/*	if (gst_element_link_many(aqueue, mux, NULL) != TRUE) {
			g_printerr("Elements could not be linked.\n");
			gst_object_unref(A->pipeline);
			return;
			}	*/
		}

		if (OutputConfigFlag < 4) {
			/* link muxer to sink element */
			if (gst_element_link_many(A->mux, A->sink, NULL) != TRUE)
			{
				g_printerr("Elements could not be linked.\n");
				gst_object_unref(A->pipeline);
				return;
			}
		}
		else if (OutputConfigFlag == 4)
		{
			if (gst_element_link_many(A->mux, A->payload, A->sink, NULL) != TRUE)
			{
				g_printerr("Elements could not be linked.\n");
				gst_object_unref(A->pipeline);
				return;
			}
		}
		
		/* we add a message handler */
		bus = gst_pipeline_get_bus(GST_PIPELINE(A->pipeline));
		bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
		gst_object_unref(bus);

		playServer();

		/* run server in saparate threads */
		std::thread thd(&ServerSetUp::runServerInFun, this);
		thd.detach();

		//g_print("Running...\n");
		//g_main_loop_run(loop);
		 
	}

	/* Thread to run the Server */
	void runServerInFun()
	{
		g_print("Running...\nServer @%s:%d",ss->IpAddress, ss->port);
		g_main_loop_run(loop);
	}

	void playServer()
	{
		if (gst_element_set_state(A->pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE)
		{
			g_printerr("Unable to set the pipeline to the playing state.\n");
			gst_object_unref(A->pipeline);
			getchar();
			return;
		}
	}

	void pauseServer()
	{
		if (gst_element_set_state(A->pipeline, GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE)
		{
			g_printerr("Unable to set the pipeline to the playing state.\n");
			gst_object_unref(A->pipeline);
			getchar();
			return;
		}
	}

	void stopServer()
	{
		gst_object_unref(bus);
		g_print("Returned, stopping playback...\n");
		gst_element_set_state(A->pipeline, GST_STATE_NULL);
		g_print("Freeing pipeline...\n");
		gst_object_unref(GST_OBJECT(A->pipeline));
		g_print("DONE with stopping pipeline...\n");
	}

private :
	static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data)
	{
		GMainLoop *loop = (GMainLoop *)data;

		switch (GST_MESSAGE_TYPE(msg)) {
			gchar  *debug;
			GError *error;

		case GST_MESSAGE_EOS:
			g_print("End of stream\n");
			g_main_loop_quit(loop);
			break;

		case GST_MESSAGE_ERROR:

			gst_message_parse_error(msg, &error, &debug);
			g_free(debug);

			g_printerr("Error: %s\n", error->message);
			g_printerr("Debug Information: %s\n", debug);
			g_error_free(error);

			g_main_loop_quit(loop);
			break;
		default:
			break;
		}

		return TRUE;
	}

	gboolean LinkElementsManually(GstElement *stream, GstElement *muxer)
	{
		gchar *req_pad_name;
		GstPad *req_pad;
		GstPad *static_pad;

		/* Get source pad from queue pipeline */
		static_pad = gst_element_get_static_pad(stream, "src");
		/* Get sink pad from muxer */
		req_pad = gst_element_request_pad(muxer, gst_element_class_get_pad_template(GST_ELEMENT_GET_CLASS(muxer), "sink_%d"), NULL, NULL);

		req_pad_name = gst_pad_get_name(req_pad);

		g_print("stream Pad name for Muxer %s\n", req_pad_name);
		g_free(req_pad_name);
		/* Link Both src-> sink pads */
		if (GST_IS_PAD(static_pad) && GST_IS_PAD(req_pad))
		{
			int ret = gst_pad_link(static_pad, req_pad);
			if (ret == GST_PAD_LINK_OK)
				return 1; //success
			else
				g_print("Error %s\n", ret);
			//return 0; // failure
		}
		else
			return 0; // failure
	}



};

#endif // !VIDEOAUDIOSERVER_H

