#include "drawingarea.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>

drawingarea::drawingarea(QWidget *parent) : QWidget(parent)
{

}

void drawingarea::mouseMoveEvent(QMouseEvent *e_mouse)
{
    ui->mposition->setText("(" + QString::number(e_mouse->x()) + "," + QString::number(e_mouse->y()) + ")");
}
