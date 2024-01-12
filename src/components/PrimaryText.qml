import QtQuick 2.15
import QtQuick.Controls.Material 6.5

Text {
    color: Material.primaryTextColor
    font.family: globalFont.font.family
    wrapMode: Text.Wrap
    font.pixelSize: Math.max(14,
                            Math.min(20,
                                    8 + root.width / 100))
}
