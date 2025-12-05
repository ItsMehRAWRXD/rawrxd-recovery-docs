#pragma once

#include <QString>
#include <QJsonObject>
#include <QObject>

/**
 * @class SemanticDiffAnalyzer
 * @brief Analyzes git diffs with AI to detect semantic changes and breaking changes
 * 
 * Provides:
 * - Semantic diff analysis (not just line-by-line)
 * - Breaking change detection
 * - Impact analysis
 * - Diff context enrichment with AI insights
 */
class SemanticDiffAnalyzer : public QObject {
    Q_OBJECT

public:
    explicit SemanticDiffAnalyzer(QObject* parent = nullptr);
    ~SemanticDiffAnalyzer() = default;

    // TODO: Implement in Phase 5
    // Placeholder for now
};
