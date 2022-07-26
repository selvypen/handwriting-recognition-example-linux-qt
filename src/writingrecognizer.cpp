/*!
 *  @date 2022/07/20
 *  @file writingrecognizer.cpp
 *  @author SELVAS AI
 *
 *  Copyright 2022. SELVAS AI Inc. All Rights Reserved.
 */

#include <QDebug>
#include "writingrecognizer.h"

WritingRecognizer *WritingRecognizer::instance = nullptr;

WritingRecognizer::WritingRecognizer(QObject *parent) : QObject(parent)
{
    initialize();
}

WritingRecognizer::~WritingRecognizer()
{
    destroy();
}

WritingRecognizer *WritingRecognizer::getInstance()
{
    if (instance == nullptr) {
        instance = new WritingRecognizer();
    }
    return instance;
}

QObject *WritingRecognizer::registerWritingRecognizer(QQmlEngine *engine, QJSEngine *jsEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(jsEngine)
    return getInstance();
}

bool WritingRecognizer::initialize()
{
    quint32 status = DHWRCreate(LICENSE_PATH);
    if (inkObj == nullptr) {
        inkObj = DHWRCreateInkObject();
    }
    if (settingObj == nullptr) {
        settingObj = DHWRCreateSettingObject();
    }
    if (resultObj == nullptr) {
        resultObj = DHWRCreateResultObject();
    }
    DHWRSetExternalResourcePath(RESOURCE_PATH);
    DHWRSetExternalLibraryPath(LIBRARY_PATH);
    DHWRSetRecognitionMode(settingObj, MULTICHAR);
    DHWRSetCandidateSize(settingObj, MAX_CANDIDATES);
    setLanguage(DLANG_KOREAN);

    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to initialize:" << status;
        return false;
    }
}

bool WritingRecognizer::recognize()
{
    quint32 status = DHWRecognize(inkObj, resultObj);
    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to recognize:" << status;
        return false;
    }
}

QStringList WritingRecognizer::getCandidates()
{
    bool exit = false;
    QStringList candidates = QStringList();
    quint32 lineSize = DHWRGetLineSize(resultObj);
    if (lineSize == 0) {
        return candidates;
    }

    for (quint32 i = 0; i < MAX_CANDIDATES; i++) {
        for (quint32 j = 0; j < lineSize; j++) {
            DHWRResultLine line = DHWRGetLine(resultObj, j);
            quint32 blockSize = DHWRGetBlockSize(line);
            QString candidaeWords;
            for (quint32 k = 0; k < blockSize; k++) {
                DHWRResultBlock block = DHWRGetBlock(line, k);
                if (DHWRGetCandidateSize(block) <= i) {
                    exit = true;
                    break;
                }
                quint32 length = 0;
                candidaeWords.append(QString::fromWCharArray(DHWRGetCandidate(block, i, &length)));
                if (k + 1 < blockSize) {
                    candidaeWords.append(" ");
                }
            }
            candidates.append(candidaeWords);
            if (exit) {
                break;
            }
            if (j + 1 < lineSize) {
                candidates.append("\n");
            }
        }
        if (exit) {
            break;
        }
    }
    return candidates;
}

bool WritingRecognizer::setLanguage(qint32 language)
{
    DHWRClearLanguage(settingObj);

    if (language == DLANG_KOREAN) {
        DHWRAddLanguage(settingObj, DLANG_KOREAN, static_cast<DHWR_TYPE>(DTYPE_KOREAN | DTYPE_UPPERCASE | DTYPE_LOWERCASE));
    } else if (language == DLANG_ENGLISH) {
        DHWRAddLanguage(settingObj, DLANG_ENGLISH, static_cast<DHWR_TYPE>(DTYPE_UPPERCASE | DTYPE_LOWERCASE));
    } else if (language == DLANG_CHINA) {
        DHWRAddLanguage(settingObj, DLANG_CHINA, DTYPE_SIMP);
    } else if (language == DLANG_TAIWAN) {
        DHWRAddLanguage(settingObj, DLANG_TAIWAN, DTYPE_TRAD);
    } else if (language == DLANG_JAPANESE) {
        DHWRAddLanguage(settingObj, DLANG_JAPANESE, static_cast<DHWR_TYPE>(DTYPE_HIRAGANA | DTYPE_KATAKANA | DTYPE_KANJI));
    } else {
        qWarning() << "Invalid langauge:" << language;
    }

    quint32 status = DHWRSetAttribute(settingObj);
    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to set attribue:" << status;
        return false;
    }
}

bool WritingRecognizer::addPoint(const qint32 x, const qint32 y)
{
    quint32 status = DHWRAddPoint(inkObj, x, y);
    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to add point:" << status;
        return false;
    }
}

bool WritingRecognizer::endStroke()
{
    quint32 status = DHWREndStroke(inkObj);
    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to end stroke:" << status;
        return false;
    }
}

bool WritingRecognizer::clear()
{
    quint32 status = DHWRInkClear(inkObj);
    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to clear ink:" << status;
        return false;
    }
}

bool WritingRecognizer::destroy()
{
    if (inkObj != nullptr) {
        DHWRDestroyInkObject(inkObj);
        inkObj = nullptr;
    }
    if (settingObj != nullptr) {
        DHWRDestroySettingObject(settingObj);
        settingObj = nullptr;
    }
    if (resultObj != nullptr) {
        DHWRDestroyResultObject(resultObj);
        resultObj = nullptr;
    }

    quint32 status = DHWRClose();
    if (status == ERR_SUCCESS) {
        return true;
    } else {
        qCritical() << "Failed to destroy:" << status;
        return false;
    }
}

QString WritingRecognizer::getVersion()
{
    char *version = nullptr;
    quint32 status = DHWRGetRevision(&version);
    if (status == ERR_SUCCESS) {
        return QString::fromLatin1(version);
    } else {
        qCritical() << "Failed to get revision:" << status;
        return "unknown";
    }
}
