#ifndef EDITORROOTITEM_H
#define EDITORROOTITEM_H

#include<QQuickItem>
#include<QColor>

/**
*   @brief Item used by the editor to control drag and drog of components.
*/

class EditorRootItem : public QQuickItem
{
    Q_OBJECT
    //little hack, cause the engine had it there
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    EditorRootItem(QQuickItem* parent = 0);
    ~EditorRootItem();

    QColor color(){ return m_color; }
    void setColor(QColor c){
        if(c != m_color){
            c = m_color;
            emit colorChanged();
        }
    }


protected:
    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dragLeaveEvent(QDragLeaveEvent *event);

    void dropEvent(QDropEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void hoverEnterEvent(QHoverEvent *event);


signals:
    void colorChanged();
private:
    QColor m_color;
    QList<QQuickItem*> m_selected;
    QQuickItem* transformedItem = nullptr;

    bool mTransforming = false;

};

Q_DECLARE_METATYPE(EditorRootItem*)

#endif // EDITORROOTITEM_H
