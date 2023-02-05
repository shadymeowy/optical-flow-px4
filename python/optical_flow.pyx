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
    
    optical_flow_t* optical_flow_new(double hfov, int rate, double first_frame_time_)
    int optical_flow_feed(optical_flow_t *flow, uint8_t *image, double frame_time,
		      optical_flow_msg_t *msg)
    int optical_flow_gyro(optical_flow_t *flow, double now,
		      double gyro_angular_vel[3])
    int optical_flow_destroy(optical_flow_t* optical_flow)

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

    def __cinit__(self, double hfov, rate, first_frame_time):
        self.flow = optical_flow_new(hfov, rate, first_frame_time)

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

    def __dealloc__(self):
        pass
