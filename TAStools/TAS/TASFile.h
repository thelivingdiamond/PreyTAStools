//
// Created by theli on 1/2/2023.
//

#ifndef TASTOOLS_TASFILE_H
#define TASTOOLS_TASFILE_H

#include "TASInfo.h"
#include "TASActions.h"

class TASFile {
public:
    TASFile() = default;
    ~TASFile() = default;
    void loadTASFile(fs::path szTASFile);
    void saveTASFile(fs::path szTASFile);

    TASInfo m_TASInfo;
    TASActions m_TASActions;
};


#endif //TASTOOLS_TASFILE_H
