set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_PACKAGE_CONTACT "yourname@example.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "R-Type Game")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_FILE_NAME "R-Type-${CPACK_PACKAGE_VERSION_MAJOR}-${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "/${CPACK_PACKAGE_NAME}")
set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE ON)

install(TARGETS rtype
       COMPONENT binaries
       RUNTIME DESTINATION bin
       )

install(DIRECTORY assets
       COMPONENT assets
       DESTINATION share/${PROJECT_NAME}
       )

install(FILES dependencies/*.dll
       COMPONENT dependencies
       DESTINATION lib
       )

# Spécifier les composants à inclure dans le package
set(CPACK_COMPONENTS_ALL
   binaries
   assets
   dependencies
)

# Définir les chemins pour chaque composant
set(CPACK_COMPONENT_binaries_HIDDEN FALSE)
set(CPACK_COMPONENT_assets_HIDDEN FALSE)
set(CPACK_COMPONENT_dependencies_HIDDEN FALSE)

set(CPACK_COMPONENT_binaries_DISPLAY_NAME "Binaries")
set(CPACK_COMPONENT_assets_DISPLAY_NAME "Assets")
set(CPACK_COMPONENT_dependencies_DISPLAY_NAME "Dependencies")

set(CPACK_COMPONENT_binaries_DESCRIPTION "Game Binaries")
set(CPACK_COMPONENT_assets_DESCRIPTION "Game Assets")
set(CPACK_COMPONENT_dependencies_DESCRIPTION "Game Dependencies")

set(CPACK_COMPONENT_GROUPING "Required")

# Exécuter Cpack
cpack
