# ADR-0003: Initial IMU

- Status: Accepted
- Date: 2026-09-25

## Context

The air mouse needs an inertial sensor for motion input. The first prototype should use a common, inexpensive, well-documented module.

## Decision

Use the **MPU6050** as the initial IMU.

The gyroscope is the primary source for cursor movement. Accelerometer data remains available for calibration, orientation estimation, motion detection, stabilization experiments, and future features.

Communication will use I2C.

## Consequences

### Positive
- Simple digital interface
- Widely available modules
- Suitable for early cursor-control experiments
- Provides both gyroscope and accelerometer data

### Negative
- Sensor noise and drift require calibration and filtering
- Mechanical mounting and orientation affect the motion pipeline
- The initial sensor is not necessarily the final sensor

## Constraints

Sensor orientation, I2C address, electrical levels, and exact wiring must be verified before implementation.
