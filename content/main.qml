import QtQuick 2.9
import QtQuick.Controls 1.5
import WritingRecognizer 1.0

ApplicationWindow {
    width: 800
    height: 480
    visible: true
    title: qsTr("Handwriting Recognition Example - " + WritingRecognizer.getVersion())

    property int buttonWidth: 120
    property int buttonHeight: 30

    Row {
        id: languageBar
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            leftMargin: 8
            rightMargin: 8
            topMargin: 20
        }
        spacing: 12

        readonly property int language_korean: 101
        readonly property int language_simplified_chinese: 102
        readonly property int language_traditional_chinese: 103
        readonly property int language_japanese: 105

        ExclusiveGroup {
            id: languageGroup
        }

        RadioButton {
            checked: true
            id: korean
            text: qsTr("Korean + English")
            exclusiveGroup: languageGroup
            onCheckedChanged: parent.handleCheckedChanged(checked, languageBar.language_korean)
        }
        RadioButton {
            id: simplifiedChinese
            text: qsTr("Simplified Chinese")
            exclusiveGroup: languageGroup
            onCheckedChanged: parent.handleCheckedChanged(checked, languageBar.language_simplified_chinese)

        }
        RadioButton {
            id: traditionalChinese
            text: qsTr("Traditional Chinese")
            exclusiveGroup: languageGroup
            onCheckedChanged: parent.handleCheckedChanged(checked, languageBar.language_traditional_chinese)
        }
        RadioButton {
            id: japanese
            text: qsTr("Japanese")
            exclusiveGroup: languageGroup
            onCheckedChanged: parent.handleCheckedChanged(checked, languageBar.language_japanese)
        }

        function handleCheckedChanged(checked, language) {
            if (checked) {
                WritingRecognizer.setLanguage(language)
            }
        }
    }

    Row {
        id: toolbar
        anchors {
            left: parent.left
            right: parent.right
            top: languageBar.bottom
            leftMargin: 8
            rightMargin: 8
            topMargin: 20
        }
        spacing: 8

        Button {
            id: recognize
            text: qsTr("Recognize")
            width: buttonWidth
            height: buttonHeight
            onClicked: {
                candidatesBar.clear()
                if (WritingRecognizer.recognize()) {
                    var candidateList = WritingRecognizer.getCandidates()
                    for (var i = 0; i < candidateList.length; i++) {
                        candidatesBar.append(candidateList[i])
                    }
                } else {
                    candidatesBar.append(qsTr("No result."))
                }
            }
        }
        Button {
            id: clear
            text: qsTr("Clear")
            width: buttonWidth
            height: buttonHeight
            onClicked: {
                canvas.clear()
                candidatesBar.clear()
                WritingRecognizer.clear()
            }
        }
    }

    Rectangle {
        id: candidatesBar
        height: 48
        color: "black"
        anchors {
            left: parent.left
            right: parent.right
            top: toolbar.bottom
            margins: 8
        }

        ListModel {
            id: candidateModel
        }
        ListView {
            id: candidateListView
            anchors.fill: parent
            model: candidateModel
            delegate: candidatesDelegate
            orientation: ListView.Horizontal

            Component {
                id: candidatesDelegate
                Rectangle {
                    color: "transparent"
                    height: candidateListView.height
                    width: candidateText.paintedWidth + 20
                    Text {
                        id: candidateText
                        text: candidate
                        color: "white"
                        font.pixelSize: 20
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        anchors.margins: 8
                    }
                }
            }
        }

        function clear() {
            candidateModel.clear()
        }

        function append(text) {
            candidateModel.append({ candidate: text })
        }
    }

    Rectangle {
        id: writingArea
        color: "#fafad2"
        border.color: "#888888"
        border.width: 1

        anchors {
            left: parent.left
            right: parent.right
            top: candidatesBar.bottom
            bottom: parent.bottom
            margins: 8
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("Write here.")
            color: "#aaaaaa"
            font.pointSize: 12
        }

        Canvas {
            id: canvas
            anchors.fill: parent
            property point lastPoint
            property color backgroundColor: "transparent"
            property color lineColor: "black"
            property real lineWidth: 1.5

            onPaint: {
                drawLine()
            }

            MouseArea {
                id: area
                anchors.fill: parent
                onPressed: {
                    canvas.lastPoint.x = mouseX
                    canvas.lastPoint.y = mouseY
                }
                onPositionChanged: {
                    canvas.requestPaint()
                    WritingRecognizer.addPoint(mouseX, mouseY)
                }
                onReleased: {
                    WritingRecognizer.endStroke()
                }
            }

            function clear() {
                var ctx = getContext('2d')
                ctx.reset()
                canvas.requestPaint()
            }

            function drawLine() {
                var ctx = getContext('2d')
                ctx.strokeStyle = lineColor
                ctx.lineWidth = lineWidth
                ctx.beginPath()
                ctx.moveTo(lastPoint.x, lastPoint.y)
                ctx.lineTo(area.mouseX, area.mouseY)
                ctx.stroke()
                ctx.closePath()
                lastPoint.x = area.mouseX
                lastPoint.y = area.mouseY
            }
        }
    }
}
