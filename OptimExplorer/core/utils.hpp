#if defined(_MSC_VER) && !defined(NDEBUG)
  #define DISABLE_WARNING_PUSH __pragma(warning(push))
  #define DISABLE_WARNING_PUSH_ALL __pragma(warning(push, 0))
  #define DISABLE_WARNING_POP __pragma(warning(pop))
  #define DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))

  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER DISABLE_WARNING(4100)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION DISABLE_WARNING(4505)
  #define DISABLE_WARNING_PRAGMAS                             
  #define DISABLE_WARNING_SIGN_CONVERSION                     
  #define DISABLE_WARNING_OLD_STYLE_CAST                      
  #define DISABLE_WARNING_IMPLICIT_INT_CONVERSION             
  #define DISABLE_WARNING_SHORTEN_64_TO_32                    
  #define DISABLE_WARNING_MISSING_PROTOTYPES                  
  #define DISABLE_WARNING_SHADOW                              
  #define DISABLE_WARNING_ENUM_ENUM_CONVERSION                
  #define DISABLE_WARNING_USELESS_CAST                        
  #define DISABLE_WARNING_PEDANTIC                            
  #define DISABLE_WARNING_IGNORED_QUALIFIERS                  
  #define DISABLE_WARNING_REORDER                             
  #define DISABLE_WARNING_NON_VIRTUAL_DTOR

#elif (defined(__GNUC__) || defined(__clang__))  && !defined(NDEBUG)
  #define DO_PRAGMA(X) _Pragma(#X)
  #define DISABLE_WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
  #define DISABLE_WARNING_POP DO_PRAGMA(GCC diagnostic pop)
  #define DISABLE_WARNING(warningName) DO_PRAGMA(GCC diagnostic ignored #warningName)

  #define DISABLE_WARNING_PRAGMAS DISABLE_WARNING(-Wpragmas)
  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER DISABLE_WARNING(-Wunused-parameter)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION DISABLE_WARNING(-Wunused-function)
  #define DISABLE_WARNING_SIGN_CONVERSION DISABLE_WARNING(-Wsign-conversion)
  #define DISABLE_WARNING_OLD_STYLE_CAST DISABLE_WARNING(-Wold-style-cast)
  #define DISABLE_WARNING_IMPLICIT_INT_CONVERSION DISABLE_WARNING(-Wimplicit-int-conversion)
  #define DISABLE_WARNING_SHORTEN_64_TO_32 DISABLE_WARNING(-Wshorten-64-to-32)
  #define DISABLE_WARNING_MISSING_PROTOTYPES DISABLE_WARNING(-Wmissing-prototypes)
  #define DISABLE_WARNING_SHADOW DISABLE_WARNING(-Wshadow)
  #define DISABLE_WARNING_ENUM_ENUM_CONVERSION DISABLE_WARNING(-Wenum-enum-conversion)
  #define DISABLE_WARNING_USELESS_CAST DISABLE_WARNING(-Wuseless-cast)
  #define DISABLE_WARNING_PEDANTIC DISABLE_WARNING(-Wpedantic)
  #define DISABLE_WARNING_IGNORED_QUALIFIERS DISABLE_WARNING(-Wignored-qualifiers)
  #define DISABLE_WARNING_REORDER DISABLE_WARNING(-Wreorder)
  #define DISABLE_WARNING_NON_VIRTUAL_DTOR DISABLE_WARNING(-Wnon-virtual-dtor)

  #define DISABLE_WARNING_PUSH_ALL                      \
    DISABLE_WARNING_PRAGMAS                             \
    DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER       \
    DISABLE_WARNING_UNREFERENCED_FUNCTION               \
    DISABLE_WARNING_SIGN_CONVERSION                     \
    DISABLE_WARNING_OLD_STYLE_CAST                      \
    DISABLE_WARNING_IMPLICIT_INT_CONVERSION             \
    DISABLE_WARNING_SHORTEN_64_TO_32                    \
    DISABLE_WARNING_MISSING_PROTOTYPES                  \
    DISABLE_WARNING_SHADOW                              \
    DISABLE_WARNING_ENUM_ENUM_CONVERSION                \
    DISABLE_WARNING_USELESS_CAST                        \
    DISABLE_WARNING_PEDANTIC                            \
    DISABLE_WARNING_IGNORED_QUALIFIERS                  \
    DISABLE_WARNING_REORDER                             \
    DISABLE_WARNING_NON_VIRTUAL_DTOR

#else
  #define DISABLE_WARNING_PUSH_ALL
  #define DISABLE_WARNING_PUSH
  #define DISABLE_WARNING_POP
  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION
  #define DISABLE_WARNING_PRAGMAS                             
  #define DISABLE_WARNING_SIGN_CONVERSION                     
  #define DISABLE_WARNING_OLD_STYLE_CAST                      
  #define DISABLE_WARNING_IMPLICIT_INT_CONVERSION             
  #define DISABLE_WARNING_SHORTEN_64_TO_32                    
  #define DISABLE_WARNING_MISSING_PROTOTYPES                  
  #define DISABLE_WARNING_SHADOW                              
  #define DISABLE_WARNING_ENUM_ENUM_CONVERSION                
  #define DISABLE_WARNING_USELESS_CAST                        
  #define DISABLE_WARNING_PEDANTIC                            
  #define DISABLE_WARNING_IGNORED_QUALIFIERS                  
  #define DISABLE_WARNING_REORDER                             
  #define DISABLE_WARNING_NON_VIRTUAL_DTOR

#endif