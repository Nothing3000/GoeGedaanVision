#ifndef QBWIMAGE_H
#define QBWIMAGE_H

#include <QImage>
#include <QVector>
#include <QLinkedList>
#include <QPoint>

class QBWImage : public QImage
{
public:
    typedef QPoint coord;
    typedef QLinkedList<coord> objectCoords;
    typedef QVector<objectCoords> objectsVector;
public:
    QBWImage();
    QBWImage(int width, int height);
    QBWImage(int width, int height, objectCoords object);
    QBWImage(int width, int height, objectsVector objects);
    QBWImage(const QBWImage&) = delete;
    QBWImage& operator=(const QBWImage& qBWImage) = delete;
    QBWImage(QBWImage&& qBWImage) = default;
    QBWImage& operator=(QBWImage&& qBWImage) = default;
    virtual ~QBWImage();
public:
    QBWImage erode(int size = 3) const;
    QBWImage dilate(int size = 3) const;
    QBWImage open(int size = 3) const;
    QBWImage close(int size = 3) const;
    objectsVector conncomp() const;
    QBWImage areaopen(int size) const;
    QBWImage operator~() const;
private:
    void floodFillAdd(coord coord,objectsVector& result, QImage& labelImage) const;
    bool erosionCheck(int x, int y, int size) const;
    bool dilationCheck(int x, int y, int size) const;
    static void cleanupImage(void *info);
};

#endif // QBWIMAGE_H
