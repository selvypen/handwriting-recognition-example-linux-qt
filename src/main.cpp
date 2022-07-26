/*!
 *  @brief Handwriting Recognition Example
 *  @date 2022/07/20
 *  @file main.cpp
 *  @author SELVAS AI
 *
 *  Copyright 2022. SELVAS AI Inc. All Rights Reserved.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "writingrecognizer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    qmlRegisterSingletonType<WritingRecognizer>("WritingRecognizer", 1, 0, "WritingRecognizer", WritingRecognizer::registerWritingRecognizer);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
