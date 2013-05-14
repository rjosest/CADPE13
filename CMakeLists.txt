PROJECT(CADPE13)

cmake_minimum_required(VERSION 2.6)

SET( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" )


FIND_PACKAGE ( ITK )
IF ( ITK_FOUND )
  INCLUDE(${ITK_USE_FILE})
ELSE ( ITK_FOUND )
  MESSAGE ( FATAL_ERROR "Cannot build without ITK" )
ENDIF ( ITK_FOUND )


SET(Libraries
  ITKBasicFilters
  ITKCommon
  ITKIO
)


SET ( BinaryThinning3D_SOURCE BinaryThinning3D.cxx )
ADD_EXECUTABLE( BinaryThinning3D ${BinaryThinning3D_SOURCE})
TARGET_LINK_LIBRARIES ( BinaryThinning3D ${ITK_LIBRARIES} )

SET ( GeneratePartialLungLabelMap_SOURCE GeneratePartialLungLabelMap.cxx )

ADD_EXECUTABLE ( GeneratePartialLungLabelMap ${GeneratePartialLungLabelMap_SOURCE})
TARGET_LINK_LIBRARIES ( GeneratePartialLungLabelMap ${ITK_LIBRARIES} )


