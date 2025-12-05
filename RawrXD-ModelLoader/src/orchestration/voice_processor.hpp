#pragma once

#include <QString>
#include <QJsonObject>
#include <QObject>

/**
 * @class VoiceProcessor
 * @brief Handles speech-to-text, intent recognition, and text-to-speech
 * 
 * Provides:
 * - Audio input capture and transcription
 * - Intent detection from voice commands
 * - Plan generation from voice input
 * - Text-to-speech feedback
 */
class VoiceProcessor : public QObject {
    Q_OBJECT

public:
    explicit VoiceProcessor(QObject* parent = nullptr);
    ~VoiceProcessor() = default;

    // TODO: Implement in Phase 3
    // Placeholder for now
};
