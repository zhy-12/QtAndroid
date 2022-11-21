#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>

class imageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit imageLabel(QWidget *parent = nullptr);
    void setImage(QImage image);
    void paintEvent(QPaintEvent *) override;
private:
    QImage m_image;
    QPixmap m_cachedPixmap;

signals:

};

#endif // IMAGELABEL_H
