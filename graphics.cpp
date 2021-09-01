#include "graphics.h"
#include "mainwindow.h"

Graphics::Graphics(){}
Graphics::Graphics(MainWindow* window) : window_{window} {}

void Graphics::update(){
    window_->repaint();
}