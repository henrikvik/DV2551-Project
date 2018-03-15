#ifndef EDITOR_H
#define EDITOR_H

class Renderer;
class Editor
{
public:
    enum _WINDOW_FLAG
    {
        MAIN_WINDOW     = 1 << 1,
        SETTINGS_WINDOW = 1 << 2
    };
    
    Editor(Renderer* _renderer);
    ~Editor();

    void update();
    void update_main_window();
    void update_settings_window();

    void render();

private:
    Renderer*   renderer;
    long        wnd_flags;
};

#endif // !EDITOR_H
