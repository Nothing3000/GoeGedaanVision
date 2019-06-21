#ifndef QBWIMAGE_H
#define QBWIMAGE_H

#include <QImage>
#include <QVector>
#include <QLinkedList>
#include <QPoint>

class QBWImage : public QImage
{
public:
    typedef QPoint Coord;
    typedef QLinkedList<Coord> ObjectCoords;
    typedef QVector<ObjectCoords> ObjectsVector;
    enum findType
    {
        top,
        left,
        right,
        bottom
    };

public:
    QBWImage();
    QBWImage(int width, int height);
    QBWImage(int width, int height, ObjectCoords object);
    QBWImage(int width, int height, ObjectsVector objects);
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
    ObjectsVector conncomp() const;
    QBWImage areaopen(int size) const;
    QBWImage operator~() const;
    QRect boundingBox() const;
    static QRect boundingBox(const ObjectCoords& object);
    Coord find(findType type) const;
    QBWImage cleanBorder() const;
    QBWImage copy() const;
    QBWImage copy(const QRect& rect) const;
    double corr2(const QBWImage& compareImage) const;
    int mean2() const;
private:
    void floodFillAdd(Coord Coord,ObjectsVector& result, QImage& labelImage) const;
    void floodFillRemove(Coord coord);
    bool erosionCheck(int x, int y, int size) const;
    bool dilationCheck(int x, int y, int size) const;
    static void cleanupImage(void *info);
};

#endif // QBWIMAGE_H
