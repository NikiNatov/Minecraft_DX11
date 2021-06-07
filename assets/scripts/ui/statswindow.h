#pragma once

#include <meteor.h>

using namespace meteor;

class StatsWindow
{
public:
    StatsWindow(mtrU32 gameVersionMajor, mtrU32 gameVersionMinor, Entity player);

    void    OnImGuiRender();
private:
    Entity  m_Player;
    mtrU32  m_GameVersionMajor;
    mtrU32  m_GameVersionMinor;
};