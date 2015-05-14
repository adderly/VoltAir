#ifndef RESIZECOMPONENT_H
#define RESIZECOMPONENT_H

#include <QQuickPaintedItem>
#include <Actor.h>
#include <QMatrix4x4>
#include <graphics/Graphic.h>

/**
*
*/
class ResizeControl :public Graphic
{
    Q_OBJECT
public:
    ResizeControl(QQuickItem* parent = nullptr);
    ~ResizeControl();

    enum AXIS:int{
        X_AXIS,
        Y_AXIS,
        X_AND_Y_AXIS
    };

    enum AXIS_TYPE:int{
        ROTATION,
        SCALE
    };

    void componentComplete() override;

signals:

public slots:
    virtual void initControls();

protected:
    virtual QSGNode* updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    std::vector<QQuickItem*> m_controls;
    QMatrix4x4 _m;
};
class Control{
public:
    ResizeControl::AXIS axis;
    ResizeControl::AXIS_TYPE axis_type;
    QRect m_pos;
};

#endif // RESIZECOMPONENT_H
