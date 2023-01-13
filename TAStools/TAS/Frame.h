//
// Created by theli on 1/2/2023.
//

#ifndef TASTOOLS_FRAME_H
#define TASTOOLS_FRAME_H

#include "Actions.h"


class Frame {
public:
    Frame() = default;
    ~Frame() = default;
    explicit Frame(uint64_t frameNumber);
    Frame(std::string &inputString, uint64_t frameNumber);
    Frame(std::vector<SInputEvent> &events, uint64_t frameNumber);
    void parseString(std::string & inputString);
    void parseEvents(std::vector<SInputEvent> & events);
    std::string toString();
    void setFrameNumber(uint64_t frameNumber){ m_frameNumber = frameNumber; }
    uint64_t getFrameNumber() const{ return m_frameNumber; }
    void execute();
    void addAction(Action * action);
    void removeAction(Action * action);
    void clearActions();
    bool empty() {
        return m_actions.empty();
    }

    std::list<Action*> m_actions;
    std::string m_inputString;
    uint64_t m_frameNumber;

    // create operators for sorting and searching by frame number
    inline bool operator<(const Frame & other) const { return m_frameNumber < other.m_frameNumber; }
    inline bool operator==(const Frame & other) const { return m_frameNumber == other.m_frameNumber; }
    inline bool operator!=(const Frame & other) const { return !(*this == other); }
    inline bool operator>(const Frame & other) const { return !(*this < other) && (*this != other); }
    inline bool operator<=(const Frame & other) const { return (*this < other) || (*this == other); }
    inline bool operator>=(const Frame & other) const { return !(*this < other); }

    // operators for sorting and searching by frame number compared to a uint64_t
    inline bool operator<(const uint64_t & other) const { return m_frameNumber < other; }
    inline bool operator==(const uint64_t & other) const { return m_frameNumber == other; }
    inline bool operator!=(const uint64_t & other) const { return !(*this == other); }
    inline bool operator>(const uint64_t & other) const { return !(*this < other) && (*this != other); }
    inline bool operator<=(const uint64_t & other) const { return (*this < other) || (*this == other); }
    inline bool operator>=(const uint64_t & other) const { return !(*this < other); }


    bool verify(std::vector<SInputEvent> inputs);
};


#endif //TASTOOLS_FRAME_H
