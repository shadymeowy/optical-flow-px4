import cython
from cython cimport view
from libc.stdint cimport *

import numpy as np
cimport numpy as np

try:
    import typing
    import dataclasses
except ImportError:
    pass  # The modules don't actually have to exists for Cython to use them as annotations

cdef:
    _DEFAULT_OUTPUT_RATE = 15
    _DEFAULT_IMAGE_WIDTH = 64
    _DEFAULT_IMAGE_HEIGHT = 64
    _DEFAULT_NUMBER_OF_FEATURES = 20
    _DEFAULT_CONFIDENCE_MULTIPLIER = 1.645

DEFAULT_OUTPUT_RATE = _DEFAULT_OUTPUT_RATE
DEFAULT_IMAGE_WIDTH = _DEFAULT_IMAGE_WIDTH
DEFAULT_IMAGE_HEIGHT = _DEFAULT_IMAGE_HEIGHT
DEFAULT_NUMBER_OF_FEATURES = _DEFAULT_NUMBER_OF_FEATURES
DEFAULT_CONFIDENCE_MULTIPLIER = _DEFAULT_CONFIDENCE_MULTIPLIER

cdef extern from "../../src/include/cwrapper.h":
    ctypedef struct optical_flow_msg_t:
        int64_t time_usec
        int32_t sensor_id
        int32_t integration_time_us
        float integrated_x
        float integrated_y
        float integrated_xgyro
        float integrated_ygyro
        float integrated_zgyro
        float temperature
        int32_t quality
        int32_t time_delta_distance_us
        float distance
        
    ctypedef struct optical_flow_t:
        pass
    
    optical_flow_t* optical_flow_new(float f_length_x, float f_length_y,
				 int output_rate, int img_width, int img_height,
				 int num_feat, float conf_multi)
    int optical_flow_feed(optical_flow_t *flow, uint8_t *image, double frame_time,
		      optical_flow_msg_t *msg)
    int optical_flow_gyro(optical_flow_t *flow, double now,
		      double gyro_angular_vel[3])
    int optical_flow_destroy(optical_flow_t* optical_flow)

    void optical_flow_set_num_features(optical_flow_t *flow, int num_features)
    int optical_flow_get_num_features(optical_flow_t *flow)
    void optical_flow_set_conf_multiplier(optical_flow_t *flow,
                        float conf_multiplier)
    float optical_flow_get_conf_multiplier(optical_flow_t *flow);
    void optical_flow_set_cam_matrix(optical_flow_t *flow, float focal_len_x,
                    float focal_len_y, float principal_point_x,
                    float principal_point_y)
    void optical_flow_set_distortion(optical_flow_t *flow, float k1, float k2,
                    float k3, float p1, float p2)
    void optical_flow_set_img_width(optical_flow_t *flow, int width)
    int optical_flow_get_img_width(optical_flow_t *flow)
    void optical_flow_set_img_height(optical_flow_t *flow, int height)
    int optical_flow_get_img_height(optical_flow_t *flow)
    void optical_flow_set_focal_length_x(optical_flow_t *flow, float focal_len_x)
    float optical_flow_get_focal_length_x(optical_flow_t *flow)
    void optical_flow_set_focal_length_y(optical_flow_t *flow, float focal_len_y)
    float optical_flow_get_focal_length_y(optical_flow_t *flow)
    void optical_flow_set_output_rate(optical_flow_t *flow, int output_rate)
    int optical_flow_get_output_rate(optical_flow_t *flow)

@cython.dataclasses.dataclass
cdef class OpticalFlowMsg:
    time_usec: int64_t = 0
    sensor_id: int32_t = 0
    integration_time_us: int32_t = 0
    integrated_x: float = 0.0
    integrated_y: float = 0.0
    integrated_xgyro: float = 0.0
    integrated_ygyro: float = 0.0
    integrated_zgyro: float = 0.0
    temperature: float = 0.0
    quality: int32_t = 0
    time_delta_distance_us: int32_t = 0
    distance: float = 0.0

