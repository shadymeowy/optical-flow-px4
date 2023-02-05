#include <cwrapper.h>

#include <stdio.h>
#include <stdlib.h>

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
	optical_flow_t *flow = optical_flow_new(1.3962634, 20, 0.0);

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