#pragma once

#include <meteor.h>

using namespace meteor;

class PauseMenu
{
public:
    PauseMenu() = default;

    void            OnImGuiRender();

    void            Toggle();
    inline mtrBool  IsOpen() const { return m_Open; }
private:
    mtrBool         m_Open = false;
};
