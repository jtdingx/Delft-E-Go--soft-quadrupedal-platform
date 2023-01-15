//
// Created by shuoy on 11/1/21.
//

#ifndef A1_CPP_A1BASICEKF_H
#define A1_CPP_A1BASICEKF_H

#include "Robotpara/A1Params.h"
#include "Robotpara/A1CtrlStates.h"
#include "utils/Utils.h"
#include "/home/jiatao/Documents/unitree_sdk_hardware_test/go1_remote_control/src/unitree_ros_to_real/unitree_legged_real/src/go1_rt_control/src/Robotpara/robot_const_para_config.h"

// state estimator parameters: the bigger, the smother
#define STATE_SIZE 18
#define MEAS_SIZE 28
// #define PROCESS_NOISE_PIMU 0.01
// #define PROCESS_NOISE_VIMU 0.01
// #define PROCESS_NOISE_PFOOT 0.01
// #define SENSOR_NOISE_PIMU_REL_FOOT 0.001
// #define SENSOR_NOISE_VIMU_REL_FOOT 0.1
// #define SENSOR_NOISE_ZFOOT 0.001

// #define PROCESS_NOISE_PIMU 0.08
// #define PROCESS_NOISE_VIMU 0.05
// #define PROCESS_NOISE_PFOOT 0.08
// #define SENSOR_NOISE_PIMU_REL_FOOT 0.008
// #define SENSOR_NOISE_VIMU_REL_FOOT 0.005
// #define SENSOR_NOISE_ZFOOT 0.008
// #define PROCESS_NOISE_PIMU 0.1
// #define PROCESS_NOISE_VIMU 0.1
// #define PROCESS_NOISE_PFOOT 0.1
// #define SENSOR_NOISE_PIMU_REL_FOOT 0.001
// #define SENSOR_NOISE_VIMU_REL_FOOT 0.1
// #define SENSOR_NOISE_ZFOOT 0.001


// implement a basic error state KF to estimate robot pose
// assume orientation is known from a IMU (state.root_rot_mat)
class A1BasicEKF {
public:
    A1BasicEKF ();
    //A1BasicEKF (bool assume_flat_ground_);
    void init_state(A1CtrlStates& state);
    void update_estimation(A1CtrlStates& state, double dt, Eigen::Vector3d estimated_root_pos_offset);
    bool is_inited() {return filter_initialized;}
// private:
    bool filter_initialized = false;
    // state
    // 0 1 2 pos 3 4 5 vel 6 7 8 foot pos FL 9 10 11 foot pos FR 12 13 14 foot pos RL 15 16 17 foot pos RR
    Eigen::Matrix<double, STATE_SIZE, 1> x; // estimation state
    Eigen::Matrix<double, STATE_SIZE, 1> xbar; // estimation state after process update
    Eigen::Matrix<double, STATE_SIZE, STATE_SIZE> P; // estimation state covariance
    Eigen::Matrix<double, STATE_SIZE, STATE_SIZE> Pbar; // estimation state covariance after process update
    Eigen::Matrix<double, STATE_SIZE, STATE_SIZE> A; // estimation state transition
    Eigen::Matrix<double, STATE_SIZE, 3> B; // estimation state transition
    Eigen::Matrix<double, STATE_SIZE, STATE_SIZE> Q; // estimation state transition noise

    // observation
    // 0 1 2   FL pos residual
    // 3 4 5   FR pos residual
    // 6 7 8   RL pos residual
    // 9 10 11 RR pos residual
    // 12 13 14 vel residual from FL
    // 15 16 17 vel residual from FR
    // 18 19 20 vel residual from RL
    // 21 22 23 vel residual from RR
    // 24 25 26 27 foot height
    Eigen::Matrix<double, MEAS_SIZE, 1> y; //  observation
    Eigen::Matrix<double, MEAS_SIZE, 1> yhat; // estimated observation
    Eigen::Matrix<double, MEAS_SIZE, 1> error_y; // estimated observation
    Eigen::Matrix<double, MEAS_SIZE, 1> Serror_y; // S^-1*error_y
    Eigen::Matrix<double, MEAS_SIZE, STATE_SIZE> C; // estimation state observation
    Eigen::Matrix<double, MEAS_SIZE, STATE_SIZE> SC; // S^-1*C
    Eigen::Matrix<double, MEAS_SIZE, MEAS_SIZE> R; // estimation state observation noise
    // helper matrices
    Eigen::Matrix<double, 3, 3> eye3; // 3x3 identity
    Eigen::Matrix<double, MEAS_SIZE, MEAS_SIZE> S; // Innovation (or pre-fit residual) covariance
    Eigen::Matrix<double, STATE_SIZE, MEAS_SIZE> K; // kalman gain




    // variables to process foot force
    double smooth_foot_force[4];
    double estimated_contacts[4];

    ///load yaml
    void config_set();

    double PROCESS_NOISE_PIMU;
    double PROCESS_NOISE_VIMU;
    double PROCESS_NOISE_PFOOT;
    double SENSOR_NOISE_PIMU_REL_FOOT;
    double SENSOR_NOISE_VIMU_REL_FOOT;
    double SENSOR_NOISE_ZFOOT;
    double force_z_limt;
    double assume_flat_ground;

};


#endif //A1_CPP_A1BASICEKF_H
