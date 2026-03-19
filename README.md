# random_company_cpp

GUI program to pick a random company in C++

## Depedency requirements
Depedency requirements for this project:
- LLVM: https://releases.llvm.org/
- Make: https://gnuwin32.sourceforge.net/packages/make.htm
- vcpkg: https://vcpkg.io/en/index.html
- CMake: https://cmake.org/download/
- Visual Studio: https://visualstudio.microsoft.com/

## Environmental variables
1. On search bar, search: Edit environment variables for your account and click the first result
2. Highlight Path entry of User variables for <account name>
3. Click the edit button
4. Append the following paths:
- C:\Program Files (x86)\GnuWin32\bin
- C:\Users\<account_name>\Documents\git\vcpkg

# Install vcpkg
Look at this link to setup vcpkg: 
[Tutorial: Install and use packages with CMake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)

## Setup project
In `CMakeUserPresets.json`, make sure that the VCPKG_ROOT variable has
the correct path to the pcpkg git repository
```
"VCPKG_ROOT": "C:\\Users\\salsa\\Documents\\git\\vcpkg"
```
