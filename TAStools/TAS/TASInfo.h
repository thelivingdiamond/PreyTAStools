//
// Created by theli on 1/2/2023.
//

#ifndef TASTOOLS_TASINFO_H
#define TASTOOLS_TASINFO_H

#include <Prey/CryCore/StdAfx.h>
#include <Chairloader/SemanticVersion.h>
#include <pugixml.hpp>
#include <utility>

struct TASInfo {
    //! the name of the TAS file
    std::string m_tasName;
    //! the version of the TAS file
    SemanticVersion m_version;
    //! A list of the authors of the TAS file
    std::vector<std::string> m_authors;
    //! the FPS the movie is recorded at
    float m_movieFPS;
    //! the total frame count of the movie
    uint64_t m_movieFrameCount;
    //! the physics time step
    float m_physicsFrameTime;
    //! the total number of physics frames
    uint64_t m_physicsFrameCount;
    //! the TAStools version used to create the TAS file
    SemanticVersion m_TAStoolsVersion;
    //! A list of starting CVar values for the TAS
    std::vector<std::pair<std::string, std::string>> m_CVars;

    //! The default constructor
    TASInfo() = default;

    //! The constructor
    TASInfo(std::string tasName, std::string version, std::vector<std::string> authors, float movieFPS, uint64_t movieFrameCount, float physicsFrameTime, uint64_t physicsFrameCount, std::string TAStoolsVersion, std::vector<std::pair<std::string, std::string>> CVars) :
        m_tasName(std::move(tasName)),
        m_version(std::move(version)),
        m_authors(std::move(authors)),
        m_movieFPS(movieFPS),
        m_movieFrameCount(movieFrameCount),
        m_physicsFrameTime(physicsFrameTime),
        m_physicsFrameCount(physicsFrameCount),
        m_TAStoolsVersion(std::move(TAStoolsVersion)),
        m_CVars(std::move(CVars)) {}

    //! load from pugi::xml_node
    bool loadFromXML(const pugi::xml_node& node){
        m_tasName = node.attribute("name").as_string();
        m_version = SemanticVersion((const char *)node.attribute("version").value());
        m_movieFPS = node.attribute("movie_fps").as_float();
        m_movieFrameCount = node.attribute("movie_frame_count").as_ullong();
        m_physicsFrameTime = node.attribute("physics_frame_time").as_float();
        m_physicsFrameCount = node.attribute("physics_frame_count").as_ullong();
        m_TAStoolsVersion = SemanticVersion((const char *)node.attribute("tas_tools_version").value());
        m_CVars.clear();
        for (auto& cvar : node.child("CVARS").children("CVAR")) {
            m_CVars.emplace_back(cvar.attribute("name").as_string(), cvar.attribute("value").as_string());
        }
    }

};

#endif //TASTOOLS_TASINFO_H
