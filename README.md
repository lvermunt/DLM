How I got it to work:

```
#Master DLM is currently broken with GentleFemto
git checkout https://github.com/dimihayl/DLM/commit/e4444b3e33dd4122a9afceb63ac6d12f6dc46f70
git checkout -b <your_branchname>
```

Some includes had to be modified in my case
```diff
diff --git a/CATS/CATS.cpp b/CATS/CATS.cpp
index 3b994fd..d2f5ef9 100644
--- a/CATS/CATS.cpp
+++ b/CATS/CATS.cpp
@@ -6,9 +6,9 @@
 #include <stdio.h>
 #include <string.h>
 
-#include "gsl_sf_coulomb.h"
-#include "gsl_sf_bessel.h"
-#include "gsl_sf_legendre.h"
+#include "gsl/gsl_sf_coulomb.h"
+#include "gsl/gsl_sf_bessel.h"
+#include "gsl/gsl_sf_legendre.h"
 
 #include "DLM_CppTools.h"
 #include "DLM_Histo.h"
```
```diff
diff --git a/CATS/CATStools.cpp b/CATS/CATStools.cpp
index 1e2d8d0..4e466cf 100644
--- a/CATS/CATStools.cpp
+++ b/CATS/CATStools.cpp
@@ -5,7 +5,7 @@
 #include <stdio.h>
 #include <string.h>
 
-#include "gsl_sf_gamma.h"
+#include "gsl/gsl_sf_gamma.h"
 
 #include "DLM_CppTools.h"
 #include "CATSconstants.h"
```
```diff
diff --git a/CATS_Extentions/DLM_CkModels.cpp b/CATS_Extentions/DLM_CkModels.cpp
index fe54e82..6d9cb7a 100644
--- a/CATS_Extentions/DLM_CkModels.cpp
+++ b/CATS_Extentions/DLM_CkModels.cpp
@@ -1,6 +1,6 @@
 
 #include "DLM_CkModels.h"
-#include "gsl_sf_dawson.h"
+#include "gsl/gsl_sf_dawson.h"
 #include "CATSconstants.h"
 #include "CATStools.h"
 #include "DLM_Integration.h"
```

Next one should change the paths in `CMake/CMakeList.txt`. I used the following paths
```diff
diff --git a/CMake/CMakeLists.txt b/CMake/CMakeLists.txt
index 3977284..4afe738 100644
--- a/CMake/CMakeLists.txt
+++ b/CMake/CMakeLists.txt
@@ -10,11 +10,11 @@ SET(PROJECT_NAME "CATS")

 project(${PROJECT_NAME})
 # SET PATHS #
-SET(CATS_DESTINATION "/home/dmihaylov/Apps/CATS")#where to install CATS. Folder must exist.
-SET(ROOT_PATH "/home/dmihaylov/Apps/root-6.14.00/obj")#path to ROOT
-SET(GSL_INCLUDE "/usr/include/gsl")#where are all GSL related .h files
-SET(GSL_LIB "/usr/lib")#where are the GSL .a and .so files
-SET(CATS_FOLDER "/home/dmihaylov/Dudek_Ubuntu/DLM_GitHub")#where is the CATS main folder located
+SET(CATS_DESTINATION "/home/lvermunt/FemtoAnalysis/CATS")#where to install CATS. Folder must exist.
+SET(ROOT_PATH "/home/lvermunt/simulation/root_install/")#path to ROOT
+SET(GSL_INCLUDE "/cvmfs/sft.cern.ch/lcg/releases/GSL/2.6-ecdfc/x86_64-ubuntu1804-gcc7-opt/include/")#where are all GSL related .h files
+SET(GSL_LIB "/cvmfs/sft.cern.ch/lcg/releases/GSL/2.6-ecdfc/x86_64-ubuntu1804-gcc7-opt/lib")#where are the GSL .a and .so files
+SET(CATS_FOLDER "/home/lvermunt/FemtoAnalysis/DLM")#where is the CATS main folder located
 # DO NOT CHANGE THE REST #

 SET(VERSION_MAJOR 3)
```

Now: Build and install:
```
cd CMake
source configure.sh
#When there are no errors in the terminal, run:
make
make install
```
