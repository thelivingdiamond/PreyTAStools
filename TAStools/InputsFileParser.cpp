//
// Created by theli on 12/30/2022.
//

#include <sstream>
#include "InputsFileParser.h"
#include <fstream>

InputsFileParser::InputsFileParser() {
}

void InputsFileParser::parseInputsFile(fs::path inputsFilePath) {
    if(!fs::exists(inputsFilePath)) {
        OverlayError("InputsFileParser::parseInputsFile: inputs file does not exist");
        return;
    }
    // inputs file is a text file
    // format is:
    // each line represents a frame
    // each frame has sections delineated by a pipe
    // construct a new FrameInputs object for each line

    std::ifstream inputsFile(inputsFilePath);
    if(!inputsFile.is_open()) {
        OverlayError("InputsFileParser::parseInputsFile: inputs file could not be opened");
        return;
    }
    m_frameInputs.clear();
    m_loadedInputsFilePath = inputsFilePath;
    std::string line;
    uint64_t frameNumber = 0;
    while(std::getline(inputsFile, line)) {
        FrameInputs frameInputs;
        if(!frameInputs.parseString(line)){
            CryWarning("FrameInputs::parseInputsFile: could not parse line: {}", line.c_str());
            continue;
        }
        frameInputs.setFrameNumber(frameNumber);
        m_frameInputs.push_back(frameInputs);
        frameNumber++;
    }
    inputsFile.close();
    std::sort(m_frameInputs.begin(), m_frameInputs.end());
    m_frameCount = m_frameInputs.size();
}
