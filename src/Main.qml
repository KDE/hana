/*
 * SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
            Layout.margins: 20

            FilesView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            Layout.margins: 20

            GridLayout {
                id: scrollLayout

                columns: 2

                ImageSaveLocationField {
                    Layout.columnSpan: 2
                }

                Label {
                    text: i18nc("@label", "Thumbnail settings")
                    font.pointSize: 14
                    Layout.columnSpan: 2
                    Layout.topMargin: 20
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

                Label {
                    text: i18nc("@label", "Video info settings")
                    font.pointSize: 14
                    Layout.columnSpan: 2
                    Layout.topMargin: 20
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
