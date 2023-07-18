project(Comrpc)

find_package(${NAMESPACE}Core REQUIRED)
find_package(${NAMESPACE}COM REQUIRED)
find_package(${NAMESPACE}Plugins REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${NAMESPACE}Core::${NAMESPACE}Core
    ${NAMESPACE}COM::${NAMESPACE}COM
    ${NAMESPACE}Plugins::${NAMESPACE}Plugins
    )