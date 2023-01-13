//
// Created by theli on 1/12/2023.
//

#ifndef TASTOOLS_TASPOSITIONS_H
#define TASTOOLS_TASPOSITIONS_H

#include "Positions.h"

class TASPositions {
public:
    TASPositions();
    ~TASPositions();

    void parseFile(fs::path & filePath);
    void saveFile(fs::path & filePath);
    void addFrame(Position& position);
    void removeFrame(uint64_t frameNumber);
    void executeFrame();
    void resetFrameNumber();

    void clear();
    void addFrame(IEntity* pEntity);

    std::vector<Position> m_frames;
    uint64_t m_currentFrameNumber = 0;
    uint64_t m_frameCount = 0;
    fs::path m_loadedFilePath;
};


#endif //TASTOOLS_TASPOSITIONS_H
