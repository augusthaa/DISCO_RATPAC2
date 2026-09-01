add_library( discoobj OBJECT
  # Append each .cc file
  Disco.cc
  GeoMuTrackerFactory.cc
  GeoTestCellFactory.cc
  GeoPMTHousingFactory.cc
)

target_include_directories( discoobj PUBLIC
  $<BUILD_INTERFACE:${DISCO_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

file(COPY include/ DESTINATION ${DISCO_INCLUDE_DIR})

add_executable(disco disco.cpp)
target_link_libraries(disco PUBLIC 
  discoobj ${Ratpac_LIBRARIES} ${ROOT_LIBRARIES} ${Geant4_LIBRARIES})

set_target_properties(discoobj disco
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${DISCO_LIBRARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${DISCO_LIBRARY_DIR})

install(TARGETS discoobj disco
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)

install(DIRECTORY ${DISCO_INCLUDE_DIR}
    DESTINATION ${CMAKE_INSTALL_PREFIX})
