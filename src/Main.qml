/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.ki18n
import org.kde.kirigami as Kirigami

import org.kde.hana.components

ApplicationWindow {
    width: 800
    minimumWidth: 600
    height: 600
    minimumHeight: 300
    visible: true
    title: "Hana"

    footer: Footer {}

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: Kirigami.Units.largeSpacing

            FilesView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            Layout.margins: Kirigami.Units.largeSpacing

            GridLayout {
                id: scrollLayout

                columns: 2

                ImageSaveLocationField {
                    Layout.columnSpan: 2
                }

                Kirigami.Heading {
                    text: KI18n.i18nc("@label", "Thumbnail settings")
                    level: 2
                    Layout.columnSpan: 2
                    Layout.topMargin: Kirigami.Units.largeSpacing
                }
                ColumnsField {}
                RowsField {}
                SpacingField {}
                ThumbWidthField {}
                BackgroundField {
                    Layout.columnSpan: 2
                }
                AvoidDarkFramesField {
                    Layout.columnSpan: 2
                }
                ShowThumbTimeField {
                    Layout.columnSpan: 2
                }

                Kirigami.Heading {
                    text: KI18n.i18nc("@label", "Video info settings")
                    level: 2
                    Layout.columnSpan: 2
                    Layout.topMargin: Kirigami.Units.largeSpacing
                }
                ShowVideoInfoField {
                    Layout.columnSpan: 2
                }
                VideoInfoAlwaysShowHoursField {
                    Layout.columnSpan: 2
                }
                VideoInfoBackgroundColorField {
                    Layout.columnSpan: 2
                }
                VideoInfoTextColorField {
                    Layout.columnSpan: 2
                }
            }

        }
    }
}
