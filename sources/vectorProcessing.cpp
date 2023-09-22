#include <QVector>

void clear2DintVector(QList<QVector<int>> *v)
{
    for (int i = 0; i < v->size(); i++)
        v[i].clear();
    v->clear();
}

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

void resizeSeqLength(QList<QVector<double>> *v, int l)
{
    if (v->size() < l)
    {
        QVector<double> v0;
        while (v->size() < l)
            v->append(v0);
    }
    else if (v->size() > l)
    {
        while (v->size() > l)
            v->removeFirst();
    }
}

void resizeSeqLength(QVector<QVector<int>> *v, int l)
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

void resizeSeqLength(QVector<QVector<double>> *v, int l)
{
    if (v->size() < l)
    {
        QVector<double> v0;
        while (v->size() < l)
            v->append(v0);
    }
    else if (v->size() > l)
    {
        while (v->size() > l)
            v->removeFirst();
    }
}

bool channelToBeCalculated(int channel, int *channels, int nbrChannels)
{
    for (int i = 0; i < nbrChannels; i++)
        if (channel-1 == channels[i])
            return true;
    return false;
}
