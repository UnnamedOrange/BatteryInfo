#if defined(_WIN32) || defined(__WIN32__)
    #define DLL_IMPORT __declspec(dllimport)
    #define DLL_EXPORT __declspec(dllexport)
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #define DLL_IMPORT __attribute__((visibility("default")))
    #define DLL_EXPORT __attribute__((visibility("default")))
#else
    #define DLL_IMPORT
    #define DLL_EXPORT
#endif
