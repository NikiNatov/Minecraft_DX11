#pragma once

#include <meteor.h>

using namespace meteor;

class CrosshairWindow
{
public:
    CrosshairWindow(Ref<Texture2D> crosshairTexture);

    void            OnImGuiRender();
private:
    Ref<Texture2D>  m_Texture;
};