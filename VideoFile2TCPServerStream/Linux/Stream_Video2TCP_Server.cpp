#include <gst/gst.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#define PORT	8554
#define SERVER_ADDR	"127.0.0.1"
#define INPUT_FILE "test.mp4"


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


static void on_pad_added(GstElement *element, GstPad *pad, gpointer data)
{
	gchar *name;
	GstElement *parse = (GstElement *)data;

	name = gst_pad_get_name(pad);
	g_print("A new pad %s was created for %s\n", name, gst_element_get_name(element));
	g_free(name);

	g_print("element %s will be linked to %s\n",
		gst_element_get_name(element),
		gst_element_get_name(parse));

	gst_element_link(element, parse);
}

static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer *data) {
	GstPadLinkReturn ret;
	GstCaps *new_pad_caps = NULL;
	GstStructure *new_pad_struct = NULL;
	const gchar *new_pad_type = NULL;
	GstElement *h264parse = (GstElement *)data;

	/* Check the new pad's type */
	new_pad_caps = gst_pad_get_current_caps(new_pad);
	new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
	new_pad_type = gst_structure_get_name(new_pad_struct);

	if (g_str_has_prefix(new_pad_type, "video/x-h264"))
	{
		GstPad *sink_pad_video = gst_element_get_static_pad(h264parse, "sink");
		ret = gst_pad_link(new_pad, sink_pad_video);
	}
}

/* input format :  <exe_name> <IPaddress> <port> */
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

	GMainLoop *loop;
	GstElement  *pipeline, *source, *demux, *parser, *decoder, *encoder, *mux, *v_queue;
	GstBus *bus;
	guint bus_watch_id;
	GstElement *tcpsink, *pay;

	const gchar *input_file = INPUT_FILE;

	/* Initialization */
	gst_init(&argc, &argv);
	loop = g_main_loop_new(NULL, FALSE);

	/* Create gstreamer elements */
	pipeline = gst_pipeline_new("video-play");
	source = gst_element_factory_make("filesrc", "file-source");
	demux = gst_element_factory_make("qtdemux", "demux");
	parser = gst_element_factory_make("h264parse", "h264-parser");
	decoder = gst_element_factory_make("avdec_h264", "decoder");
	encoder = gst_element_factory_make("x264enc", "encoder");
	mux = gst_element_factory_make("mpegtsmux", "muxer");
	v_queue = gst_element_factory_make("queue", "video-queue");

	//	udpsink = gst_element_factory_make("udpsink", "udpsink");
	tcpsink = gst_element_factory_make("tcpserversink", "tcp");
	pay = gst_element_factory_make("rtpmp2tpay", "pay0");

	if (!pipeline || !source || !demux || !parser || !decoder || !encoder || !mux ) {
		g_printerr("One element could not be created. Exiting.\n");
		return -1;
	}

	/* Set input video file for source element */
	g_object_set(G_OBJECT(source), "location", INPUT_FILE);
	g_object_set(G_OBJECT(tcpsink), "port", port, "host", IpAddress, "sync", TRUE, "async", FALSE, "qos", TRUE, "qos-dscp", 46, NULL);

	/* we add a message handler */
	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
	gst_object_unref(bus);

	/* Add all elements into the pipeline */
	/* pipeline---[ filesrc + qtdemux + h264parse + avdec_h264 + d3dvideosink ] */
	gst_bin_add_many(GST_BIN(pipeline), source, demux, parser, decoder, encoder, v_queue, mux, tcpsink, NULL);

	/* Link the elements filesrc->demux together */

	if (gst_element_link_many(source, demux, NULL) != TRUE) {
		g_printerr("Element source->demux could not be linked.\n");
		gst_object_unref(pipeline);
		return -1;
	}
	/* h264parse -> avdec_h264 -> d3dvideosink */

	if (gst_element_link_many(parser, decoder, encoder, v_queue , NULL) != TRUE) {
		g_printerr("Many Elements could not be linked.\n");
		gst_object_unref(pipeline);
		return -1;
	}

	if (gst_element_link(v_queue, mux) != TRUE) {

		gst_object_unref(pipeline);
		return -1;
	}

	if (gst_element_link_many(mux, tcpsink, NULL) != TRUE) {
		g_printerr("Many Elements could not be linked.\n");
		gst_object_unref(pipeline);
		return -1;
	}

	g_signal_connect(demux, "pad-added", G_CALLBACK(pad_added_handler), parser);

	/* Set the pipeline to "playing" state */
	g_print("Now playing : %s\n", input_file);
	if (gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr("Unable to set the pipeline to the playing state.\n");
		gst_object_unref(pipeline);
		getchar();
		return -1;
	}

	g_print("Open Server at : udp://@%s:%d\nRunning...\n",IpAddress,port);
	g_main_loop_run(loop);


	/* Free resources and change state to NULL */
	gst_object_unref(bus);
	g_print("Returned, stopping playback...\n");
	gst_element_set_state(pipeline, GST_STATE_NULL);
	g_print("Freeing pipeline...\n");
	gst_object_unref(GST_OBJECT(pipeline));
	g_print("Completed. Goodbye!\n");
	getchar();
	return 0;
}