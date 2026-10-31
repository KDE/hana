/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls

import org.kde.ki18n

import org.kde.hana

ToolButton {
    id: root

    text: KI18n.i18nc("@action:button; Empty the list of files. The files are not deleted from disk", "Clear files")

    onClicked: Bridge.clearFiles()
}
