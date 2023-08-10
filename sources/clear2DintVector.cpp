#include <QVector>

void clear2DintVector(QVector<QVector<int>> *v)
{
    for (int i = 0; i < v->size(); i++)
        v[i].clear();
    v->clear();
}
