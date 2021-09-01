#ifndef GRAPHICS_H
#define GRAPHICS_H

class MainWindow; //#include "mainwindow.h"

class Graphics {
private:
    MainWindow* window_;

public:
    Graphics();
    Graphics(MainWindow*);
    void update();
};

#endif // GRAPHICS_H
