#pragma once

#include <QString>
#include <QJsonObject>
#include <QObject>

/**
 * @class SandboxedTerminal
 * @brief Terminal wrapper with process isolation and command filtering
 * 
 * Provides:
 * - Process isolation and sandboxing
 * - Command filtering and whitelisting
 * - Output filtering
 * - Security auditing
 */
class SandboxedTerminal : public QObject {
    Q_OBJECT

public:
    explicit SandboxedTerminal(QObject* parent = nullptr);
    ~SandboxedTerminal() = default;

    // TODO: Implement in Phase 6
    // Placeholder for now
};
