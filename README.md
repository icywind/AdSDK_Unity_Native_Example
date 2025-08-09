# AdSDK Unity Native Example

This project demonstrates how to integrate a C++ native library (AdSDK) with a Unity project. It includes the native C++ source code and a Unity project configured to call functions from the native library.

## Project Structure

-   `NativeAdSDK/`: Contains the source code for the C++ native library.
-   `UnityAdExample/`: Contains the Unity project that consumes the native library.

## Part 1: Building the Native Library

The native library is built using CMake. This process compiles the C++ code into a dynamic library (`.dll` on Windows, `.dylib` on macOS, or `.so` on Linux) that Unity can load.

### Prerequisites

-   A C++ compiler toolchain (e.g., Visual Studio 2019+ on Windows, Xcode Command Line Tools on macOS, or `build-essential` on Linux).
-   CMake (version 3.10 or higher).

### Build Instructions

#### Windows (Visual Studio)

1.  Open a terminal like "x64 Native Tools Command Prompt for VS" or PowerShell.
2.  Navigate to the native library's source directory:
    ```sh
    cd NativeAdSDK
    ```
3.  Create and navigate to a build directory:
    ```sh
    mkdir build
    cd build
    ```
4.  Generate the Visual Studio solution files:
    ```sh
    cmake ..
    ```
5.  Build the project in Release mode:
    ```sh
    cmake --build . --config Release
    ```
6.  The compiled library, `AdSDK.dll`, will be located in `NativeAdSDK/build/Release/`.

#### macOS

1.  Open the Terminal app.
2.  Navigate to the native library's source directory:
    ```sh
    cd NativeAdSDK
    ```
3.  Create and navigate to a build directory:
    ```sh
    mkdir build
    cd build
    ```
4.  Generate the Makefiles:
    ```sh
    cmake ..
    ```
5.  Build the project:
    ```sh
    make
    ```
6.  The compiled library, `libAdSDK.dylib`, will be located in `NativeAdSDK/build/`.

## Part 2: Testing in Unity

### 1. Copy the Native Library into the Unity Project

For Unity to find the native library, you must place it in the `Assets/Plugins` folder of the Unity project.

1.  Create the folder if it doesn't exist: `UnityAdExample/Assets/Plugins/`.
2.  Copy the compiled library into this folder:
    -   **Windows**: Copy `NativeAdSDK/build/Release/AdSDK.dll` to `UnityAdExample/Assets/Plugins/AdSDK.dll`.
    -   **macOS**: Copy `NativeAdSDK/build/libAdSDK.dylib` to `UnityAdExample/Assets/Plugins/libAdSDK.dylib`.

### 2. Open and Run the Unity Project

1.  Open Unity Hub.
2.  Click "Open" and select the `UnityAdExample` folder.
3.  Once the project is open, find the `SampleScene` in the `Assets/Scenes` folder in the Project window and open it.
4.  Press the **Play** button in the Unity Editor.

### 3. How to Test

The example scene is set up to demonstrate the integration.

-   **Preloading an Ad**: When you enter Play mode, the `AdBridge` script automatically calls the native `AdSDK_PreloadAd` function. After a 500ms delay (simulated in the C++ code), a success callback is triggered. Check the Unity Console for the following message:
    ```
    [Ad] Loaded: video_ad_001
    ```

-   **Getting an Ad Thumbnail**: The scene contains a UI Button labeled "Get Thumbnail".
    1.  Make sure you are in Play mode.
    2.  Click the "Get Thumbnail" button in the Game view.
    3.  This calls the native `AdSDK_GetAdThumbnail` function. The native code generates a sample image and passes the raw pixel data back to Unity.
    4.  The `RawImage` UI element on screen will update to display the generated checkerboard thumbnail.
