#ifndef _STATE_H_
#define _STATE_H_

#include <stdint.h>
#include <ch.h>
#include "Recorder.h"
#include "constants.h"

namespace motor_driver {
namespace state {

struct Results {
  float foc_d_current = 0;                  // Measured FOC direct current (amperes)
  float foc_q_current = 0;                  // Measured FOC quadrature current (amperes)
  float foc_d_voltage = 0;                  // Measured FOC direct voltage (volts)
  float foc_q_voltage = 0;                  // Measured FOC quadrature voltage (volts)

  float id_output = 0;                      // id output from PID loop to motor
  float iq_output = 0;                      // iq output from PID loop to motor

  float duty_a = 0;                         // Calculated duty cycle for phase A
  float duty_b = 0;                         // Calculated duty cycle for phase B
  float duty_c = 0;                         // Calculated duty cycle for phase C

  uint8_t encoder_mode = consts::encoder_mode_none; // Encoder mode
  uint16_t raw_enc_value = 0;                       // Raw encoder value, wraps around
  float enc_pos = 0;                                // Corrected encoder position, wraps around (radians)
  uint32_t encoder_diag = 0;                        // Encoder diagnostics

  int16_t rotor_revs = 0;                   // Total number of rotor revolutions
  float rotor_pos = 0;                      // Rotor position (radians)
  float hf_rotor_vel = 0;                   // Rotor velocity High Frequency Estimate (radians/second)
  float lf_rotor_vel = 0;                   // Rotor velocity Low Frequency Estimate (radians/second)

  float va = 0;                             // voltage on phase A (volts)
  float vb = 0;                             // voltage on phase B (volts)
  float vc = 0;                             // voltage on phase C (volts)
  float vin = 0;                            // supply voltage (volts)
  float ia = 0;                             // current into phase A (amperes)
  float ib = 0;                             // current into phase B (amperes)
  float ic = 0;                             // current into phase C (amperes)

  int16_t xl_x = 0;                         // X-acceleration in milli-g's
  int16_t xl_y = 0;                         // Y-acceleration in milli-g's
  int16_t xl_z = 0;                         // Z-acceleration in milli-g's

  float temperature = 0;                    // Temperature in degrees Celsius

  Results() {}
};

struct Calibration {
  uint16_t start_sequence = consts::calib_ss;   // Start sequence to determine whether this is a valid calibration
  uint16_t erev_start = 0;                      // Encoder reading at the start of an electrical revolution
  uint8_t erevs_per_mrev = 1;                   // Electrical revolutions per mechanical revolution
  uint8_t flip_phases = false;                  // Phases A, B, C are arranged in clockwise instead of ccw order
  float foc_kp_d = 0.5f;                        // Proportional gain for FOC/d PI loop
  float foc_ki_d = 0.1f;                        // Integral gain for FOC/d PI loop
  float foc_kp_q = 1.0f;                        // Proportional gain for FOC/q PI loop
  float foc_ki_q = 0.2f;                        // Integral gain for FOC/q PI loop
  float velocity_kp = 0.1f;                     // Proportional gain for velocity PI loop
  float velocity_kd = 1e-3f;                    // Integral gain for velocity PI loop
  float position_kp = 5.0f;                     // Proportional gain for position PI loop
  float position_kd = 0.0f;                    // Integral gain for position PI loop
  float current_limit = 2.0f;                   // Current limit (A)
  float torque_limit = 3.0f;                    // Torque limit (N*m)
  float velocity_limit = 10.0f;                 // Velocity limit (rad/s)
  float position_lower_limit = 0.0f;            // Position lower limit (rad)
  float position_upper_limit = 0.0f;            // Position upper limit (rad)
  float motor_resistance = 17.8f;               // Motor resistance (ohm)
  float motor_inductance = 0.0f;                // Motor inductance (henries)
  float motor_torque_const = 0.0f;              // Motor torque constant (newton-meters per ampere)
  uint16_t control_timeout = 0;                 // Control timeout (ms)
  float hf_velocity_filter_param = 0.01f;       // Parameter for high frequency velocity estimate 
  float lf_velocity_filter_param = (1.0-.9975); // Parameter for low frequency velocity estimate 
  float position_offset = 0.0f;                 // Position offset
  float ia_offset = 0.0f;                       // Current Offset for Phase A
  float ib_offset = 0.0f;                       // Current Offset for Phase B
  float ic_offset = 0.0f;                       // Current Offset for Phase C
  float enc_ang_corr_scale = 0.0f;              // Encoder angle correction scale (rad)
  float enc_ang_corr_offset = 0.0f;             // Encoder angle correction offset (rad)
  int8_t enc_ang_corr_table_values[consts::enc_ang_corr_table_size]; // Encoder angle correction table values

  Calibration() {}
};

struct Parameters {
  uint8_t control_mode = consts::control_mode_foc_current;  // Control mode
  float foc_q_current_sp = 0.0f;                            // FOC quadrature current setpoint in amperes
  float foc_d_current_sp = 0.0f;                            // FOC direct current setpoint in amperes
  bool override_led_color = false;                          // Override normal status LED behavior
  uint8_t led_red_intensity = 0;                            // Status LED red intensity
  uint8_t led_green_intensity = 0;                          // Status LED green intensity
  uint8_t led_blue_intensity = 0;                           // Status LED blue intensity

  float phase0 = 0.0f;                                      // Phase 0 duty cycle
  float phase1 = 0.0f;                                      // Phase 1 duty cycle
  float phase2 = 0.0f;                                      // Phase 2 duty cycle

  float torque_sp = 0.0f;                                   // Torque control setpoint (N*m)
  float velocity_sp = 0.0f;                                 // Velocity control setpoint (rad/s)
  float position_sp = 0.0f;                                 // Position control setpoint (rad)
  float feed_forward = 0.0f;                                // Feed forward term for load compensation (A)
  float pwm_drive = 0.0f;

  bool gate_active = false;                                 // Flag for whether the gates are active or not
  bool gate_fault = false;                                  // Flag for whether the gate has a fault

  bool timeout_flag = false;
  Parameters() {}
};

/**
 * Result values written by the control thread
 */
extern Results results;

/**
 * Calibration values
 */
extern Calibration calibration;

/**
 * Parameter values written by the comms thread
 */
extern Parameters parameters;

/**
 * Recorder
 */
extern Recorder recorder;

/**
 * Results synchronization was requested
 */
extern volatile bool should_copy_results;

/**
 * Parameter synchronization was requested
 */
extern volatile bool should_copy_parameters;

void initState();

void storeCalibration();

void loadCalibration();

void clearCalibration();

} // namespace state
} // namespace motor_driver

#endif /* _STATE_H_ */
