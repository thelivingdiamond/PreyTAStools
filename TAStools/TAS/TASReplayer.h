//
// Created by theli on 1/2/2023.
//

#ifndef TASTOOLS_TASREPLAYER_H
#define TASTOOLS_TASREPLAYER_H

struct Action;

//! this class will be responsible for replaying the TAS file, and as such will handle all the actions that are in the file
class TASReplayer {
public:
    TASReplayer();
    ~TASReplayer();

    static TASReplayer* getInstance();

    void Update();
    void submitAction(Action* action);

    void needToReleaseKey(Action* action);

    std::queue<Action*> m_actions;
    std::vector<EKeyId> m_keysToRelease;
    std::vector<EKeyId> m_keysPressed;

    bool wasKeyJustPressed(EKeyId key);


};


#endif //TASTOOLS_TASREPLAYER_H
