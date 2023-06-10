cmake_minimum_required(VERSION 3.6.0)

project(DIC
	VERSION 0.0.1
	DESCRIPTION " Qt OBD II Digital Instrument Cluster")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)

include(FeatureSummary)
include(GNUInstallDirs)

set(QT_MIN_VERSION "5.11.0")
find_package(Qt5 ${QT_MIN_VERSION} COMPONENTS Gui Qml Quick SerialPort LinguistTools REQUIRED)
find_package(Qt5QuickCompiler)

add_subdirectory(src)

# Translations
file(GLOB TS_FILES translations/*.ts)
qt5_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations DEPENDS ${QM_FILES})
add_dependencies(DIC translations)

install(FILES ${QM_FILES}
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/DIC/translations)

install(FILES DIC.desktop
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(DIRECTORY icons
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/DIC)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
