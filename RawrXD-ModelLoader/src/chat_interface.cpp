// Chat Interface - Chat UI component
#include "chat_interface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>
#include <QFileInfo>

ChatInterface::ChatInterface(QWidget* parent) : QWidget(parent), maxMode_(false) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Header with title
    QLabel* title = new QLabel("Agent Chat", this);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(title);
    
    // Model selector row
    QHBoxLayout* modelLayout = new QHBoxLayout();
    
    QLabel* modelLabel = new QLabel("Model:", this);
    modelLayout->addWidget(modelLabel);
    
    modelSelector_ = new QComboBox(this);
    modelSelector_->setMinimumWidth(200);
    modelSelector_->addItem("No Model Selected");
    loadAvailableModels();
    connect(modelSelector_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChatInterface::onModelChanged);
    modelLayout->addWidget(modelSelector_);
    
    modelLayout->addStretch();
    
    // Max Mode toggle
    maxModeToggle_ = new QCheckBox("Max Mode", this);
    maxModeToggle_->setToolTip("Enable maximum context and response length");
    connect(maxModeToggle_, &QCheckBox::toggled, this, &ChatInterface::onMaxModeToggled);
    modelLayout->addWidget(maxModeToggle_);
    
    // Refresh models button
    QPushButton* refreshBtn = new QPushButton("🔄", this);
    refreshBtn->setMaximumWidth(30);
    refreshBtn->setToolTip("Refresh model list");
    connect(refreshBtn, &QPushButton::clicked, this, &ChatInterface::refreshModels);
    modelLayout->addWidget(refreshBtn);
    
    layout->addLayout(modelLayout);
    
    // Message history
    message_history_ = new QTextEdit(this);
    message_history_->setReadOnly(true);
    message_history_->setStyleSheet(
        "QTextEdit { background-color: #1e1e1e; color: #d4d4d4; border: 1px solid #3c3c3c; }");
    layout->addWidget(message_history_);
    
    // Input area
    QHBoxLayout* inputLayout = new QHBoxLayout();
    message_input_ = new QLineEdit(this);
    message_input_->setPlaceholderText("Type your message here...");
    message_input_->setStyleSheet(
        "QLineEdit { background-color: #252526; color: #d4d4d4; border: 1px solid #3c3c3c; padding: 5px; }");
    connect(message_input_, &QLineEdit::returnPressed, this, &ChatInterface::sendMessage);
    
    QPushButton* sendButton = new QPushButton("Send", this);
    sendButton->setStyleSheet(
        "QPushButton { background-color: #0e639c; color: white; padding: 5px 15px; border: none; }"
        "QPushButton:hover { background-color: #1177bb; }");
    connect(sendButton, &QPushButton::clicked, this, &ChatInterface::sendMessage);
    
    inputLayout->addWidget(message_input_);
    inputLayout->addWidget(sendButton);
    layout->addLayout(inputLayout);
    
    // Status label
    statusLabel_ = new QLabel("Ready", this);
    statusLabel_->setStyleSheet("color: #888888; font-size: 11px;");
    layout->addWidget(statusLabel_);
}

void ChatInterface::loadAvailableModels() {
    // Check common GGUF model locations
    QStringList searchPaths = {
        "D:/OllamaModels",
        QDir::homePath() + "/.ollama/models",
        QDir::homePath() + "/models",
        "C:/models",
        "./models"
    };
    
    for (const QString& path : searchPaths) {
        QDir dir(path);
        if (dir.exists()) {
            QStringList filters;
            filters << "*.gguf";
            QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
            for (const QFileInfo& file : files) {
                modelSelector_->addItem(file.fileName(), file.absoluteFilePath());
            }
        }
    }
    
    if (modelSelector_->count() == 1) {
        statusLabel_->setText("No GGUF models found. Add models to D:/OllamaModels or ~/models");
    }
}

void ChatInterface::refreshModels() {
    QString currentModel = modelSelector_->currentData().toString();
    modelSelector_->clear();
    modelSelector_->addItem("No Model Selected");
    loadAvailableModels();
    
    // Try to restore previous selection
    int idx = modelSelector_->findData(currentModel);
    if (idx >= 0) {
        modelSelector_->setCurrentIndex(idx);
    }
    
    statusLabel_->setText("Model list refreshed");
}

void ChatInterface::onModelChanged(int index) {
    if (index > 0) {
        QString modelPath = modelSelector_->currentData().toString();
        QString modelName = modelSelector_->currentText();
        statusLabel_->setText("Selected: " + modelName);
        emit modelSelected(modelPath);
    } else {
        statusLabel_->setText("No model selected");
    }
}

void ChatInterface::onMaxModeToggled(bool enabled) {
    maxMode_ = enabled;
    if (enabled) {
        statusLabel_->setText("Max Mode enabled - Extended context and responses");
    } else {
        statusLabel_->setText("Standard mode");
    }
    emit maxModeChanged(enabled);
}

QString ChatInterface::selectedModel() const {
    return modelSelector_->currentData().toString();
}

bool ChatInterface::isMaxMode() const {
    return maxMode_;
}

void ChatInterface::addMessage(const QString& sender, const QString& message) {
    QString color = (sender == "User") ? "#569cd6" : "#4ec9b0";
    message_history_->append("<span style='color:" + color + ";font-weight:bold;'>" + sender + ":</span> " + message);
}

void ChatInterface::displayResponse(const QString& response) {
    addMessage("Agent", response);
    statusLabel_->setText("Response received");
}

void ChatInterface::focusInput() {
    message_input_->setFocus();
}

void ChatInterface::sendMessage() {
    QString message = message_input_->text().trimmed();
    if (!message.isEmpty()) {
        addMessage("User", message);
        statusLabel_->setText("Processing...");
        emit messageSent(message);
        message_input_->clear();
    }
}
