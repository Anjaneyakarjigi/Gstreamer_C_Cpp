#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>

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



int main(int argc, char *argv[])
{

	if(argc > 3 || argc < 3)
	{
		printf("Error Arguments, please input all arguments");
		return 1;
	}
	
	char *IpAddress = argv[1];
	int port = 0;
	if (argv[2] > 0)
	port = atoi(argv[2]);

	/* Initialize GStreamer */
	gst_init(&argc, &argv);

	GstElement *pipeline, *videosource, *videoconvert, *videoencoder, *videoqueue, *mux, *sink;
	GstBus *bus;
	GMainLoop *loop;
	GstMessage *msg;
	GstStateChangeReturn ret;

	loop = g_main_loop_new(NULL, FALSE);

	/* Create the empty pipeline */
	pipeline = gst_pipeline_new("webcam2file");
	
	/* Create the elements */
	videosource = gst_element_factory_make("ksvideosrc", "videosource");
	videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
	videoencoder = gst_element_factory_make("x264enc", "videoencoder");
	videoqueue = gst_element_factory_make("queue", "videoqueue");
	mux = gst_element_factory_make("mpegtsmux", "mux");

		if (!videosource || !videoconvert || !videoencoder || !videoqueue) {
			g_printerr("Not all video elements could be created.\n");
			return -1;
		}
		
	//mux = gst_element_factory_make("mp4mux", "muxer"); //mpegtsmux
	sink = gst_element_factory_make("udpsink", "sink");
	g_object_set(G_OBJECT(sink), "port", port, "host", IpAddress, "sync", TRUE, "async", FALSE, "qos", TRUE, "qos-dscp", 46, NULL);

	
	/* Modify the source's properties */
	g_object_set(videosource, "device-index", 0, NULL);	
	g_object_set(G_OBJECT(videoencoder), "bitrate", 200000, "ref", 4, "pass", 4, "key-int-max", 0, "byte-stream", TRUE, "tune", 0x00000004/*zero-latency*/, "noise-reduction", 1000, NULL);
	g_object_set(videoqueue, "max-size-buffers", 0, "max-size-time", 0, NULL);

	/* Build the pipeline */
	gst_bin_add_many(GST_BIN(pipeline), videosource, videoconvert, videoencoder, videoqueue, mux , sink, NULL);
	if (gst_element_link_many(videosource, videoconvert, videoencoder, videoqueue,mux , sink, NULL) != TRUE) {
		g_printerr("Elements could not be linked.\n");
		gst_object_unref(pipeline);
		return -1;
	}

	
	/* Start playing */
	ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr("Unable to set the pipeline to the playing state.\n");
		gst_object_unref(pipeline);
		return -1;
	}

	/* add a message handler */
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	guint bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
	gst_object_unref(bus);

	
	g_print("Open Server at > udp://%s:%d\n",IpAddress , port);
	g_print("Running...\n");
	g_main_loop_run(loop);

	g_print("Stopped\n");
	/* Free resources */
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	getchar();
	return 0;
}
