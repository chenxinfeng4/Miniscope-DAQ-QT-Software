#ifndef TRACEDISPLAY_H
#define TRACEDISPLAY_H

#include "newquickview.h"

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLTexture>

#include <QJsonObject>
#include <QVector>


class TraceDisplayBackend : public QObject
{
    Q_OBJECT
public:
    TraceDisplayBackend(QObject *parent = nullptr, QJsonObject ucTraceDisplay = QJsonObject());
    void createView();

public slots:
    void close();

private:
    NewQuickView *view;
    QJsonObject m_ucTraceDisplay;
};

class TraceDisplayRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    TraceDisplayRenderer();
    ~TraceDisplayRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

    void initPrograms();

    // Display update funtions
    void updateGridV();
    void drawGridV();
    QVector<float> position;
    QVector<float> color;
    QVector<float> index;

    float pan[2];
    float scale[2];
    float magnify[2];

    float windowSize; // in seconds
    float gridSpacingV;

//signals:

public slots:
    void paint();

private:
    QSize m_viewportSize;
    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *m_texture;
    QQuickWindow *m_window;

    qreal m_t;

    // Vars for display


    // Programs used for trace display
    QOpenGLShaderProgram *m_programGridV;
    QOpenGLShaderProgram *m_programGridH;

};

class TraceDisplay : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    TraceDisplay();
    qreal t() const { return m_t; }
    void setT(qreal t);


signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
    TraceDisplayRenderer *m_renderer;

};

#endif // TRACEDISPLAY_H
