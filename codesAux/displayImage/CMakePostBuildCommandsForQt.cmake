FUNCTION(RUN_POST_BUILD_COMMANDS_FOR_QT_ON_UNIX targetName)
    IF(UNIX)
        ADD_CUSTOM_COMMAND (TARGET ${targetName} POST_BUILD
                            COMMAND  rm *automoc*.cpp 2> /dev/null || echo ""
                            COMMENT "rm *automoc*.cpp"
        )
        ADD_CUSTOM_COMMAND (TARGET ${targetName} POST_BUILD
                            COMMAND rm moc*.cpp 2> /dev/null || echo ""
                            COMMENT "rm moc*.cpp" 
        )
    ENDIF()
ENDFUNCTION(RUN_POST_BUILD_COMMANDS_FOR_QT_ON_UNIX)

FUNCTION(RUN_POST_BUILD_COMMANDS_FOR_QT_ON_MSVC targetName QT5dir useQTIn64bits)
    IF(MSVC)
        #copy qt5 dll to build folder======================================
        IF(useQTIn64bits)
            ADD_CUSTOM_COMMAND (TARGET ${targetName}  POST_BUILD
                COMMAND  copy ${QT5dir}\\msvc2012_64\\bin\\*.dll  $(Configuration)\\             
            )         
        ELSE()
            ADD_CUSTOM_COMMAND (TARGET ${targetName}  POST_BUILD
                COMMAND  copy ${QT5dir}\\msvc2012\\bin\\*.dll  $(Configuration)\\             
         )
        ENDIF()
        

        #end================================================================
        ADD_CUSTOM_COMMAND (TARGET ${targetName}  POST_BUILD
             COMMAND  IF EXIST *automoc*.cpp del *automoc*.cpp             
         )
         ADD_CUSTOM_COMMAND (TARGET ${targetName}  POST_BUILD
             COMMAND  IF EXIST moc*.cpp del moc*.cpp             
         )
    ENDIF()
ENDFUNCTION(RUN_POST_BUILD_COMMANDS_FOR_QT_ON_MSVC)
