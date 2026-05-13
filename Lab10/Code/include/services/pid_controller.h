#ifndef SERVICES_PID_CONTROLLER_H
#define SERVICES_PID_CONTROLLER_H

float pidControllerComputeCoolingPercent(float errorC,
                                         float dtSeconds,
                                         float kp,
                                         float ki,
                                         float kd,
                                         float* integral,
                                         float* previousError);

void pidControllerReset(float* integral, float* previousError);

#endif
