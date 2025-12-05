#include "gguf_loader.hpp"
#include <cstring>
#include <QDebug>

GGUFLoader::GGUFLoader(const QString& path)
{
    file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open GGUF file:" << path;
        return;
    }
    
    QDataStream ds(&file);
    ds.setByteOrder(QDataStream::LittleEndian);
    
    ds.readRawData(head.magic, 4);
    if (memcmp(head.magic, "GGUF", 4) != 0) {
        qWarning() << "Invalid GGUF magic:" << QByteArray(head.magic, 4).toHex();
        file.close();
        return;
    }
    
    ds >> head.version >> head.tensorCount >> head.metadataSize;
    qDebug() << "GGUF version:" << head.version << "tensors:" << head.tensorCount;

    /* Simplified offset map - replace with real GGUF parser */
    for (quint64 i = 0; i < head.tensorCount; ++i) {
        quint32 nameLen;
        ds >> nameLen;
        if (nameLen > 1024) { // Sanity check
            qWarning() << "Suspicious tensor name length:" << nameLen;
            break;
        }
        QByteArray name(nameLen, Qt::Uninitialized);
        ds.readRawData(name.data(), nameLen);
        quint64 offset;
        ds >> offset;
        offsetMap[QString::fromUtf8(name)] = offset;
    }
    qDebug() << "Loaded" << offsetMap.size() << "tensor offsets";
}

GGUFLoader::~GGUFLoader() = default;

QByteArray GGUFLoader::inflateWeight(const QString& tensor)
{
    auto it = offsetMap.constFind(tensor);
    if (it == offsetMap.constEnd()) {
        qWarning() << "Tensor not found:" << tensor;
        return {};
    }
    
    file.seek(*it);
    quint32 packedSz;
    file.read(reinterpret_cast<char*>(&packedSz), 4);
    
    QByteArray packed = file.read(packedSz);
    if (packed.size() != static_cast<int>(packedSz)) {
        qWarning() << "Read mismatch: expected" << packedSz << "got" << packed.size();
        return {};
    }
    
    // For now, return the raw data (add inflate later if compressed)
    return packed;
}
