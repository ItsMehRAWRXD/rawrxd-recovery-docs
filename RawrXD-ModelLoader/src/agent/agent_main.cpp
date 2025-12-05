#include "planner.hpp"
#include "self_patch.hpp"
#include "release_agent.hpp"
#include "meta_learn.hpp"
#include "self_test_gate.hpp"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QProcess>
#include <QStringList>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    app.setApplicationName("RawrXD-Agent");
    app.setApplicationVersion("1.0.0");
    
    QCommandLineParser parser;
    parser.setApplicationDescription("RawrXD Autonomous Agent - Zero-touch IDE automation");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("wish", "Natural language wish, e.g. 'Add Q8_K kernel'");
    parser.process(app);
    
    QString wish = parser.positionalArguments().value(0);
    
    if (wish.isEmpty()) {
        qCritical() << "No wish provided. Usage: RawrXD-Agent.exe \"Add Q8_K kernel\"";
        return 1;
    }
    
    qDebug() << "Agent wish:" << wish;
    
    // Step 1: Plan
    Planner planner;
    QJsonArray tasks = planner.plan(wish);
    
    if (tasks.isEmpty()) {
        qCritical() << "Failed to generate plan for:" << wish;
        return 1;
    }
    
    qDebug() << "Generated" << tasks.size() << "tasks";
    
    // Step 2: Execute
    SelfPatch patch;
    ReleaseAgent rel;
    MetaLearn ml;
    
    bool success = true;
    
    for (const QJsonValue& job : tasks) {
        QJsonObject task = job.toObject();
        QString type = task["type"].toString();
        
        qDebug() << "Executing:" << type;
        
        if (type == "add_kernel") {
            success = patch.addKernel(task["target"].toString(), task["template"].toString());
        } else if (type == "add_cpp") {
            QString deps;
            if (task["deps"].isArray()) {
                QStringList parts;
                for (const QJsonValue& val : task["deps"].toArray())
                    parts << val.toString();
                deps = parts.join(",");
            } else {
                deps = task["deps"].toString();
            }
            success = patch.addCpp(task["target"].toString(), deps);
        } else if (type == "build") {
            QString target = task.value("target").toString();
            QStringList args = {"--build", "build", "--config", "Release"};
            if (!target.isEmpty()) {
                args << "--target" << target;
            }
            int rc = QProcess::execute("cmake", args);
            success = (rc == 0);
        } else if (type == "hot_reload") {
            success = patch.hotReload();
        } else if (type == "bump_version") {
            success = rel.bumpVersion(task["part"].toString());
        } else if (type == "tag") {
            success = rel.tagAndUpload();
        } else if (type == "tweet") {
            success = rel.tweet(task["text"].toString());
        } else if (type == "meta_learn") {
            success = ml.record(
                task.value("quant").toString(),
                task.value("kernel").toString(),
                task.value("gpu").toString(),
                task.value("tps").toDouble(),
                task.value("ppl").toDouble()
            );
        } else if (type == "bench" || type == "bench_all") {
            qDebug() << "Benchmark (handled by build)";
        } else if (type == "self_test") {
            qInfo() << "Running self-test gate...";
            success = runSelfTestGate();
            if (success) {
                qInfo() << "Self-test gate PASSED";
            } else {
                qCritical() << "Self-test gate FAILED";
            }
        }
        
        if (!success) {
            qCritical() << "Task failed:" << type;
            return 1;
        }
    }
    
    QString suggested = ml.suggestQuant();
    qDebug() << "Meta-learn suggests quant:" << suggested;
    qDebug() << "Agent completed successfully!";
    return 0;
}
