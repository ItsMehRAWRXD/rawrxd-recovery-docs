#pragma once
#include <cstdint>
#include <cstdlib>
#include <QtCore/QByteArray>

extern "C" {
// Brutal MASM deflate (stored blocks only)
// Returns heap buffer (must free) or nullptr on failure
std::uint8_t* deflate_brutal_masm(const std::uint8_t* src,
                                  std::uint64_t       len,
                                  std::uint64_t*      out_len);
}

namespace brutal {

/**
 * @brief Compress QByteArray using brutal MASM stored-block gzip
 * @param in Raw input data
 * @return Compressed gzip stream (RFC 1952 compliant)
 * 
 * Ultra-fast, deterministic-size gzip compression using only stored blocks.
 * No Huffman, no LZ77 – pure memcpy speed with gzip framing.
 * Perfect for GGUF tensor caching, streaming inference, or speed-critical paths.
 */
inline QByteArray compress(const QByteArray& in)
{
    if (in.isEmpty()) return {};
    
    std::uint64_t packedSz = 0;
    std::uint8_t* p = deflate_brutal_masm(
        reinterpret_cast<const std::uint8_t*>(in.constData()),
        static_cast<std::uint64_t>(in.size()),
        &packedSz
    );
    
    if (!p) return {};  // malloc failure
    
    QByteArray out(reinterpret_cast<const char*>(p), static_cast<int>(packedSz));
    std::free(p);
    return out;
}

/**
 * @brief Compress raw buffer using brutal MASM stored-block gzip
 * @param data Raw input pointer
 * @param size Input size in bytes
 * @return Compressed gzip stream (RFC 1952 compliant)
 */
inline QByteArray compress(const void* data, std::size_t size)
{
    if (!data || size == 0) return {};
    
    std::uint64_t packedSz = 0;
    std::uint8_t* p = deflate_brutal_masm(
        static_cast<const std::uint8_t*>(data),
        static_cast<std::uint64_t>(size),
        &packedSz
    );
    
    if (!p) return {};
    
    QByteArray out(reinterpret_cast<const char*>(p), static_cast<int>(packedSz));
    std::free(p);
    return out;
}

/**
 * @brief Calculate worst-case compressed size for planning/allocation
 * @param rawSize Input size
 * @return Maximum possible compressed size (gzip header + stored blocks + footer)
 * 
 * Formula: header(10) + ceil(rawSize/65535)*5 + rawSize + footer(8)
 */
inline std::size_t maxCompressedSize(std::size_t rawSize)
{
    std::size_t blockCount = (rawSize + 65534) / 65535;
    return 10 + (blockCount * 5) + rawSize + 8;
}

} // namespace brutal
