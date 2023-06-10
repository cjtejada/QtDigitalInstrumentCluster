set(SRC 
    instrumentcluster.h
    main.cpp
    pids.h
    serialgps.cpp
    serialgps.h
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
)

install(TARGETS DIC RUNTIME
	DESTINATION ${CMAKE_INSTALL_BINDIR})
