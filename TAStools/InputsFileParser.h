//
// Created by theli on 12/30/2022.
//

#ifndef TASTOOLS_INPUTSFILEPARSER_H
#define TASTOOLS_INPUTSFILEPARSER_H

#include <Prey/CryCore/StdAfx.h>
#include <pugixml.hpp>
#include "Inputs.h"

class InputsFileParser {
public:
    InputsFileParser();
    ~InputsFileParser() = default;
    // sorted list of frame inputs
    std::vector<FrameInputs> m_frameInputs;
    fs::path m_loadedInputsFilePath;
    uint64_t m_frameCount = 0;

    void parseInputsFile(fs::path inputsFilePath);
};


#endif //TASTOOLS_INPUTSFILEPARSER_H
