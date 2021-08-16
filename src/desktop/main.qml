import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

import FbSpy 1.0

Window {
    id: root

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

        QtObject {
            id: priv

            function updateTextureSize() {
                fbView.textureSize = Qt.size(spinboxWidth.value, spinboxHeight.value)
            }
        }

        FbFileSpy {
            id: fbSpy

            running: true

            onVirtualSizeChanged: {
                spinboxWidth.value = virtualSize.width
                spinboxHeight.value = virtualSize.height
            }
        }

        Flickable {
            anchors.fill: parent
            contentWidth: fbView.width * fbView.scale
            contentHeight: fbView.height * fbView.scale

            FbView {
                id: fbView

                anchors.centerIn: parent
                spy: fbSpy

                PinchArea {
                    property real lastScale: 1.0

                    function clamp(min, value, max) {
                        return Math.max(min, Math.min(value, max))
                    }

                    anchors.fill: parent

                    onPinchStarted: lastScale = fbView.scale
                    onPinchUpdated: {
                        fbView.scale = clamp(0.3, lastScale * pinch.scale, 4.0)
                    }

                   MouseArea {
                       anchors.fill: parent

                       onDoubleClicked: {
                            fbView.scale = 1.0
                        }
                    }
                }
            }
        }

        Column {
            anchors.bottom: parent.bottom
            width: parent.width

            Row {
                SpinBox {
                    id: spinboxWidth

                    value: to
                    from: 0
                    to: fbSpy.virtualSize.width
                    stepSize: 1

                    onValueChanged: priv.updateTextureSize()
                }

                Label {
                    text: "Width"
                }
            }

            Row {
                SpinBox {
                    id: spinboxHeight

                    value: to / 2
                    from: 0
                    to: fbSpy.virtualSize.height
                    stepSize: 1

                    onValueChanged: priv.updateTextureSize()
                }

                Label {
                    text: "Width"
                }
            }
        }
}

