//
// Created by theli on 1/12/2023.
//

#ifndef TASTOOLS_POSITIONS_H
#define TASTOOLS_POSITIONS_H


struct Position {
    Vec3 m_position;
    Quat m_rotation;
    Vec3 m_velocity;
    Vec3 m_angularVelocity;
    Vec3 m_acceleration;
    Vec3 m_angularAcceleration;
    uint64_t m_frameNumber;
};


#endif //TASTOOLS_POSITIONS_H
