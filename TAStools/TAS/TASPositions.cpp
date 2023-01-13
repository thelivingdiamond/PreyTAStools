//
// Created by theli on 1/12/2023.
//

#include "TASPositions.h"
#include <Prey/CryPhysics/physinterface.h>

void TASPositions::addFrame(IEntity *pEntity) {
    if (pEntity == nullptr) {
        CryError("TASPositions::addFrame: pEntity is null");
        return;
    }
    auto status = new pe_status_dynamics;
    pEntity->GetPhysics()->GetStatus(status);
    Position position;
    position.m_frameNumber = m_currentFrameNumber;
    position.m_position = pEntity->GetPos();
    position.m_rotation = pEntity->GetRotation();
    position.m_velocity = status->v;
    position.m_angularVelocity = status->w;
    position.m_acceleration = status->a;
    position.m_angularAcceleration = status->wa;
    m_frames.push_back(position);
    m_currentFrameNumber++;
}

TASPositions::TASPositions() {
    m_currentFrameNumber = 0;
    m_frameCount = 0;

}

TASPositions::~TASPositions() {

}

void TASPositions::parseFile(fs::path &filePath) {

}

void TASPositions::saveFile(fs::path &filePath) {

}

void TASPositions::addFrame(Position &position) {

}

void TASPositions::removeFrame(uint64_t frameNumber) {

}

void TASPositions::executeFrame() {

}

void TASPositions::resetFrameNumber() {

}

void TASPositions::clear() {

}
