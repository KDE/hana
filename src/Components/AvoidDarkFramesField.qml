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

    text: i18nc("@option:check", "Avoid dark frames")
    checked: HanaSettings.avoidDarkFrames

    onClicked: {
        HanaSettings.avoidDarkFrames = root.checked
        HanaSettings.save()
    }

    ToolTip {
        text: i18nc("@info:tooltip", "Checked: tries to avoid very dark frames by changing the time the thumbnail is taken<br>"+
                   "<b>Can be very slow</b><br><br>"+
                   "Unchecked: thumbnail is taken regardless of its content")
    }
}
