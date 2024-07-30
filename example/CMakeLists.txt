#   _______  __    _    __  __ ____  _     _____ ____  
#  | ____\ \/ /   / \  |  \/  |  _ \| |   | ____/ ___| 
#  |  _|  \  /   / _ \ | |\/| | |_) | |   |  _| \___ \ 
#  | |___ /  \  / ___ \| |  | |  __/| |___| |___ ___) |
#  |_____/_/\_\/_/   \_\_|  |_|_|   |_____|_____|____/ 
#                                                     

add_executable(takephoto takephoto.cpp)
target_link_libraries(takephoto lccv_static ${OpenCV_LIBS})

add_executable(takevideo takevideo.cpp)
target_link_libraries(takevideo lccv_static ${OpenCV_LIBS})
