#ifndef CWRAPPER_H
#define CWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define WIDTH 64
#define DEFAULT_RATE 20

typedef struct _optical_flow_msg_t {
	int64_t time_usec;
	int32_t sensor_id;
	int32_t integration_time_us;
	float integrated_x;
	float integrated_y;
	float integrated_xgyro;
	float integrated_ygyro;
	float integrated_zgyro;
	float temperature;
	int32_t quality;
	int32_t time_delta_distance_us;
	float distance;
} optical_flow_msg_t;

struct optical_flow_t;
typedef struct optical_flow_t optical_flow_t;

optical_flow_t *optical_flow_new(double hfov, int rate, double first_frame_time_);

int optical_flow_feed(optical_flow_t *flow, uint8_t *image, double frame_time,
		      optical_flow_msg_t *msg);

int optical_flow_gyro(optical_flow_t *flow, double now,
		      double gyro_angular_vel[3]);

int optical_flow_destroy(optical_flow_t *flow);

void optical_flow_set_num_features(optical_flow_t *flow, int num_features);
int optical_flow_get_num_features(optical_flow_t *flow);
void optical_flow_set_conf_multiplier(optical_flow_t *flow,
				      float conf_multiplier);
float optical_flow_get_conf_multiplier(optical_flow_t *flow);
void optical_flow_set_cam_matrix(optical_flow_t *flow, float focal_len_x,
				 float focal_len_y, float principal_point_x,
				 float principal_point_y);
void optical_flow_set_distortion(optical_flow_t *flow, float k1, float k2,
				 float k3, float p1, float p2);
void optical_flow_set_img_width(optical_flow_t *flow, int width);
int optical_flow_get_img_width(optical_flow_t *flow);
void optical_flow_set_img_height(optical_flow_t *flow, int height);
int optical_flow_get_img_height(optical_flow_t *flow);
void optical_flow_set_focal_length_x(optical_flow_t *flow, float focal_len_x);
float optical_flow_get_focal_length_x(optical_flow_t *flow);
void optical_flow_set_focal_length_y(optical_flow_t *flow, float focal_len_y);
float optical_flow_get_focal_length_y(optical_flow_t *flow);
void optical_flow_set_output_rate(optical_flow_t *flow, int output_rate);
int optical_flow_get_output_rate(optical_flow_t *flow);


#ifdef __cplusplus
}
#endif

#endif /* CWRAPPER_H */