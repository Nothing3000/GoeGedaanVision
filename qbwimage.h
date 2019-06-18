#ifndef QBWIMAGE_H
#define QBWIMAGE_H

#include <QImage>
#include <QVector>

class QBWImage : public QImage
{
public:
    struct concompResult
    {
        int numObjects;
        QVector<QSet<int>> pixelList;
    };
public:
    QBWImage();
    QBWImage(int width, int height);
    QBWImage(const QBWImage&) = delete;
    QBWImage& operator=(const QBWImage& qBWImage) = delete;
    QBWImage(QBWImage&& qBWImage) = default;
    QBWImage& operator=(QBWImage&& qBWImage) = default;
    virtual ~QBWImage();
public:
    QBWImage erode(int size = 3);
    QBWImage dilate(int size = 3);
    QBWImage open(int size = 3);
    QBWImage close(int size = 3);
    concompResult concomp();
private:
    bool erosionCheck(int width, int height, int size);
    bool dilationCheck(int width, int height, int size);
    static void cleanupImage(void *info);
private:
    QVector<uchar> *imageData;
};

#endif // QBWIMAGE_H