cdef class OpticalFlow:
    cdef optical_flow_t *flow
    cdef optical_flow_msg_t msg

    def __cinit__(self,
            double f_length_x,
            double f_length_y,
            int output_rate = _DEFAULT_OUTPUT_RATE,
            int img_width = _DEFAULT_IMAGE_WIDTH,
            int img_height = _DEFAULT_IMAGE_HEIGHT,
			int num_feat = _DEFAULT_NUMBER_OF_FEATURES,
            double conf_multi = _DEFAULT_CONFIDENCE_MULTIPLIER):

        self.flow = optical_flow_new(f_length_x, f_length_y,
                    output_rate, img_width, img_height,
                    num_feat, conf_multi)

    cpdef OpticalFlowMsg feed(self, object image, double frame_time):
        cdef:
            np.ndarray[uint8_t, ndim=2] image0 = np.array(image)
            uint8_t[:, ::view.contiguous] image1 = image0
            uint8_t* image2 = &image1[0, 0]
        
        cdef int ret = optical_flow_feed(self.flow, image2, frame_time, &self.msg)
        cdef OpticalFlowMsg msg = OpticalFlowMsg()
        msg.time_usec = self.msg.time_usec
        msg.sensor_id = self.msg.sensor_id
        msg.integration_time_us = self.msg.integration_time_us
        msg.integrated_x = self.msg.integrated_x
        msg.integrated_y = self.msg.integrated_y
        msg.integrated_xgyro = self.msg.integrated_xgyro
        msg.integrated_ygyro = self.msg.integrated_ygyro
        msg.integrated_zgyro = self.msg.integrated_zgyro
        msg.temperature = self.msg.temperature
        msg.quality = self.msg.quality
        msg.time_delta_distance_us = self.msg.time_delta_distance_us
        msg.distance = self.msg.distance
        return msg
    
    cpdef int gyro(self, double now, double[:] gyro_angular_vel):
        cdef double _gyro_angular_vel[3]
        _gyro_angular_vel[0] = gyro_angular_vel[0]
        _gyro_angular_vel[1] = gyro_angular_vel[1]
        _gyro_angular_vel[2] = gyro_angular_vel[2]
        cdef int ret = optical_flow_gyro(self.flow, now, &_gyro_angular_vel[0])
        return ret

    # num_features, conf_multiplier, img_width, img__height, focal_length_x, focal_length_y, output_rate
    @property
    def num_features(self):
        return optical_flow_get_num_features(self.flow)
    
    @num_features.setter
    def num_features(self, int num_features):
        optical_flow_set_num_features(self.flow, num_features + 1)
    
    @property
    def conf_multiplier(self):
        return optical_flow_get_conf_multiplier(self.flow)
    
    @conf_multiplier.setter
    def conf_multiplier(self, float conf_multiplier):
        optical_flow_set_conf_multiplier(self.flow, conf_multiplier)
    
    @property
    def img_width(self):
        return optical_flow_get_img_width(self.flow)

    @img_width.setter
    def img_width(self, int width):
        optical_flow_set_img_width(self.flow, width)

    @property
    def img_height(self):
        return optical_flow_get_img_height(self.flow)
    
    @img_height.setter
    def img_height(self, int height):
        optical_flow_set_img_height(self.flow, height)
    
    @property
    def focal_length_x(self):
        return optical_flow_get_focal_length_x(self.flow)
    
    @focal_length_x.setter
    def focal_length_x(self, float focal_len_x):
        optical_flow_set_focal_length_x(self.flow, focal_len_x)

    @property
    def focal_length_y(self):
        return optical_flow_get_focal_length_y(self.flow)
    
    @focal_length_y.setter
    def focal_length_y(self, float focal_len_y):
        optical_flow_set_focal_length_y(self.flow, focal_len_y)
    
    @property
    def output_rate(self):
        return optical_flow_get_output_rate(self.flow)
    
    @output_rate.setter
    def output_rate(self, int output_rate):
        optical_flow_set_output_rate(self.flow, output_rate)
    
    def __dealloc__(self):
        pass