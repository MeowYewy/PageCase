cmake_minimum_required(VERSION 3.21)
project(ProjectP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(DEFINED ENV{QT_DIR})
    set(QT_ROOT "$ENV{QT_DIR}" CACHE PATH "Qt installation path")
elseif(NOT DEFINED QT_ROOT)
    set(QT_ROOT "D:/Qt/6.11.1/mingw_64" CACHE PATH "Qt installation path")
endif()
list(APPEND CMAKE_PREFIX_PATH "${QT_ROOT}")

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    QuickControls2
    Widgets
    Network
    Concurrent
)

find_package(Qt6 COMPONENTS Pdf QUIET)

qt_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/appsettings.cpp
    src/appcontroller.cpp
    src/filelistmodel.cpp
    src/pdfpreviewmodel.cpp
    src/officetextextractor.cpp
    src/pdfengine.cpp
    src/pdfpagerenderer.cpp
    src/previewimageprovider.cpp
    src/updatechecker.cpp
    src/watermarklayout.cpp
)

set(HEADERS
    src/appsettings.h
    src/appcontroller.h
    src/filelistmodel.h
    src/pdfpreviewmodel.h
    src/officetextextractor.h
    src/pdfengine.h
    src/pdfpagerenderer.h
    src/previewimageprovider.h
    src/updatechecker.h
    src/watermarklayout.h
)

qt_add_executable(ProjectP ${SOURCES} ${HEADERS})

set_source_files_properties(qml/style/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

set(PROJECTP_RESOURCES
    resources/logo.svg
    resources/changelog.json
    resources/update.json
)
if(EXISTS "${CMAKE_SOURCE_DIR}/resources/app-icon.png")
    list(APPEND PROJECTP_RESOURCES resources/app-icon.png)
endif()

qt_add_qml_module(ProjectP
    URI ProjectP
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/style/Theme.qml
        qml/components/AppHeader.qml
        qml/components/FeatureTabs.qml
        qml/components/SettingsDropdown.qml
        qml/components/StyledComboBox.qml
        qml/components/RotateComboBox.qml
        qml/components/CompactComboBox.qml
        qml/components/ConvertComboBox.qml
        qml/components/CompressComboBox.qml
        qml/components/WatermarkCountCombo.qml
        qml/components/WatermarkInputField.qml
        qml/components/DragGrip.qml
        qml/components/GlobalProgressBar.qml
        qml/components/IconDeleteButton.qml
        qml/components/StatusToast.qml
        qml/components/StyledButton.qml
        qml/components/MenuOption.qml
        qml/components/ThemeToggleButton.qml
        qml/components/ShadowCard.qml
        qml/components/FileDropZone.qml
        qml/components/FileListView.qml
        qml/components/ScrollPreview.qml
        qml/components/ActionBar.qml
        qml/components/AboutDialog.qml
        qml/components/AppLogo.qml
        qml/components/ChangelogDialog.qml
        qml/components/UpdateStatusIcon.qml
        qml/pages/PageBase.qml
        qml/pages/SplitPage.qml
        qml/pages/MergePage.qml
        qml/pages/RotatePage.qml
        qml/pages/ConvertPage.qml
        qml/pages/CompressPage.qml
        qml/pages/WatermarkPage.qml
    RESOURCES
        ${PROJECTP_RESOURCES}
)

target_link_libraries(ProjectP PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
)

if(TARGET Qt6::Pdf)
    target_link_libraries(ProjectP PRIVATE Qt6::Pdf)
    target_compile_definitions(ProjectP PRIVATE HAS_QT_PDF=1)
    message(STATUS "Qt6 Pdf found — using built-in PDF preview")
else()
    message(STATUS "Qt6 Pdf not found — using poppler pdftoppm fallback")
endif()

if(WIN32)
    set_target_properties(ProjectP PROPERTIES WIN32_EXECUTABLE TRUE)
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/app-icon.ico")
        target_sources(ProjectP PRIVATE "${CMAKE_SOURCE_DIR}/resources/app-icon.rc")
    endif()
endif()

add_custom_command(TARGET ProjectP POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/tools/qpdf"
        "$<TARGET_FILE_DIR:ProjectP>/tools/qpdf"
    COMMENT "Copying qpdf tools to build output"
)

if(EXISTS "${CMAKE_SOURCE_DIR}/tools/poppler/pdftoppm.exe")
    add_custom_command(TARGET ProjectP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/tools/poppler"
            "$<TARGET_FILE_DIR:ProjectP>/tools/poppler"
        COMMENT "Copying poppler tools to build output"
    )
endif()
