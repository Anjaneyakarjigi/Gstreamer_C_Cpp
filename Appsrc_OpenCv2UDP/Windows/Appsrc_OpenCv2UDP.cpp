#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <opencv2\opencv.hpp>

using namespace cv;

VideoCapture cap(0);
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

static void need_data(GstElement *appsrc,
	guint       unused_size,
	gpointer    user_data)
{
	static gboolean white = FALSE;
	static GstClockTime timestamp = 0;
	GstBuffer *buffer;
	guint size, depth, height, width, step, channels;
	GstFlowReturn ret;
	cv::Mat img;
	guchar *data1;
	GstMapInfo map;

	//img = cv::imread("Default_Img_1920x1080.png", CV_LOAD_IMAGE_COLOR);
	cap.read(img);
	height = img.rows;
	width = img.cols;
	step = img.step;
	//printf("
	channels = (guint)img.channels();
	depth = (guint)img.depth();
	data1 = (guchar *)img.data;
	size = height*width*channels; 

	//size = 720 * 576;
	buffer = gst_buffer_new_allocate(NULL, size, NULL);
	gst_buffer_map(buffer, &map, GST_MAP_WRITE);
	memcpy((guchar *)map.data, data1, gst_buffer_get_size(buffer));
	/* this makes the image black/white */
	//gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);

	white = !white;

	GST_BUFFER_PTS(buffer) = timestamp;
	GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, 25);

	timestamp += GST_BUFFER_DURATION(buffer);

	g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);

	if (ret != GST_FLOW_OK) {
		/* something wrong, stop pushing */
		//g_main_loop_quit(loop);
		g_signal_emit_by_name(appsrc, "end-of-stream", buffer, &ret);
	}
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
	//port = 8554;
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
	videosource = gst_element_factory_make("appsrc", "videosource");
	videoconvert = gst_element_factory_make("videoconvert", "videoconvert");
	videoencoder = gst_element_factory_make("x264enc", "videoencoder");
	videoqueue = gst_element_factory_make("queue", "videoqueue");
	mux = gst_element_factory_make("mpegtsmux", "mux");

		if (!videosource || !videoconvert || !videoencoder || !videoqueue) {
			g_printerr("Not all video elements could be created.\n");
			return -1;
		}
		
	/* setup */
	g_object_set(G_OBJECT(videosource), "caps",
		gst_caps_new_simple("video/x-raw",
			"format", G_TYPE_STRING, "BGR",
			"width", G_TYPE_INT, 640,
			"height", G_TYPE_INT, 480,
			"framerate", GST_TYPE_FRACTION, 0, 1,
			NULL), NULL);	
	
	g_object_set(G_OBJECT(videosource),
		"stream-type", 0,
		"format", GST_FORMAT_TIME, NULL);
	g_signal_connect(videosource, "need-data", G_CALLBACK(need_data), NULL);
	
	
	//mux = gst_element_factory_make("mp4mux", "muxer"); //mpegtsmux
	sink = gst_element_factory_make("udpsink", "sink");
	g_object_set(G_OBJECT(sink), "port", port, "host", IpAddress, "sync", TRUE, "async", FALSE, "qos", TRUE, "qos-dscp", 46, NULL);

	
	/* Modify the source's properties */
	//g_object_set(videosource, "device-index", 0, NULL);	
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
