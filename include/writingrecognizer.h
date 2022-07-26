/*!
 *  @date 2022/07/20
 *  @file writingrecognizer.h
 *  @author SELVAS AI
 *
 *  Copyright 2022. SELVAS AI Inc. All Rights Reserved.
 */

#ifndef WRITING_RECOGNIZER_H
#define WRITING_RECOGNIZER_H

#include <QObject>
#include <QQmlEngine>
#include "dhwr.h"

class WritingRecognizer : public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(WritingRecognizer)
    explicit WritingRecognizer(QObject *parent = nullptr);
    virtual ~WritingRecognizer() override;
    bool initialize();
    bool destroy();

    static WritingRecognizer *instance;
    DHWRInkObject inkObj = nullptr;
    DHWRResultObject resultObj = nullptr;
    DHWRSettingObject settingObj = nullptr;
    const quint32 MAX_CANDIDATES = 10;

public:
    static WritingRecognizer *getInstance();
    static QObject *registerWritingRecognizer(QQmlEngine *engine, QJSEngine *jsEngine);

    Q_INVOKABLE bool recognize();
    Q_INVOKABLE QStringList getCandidates();
    Q_INVOKABLE bool setLanguage(qint32 language);
    Q_INVOKABLE bool addPoint(qint32 x, qint32 y);
    Q_INVOKABLE bool endStroke();
    Q_INVOKABLE bool clear();
    Q_INVOKABLE QString getVersion();
};

#endif // WRITING_RECOGNIZER_H
