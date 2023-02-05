#include <cwrapper.hpp>

#include <math.h>

optical_flow_t *optical_flow_new(double hfov, int rate,
				 double first_frame_time_)
{
	auto flow = new optical_flow_t();
	// initialize optical flow
	double hfov_ = hfov; // what is this?
	double focal_length_ = (WIDTH / 2) / tan(hfov_ / 2);
	int output_rate_ = rate;
	auto optical_flow = new OpticalFlowOpenCV(focal_length_, focal_length_,
						  output_rate_);

	flow->internal = optical_flow;
	flow->optical_flow_rate[0] = 0.0;
	flow->optical_flow_rate[1] = 0.0;
	flow->optical_flow_rate[2] = 0.0;
	flow->first_frame_time_ = first_frame_time_;
	return flow;
}

int optical_flow_feed(optical_flow_t *flow, uint8_t *image, double frame_time,
		      optical_flow_msg_t *msg)
{
	double frame_time_us_ = (frame_time - flow->first_frame_time_) * 1e6;
	int dt_us_ = 0;
	float flow_x_ang = 0.0f;
	float flow_y_ang = 0.0f;
	int quality = flow->internal->calcFlow((u_char *)image, frame_time_us_,
					       dt_us_, flow_x_ang, flow_y_ang);

	// initialize optical flow packet
	msg->time_usec = 0.0 * 1e6; // now
	msg->sensor_id = 2.0;
	msg->integration_time_us = quality ? dt_us_ : 0;
	msg->integrated_x = flow_x_ang;
	msg->integrated_y = flow_y_ang;
	msg->integrated_xgyro = quality ? flow->optical_flow_rate[0] : 0.0f;
	msg->integrated_ygyro = quality ? flow->optical_flow_rate[1] : 0.0f;
	msg->integrated_zgyro = quality ? flow->optical_flow_rate[2] : 0.0f;
	msg->temperature = 20.0;
	msg->quality = quality;
	msg->time_delta_distance_us = 0;
	msg->distance = 0.0f; // set distance

	return quality;
}

int optical_flow_gyro(optical_flow_t *flow, double now,
		      double gyro_angular_vel[3])
{
	double now_us = now * 1e6;
	flow->last_dt_us = now_us;
	uint32_t dt_us = now_us - flow->last_dt_us;

	if (dt_us > 1000) {
		flow->optical_flow_rate[0] +=
			gyro_angular_vel[0] * (dt_us / 1000000.0f);
		flow->optical_flow_rate[1] +=
			gyro_angular_vel[1] * (dt_us / 1000000.0f);
		flow->optical_flow_rate[2] +=
			gyro_angular_vel[2] * (dt_us / 1000000.0f);
		flow->last_dt_us = now_us;
		return 1;
	}
	return 0;
}

int optical_flow_destroy(optical_flow_t *flow)
{
	delete flow->internal;
	delete flow;
	return 0;
}

void optical_flow_set_num_features(optical_flow_t *flow, int num_features)
{
	flow->internal->setNumFeatures(num_features);
}

int optical_flow_get_num_features(optical_flow_t *flow)
{
	return flow->internal->getNumFeatures();
}

void optical_flow_set_conf_multiplier(optical_flow_t *flow,
				      float conf_multiplier)
{
	flow->internal->setConfMultiplier(conf_multiplier);
}

float optical_flow_get_conf_multiplier(optical_flow_t *flow)
{
	return flow->internal->getConfMultiplier();
}

void optical_flow_set_cam_matrix(optical_flow_t *flow, float focal_len_x,
				 float focal_len_y, float principal_point_x,
				 float principal_point_y)
{
	flow->internal->setCameraMatrix(focal_len_x, focal_len_y, principal_point_x,
				     principal_point_y);
}

void optical_flow_set_distortion(optical_flow_t *flow, float k1, float k2,
				 float k3, float p1, float p2)
{
	flow->internal->setCameraDistortion(k1, k2, k3, p1, p2);
}

void optical_flow_set_img_width(optical_flow_t *flow, int width)
{
	flow->internal->setImageWidth(width);
}

int optical_flow_get_img_width(optical_flow_t *flow)
{
	return flow->internal->getImageWidth();
}

void optical_flow_set_img_height(optical_flow_t *flow, int height)
{
	flow->internal->setImageHeight(height);
}

int optical_flow_get_img_height(optical_flow_t *flow)
{
	return flow->internal->getImageHeight();
}

void optical_flow_set_focal_length_x(optical_flow_t *flow, float focal_len_x)
{
	flow->internal->setFocalLengthX(focal_len_x);
}

float optical_flow_get_focal_length_x(optical_flow_t *flow)
{
	return flow->internal->getFocalLengthX();
}

void optical_flow_set_focal_length_y(optical_flow_t *flow, float focal_len_y)
{
	flow->internal->setFocalLengthY(focal_len_y);
}

float optical_flow_get_focal_length_y(optical_flow_t *flow)
{
	return flow->internal->getFocalLengthY();
}

void optical_flow_set_output_rate(optical_flow_t *flow, int output_rate)
{
	flow->internal->setOutputRate(output_rate);
}

int optical_flow_get_output_rate(optical_flow_t *flow)
{
	return flow->internal->getOutputRate();
}
