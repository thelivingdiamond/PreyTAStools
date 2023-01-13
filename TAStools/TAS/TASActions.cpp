//
// Created by theli on 1/2/2023.
//

#include "TASActions.h"

void TASActions::executeFrame() {
    if (m_currentFrameNumber >= m_frameCount) {
        return;
    }
    m_frames[m_currentFrameNumber].execute();
    m_currentFrameNumber++;
}

void TASActions::parseFile(fs::path &filePath) {
    if(!fs::exists(filePath)) {
        OverlayError("InputsFileParser::parseInputsFile: inputs file does not exist");
        return;
    }
    // inputs file is a text file
    // format is:
    // each line represents a frame
    // each frame has sections delineated by a pipe
    // construct a new FrameInputs object for each line

    std::ifstream inputsFile(filePath);
    if(!inputsFile.is_open()) {
        OverlayError("InputsFileParser::parseInputsFile: inputs file could not be opened");
        return;
    }
    m_frames.clear();
    m_loadedFilePath = filePath;
    std::string line;
    uint64_t frameNumber = 0;
    while(std::getline(inputsFile, line)) {
        Frame frame(line, frameNumber);
        m_frames.push_back(frame);
        frameNumber++;
    }
    inputsFile.close();
    std::sort(m_frames.begin(), m_frames.end());
    m_frameCount = m_frames.size();


}

void TASActions::addFrame(Frame &frame) {
    m_frames.push_back(frame);
    m_frameCount++;
}

void TASActions::removeFrame(uint64_t frameNumber) {
    if(frameNumber >= m_frameCount) {
        return;
    }
    m_frames.erase(m_frames.begin() + frameNumber);
    m_frameCount--;
}

void TASActions::saveFile(fs::path &filePath) {
    std::ofstream inputsFile(filePath);
    if(!inputsFile.is_open()) {
        OverlayError("InputsFileParser::saveInputsFile: inputs file could not be opened");
        return;
    }
    for(auto & frame : m_frames) {
        inputsFile << frame.toString() << std::endl;
    }
    inputsFile.close();
}

void TASActions::resetFrameNumber() {
    m_currentFrameNumber = 0;
}

void TASActions::addFrame(std::vector<SInputEvent> &events) {
    Frame frame(events, m_frameCount);
    m_frames.push_back(frame);
    m_frameCount++;

}

void TASActions::clear() {
    m_frames.clear();
    m_frameCount = 0;
    m_currentFrameNumber = 0;
}

bool TASActions::verifyFrame(std::vector<SInputEvent> inputs) {
    if(m_currentFrameNumber >= m_frameCount) {
        return false;
    }
    return m_frames[m_currentFrameNumber].verify(inputs);
}
