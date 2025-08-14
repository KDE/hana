#
# SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#

ecm_install_icons(
    ICONS sc-apps-hana.svg
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)

install(FILES org.kde.hana.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.hana.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
