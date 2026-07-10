#include "previewimageprovider.h"

PreviewImageProvider::PreviewImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage PreviewImageProvider::requestImage(const QString &id, QSize *size,
                                          const QSize &requestedSize)
{
    Q_UNUSED(requestedSize)
    const QImage img = m_images.value(id);
    if (size)
        *size = img.size();
    return img;
}

void PreviewImageProvider::setImage(const QString &id, const QImage &image)
{
    m_images.insert(id, image);
}

void PreviewImageProvider::clear()
{
    m_images.clear();
}
