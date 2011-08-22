INCLUDE(${CMAKE_SOURCE_DIR}/IJMacros.txt)

#Change PROJECT_NAME to the name of your project
PROJECT(BoykovGraphCuts)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
#
# The packages can be specified with a version number, for example:
#
# ITK 2.8.1
# ITK 3.2.0
#
# If no version is specified, the most recent release of the package
# will be used.
SET(Required_Packages
ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
ITKBasicFilters
ITKAlgorithms
ITKCommon
ITKIO
)

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

IF(BUILD_TESTING)
SET(CurrentExe "ImageCompare")
ADD_EXECUTABLE(${CurrentExe} ImageCompare.cxx) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})
SET(CurrentExe "itkBoykovGraphCutFilterTest")
ADD_EXECUTABLE(${CurrentExe} itkBoykovGraphCutFilterTest.cxx) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})
SET(CurrentExe "itkGraphTest")
ADD_EXECUTABLE(${CurrentExe} itkGraphTest.cxx) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})
ENDIF(BUILD_TESTING)

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.

ADD_TEST(GraphTest itkGraphTest)

ADD_TEST(GCTest_1 itkBoykovGraphCutFilterTest 2 ${CMAKE_SOURCE_DIR}/r85slice.png ${CMAKE_SOURCE_DIR}/test_1.nii.gz)
ADD_TEST(GCTest_1_ImageCompare ImageCompare ${CMAKE_SOURCE_DIR}/test_1.nii.gz ${CMAKE_SOURCE_DIR}/r85slice_out.nii.gz)

ADD_TEST(GCTest_2 itkBoykovGraphCutFilterTest 2 ${CMAKE_SOURCE_DIR}/tools.jpg ${CMAKE_SOURCE_DIR}/test_2.nii.gz)
ADD_TEST(GCTest_2_ImageCompare ImageCompare ${CMAKE_SOURCE_DIR}/test_2.nii.gz ${CMAKE_SOURCE_DIR}/tools_out.nii.gz)

ADD_TEST(GCTest_3 itkBoykovGraphCutFilterTest 2 ${CMAKE_SOURCE_DIR}/peppers.jpg ${CMAKE_SOURCE_DIR}/test_3.nii.gz)
ADD_TEST(GCTest_3_ImageCompare ImageCompare ${CMAKE_SOURCE_DIR}/test_3.nii.gz ${CMAKE_SOURCE_DIR}/peppers_out.nii.gz)

ADD_TEST(GCTest_4 itkBoykovGraphCutFilterTest 2 ${CMAKE_SOURCE_DIR}/ct_scan.jpg ${CMAKE_SOURCE_DIR}/test_4.nii.gz)
ADD_TEST(GCTest_4_ImageCompare ImageCompare ${CMAKE_SOURCE_DIR}/test_4.nii.gz ${CMAKE_SOURCE_DIR}/ct_scan_out.nii.gz)

ADD_TEST(GCTest_5 itkBoykovGraphCutFilterTest 3 ${CMAKE_SOURCE_DIR}/lungs.nii.gz ${CMAKE_SOURCE_DIR}/test_5.nii.gz)
ADD_TEST(GCTest_5_ImageCompare ImageCompare ${CMAKE_SOURCE_DIR}/test_5.nii.gz ${CMAKE_SOURCE_DIR}/lungs_out.nii.gz)

#Once you're done modifying this template, you should rename it to "CMakeLists.txt"
