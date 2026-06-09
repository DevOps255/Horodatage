# Horodatage

Horodatage is a small Windows-focused C++ command-line project designed to work with file timestamp metadata. The current implementation prompts the user for a target date and time, converts that input into the native Windows `FILETIME` format, and provides the foundation for applying custom creation, last access, and last modification timestamps to files.

## Features

- Interactive command-line input for year, month, day, hour, and minute values.
- Conversion from user-friendly date and time values to Windows `FILETIME`.
- Windows API integration through `SYSTEMTIME`, `FILETIME`, and file handle primitives.
- CMake-based project setup using C++20.
- Initial file-management interface prepared for opening file handles.

## Project Structure

```text
.
|-- CMakeLists.txt     # CMake build configuration
|-- main.cpp           # Console entry point and user input flow
|-- TimeManager.h      # Timestamp conversion API declaration
|-- TimeManger.cpp     # Timestamp conversion implementation
|-- FileManager.h      # File handle API declaration
|-- README.md          # Project documentation
```

## Requirements

- Windows operating system
- C++20-compatible compiler
- CMake 4.2 or newer, as declared by the project configuration
- A development environment such as CLion, Visual Studio, or a terminal with CMake available

## Build

From the project root:

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

The generated executable is named `horodatage`.

## Usage

Run the executable and enter the requested date and time fields when prompted:

```bash
./horodatage
```

The program asks for:

- Year
- Month
- Day
- Hour
- Minute

It then converts the provided values into a Windows `FILETIME` value that can be used with Windows file timestamp APIs.

## Current Status

This project is an early-stage implementation. Timestamp conversion is implemented, while the complete file update workflow is still in progress. The `FileManager.h` interface declares a file-opening function, but the corresponding implementation and timestamp application logic still need to be added before the program can modify real file metadata.

## Planned Improvements

- Implement `OpenFileHandle`.
- Ask the user for a target file path.
- Apply the generated `FILETIME` value to creation, last access, and last modification timestamps.
- Add validation for user input ranges.
- Improve error handling for invalid dates and Windows API failures.
- Add a clear success or failure message after timestamp updates.

## Notes

Because this project uses Windows-specific APIs, it is not portable to Linux or macOS without replacing the timestamp and file-handle logic with platform-specific alternatives.
