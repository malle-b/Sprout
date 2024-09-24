# Sprout Demos 
Here you can find some demos on how to utilise Sprout and some of its core features. 

## How to run 
To run Sprout with a ROOT macro you need to tell ROOT's C++ interpreter where to find the so-called dictionary file, ```libSprout.so```. The easiest way to acheive this is to create a ```rootlogon.C``` in the same folder as your macro, containing the following: 
```
void rootlogon(){
    gSystem->Load("path_to_your_libSprout.so")
}
```
This line can also be added to an already existing ```rootlogon.C```. 

After building and compiling Sprout, this file will be located at ```your_sprout_build_folder/libSprout.so```. 
If you have installed Sprout to a different location by running ```cmake install```, you can instead find this file at ```your_sprout_install_dir/lib/libSprout.so```.

