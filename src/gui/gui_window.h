#pragma once

namespace kiv_vss::gui
{
    class IGUI_Window
    {
    public:
        virtual ~IGUI_Window() = default;

        virtual void Render() = 0;
    };
}