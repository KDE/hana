/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls

ToolButton {
    id: root

    text: i18nc("@action:button", "Close")

    onClicked: Qt.quit()
}
