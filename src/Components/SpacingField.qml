/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.ki18n

import org.kde.hana

ColumnLayout {
    id: root

    Label {
        Layout.maximumWidth: 250

        text: KI18n.i18nc("@label", "Spacing")
        elide: Text.ElideRight
    }

    SpinBox {
        id: control

        from: 0
        to: 9000
        value: HanaSettings.thumbnailsSpacing
        stepSize: 5

        onValueModified: {
            HanaSettings.thumbnailsSpacing = value
            HanaSettings.save()
        }
    }
}
