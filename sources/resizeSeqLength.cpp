#include <QVector>

void resizeSeqLength(QList<QVector<int>> *v, int l)
{
    if (v->size() < l)
    {
        QVector<int> v0;
        while (v->size() < l)
            v->append(v0);
    }
    else if (v->size() > l)
    {
        while (v->size() > l)
            v->removeFirst();
    }
}
