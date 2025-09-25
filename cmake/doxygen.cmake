

find_package(Doxygen)

if(Doxygen_FOUND)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_EXTRACT_PACKAGE YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_EXTRACT_LOCAL_CLASSES YES)
    set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../images)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_DOCSET_FEEDNAME "Dupa xDD")
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    
    set(DOXYGEN_DOT_PATH "D:/Projekty/_Tools/Documentation/Graphviz-13.0.1-win64/bin")
    
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CALL_GRAPH YES)

    doxygen_add_docs(
        documentation
        # USE_STAMP_FILE
        OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
        INPUT ${CMAKE_CURRENT_SOURCE_DIR}/source
        # EXCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/tests
        GENERATE_HTML YES
        GENERATE_LATEX NO
        GENERATE_XML YES
        GENER)
    
endif()