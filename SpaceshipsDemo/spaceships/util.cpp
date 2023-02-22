#define SPACESHIPS_IMPLEMENTATION
#include "spaceships.hpp"

namespace quark::spaceships {
  Pid create_pid(f32 kp, f32 ki, f32 kd) {
    Pid pid = {};
    pid.kp = kp;
    pid.ki = ki;
    pid.kd = kd;

    pid.error_sum = 0;
    pid.last_error = 0;

    return pid;
  }

  f32 tick_pid(Pid* pid, f32 error, f32 delta_time) {
    pid->error_sum += error * delta_time;

    // proportional term
    f32 p_out = pid->kp * error;
    f32 i_out = pid->ki * pid->error_sum;
    f32 d_out = pid->kd * ((error - pid->last_error) / delta_time);

    f32 output = p_out + i_out + d_out;

    pid->last_error = error;

    return clamp(output, -1, 1);
  }
};
