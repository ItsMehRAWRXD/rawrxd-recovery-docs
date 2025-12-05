#pragma once

#include <QWidget>
#include <QString>

class QTabWidget;

class MultiTabEditor : public QWidget {
    Q_OBJECT
public:
    explicit MultiTabEditor(QWidget* parent = nullptr);
    
public slots:
    void openFile(const QString& filepath);
    void newFile();
    void saveCurrentFile();
    void undo();
    void redo();
    void find();
    void replace();
    
    QString getCurrentText() const;
    
private:
    QTabWidget* tab_widget_;
};
