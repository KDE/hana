/*
 * SPDX-FileCopyrightText: 2026 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls

import org.kde.hana

CheckBox {
    id: root

    text: i18nc("@option:check", "Show thumbnails time")
    checked: HanaSettings.showThumbnailTime

    onClicked: {
        HanaSettings.showThumbnailTime = root.checked
        HanaSettings.save()
    }
}