# Sprout

## How to install Sprout 
1. Clone the repository
2. Build the project using cmake:
    ```
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=path_to_your_install_dir
    make
    make install
    ```

## Using Sprout in a ROOT macro 
To your rootlogon.C, add ``` gSystem->Load("your_sprout_install_dir/lib/libSprout.so")``` 
This can either be added to your default rootlogon.C but you will probably want to create a new one in the same folder as your ROOT macro. By default, ROOT will load the rootlogon.C from the directory it is launched in. If this is the case, you can use the following: 
```
void rootlogon(){
  gSystem->Load("path/to/your/install/dir/lib/libSprout.so
}
```

In your ROOT macro, you'll then be able to use Sprout by including the paths to its header files. 

## Using Sprout in an excecutable 
To your project's CMakeLists.txt, add ``` include(your_sprout_install_dir/lib/cmake/Sprout/SproutTargets.cmake) ``` and link it to your target along with ROOT::Core. Below you'll find a simple, working example of a CMakeLists.txt file that creates and links an excecutable to Sprout. 

```
cmake_minimum_required(VERSION 3.10)

project(yourProject)

add_executable(yourProject yourProject.cxx)

find_package(ROOT)
include(your_sprout_install_dir/lib/cmake/Sprout/SproutTargets.cmake)
target_link_libraries(yourProject PUBLIC Sprout::Sprout ROOT::Core)


```
