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
    std::queue<FrameInputs> m_frameInputsQueue;
    void parseInputsFile(fs::path inputsFilePath);
};


#endif //TASTOOLS_INPUTSFILEPARSER_H
