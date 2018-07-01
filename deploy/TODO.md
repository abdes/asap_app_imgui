
```
-------------------------------------------------------------------
In subdirectory "./deploy/" do:
deploy/CMakeLists.txt:
[ ] Substitute pack-template.cmake -> pack-<project>.cmake
[ ] Exchange deploy/images/logo.ico
[ ] Exchange deploy/images/logo.bmp
[ ] Exchange deploy/images/logo.png
[ ] Rename deploy/packages/pack-template.cmake -> pack-<project>.cmake
deploy/packages/pack-<project>.cmake:
[ ] Adjust OPTION_PACK_GENERATOR to your liking for all systems
[ ] Adjust package options, e.g., CPACK_DEBIAN_PACKAGE_DEPENDS, CPACK_DEBIAN_PACKAGE_SECTION, CPACK_DEBIAN_PACKAGE_PRIORITY, CPACK_RPM_PACKAGE_LICENSE, CPACK_RPM_PACKAGE_GROUP, ...
--------------------------------------------------------------------
```
