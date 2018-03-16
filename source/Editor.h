#ifndef EDITOR_H
#define EDITOR_H

#include <ctime>
#include <vector>

class Renderer;
class Editor
{
public:
    enum _WINDOW_FLAG
    {
        MAIN_WINDOW     = 1 << 1,
        SETTINGS_WINDOW = 1 << 2,
        SAVE_AS_WINDOW  = 1 << 3
    };
    
    Editor(Renderer* _renderer);
    ~Editor();

    void update();
    void update_main_window();
    void update_settings_window();
    void update_popup_save_as();

    void render();

private:
    Renderer*   renderer;
    long        wnd_flags;
    int         testing_timer;
    bool        testing;
    clock_t     begin;
    std::string      name;
    std::vector<std::vector<double>> doubles;
};

#endif // !EDITOR_H
