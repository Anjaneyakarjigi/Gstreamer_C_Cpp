#include <gst/gst.h>
#include <stdio.h>

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

	/* Initialize GStreamer */
	gst_init(&argc, &argv);

	GstElement *pipeline, *source, *audioconvert, *audioencoder, *audioresample, *queue, *mux, *sink;
	GstBus *bus;
	GMainLoop *loop;
	GstMessage *msg;
	GstStateChangeReturn ret;

	loop = g_main_loop_new(NULL, FALSE);

	/* Create the elements */
	source = gst_element_factory_make("autoaudiosrc", "source");
	audioconvert = gst_element_factory_make("audioconvert", "audioconvert");
	audioresample = gst_element_factory_make("audioresample", "audioresample");
	audioencoder = gst_element_factory_make("lamemp3enc", "mp3encoder"); //wavenc avenc_aac
	queue = gst_element_factory_make("queue", "a_queue"); //wavenc

	//mux = gst_element_factory_make("mp4mux", "muxer"); //mpegtsmux
	sink = gst_element_factory_make("filesink", "sink");

	/* Create the empty pipeline */
	pipeline = gst_pipeline_new("test-pipeline");

	if (!pipeline || !source || !audioconvert || !audioresample || !audioencoder || !queue || !sink) {
		g_printerr("Not all elements could be created.\n");
		return -1;
	}
	  
	g_object_set(G_OBJECT(audioencoder), "target", 1/* 0 -> quality 1->bitrate */, "cbr", TRUE, "bitrate", 320, NULL); // getting good bitrate
	//g_object_set(G_OBJECT(audioencoder), "target", 0/* 0 -> quality 1->bitrate */, "quality", 0.0,"encoding-engine-quality", 2,  NULL);// getting good quality
	//target=bitrate cbr=true bitrate=48
	/* Build the pipeline */
	gst_bin_add_many(GST_BIN(pipeline), source, audioconvert, audioresample, audioencoder, queue, sink, NULL);
	if (gst_element_link_many(source, audioconvert, audioencoder, queue, sink, NULL) != TRUE) {
		g_printerr("Elements could not be linked.\n");
		gst_object_unref(pipeline);
		return -1;
	}

	/* Modify the source's properties */
	g_object_set(sink, "location", "mikeRecorded.mp3", NULL);
	
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

	g_print("Running...\n");
	g_main_loop_run(loop);

	g_print("Stopped\n");
	/* Free resources */
	gst_object_unref(bus);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	return 0;
}
