set(SRC 
    instrumentcluster.h
    main.cpp
    pids.h
    serialobd.cpp
    serialobd.h
)

qtquick_compiler_add_resources(RESOURCES qml/DIC.qrc)

add_executable(DIC ${SRC} ${RESOURCES})


target_link_libraries(DIC
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	Qt5::SerialPort
	Qt5::Positioning
)

install(TARGETS DIC RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
