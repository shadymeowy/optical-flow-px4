#include <cwrapper.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 64
#define HEIGHT 64

void print_optical_flow_msg(optical_flow_msg_t *msg)
{
	printf("optical_flow_msg_t\n");
	printf("    time_usec: %ld\n", msg->time_usec);
	printf("    sensor_id: %d\n", msg->sensor_id);
	printf("    integration_time_us: %d\n", msg->integration_time_us);
	printf("    integrated_x: %f\n", msg->integrated_x);
	printf("    integrated_y: %f\n", msg->integrated_y);
	printf("    integrated_xgyro: %f\n", msg->integrated_xgyro);
	printf("    integrated_ygyro: %f\n", msg->integrated_ygyro);
	printf("    integrated_zgyro: %f\n", msg->integrated_zgyro);
	printf("    temperature: %f\n", msg->temperature);
	printf("    quality: %d\n", msg->quality);
	printf("    time_delta_distance_us: %d\n", msg->time_delta_distance_us);
	printf("    distance: %f\n", msg->distance);
}

int main()
{
	uint8_t *image = (uint8_t *)malloc(WIDTH * WIDTH * sizeof(uint8_t));
	optical_flow_msg_t msg;
	FILE *fp;

	// create optical flow
	optical_flow_t *flow = optical_flow_new(254.665283, 254.665283, 20, 64,
						64, 20, 1.645000);
	
	// print optical flow parameters
	printf("optical_flow_t\n");
	printf("    flow->output_rate: %d\n",
	       optical_flow_get_output_rate(flow));
	printf("    flow->width: %d\n", optical_flow_get_img_width(flow));
	printf("    flow->height: %d\n", optical_flow_get_img_height(flow));
	printf("    flow->num_features: %d\n",
	       optical_flow_get_num_features(flow));
	printf("    flow->conf_multiplier: %f\n",
	       optical_flow_get_conf_multiplier(flow));

	// import image as binary file
	fp = fopen("test_img/img0.bin", "rb");
	fread(image, sizeof(uint8_t), WIDTH * WIDTH, fp);
	fclose(fp);
	// feed optical flow
	optical_flow_feed(flow, image, 0 * 1.0 / 20, &msg);
	// print optical flow message
	print_optical_flow_msg(&msg);

	// import image as binary file
	fp = fopen("test_img/img1.bin", "rb");
	fread(image, sizeof(uint8_t), WIDTH * WIDTH, fp);
	fclose(fp);
	// feed optical flow
	optical_flow_feed(flow, image, 1 * 1.0 / 20, &msg);
	// print optical flow message
	print_optical_flow_msg(&msg);

	// destroy optical flow
	optical_flow_destroy(flow);
	return 0;
}