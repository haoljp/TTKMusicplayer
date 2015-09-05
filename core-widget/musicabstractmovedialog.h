#ifndef MUSICABSTRACTMOVEDIALOG_H
#define MUSICABSTRACTMOVEDIALOG_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicAbstractMoveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MusicAbstractMoveDialog(QWidget *parent = 0);
    virtual ~MusicAbstractMoveDialog();

    void drawWindowRoundedRect(QWidget *widget);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_pressAt;
    bool m_leftButtonPress;

};

#endif // MUSICABSTRACTMOVEDIALOG_H