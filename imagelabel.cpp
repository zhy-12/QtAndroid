#include "imagelabel.h"
#include <QPainter>
imageLabel::imageLabel(QWidget *parent)
    : QLabel{parent}
{
    setAlignment(Qt::AlignHCenter);
}
void imageLabel::setImage(QImage image)
{
    //Set the image and invalidate our cached pixmap
    m_image = image;
    m_cachedPixmap = QPixmap();
    update();
}

void imageLabel::paintEvent(QPaintEvent *)
{
    if ( !m_image.isNull() )
    {
        QSize scaledSize = size() * devicePixelRatio();
        if (m_cachedPixmap.size() != scaledSize)
        {
            QImage scaledImage = m_image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            m_cachedPixmap = QPixmap::fromImage(scaledImage);
            m_cachedPixmap.setDevicePixelRatio(devicePixelRatio());
            //this->resize(m_cachedPixmap.size());
        }

        QPainter p(this);
        p.drawPixmap(0, 0, m_cachedPixmap);
    }
}
