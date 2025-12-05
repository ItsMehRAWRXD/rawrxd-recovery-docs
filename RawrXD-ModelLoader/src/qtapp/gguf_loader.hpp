#pragma once
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QSharedMemory>
#include <QHash>
#include <QString>
#include <QStringList>

struct GGUFHeader {
    char magic[4];      // "GGUF"
    quint32 version;
    quint64 tensorCount;
    quint64 metadataSize;
};

struct GGUFLoader {
    explicit GGUFLoader(const QString& path);
    ~GGUFLoader();
    bool  isOpen() const { return file.isOpen(); }
    QByteArray inflateWeight(const QString& tensorName);
    QStringList tensorNames() const { return offsetMap.keys(); }

private:
    QFile file;
    GGUFHeader head{};
    QSharedMemory shm;          // holds the *inflated* blob
    QHash<QString, quint64> offsetMap; // tensor → file offset
};
