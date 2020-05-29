cmake_minimum_required(VERSION 3.14)
project(GameNetwork)

set(CMAKE_CXX_STANDARD 14)


#SET(VCPKG_ROOT "/Users/always/dev/vcpkg")
if (NOT EXISTS ${VCPKG_ROOT})
    MESSAGE(FATAL_ERROR "Cannot proceed without VCPKG_ROOT being defined. Please correct your settings.
You can get vcpkg from https://github.com/Microsoft/vcpkg")
endif ()
MESSAGE("VCPKG_ROOT          = " ${VCPKG_ROOT})
MESSAGE("Installing dependencies using VCPKG")
if (WIN32)
    execute_process(
            COMMAND cmd /c "${VCPKG_ROOT}/vcpkg.exe install libsodium:x64-windows mbedtls:x64-windows"
            OUTPUT_VARIABLE outVar
    )
elseif (UNIX)
    execute_process(
            COMMAND bash -c "${VCPKG_ROOT}/vcpkg install libsodium mbedtls"
            OUTPUT_VARIABLE outVar
    )
endif ()
MESSAGE(${outVar})


if (CLIENT)
    add_compile_definitions("CLIENT")
else ()
    add_compile_definitions("SERVER")
endif (CLIENT)


add_subdirectory(vendor/yojimbo_cmake)
include_directories(vendor/yojimbo_cmake)

set(SANDBOX_FILES sandbox/main.cpp sandbox/factory/GameMessageFactory.h sandbox/config/GameConnectionConfig.h
        sandbox/callback/ServerNetworkCallback.h sandbox/callback/ClientNetworkCallback.h
        sandbox/factory/message/GameMessageType.h sandbox/factory/message/TestMessage.h)
set(SOURCE_FILES network/server/GameServer.cpp network/server/GameServer.h
        network/client/config/ClientConfigAdapter.h network/client/GameClient.cpp network/client/GameClient.h
        network/client/message/ClientMessageAdapter.h network/server/message/ServerMessageAdapter.h
        network/NetworkConfig.h network/server/config/ServerConfigAdapter.h
        network/client/message/ClientMessageAdapter.cpp network/server/config/ServerConfigAdapter.cpp
        network/server/message/ServerMessageAdapter.cpp network/abstraction/INetworkCallback.h
        network/client/message/IClientMessageCallback.h network/server/message/IServerMessageCallback.h
        network/server/config/IServerConnectionCallback.h
        network/client/config/IClientConnectionCallback.h
        network/abstraction/INetworkBase.h network/client/base/ClientBase.cpp
        network/client/base/ClientBase.h network/client/IClientNetworkCallback.h network/server/IServerNetworkCallback.h
        network/abstraction/NetworkState.h network/server/base/ServerBase.cpp network/server/base/ServerBase.h
        network/abstraction/Network.h network/client/message/IClientMessageSender.h
        network/server/message/IServerMessageSender.h network/client/debug/IClientNetworkDebugger.h
        network/abstraction/NetworkDebug.h network/abstraction/INetworkDebuggerBase.h
        network/server/debug/IServerNetworkDebugger.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SANDBOX_FILES})
target_link_libraries(${PROJECT_NAME} ${YOJIMBO_LIBRARY})


SET("GAME_NETWORK_LIBRARY" game_network)
add_library(${GAME_NETWORK_LIBRARY} ${SOURCE_FILES})
target_link_libraries(${GAME_NETWORK_LIBRARY} PUBLIC ${YOJIMBO_LIBRARY})
target_include_directories(${GAME_NETWORK_LIBRARY} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/yojimbo_cmake>)
if (CLIENT)
    target_compile_definitions(yojimbo PUBLIC CLIENT=1)
elseif (SERVER)
    target_compile_definitions(${GAME_NETWORK_LIBRARY} PUBLIC SERVER=1)
endif (CLIENT)

set(GAME_NETWORK_LIBRARY ${GAME_NETWORK_LIBRARY} CACHE STRING "" FORCE)
mark_as_advanced(GAME_NETWORK_LIBRARY)


