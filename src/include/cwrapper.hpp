extern "C" {
#include <cwrapper.h>
}

#include <optical_flow.hpp>
#include <flow_opencv.hpp>

struct optical_flow_t {
	OpticalFlowOpenCV *internal;
	double optical_flow_rate[3];
	double first_frame_time_;
	double last_dt_us;
};