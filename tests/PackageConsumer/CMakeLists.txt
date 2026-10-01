cmake_minimum_required(VERSION 3.25)

project(MIDILARPackageConsumer LANGUAGES CXX)

find_package(MIDILAR 0.1 CONFIG REQUIRED)

add_executable(MIDILARPackageConsumer main.cpp)
target_link_libraries(MIDILARPackageConsumer PRIVATE MIDILAR::MIDILAR)

enable_testing()
add_test(NAME MIDILARPackageConsumer COMMAND MIDILARPackageConsumer)
