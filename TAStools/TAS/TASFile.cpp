//
// Created by theli on 1/2/2023.
//

#include "TASFile.h"

void TASFile::loadTASFile(fs::path szTASFile) {
    m_TASActions.parseFile(szTASFile);
    OverlayLog("Loaded TAS file: %s", szTASFile.string().c_str());
}

void TASFile::saveTASFile(fs::path szTASFile) {
    m_TASActions.saveFile(szTASFile);
}
