target_include_directories(${PROJECT_NAME} PRIVATE
    "./"
    "app"
    "driver"
    "hal"
    "utils"
    "testing"
)

target_link_directories(${PROJECT_NAME} PRIVATE
    "./"
    "app"
    "driver"
    "hal"
    "utils"
    "testing"
)