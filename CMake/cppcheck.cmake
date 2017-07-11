# derived from https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format/
#
# additional target to perform cppcheck run, requires cppcheck 
# get all project files 
# HACK this workaround is required to avoid qml files checking ^_^
#
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h) 

SET (PROJECT_TRDPARTY_DIR third-party)

foreach (SOURCE_FILE ${ALL_SOURCE_FILES}) 
  string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND) 
  if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
    list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE}) 
  endif () 
endforeach ()

add_custom_target(cppcheck 
  COMMAND /usr/bin/cppcheck
  #--enable=warning,performance,portability,information,missingInclude
  --enable=all
  --suppress=missingIncludeSystem
  --includes-file=${INCLUDE_DIRECTORIES}
  --std=${CXX_STANDARD}
  --check-config
  --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)" 
  --verbose 
  --quiet 
  ${ALL_SOURCE_FILES}
  )

