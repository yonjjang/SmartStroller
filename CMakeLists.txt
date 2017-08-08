CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(position-finder-server C)

SET(INSTALL_EXEC_PREFIX "${INSTALL_PREFIX}/bin")
SET(INSTALL_RESDIR "${INSTALL_PREFIX}/res")
SET(INSTALL_RESDIR "/home/owner/apps_rw/org.tizen.position-finder-server/res")
SET(CMAKE_VERBOSE_MAKEFILE 0)

SET(PROJECT_ROOT_DIR "${CMAKE_SOURCE_DIR}")
SET(PROJECT_RESOURCES_DIR "${PROJECT_ROOT_DIR}/res")

INCLUDE(FindPkgConfig)
pkg_check_modules(APP_PKGS REQUIRED
	dlog
	aul
	capi-appfw-application
	capi-appfw-service-application
	capi-system-peripheral-io
	ecore
	eina
	iotcon
	gio-2.0
)

FOREACH (flag ${APP_PKGS_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fvisibility=hidden -Wall -Winline -g -fno-builtin-malloc -fPIE")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

INCLUDE_DIRECTORIES(${PROJECT_ROOT_DIR}/inc)

ADD_EXECUTABLE(${PROJECT_NAME}
	${PROJECT_ROOT_DIR}/src/controller.c
	${PROJECT_ROOT_DIR}/src/controller_internal.c
	${PROJECT_ROOT_DIR}/src/connectivity.c
	${PROJECT_ROOT_DIR}/src/resource.c
	${PROJECT_ROOT_DIR}/src/resource/resource_illuminance_sensor.c
	${PROJECT_ROOT_DIR}/src/resource/resource_infrared_motion_sensor.c
	${PROJECT_ROOT_DIR}/src/resource/resource_infrared_obstacle_avoidance_sensor.c
	${PROJECT_ROOT_DIR}/src/resource/resource_touch_sensor.c
	${PROJECT_ROOT_DIR}/src/resource/resource_ultrasonic_sensor.c
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS} -lm)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${APP_PKGS_LDFLAGS})

# Install
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_EXEC_PREFIX})

INSTALL(FILES ${PROJECT_ROOT_DIR}/tizen-manifest.xml DESTINATION ${SYS_PACKAGES_DIR} RENAME org.tizen.position-finder-server.xml)
INSTALL(DIRECTORY DESTINATION ${PREFIX}/data)
INSTALL(FILES ${PROJECT_ROOT_DIR}/shared/res/position_finder_server.png DESTINATION ${SYS_ICONS_DIR})
INSTALL(FILES ${PROJECT_ROOT_DIR}/res/iotcon-test-svr-db-server.dat DESTINATION ${INSTALL_RESDIR})

# End of a file
