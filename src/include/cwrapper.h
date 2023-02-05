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

#ifdef __cplusplus
}
#endif

#endif /* CWRAPPER_H */