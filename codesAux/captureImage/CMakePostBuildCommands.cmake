#the function ADD_RESOURCE_DIRECTORY_TO_DIRECTORY_OF_PROJECT_BUILT
#copy  ${directory} at "src" to  directory of project built
FUNCTION(ADD_RESOURCE_DIRECTORY_TO_DIRECTORY_OF_PROJECT_BUILT targetName directory ) 
    SET(srcDir "src")
    IF(UNIX)
        SET(bin_dest "build")
        SET(resource_dest "${bin_dest}/${directory}")
        SET(resource_orig "${srcDir}/${directory}/")          
        ADD_CUSTOM_COMMAND (TARGET ${targetName} POST_BUILD
                            COMMAND /bin/sh -c \"if [ ! -d '${resource_dest}' ]; then mkdir ${resource_dest}\; fi\; \"
                            COMMENT "mkdir ${resource_dest}"      
        )
        ADD_CUSTOM_COMMAND (TARGET ${targetName}  POST_BUILD
                            COMMAND cp -rf ${resource_orig}*  ${resource_dest}
                            COMMENT "cp -rf ${resource_orig}*  ${resource_dest}"      
        )        
    ENDIF()
    IF(MSVC)
        SET(bin_dest "build")
        SET(resource_orig "${srcDir}/${directory}/")    
        ADD_CUSTOM_COMMAND(TARGET ${targetName} POST_BUILD
                            COMMAND  IF NOT EXIST $(Configuration)\\${directory}  mkdir  $(Configuration)\\${directory}
                        COMMENT "IF NOT EXIST $(Configuration)\\${directory}  mkdir  $(Configuration)\\${directory}"      
                        )

       FILE(TO_NATIVE_PATH ${resource_orig} resource_orig_windows)
       ADD_CUSTOM_COMMAND (TARGET ${targetName}  POST_BUILD
                           COMMAND copy ${resource_orig_windows}  $(Configuration)\\${directory}\\
                           COMMENT "copy ${resource_orig_windows}  $(Configuration)\\${directory}\\"      
                          )
                          
    ENDIF()    
ENDFUNCTION(ADD_RESOURCE_DIRECTORY_TO_DIRECTORY_OF_PROJECT_BUILT)

FUNCTION(RUN_POST_BUILD_COMMANDS targetName)

    IF(UNIX)

        #move complied program to paste build===============================
        GET_PROPERTY(outputFile TARGET ${targetName}   PROPERTY OUTPUT_NAME)
        set(bin_dest "build")        
        set(matrices_dest "${bin_dest}/matricesWithInitialConditionAndOffsetsOfGrayScott") 
        ADD_CUSTOM_COMMAND (TARGET ${targetName} POST_BUILD
                            COMMAND /bin/sh -c \"if [ ! -d '${bin_dest}' ]; then mkdir ${bin_dest}\; fi\; \"
                            COMMENT "mkdir ${bin_dest}"      
        )
        ADD_CUSTOM_COMMAND (TARGET ${targetName} POST_BUILD
                            COMMAND /bin/sh -c \"if [ ! -d '${outputFile}' ]; then mv ${outputFile} ${bin_dest}\; fi\; \"
                            COMMENT "mv ${outputFile} ${bin_dest}/" 
        )
        #end================================================================
        
    ENDIF()
    

ENDFUNCTION(RUN_POST_BUILD_COMMANDS)
