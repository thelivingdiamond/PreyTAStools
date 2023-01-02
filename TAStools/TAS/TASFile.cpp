//
// Created by theli on 1/2/2023.
//

#include "TASFile.h"

void TASFile::loadTASFile(fs::path szTASFile) {
    m_TASActions.parseFile(szTASFile);

}

void TASFile::saveTASFile(fs::path szTASFile) {
    m_TASActions.saveFile(szTASFile);
}
