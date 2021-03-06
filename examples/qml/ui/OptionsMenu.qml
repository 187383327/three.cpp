import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

Rectangle {
    id: main
    color: "darkgray"

    property string title
    property color textColor: "white"
    default property list<QtObject> properties

    property list<Item> controls
    property list<MenuChoice> menuChoices

    Item {
        id: titleRow
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30

        Label {
            anchors.centerIn: parent
            text: title
            font.bold: true
            color: main.textColor
        }
    }

    Component {
        id: bool_control
        Item {
            property Item prev
            property alias labelWidth: boolControlLabel.width
            anchors.top: prev.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            implicitHeight: 40

            property QtObject prop
            property QtObject target

            function reset() {
                bool_check.checked = target.value
            }
            Row {
                anchors.fill: parent
                spacing: 5

                Label {
                    id: boolControlLabel
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    width: parent.width - bool_check.implicitWidth - 5
                    text: !!prop.label ? prop.label : prop.name
                    color: prop.textColor ? prop.textColor : main.textColor
                    font.bold: true
                }
                Switch {
                    id: bool_check
                    width: implicitWidth
                    checked: target.value

                    onCheckedChanged: {
                        target.value = checked
                    }
                }
            }
        }
    }

    Component {
        id: listChoiceFactory
        Item {
            property Item prev
            property alias labelWidth: listChoiceLabel.width
            anchors.top: prev.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            implicitHeight: 40

            property QtObject prop
            property QtObject target

            function reset() {
                listControl.currentIndex = -1
            }
            Row {
                anchors.fill: parent
                spacing: 5
                padding: 2

                Label {
                    id: listChoiceLabel
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    text: !!prop.label ? prop.label : prop.name
                    color: prop.textColor ? prop.textColor : main.textColor
                    font.bold: true
                }
                ComboBox {
                    id: listControl
                    width: Math.max(implicitWidth, parent.width - listChoiceLabel.width - 5)
                    model: !!target.names ? target.names : target.values
                    currentIndex: target.selectedIndex

                    onCurrentIndexChanged: {
                        target.selectedIndex = currentIndex
                    }
                }
            }
        }
    }

    Component {
        id: menuChoiceFactory
        Rectangle {
            property Item prev
            property alias labelWidth: menuLabel.width
            anchors.top: prev.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            implicitHeight: 30

            property QtObject prop
            property QtObject target

            color: target.value ? target.selectedColor : target.color

            function reset() {
                color = target.color
            }
            Rectangle {
                border.width: 0.5
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 0.5
            }
            Label {
                id: menuLabel
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: prop.name
                color: prop.textColor ? prop.textColor : main.textColor

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        for(var i=0; i<menuChoices.length; i++) {
                            menuChoices[i].target.value = (menuChoices[i].target === target)
                        }
                    }
                }
            }
        }
    }

    Component {
        id: floatControlFactory
        Item {
            property alias labelWidth: floatControlLabel.width
            property Item prev
            anchors.top: prev.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            implicitHeight: 40

            property QtObject prop
            property alias from: floatSlider.from
            property alias to: floatSlider.to
            property QtObject target

            function reset() {
                floatSlider.value = target.value
            }
            Row {
                anchors.fill: parent
                spacing: 5

                Label {
                    id: floatControlLabel
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    text: prop.label ? prop.label : prop.name
                    font.bold: true
                    color: prop.textColor ? prop.textColor : main.textColor
                }
                Slider {
                    id: floatSlider
                    Layout.fillWidth: true
                    from: from
                    to: to
                    value: target.value

                    property int precision: 0

                    onValueChanged: {
                        target.value = value
                    }
                    handle: Rectangle {
                        x: floatSlider.leftPadding + floatSlider.visualPosition * (floatSlider.availableWidth - width)
                        y: floatSlider.topPadding + floatSlider.availableHeight / 2 - height / 2
                        implicitWidth: 60
                        implicitHeight: 30
                        radius: 20
                        color: floatSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                        border.color: "#bdbebf"

                        Label {
                            anchors.centerIn: parent
                            text: floatSlider.value.toFixed(floatSlider.precision)
                        }
                    }
                    onFromChanged: precision = from > 1 && to > from * 10 ? 0 : 2
                    onToChanged: precision = from > 1 && to > from * 10 ? 0 : 2
                }
            }
        }
    }

    function reset()
    {
        for(var index=0; index < properties.length; index++) {
            properties[index].reset()
        }
        for(var index=0; index < controls.length; index++) {
            controls[index].reset()
        }
    }

    FontMetrics {
        id: fontMetric
    }

    Component.onCompleted: {
        var height = 10 + titleRow.height
        var prev = titleRow

        var maxWidth = 0
        for(var index=0; index < properties.length; index++) {
            var prop = properties[index]
            maxWidth = Math.max(maxWidth, fontMetric.averageCharacterWidth * (prop.label ? prop.label.length : prop.name.length))
        }
        maxWidth += fontMetric.averageCharacterWidth * 3

        for(var index=0; index < properties.length; index++) {
            var prop = properties[index]
            if(prop.type === "float") {
                prev = floatControlFactory.createObject(main, {"labelWidth": maxWidth, "anchors.top": prev.bottom,
                                                      prop: prop, from: prop.from, to: prop.to, target: prop})
                controls.push(prev)
            }
            else if(prop.type === "bool") {
                prev = bool_control.createObject(main, {"labelWidth": maxWidth,
                                                        "anchors.top": prev.bottom, prop: prop, target: prop})
                controls.push(prev)
            }
            else if(prop.type === "list") {
                prev = listChoiceFactory.createObject(main, {"labelWidth": maxWidth,
                                                        "anchors.top": prev.bottom, prop: prop, target: prop})
                menuChoices.push(prev)
            }
            else if(prop.type === "menuchoice") {
                prev = menuChoiceFactory.createObject(main, {"labelWidth": maxWidth,
                                                        "anchors.top": prev.bottom, prop: prop, target: prop})
                menuChoices.push(prev)
            }
            height += prev.implicitHeight
        }
        main.height = height
    }
}
