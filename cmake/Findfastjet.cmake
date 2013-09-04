# Package finder for FastJet
# Variables that must be set:
# - FASTJET_FOUND
# - FastJet_INCLUDE_DIRS
# - FastJet_LIBRARIES
# - FastJet_DEFINITIONS

set (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

# Search PATH for FastJet-config
execute_process (COMMAND which fastjet-config
  OUTPUT_VARIABLE fastjet_CONFIG
)

if (fastjet_CONFIG)
  execute_process (COMMAND fastjet-config --prefix
    OUTPUT_VARIABLE fastjet_PREFIX
    )
  if (fastjet_PREFIX)
    string (STRIP ${fastjet_PREFIX} fastjet_PREFIX)
    message (STATUS "Found fastjet-config. FastJet location is ${fastjet_PREFIX}")
  endif ()
  
  # Check FastJet version high enough
  execute_process (COMMAND fastjet-config --version
    OUTPUT_VARIABLE fastjet_VERSION
    )
  if (NOT fastjet_VERSION VERSION_GREATER "2.9")
    message (STATUS "PSTree requires FastJet 3.0+")
  endif ()

  # Get library info from fastjet-config
  execute_process (COMMAND fastjet-config --libs --rpath --plugins
    OUTPUT_VARIABLE FASTJETLIBS
    )
  string (STRIP "${FASTJETLIBS}" FASTJETLIBS)
  separate_arguments (FASTJETLIBS)
  string (REGEX MATCHALL "-L[^;]+" FastJet_LIBRARY_DIR "${FASTJETLIBS}")
  string (REPLACE "-L" "" FastJet_LIBRARY_DIR "${FastJet_LIBRARY_DIR}")
  string (REPLACE ";" " " FastJet_LIBRARY_DIR "${FastJet_LIBRARY_DIR}")
  string (REGEX MATCHALL "-l[^;]+" FastJet_LIBNAMES "${FASTJETLIBS}")
  string (REPLACE "-l" "" FastJet_LIBNAMES "${FastJet_LIBNAMES}")

  # Get include info from FastJet-config
  execute_process (COMMAND fastjet-config --cxxflags
    OUTPUT_VARIABLE FASTJETFLAGS
    )
  string (STRIP "${FASTJETFLAGS}" FASTJETFLAGS)
  separate_arguments (FASTJETFLAGS)
  string (REGEX MATCHALL "-I[^;]+" FastJet_INCLUDE_DIR "${FASTJETFLAGS}")
  string (REPLACE "-I" "" FastJet_INCLUDE_DIR "${FastJet_INCLUDE_DIR}")
  string (REPLACE ";" " " FastJet_INCLUDE_DIR "${FastJet_INCLUDE_DIR}")
endif()

# Find an example header file
find_path (FastJet_INCLUDE_DIRS fastjet/ClusterSequence.hh PATH ${FastJet_INCLUDE_DIR})

# Find required libraries
foreach (libname ${FastJet_LIBNAMES})
  unset (lib CACHE)
  find_library (lib ${libname} ${FastJet_LIBRARY_DIR})
  if (lib)
    set (FastJet_LIBRARIES ${FastJet_LIBRARIES} ${lib})
  else()
    message (STATUS "Could NOT find library: ${libname}")
  endif()
endforeach()

# Return FastJet_FOUND=TRUE if everything OK
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FastJet DEFAULT_MSG FastJet_LIBRARIES FastJet_INCLUDE_DIRS)

message(" *** fastjet dir ---> ${FastJet_INCLUDE_DIRS}")
message(" *** fastjet lib ---> ${FastJet_LIBRARIES}")
message(" *** fastjet def ---> ${FastJet_DEFINITIONS}")
message(" *** fastjet ver ---> ${FastJet_VERSION}")
