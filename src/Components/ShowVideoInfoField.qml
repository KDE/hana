/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls

import org.kde.hana

CheckBox {
    id: root

    text: i18nc("@option:check", "Show video info")
    checked: HanaSettings.showVideoInfo

    onClicked: {
        HanaSettings.showVideoInfo = root.checked
        HanaSettings.save()
    }
}
