
/*
	@Note
	- 불칸 라이브러리 연결 해제
*/

bool ConnectWithVulkanLoaderLibrary( LIBRARY_TYPE & vulkan_library ) 
{
#if defined _WIN32
	vulkan_library = LoadLibrary( "vulkan-1.dll" );
#elif defined __linux
	vulkan_library = dlopen( "libvulkan.so.1", RTLD_NOW );
#endif
	if( vulkan_library == nullptr ) 
	{
	  std::cout << "Could not connect with a Vulkan Runtime library." << std::endl;
	  return false;
	}
	return true;
}

void ReleaseVulkanLoaderLibrary( LIBRARY_TYPE & vulkan_library ) 
{
	if( nullptr != vulkan_library ) 
	{
#if defined _WIN32
		FreeLibrary( vulkan_library );
#elif defined __linux
		dlclose( vulkan_library );
#endif
		vulkan_library = nullptr;
	}
}

//------------------------------------------------------------------------------------------
/*
	@Note
	- 전역 함수 포인터
	VulkanFunctions.h
	VulkanFunctions.cpp
*/

//header
#define EXPORTED_VULKAN_FUNCTION( name ) extern PFN_##name name;
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) extern PFN_##name name;
#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) extern PFN_##name name;

#include "ListOfVulkanFunctions.inl"

//cpp
#define EXPORTED_VULKAN_FUNCTION( name ) PFN_##name name;
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;
#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;

#include "ListOfVulkanFunctions.inl"

//------------------------------------------------------------------------------------------

/*
	@Note
	- 함수 로드 부분
	1. 글로벌 
	2. 인스턴스
	3. 디바이스
*/

bool LoadFunctionExportedFromVulkanLoaderLibrary( LIBRARY_TYPE const & vulkan_library ) {
#if defined _WIN32
	#define LoadFunction GetProcAddress
#elif defined __linux
	#define LoadFunction dlsym
#endif

	#define EXPORTED_VULKAN_FUNCTION( name )                              \
	name = (PFN_##name)LoadFunction( vulkan_library, #name );         \
	if( name == nullptr ) {                                           \
	  std::cout << "Could not load exported Vulkan function named: "  \
		#name << std::endl;                                           \
	  return false;                                                   \
	}

	#include "ListOfVulkanFunctions.inl"

	return true;
}

bool LoadGlobalLevelFunctions() {
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                              \
	name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );           \
	if( name == nullptr ) {                                               \
	  std::cout << "Could not load global level Vulkan function named: "  \
		#name << std::endl;                                               \
	  return false;                                                       \
	}

	#include "ListOfVulkanFunctions.inl"
	
	return true;
}

bool LoadInstanceLevelFunctions( VkInstance instance,
							   std::vector<char const *> const & enabled_extensions ) {
	// Load core Vulkan API instance-level functions
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name )                                  \
	name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );                \
	if( name == nullptr ) {                                                     \
	  std::cout << "Could not load instance-level Vulkan function named: "      \
		#name << std::endl;                                                     \
	  return false;                                                             \
	}

	// Load instance-level functions from enabled extensions
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )        \
	for( auto & enabled_extension : enabled_extensions ) {                      \
	  if( std::string( enabled_extension ) == std::string( extension ) ) {      \
		name = (PFN_##name)vkGetInstanceProcAddr( instance, #name );            \
		if( name == nullptr ) {                                                 \
		  std::cout << "Could not load instance-level Vulkan function named: "  \
			#name << std::endl;                                                 \
		  return false;                                                         \
		}                                                                       \
	  }                                                                         \
	}

	#include "ListOfVulkanFunctions.inl"

	return true;
}

bool LoadDeviceLevelFunctions( VkDevice logical_device,
							 std::vector<char const *> const & enabled_extensions ) {
	// Load core Vulkan API device-level functions
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name )                                    \
	name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );            \
	if( name == nullptr ) {                                                     \
	  std::cout << "Could not load device-level Vulkan function named: "        \
		#name << std::endl;                                                     \
	  return false;                                                             \
	}

	// Load device-level functions from enabled extensions
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )          \
	for( auto & enabled_extension : enabled_extensions ) {                      \
	  if( std::string( enabled_extension ) == std::string( extension ) ) {      \
		name = (PFN_##name)vkGetDeviceProcAddr( logical_device, #name );        \
		if( name == nullptr ) {                                                 \
		  std::cout << "Could not load device-level Vulkan function named: "    \
			#name << std::endl;                                                 \
		  return false;                                                         \
		}                                                                       \
	  }                                                                         \
	}

	#include "ListOfVulkanFunctions.inl"

	return true;
}
