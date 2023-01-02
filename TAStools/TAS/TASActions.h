//
// Created by theli on 1/2/2023.
//

#ifndef TASTOOLS_TASACTIONS_H
#define TASTOOLS_TASACTIONS_H

#include "Frame.h"

class TASActions {
public:
    TASActions() = default;
    ~TASActions() = default;

    std::vector<Frame> m_frames;
    uint64_t m_currentFrameNumber = 0;
    uint64_t m_frameCount = 0;
    fs::path m_loadedFilePath;

    void parseFile(fs::path & filePath);
    void saveFile(fs::path & filePath);
    void addFrame(Frame & frame);
    void removeFrame(uint64_t frameNumber);
    void executeFrame();

};


#endif //TASTOOLS_TASACTIONS_H
