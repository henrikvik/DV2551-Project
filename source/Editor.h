#ifndef EDITOR_H
#define EDITOR_H

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
    double      test_timer_sec;
};

#endif // !EDITOR_H
